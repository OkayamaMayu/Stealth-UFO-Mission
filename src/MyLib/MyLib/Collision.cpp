#include"Collision.h"
#include"MyMath.h"
#include <algorithm>
using namespace std;

//IsHitRectの定義（四角）
bool Collision::Rect(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH)
{
	if (bX < aX + aW &&
		bX + bW > aX &&
		bY + bH > aY &&
		bY < aY + aH)
	{
		return true;
	}
	return false;
}
bool Collision::Rect(float aX, float aY, int aW, int aH, float bX, float bY, int bW, int bH)
{
	if (bX < aX + aW &&
		bX + bW > aX &&
		bY + bH > aY &&
		bY < aY + aH)
	{
		return true;
	}
	return false;
}
bool Collision::Rect(VECTOR aPos, VECTOR aSize, VECTOR bPos, VECTOR bSize)
{
	if (bPos.x < aPos.x + aSize.x &&
		bPos.x + bSize.x > aSize.x &&
		bPos.y + bSize.y > aPos.y &&
		bPos.y < aPos.y + aSize.y)
	{
		return true;
	}
	return false;
}
//四角（座標中央から判定）：x, y 使用 / sizeは直径
bool Collision::RectCenter(VECTOR aPos, VECTOR aSize, VECTOR bPos, VECTOR bSize)
{
	VECTOR aS = VScale(aSize, 0.5f);
	VECTOR bS = VScale(bSize, 0.5f);
	if (bPos.x - bS.x < aPos.x + aS.x &&
		bPos.x + bS.x > aPos.x - aS.x &&

		bPos.y - bS.y < aPos.y + aS.y &&
		bPos.y + bS.y > aPos.y - aS.y)
	{
		return true;
	}
	return false;
}

//IsHitRectの定義（四角）：3D
bool Collision::Rect3D(VECTOR aCenterPos, VECTOR aSize, VECTOR bCenterPos, VECTOR bSize)
{
	VECTOR aHalfSize = VScale(aSize, 0.5f);
	VECTOR bHalfSize = VScale(bSize, 0.5f);

	if (bCenterPos.x - bHalfSize.x < aCenterPos.x + aHalfSize.x &&
		bCenterPos.x + bHalfSize.x > aCenterPos.x - aHalfSize.x &&

		bCenterPos.y - bHalfSize.y < aCenterPos.y + aHalfSize.y &&
		bCenterPos.y + bHalfSize.y > aCenterPos.y - aHalfSize.y &&

		bCenterPos.z - bHalfSize.z < aCenterPos.z + aHalfSize.z &&
		bCenterPos.z + bHalfSize.z > aCenterPos.z - aHalfSize.z
		)
	{
		return true;
	}
	return false;
}

//IsHitCircleの定義（円形）
bool Collision::Circlr(int aX, int aY, int aR, int bX, int bY, int bR)
{
	if ((aR + bR) * (aR + bR) > (aX - bX) * (aX - bX) + (aY - bY) * (aY - bY))
	{
		return true;
	}
	return false;
}

//IsHitCircleの定義（円形）：3D
bool Collision::Circlr3D(VECTOR aPos, float aR, VECTOR bPos, float bR)
{
	if ((aR + bR) * (aR + bR) >
		(aPos.x - bPos.x) * (aPos.x - bPos.x) +
		(aPos.y - bPos.y) * (aPos.y - bPos.y) +
		(aPos.z - bPos.z) * (aPos.z - bPos.z))
	{
		return true;
	}
	return false;
}

//----------------------------

//箱と箱の当たり判定
bool Collision::IsCollidingAABBToAABB(AABB aabbA, AABB aabbB) {
	//モデルの中心座標
	VECTOR aabbACenterPos = aabbA.centerPos;
	VECTOR aabbBCenterPos = aabbB.centerPos;

	//箱の最小点と最大点
	VECTOR aabbAMinPos = VSub(aabbA.centerPos, aabbA.size);
	VECTOR aabbAMaxPos = VAdd(aabbA.centerPos, aabbA.size);
	//箱の最小点と最大点
	VECTOR aabbBMinPos = VSub(aabbB.centerPos, aabbB.size);
	VECTOR aabbBMaxPos = VAdd(aabbB.centerPos, aabbB.size);

	if (aabbAMinPos.x < aabbBMaxPos.x && aabbAMaxPos.x > aabbBMinPos.x &&
		aabbAMinPos.y < aabbBMaxPos.y && aabbAMaxPos.y > aabbBMinPos.y &&
		aabbAMinPos.z < aabbBMaxPos.z && aabbAMaxPos.z > aabbBMinPos.z)
		return true;
	
	return false;
}

//箱と球の当たり判定
bool Collision::IsCollidingAABBToSphere(AABB aabb, Sphere sphere) {
	//モデルの中心座標
	VECTOR aabbCenterPos = aabb.centerPos;
	VECTOR sphereCenterPos = sphere.centerPos;

	//判定の半径
	float sphereRadius = sphere.radius;

	//箱の最小点
	VECTOR aabbMinPos = VSub(aabb.centerPos, aabb.size);
	//箱の最大点
	VECTOR aabbMaxPos = VAdd(aabb.centerPos, aabb.size);

	//箱の一番近い点を求める
	VECTOR nearestPos = Math::Clamp(sphereCenterPos, aabbMinPos, aabbMaxPos);

	//近い点と球で判定
	if (Math::GetDistance(nearestPos, sphereCenterPos) < sphereRadius)
		return true;

	return false;
}

//箱と線分の当たり判定
bool Collision::IsCollidingAABBToLineSegment(AABB aabb, LineSegment lineSegment) {
	//箱の最小点
	VECTOR aabbMinPos = VSub(aabb.centerPos, aabb.size);
	//箱の最大点
	VECTOR aabbMaxPos = VAdd(aabb.centerPos, aabb.size);

	//線分のベクトル
	VECTOR lineVec = VSub(lineSegment.endPos, lineSegment.startPos);

	//それぞれのスラグに入る時間
	float nearX = (aabbMinPos.x - lineSegment.startPos.x) / lineVec.x;
	float nearY = (aabbMinPos.y - lineSegment.startPos.y) / lineVec.y;
	float nearZ = (aabbMinPos.z - lineSegment.startPos.z) / lineVec.z;

	//それぞれのスラグを出る時間
	float farX = (aabbMaxPos.x - lineSegment.startPos.x) / lineVec.x;
	float farY = (aabbMaxPos.y - lineSegment.startPos.y) / lineVec.y;
	float farZ = (aabbMaxPos.z - lineSegment.startPos.z) / lineVec.z;

	//ベクトルがマイナスの場合はnearとfarを逆転させる
	if (lineVec.x < 0)swap(nearX, farX);
	if (lineVec.y < 0)swap(nearY, farY);
	if (lineVec.z < 0)swap(nearZ, farZ);

	//nearの最大値を選出
	float lineNear = max(nearX, nearY);
	lineNear = max(lineNear, nearZ);
	//farの最小値を選出
	float lineFar = min(farX, farY);
	lineFar = min(lineFar, farZ);

	//重なった時間を調べる
	float overlappingTime = lineFar - lineNear;	

	//0～1の範囲に入っている
	//正の数だと重なっている
	if (1.0f >= lineNear && lineFar >= 0.0f && 
		overlappingTime >= 0) return true;

	return false;
}

//球と球の当たり判定
bool Collision::IsCollidingSphereToSphere(Sphere sphereA, Sphere sphereB){
	//モデルの中心座標
	VECTOR sphereACenterPos = sphereA.centerPos;
	VECTOR sphereBCenterPos = sphereB.centerPos;

	//判定の半径
	float sphereARadius = sphereA.radius;
	float sphereBRadius = sphereB.radius;

	if ((sphereARadius + sphereBRadius) * (sphereARadius + sphereBRadius) >
		(sphereACenterPos.x - sphereBCenterPos.x) * (sphereACenterPos.x - sphereBCenterPos.x) +
		(sphereACenterPos.y - sphereBCenterPos.y) * (sphereACenterPos.y - sphereBCenterPos.y) +
		(sphereACenterPos.z - sphereBCenterPos.z) * (sphereACenterPos.z - sphereBCenterPos.z))
		return true;
	
	return false;
}

//球と線分の当たり判定
bool Collision::IsCollidingSphereToLineSegment(Sphere sphere, LineSegment lineSegment) {
	//球の中心座標
	VECTOR sphereCenterPos = sphere.centerPos;
	//球の半径
	float sphereRadius = sphere.radius;

	//線分のベクトル
	VECTOR lineVec = VSub(lineSegment.endPos, lineSegment.startPos);
	//線分の開始地点と球のベクトル
	VECTOR lineToSphereVec = VSub(sphereCenterPos, lineSegment.startPos);
	//球ともっとも近い位置の線分の割合
	float t = VDot(lineToSphereVec, lineVec) / VDot(lineVec, lineVec);

	//端にあわせる
	t = Math::Clamp(t, 0.0f, 1.0f);

	//線上の球ともっとも近い座標を求める
	VECTOR lineMove = VScale(lineVec, t);
	VECTOR lineNear = VAdd(lineSegment.startPos, lineMove);
	if (Math::GetDistance(lineNear, sphereCenterPos) <= sphereRadius) return true;

	return false;
}

//===========================================

//ひし形の当たり判定中身：一辺の始まりと終わり, 調べたいものの座標(点)
bool Collision::DiamondSide(float aX, float aY, float bX, float bY, float pX, float pY)
{
	float abX = aX - bX;
	float abY = aY - bY;

	float paX = aX - pX;
	float paY = aY - pY;

	float num = abX * paY - paX * abY;

	//点が0以上だと線の右側(内側)
	if (num >= 0)
	{
		return true;
	}
	//マイナスだと左側(外側)
	else
	{
		return false;
	}
}
bool Collision::DiamondSide(VECTOR a, VECTOR b, VECTOR p)
{
	float abX = a.x - b.x;
	float abY = a.y - b.y;

	float paX = a.x - p.x;
	float paY = a.y - p.y;

	float num = abX * paY - paX * abY;

	//点が0以上だと線の右側(内側)
	if (num >= 0)
	{
		return true;
	}
	//マイナスだと左側(外側)
	else
	{
		return false;
	}
}

//ひし形の当たり判定：頂点を時計回り, 調べたいものの座標(点)
//ひし形から出ていたらtrueを返す
bool Collision::DiamondToPoint(
	float aX, float aY,
	float bX, float bY,
	float cX, float cY,
	float dX, float dY,
	float pX, float pY)
{
	//一辺ずつ調べる
	bool a = DiamondSide(aX, aY, bX, bY, pX, pY);
	bool b = DiamondSide(bX, bY, cX, cY, pX, pY);
	bool c = DiamondSide(cX, cY, dX, dY, pX, pY);
	bool d = DiamondSide(dX, dY, aX, aY, pX, pY);

	//全部trueなら点は内側にある
	if (a && b && c && d)
	{
		return false;
	}
	//falseが一つでもあれば外
	else
	{
		return true;
	}
}
bool Collision::DiamondToPoint(VECTOR a, VECTOR b, VECTOR c, VECTOR d, VECTOR p)
{
	//一辺ずつ調べる
	bool A = DiamondSide(a, b, p);
	bool B = DiamondSide(b, c, p);
	bool C = DiamondSide(c, d, p);
	bool D = DiamondSide(d, a, p);

	//全部trueなら点は内側にある
	if (A && B && C && D)
	{
		return false;
	}
	//falseが一つでもあれば外
	else
	{
		return true;
	}
}

//ひし形と矩形の当たり判定：ひし形の頂点時計回り, 調べたいものの座標とサイズ
bool Collision::DiamondToRect(
	VECTOR a, VECTOR b, VECTOR c, VECTOR d,
	VECTOR p, int sizeX, int sizeY,
	int type)
{
	VECTOR pp = p;

	//対象左上判定
	pp.x -= sizeX / 2;
	pp.y -= sizeY / 2;
	bool A = DiamondToPoint(a, b, c, d, pp);
	pp = p;

	//対象右上判定
	pp.x += sizeX / 2;
	pp.y -= sizeY / 2;
	bool B = DiamondToPoint(a, b, c, d, pp);
	pp = p;

	//対象左下判定
	pp.x -= sizeX / 2;
	pp.y += sizeY / 2;
	bool C = DiamondToPoint(a, b, c, d, pp);
	pp = p;

	//対象右下判定
	pp.x += sizeX / 2;
	pp.y += sizeY / 2;
	bool D = DiamondToPoint(a, b, c, d, pp);
	pp = p;

	if (type == 0)
	{
		//どれかがtrueだと内側
		if (A || B || C || D)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (A && B && C && D)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

