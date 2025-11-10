#include"DxLib.h"
#define _USE_MATH_DEFINES	//円周率
#include"../MyLib/DebugString.h"

#include"MyMath.h"

//指定した座標に向かう：動かしたいもの  ターゲット座標  増加量
VECTOR Math::GetMoving(VECTOR movePos, VECTOR targetPos, float speed)
{
	VECTOR v;

	//差を計算
	v = VSub(targetPos, movePos);
   
	//斜辺を計算する
	float n = v.x * v.x + v.y * v.y + v.z * v.z;

	if(n!=0.0f)
	{
		//平方根を外す
		n = sqrtf(n);

		//正規化してから移動量を計算
		v.x = v.x / n * speed;
		v.y = v.y / n * speed;
		v.z = v.z / n * speed;
	}
	
	return v;
}

//追尾 回転：動かしたい物の座標 ターゲット座標 増加量
VECTOR Math::GetMovingRot(VECTOR movePos, VECTOR moveRot, VECTOR targetPos, float speed)
{
	VECTOR MovePos = movePos;
	VECTOR MoveRot = moveRot;

	VECTOR vec;
	//差を計算
	vec.x = targetPos.x - movePos.x;
	vec.y = 0.0f;
	vec.z = targetPos.z - movePos.z;

	VECTOR MoveVec;
	//現在の進行方向のベクトルを計算
	MoveVec.x = sinf(moveRot.y) * -1.0f;
	MoveVec.y = 0.0f;
	MoveVec.z = cosf(moveRot.y) * -1.0f;

	float fCrossZ;
	//2つのベクトルの外積を計算
	fCrossZ = vec.x * MoveVec.z - MoveVec.x * vec.z;

	//fCrossZの計算結果で左右の判定を行う
	if (fCrossZ > 0)
	{
		//その方向に回転
		MoveRot.y += speed;
	}
	else if (fCrossZ < 0)
	{
		//その方向に回転
		MoveRot.y -= speed;
	}

	return MoveRot;
}

//2点間の距離：開始地点　終了地点
float Math::GetDistance(VECTOR startPos, VECTOR endPos)
{
	float num = 0.0f;
	//２点間の距離を計算
	num = ((endPos.x - startPos.x) * (endPos.x - startPos.x)) + ((endPos.y - startPos.y) * (endPos.y - startPos.y)) + ((endPos.z - startPos.z) * (endPos.z - startPos.z));
	//平方根を外す
	num = sqrtf(num);

	return num;
}
//2点間の距離（VECTOR使用用２D）：開始地点　終了地点
float Math::GetDistance2D(VECTOR startPos, VECTOR endPos)
{
	float num = 0.0f;
	//２点間の距離を計算
	num = ((endPos.x - startPos.x) * (endPos.x - startPos.x)) + ((endPos.y - startPos.y) * (endPos.y - startPos.y));
	//平方根を外す
	num = sqrtf(num);

	return num;
}

//対象からターゲットまでの角度(ラジアン)：向かせる対象の座標, ターゲットの座標
float Math::GetAngle(VECTOR pos, VECTOR targetPos)
{
	float num = 0.0f;
	
	//角度を計算
	num = atan2f(pos.x - targetPos.x, pos.z - targetPos.z);

	return num;
}
//対象からターゲットまでの角度(ラジアン)：向かせる対象の座標, ターゲットの座標, 格納先
void Math::GetAngle(VECTOR pos, VECTOR targetPos, VECTOR& setRot)
{
	float num = 0.0f;

	VECTOR rot = {};

	//角度を計算
	//X回転も計算する
	rot.y = atan2f(targetPos.x - targetPos.x, targetPos.z - targetPos.z);
	rot.x = atan2f(targetPos.z - targetPos.z, targetPos.y - targetPos.y);

	setRot = rot;
}

//ベクトルの内積を求めて正か負を返す：向かせる対象の座標, ターゲットの座標
bool Math::GetInnerProduct(VECTOR pos, VECTOR targetPos)
{
	float num = 0.0f;

	//ない席を計算
	num = targetPos.x * pos.x + targetPos.z * pos.z;
	//0を基準に判定
	if (num > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//度をラジアンに変換（引数：度）
float Math::ChangeDegToRad(float degree)
{
	return degree * (float)M_PI / 180.0f;
}

//ラジアンを度に変換（引数：ラジアン）
float Math::ChangeRadToDeg(float radian)
{
	return radian * 180.0f / (float)M_PI;
}

//向いてる方向に進む：斜辺, 移動量
VECTOR Math::GetMoveVec(float degree, float speed)
{
	VECTOR moveNum = VECTOR_ZERO;

	//斜辺から計算
	moveNum.z = cosf(degree) * speed;
	moveNum.x = sinf(degree) * speed;

	return moveNum;
}
//向いてる方向に進む：斜辺, 移動量
VECTOR Math::GetMoveVec(VECTOR degree, float speed)
{
	VECTOR moveNum = VECTOR_ZERO;

	//斜辺から計算
	moveNum.z = cosf(degree.y) * speed;
	moveNum.x = sinf(degree.y) * speed;
	moveNum.y = tanf(degree.x) * speed;

	return moveNum;
}

//指定の数に近付ける：変更したい変数, 指定の数, 増加量
bool Math::MatchSpecifiedNum(float& fChangeNum, float fSpecified, float fIncrease)
{
	//増やすか減らすか判定
	if (fChangeNum > fSpecified)
	{
		fChangeNum -= fIncrease;
	}
	else if (fChangeNum < fSpecified)
	{
		fChangeNum += fIncrease;
	}

	//指定の数を越えた
	if (fChangeNum > fSpecified - fIncrease && fChangeNum < fSpecified + fIncrease)
	{
		//変数を固定
		fChangeNum = fSpecified;

		return true;
	}

	return false;
}
//指定の数に近付ける：変更したい変数, 指定の数, 増加量
bool Math::MatchSpecifiedNum(int& iChangeNum, int iSpecified, int iIncrease)
{
	//増やすか減らすか判定
	if (iChangeNum > iSpecified)
	{
		iChangeNum -= iIncrease;
	}
	else if (iChangeNum < iSpecified)
	{
		iChangeNum += iIncrease;
	}

	//指定の数を越えた
	if (iChangeNum > iSpecified - iIncrease && iChangeNum < iSpecified + iIncrease)
	{
		//変数を固定
		iChangeNum = iSpecified;

		return true;
	}

	return false;
}

//視野内判定：視点発生座標,視点の向き, 捜索先座標, 視界範囲
bool Math::CheckVision(VECTOR centerPos, float centerRot, VECTOR attentionPos, float range, float viewRange)
{
	VECTOR v1;

	float fRot = 0.0f;	// 角度を保存する変数
	float fDot = 0.0f;	// 内積を保存する変数

	//正規化する
	VECTOR vEye = { 0.0f, 0.0f, 0.0f };
	vEye = GetMoveVec(centerRot, 1.0f);

	// キャラクター1と2のベクトルを求める
	v1.x = attentionPos.x - centerPos.x;
	v1.z = attentionPos.z - centerPos.z;

	// ベクトルの長さを求める
	float f1 = sqrtf(v1.x * v1.x + v1.z * v1.z);

	// 長さが0なら無視
	if (f1 > 0.0f)
	{
		// 長さを利用してベクトルを正規化する
		v1.x /= f1;
		v1.z /= f1;

		// 内積を求める
		fDot = v1.x * vEye.x + v1.z * vEye.z;
		// コサインθを利用してラジアン角を求める
		fRot = acosf(fDot);
		// ラジアン角で分かりにくい場合は、通常の角度へ変換
		fRot = ChangeRadToDeg(fRot);
	}

	bool checkFrg = false;
	//捜索先が範囲内だったら
	if(GetDistance(centerPos,attentionPos)<=range)
	{
		//指定の角度の半分未満なら視野内
		if (fabsf(fRot) < viewRange / 2)
		{
			checkFrg= true;
		}
	}

	return checkFrg;
}

//視野内判定(画面中央からしか取れない)：視点発生座標, カメラの視点, 視点の向き, 捜索先座標, 視界範囲, 視野角
bool Math::CheckVision(VECTOR centerPos, VECTOR vForcus, float centerRot, VECTOR attentionPos, float range, float viewRange)
{
	bool checkFrg = false;
	VECTOR cenPos2D = { 1280.0f/2.0f,720.0f/2.0f,0.0f };
	VECTOR attenPos2D = { 0.0f,0.0f,0.0f };
	attenPos2D = ConvWorldPosToScreenPos(attentionPos);

	if (GetDistance(centerPos, attentionPos) <= range)
	{
		if (GetDistance2D(cenPos2D, attenPos2D) <= viewRange)
		{
			checkFrg = true;
		}
	}

	return checkFrg;
}

//回転が一周(180度)をこえると変数をリセット
void Math::RotReset(VECTOR &vRot)
{
	//180~-180の範囲外にならないようにする
	if (vRot.y >= ChangeDegToRad(180.0f))
	{
		vRot.y = vRot.y - DX_PI_F * 2.0f;
	}
	else if (vRot.y <= ChangeDegToRad(-180.0f))
	{
		vRot.y = 2.0f * DX_PI_F + vRot.y;
	}
}
//回転が一周(180度)をこえると変数をリセット
void Math::RotReset(float& fRot)
{
	//180~-180の範囲外にならないようにする
	if (fRot >= ChangeDegToRad(180.0f))
	{
		fRot = fRot - DX_PI_F * 2.0f;
	}
	else if (fRot <= ChangeDegToRad(-180.0f))
	{
		fRot = 2.0f * DX_PI_F + fRot;
	}
}

//基準から回転：基準となる座標, 回転する物の座標, 回転速度
VECTOR Math::BaseRotationPosition(VECTOR basePos, VECTOR rotPos, float rot)
{
	VECTOR ret = rotPos;

	MATRIX mPos1 = MGetTranslate(VScale(basePos, -1.0f));	//基準に原点 に 戻す行列
	MATRIX mPos2 = MGetTranslate(basePos);					//基準に原点 から 戻す行列
	MATRIX mRot = MGetRotZ(rot);							//Y軸回転の行列

	//行列の合成
	MATRIX matrix;
	//原点に戻す、回転
	matrix = MMult(mPos1, mRot);
	//元の位置に戻す
	matrix = MMult(matrix, mPos2);

	//座標の変換
	ret = VTransform(ret, matrix);

	return ret;
}

//範囲内にする
//引数：調べる値, 最小値, 最大値
float Math::Clamp(float value, float min, float max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

//範囲内にする
//引数：調べる値, 最小値, 最大値
VECTOR Math::Clamp(VECTOR value, VECTOR min, VECTOR max) {
	VECTOR ret = {};

	ret.x = Clamp(value.x, min.x, max.x);
	ret.y = Clamp(value.y, min.y, max.y);
	ret.z = Clamp(value.z, min.z, max.z);

	return ret;
}