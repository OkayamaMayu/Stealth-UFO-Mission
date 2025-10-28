#include"Collision.h"
#include"MyMath.h"

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

//線と矩形の当たり判定：線の開始座標,線の向き, 矩形の座標, 矩形のサイズ
bool LineToRect(VECTOR linePos, VECTOR lineRot, VECTOR RectPos, VECTOR RectSize)
{
	//線のベクトル
	//向いている方向に伸ばす
	VECTOR lineVec = Math::GetMoveVec(lineRot.y, 1.0f);

	//外積の計算


	//すべて正の数もしくはすべて負の数なら当たっていない
	//0がある場合は線と重なっている

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