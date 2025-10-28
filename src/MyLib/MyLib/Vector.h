#pragma once
#include"DxLib.h"
#include <math.h>

//ベクター系まとめ
class Vector
{
public:
	//指定された数をすべてに適応する
	static VECTOR	MakeVec(float set) { return { set,set,set }; }

	//ベクトル作成
	static VECTOR	VecCreate(VECTOR PosA, VECTOR PosB);

	//ベクトルの長さ
	static float	VecLong(VECTOR PosA, VECTOR PosB);

	//ベクトルの足し算
	static VECTOR	VecAdd(VECTOR vecA, VECTOR vecB);

	//ベクトルの引き算
	static VECTOR	VecSubtract(VECTOR vecA, VECTOR vecB);

	//ベクトルのスカラー倍
	static VECTOR	VecScale(VECTOR vec, float scale);

	//ベクトルの割り算
	static VECTOR	VecDivision(VECTOR vec, float division);

	//ベクトルの掛け算
	static VECTOR	VecMultiplication(VECTOR vecA, VECTOR vecB);

	//ベクトルの内積
	static float	VecDot(VECTOR vecA, VECTOR vecB);

	//ベクトルの外積
	static VECTOR	VecCross(VECTOR vecA, VECTOR vecB);

	//ベクトルの正規化
	static VECTOR	VecNormalize(VECTOR vec);
};