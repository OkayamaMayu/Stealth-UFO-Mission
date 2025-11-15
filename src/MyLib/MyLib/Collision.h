#pragma once
#include"DxLib.h"
#include <math.h>

//箱情報
struct AABB{
	VECTOR	centerPos;		//中心座標
	VECTOR	size;			//大きさ
};

//球座標
struct Sphere{
	VECTOR	centerPos;		//中心座標
	float	radius;			//半径
};

//線分
struct LineSegment{
	VECTOR	startPos;		//開始点座標
	VECTOR	endPos;			//終了点座標
};

//当たり判定
class Collision
{
private:
	//ひし形の当たり判定中身：一辺の始まりと終わり, 調べたいものの座標(点)
	static bool DiamondSide(float aX, float aY, float bX, float bY, float pX, float pY);
	static bool DiamondSide(VECTOR a, VECTOR b, VECTOR p);

public:
	//四角：X,Yは座標、Wが横幅、Hが縦幅
	static bool Rect(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH);
	static bool Rect(float aX, float aY, int aW, int aH, float bX, float bY, int bW, int bH);
	//四角：x, y 使用
	static bool Rect(VECTOR aPos, VECTOR aSize, VECTOR bPos, VECTOR bSize);
	//四角（座標中央から判定）：x, y 使用 / sizeは直径
	static bool RectCenter(VECTOR aPos, VECTOR aSize, VECTOR bPos, VECTOR bSize);

	//四角(3D)：それぞれの座標VECと大きさVEC
	static bool Rect3D(VECTOR aCenterPos, VECTOR aSize, VECTOR bCenterPos, VECTOR bSize);

	//円形：それぞれの座標と半径
	static bool Circlr(int aX, int aY, int aR, int bX, int bY, int bR);

	//円形(3D)：それぞれの座標と半径
	static bool Circlr3D(VECTOR aPos, float aR, VECTOR bPos, float bR);

	//----------------------------

	//箱と箱の当たり判定
	static bool IsCollidingAABBToAABB(AABB aabbA, AABB aabbB);

	//箱と球の当たり判定
	static bool IsCollidingAABBToSphere(AABB aabb, Sphere sphere);

	//箱と線分の当たり判定
	static bool IsCollidingAABBToLineSegment(AABB aabb, LineSegment lineSegment);

	//球と球の当たり判定
	static bool IsCollidingSphereToSphere(Sphere sphereA, Sphere sphereB);

	//球と線分の当たり判定
	static bool IsCollidingSphereToLineSegment(Sphere sphere, LineSegment lineSegment);

	//==========================================

	//ひし形の当たり判定：頂点を時計回り, 調べたいものの座標(点)
	//ひし形から出ていたらtrueを返す
	static bool DiamondToPoint(
		float aX, float aY,
		float bX, float bY,
		float cX, float cY,
		float dX, float dY,
		float pX, float pY);
	static bool DiamondToPoint(VECTOR a, VECTOR b, VECTOR c, VECTOR d, VECTOR p);

	//ひし形と矩形の当たり判定：ひし形の頂点時計回り, 調べたいものの座標とサイズ
	//0がひし形の内側, 1がひし形の外側でとる
	static bool DiamondToRect(VECTOR a, VECTOR b, VECTOR c, VECTOR d, VECTOR p, int sizeX, int sizeY, int type = 0);
};