#pragma once
#include <math.h>

//追加分数学
class Math
{
public:
	//指定した座標に向かう
	//引数：動かしたいもの, ターゲット座標, 増加量
	static VECTOR	GetMoving			(VECTOR movePos, VECTOR targetPos, float speed);

	//追尾 回転
	//引数：動かしたい物の座標, ターゲット座標 増加量
	static VECTOR	GetMovingRot		(VECTOR movePos, VECTOR moveRot, VECTOR targetPos, float speed = 0.01f);

	//2点間の距離
	//引数：開始地点, 終了地点
	static float	GetDistance(VECTOR startPos, VECTOR endPos);
	//2点間の距離（VECTOR使用用２D）
	//引数：開始地点, 終了地点
	static float	GetDistance2D(VECTOR startPos, VECTOR endPos);

	//対象からターゲットまでの角度(ラジアン)
	//引数：向かせる対象の座標, ターゲットの座標
	static float	GetAngle(VECTOR pos, VECTOR targetPos);
	//対象からターゲットまでの角度(ラジアン)
	//引数：向かせる対象の座標, ターゲットの座標, 格納先
	static void		GetAngle(VECTOR pos, VECTOR targetPos, VECTOR& setRot);

	//ベクトルの内積を求めて正か負を返す
	//引数：向かせる対象の座標, ターゲットの座標
	static bool		GetInnerProduct(VECTOR pos, VECTOR targetPos);

	//度をラジアンに変換
	//引数：度
	static float	ChangeDegToRad(float degree);
	//ラジアンを度に変換
	//引数：ラジアン
	static float	ChangeRadToDeg(float radian);

	//向いてる方向に進む
	//引数：斜辺, 移動量
	static VECTOR	GetMoveVec(float degree, float speed);
	//向いてる方向に進む（Y,X回転）
	//引数：斜辺, 移動量
	static VECTOR	GetMoveVec(VECTOR degree, float speed);

	//指定の数に近付ける
	//引数：変更したい変数, 指定の数, 増加量
	static bool		MatchSpecifiedNum(float& fChangeNum, float fSpecified, float fIncrease = 1.0f);
	//指定の数に近付ける
	//引数：変更したい変数, 指定の数, 増加量
	static bool		MatchSpecifiedNum(int& iChangeNum, int iSpecified, int iIncrease = 1);

	//視野内判定
	//引数：視点発生座標, 視点の向き, 捜索先座標, 視界範囲, 視野角
	static bool		CheckVision(VECTOR centerPos, float centerRot, VECTOR attentionPos, float range, float viewRange = 90.0f);
	//視野内判定(画面中央からしか取れない)
	//引数：視点発生座標, カメラの視点, 視点の向き, 捜索先座標, 視界範囲, 視野角
	static bool		CheckVision(VECTOR centerPos, VECTOR vForcus, float centerRot, VECTOR attentionPos, float range, float viewRange = 90.0f);

	//回転が一周(180度)をこえると変数をリセット
	static void		RotReset(VECTOR &vRot);
	//回転が一周(180度)をこえると変数をリセット
	static void		RotReset(float &fRot);

	//起点から回転
	//引数：起点となる座標, 回転する物の座標, 回転速度
	static VECTOR	BaseRotationPosition(VECTOR basePos, VECTOR rotPos, float rot);
};