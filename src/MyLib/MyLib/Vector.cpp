#include"Vector.h"

//ベクトル生成
VECTOR Vector::VecCreate(VECTOR vPosA, VECTOR vPosB)
{
	VECTOR result;

	result.x = vPosB.x - vPosA.x;
	result.y = vPosB.y - vPosA.y;
	result.z = vPosB.z - vPosA.z;

	return result;
}

//ベクトルの長さを返す
float Vector::VecLong(VECTOR PosA, VECTOR PosB)
{
	float result;

	result = sqrtf((PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y) + (PosA.z - PosB.z) * (PosA.z - PosB.z));

	return result;
}

//ベクトルの足し算
VECTOR Vector::VecAdd(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.x + vecB.x;
	result.y = vecA.y + vecB.y;
	result.z = vecA.z + vecB.z;

	return result;
}

//ベクトルの引き算
VECTOR Vector::VecSubtract(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.x - vecB.x;
	result.y = vecA.y - vecB.y;
	result.z = vecA.z - vecB.z;

	return result;

}

//ベクトルのスカラー倍
VECTOR Vector::VecScale(VECTOR vec, float scale)
{
	VECTOR result;

	result.x = vec.x * scale;
	result.y = vec.y * scale;
	result.z = vec.z * scale;

	return result;
}

//ベクトルの割り算
VECTOR Vector::VecDivision(VECTOR vec, float division)
{
	VECTOR result;

	result.x = vec.x * division;
	result.y = vec.y * division;
	result.z = vec.z * division;

	return result;
}

//ベクトルの掛け算
VECTOR Vector::VecMultiplication(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.x * vecB.x;
	result.y = vecA.y * vecB.y;
	result.z = vecA.z * vecB.z;

	return result;
}

//ベクトルの内積
float Vector::VecDot(VECTOR vecA, VECTOR vecB)
{
	float result;

	result = vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;

	return result;
}

//ベクトルの外積
VECTOR Vector::VecCross(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.y * vecB.z - vecA.z * vecB.y;
	result.y = vecA.z * vecB.x - vecA.x * vecB.z;
	result.z = vecA.x * vecB.y - vecA.y * vecB.x;

	return result;
}

//ベクトルの正規化
VECTOR Vector::VecNormalize(VECTOR vec)
{
	float size;
	VECTOR result;

	size = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

	size = sqrtf(size);

	result.x = vec.x / size;
	result.y = vec.y / size;
	result.z = vec.z / size;

	return result;
}