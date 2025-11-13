#pragma once
#include"DxLib.h"

class DebugCamera
{
private:
	const float MOVE_SPEED = 1.0f;		//移動速度
	const float ROTATE_SPEED = 0.02f;	//回転速度

private:
	VECTOR		pos;					//座標
	VECTOR		rot;					//回転角度

public:
	DebugCamera();
	~DebugCamera();

	//初期化
	void Init();
	//初期化：視点, 注視点, 上方向,
	void Init(VECTOR vPos, float fRot=0.0f);

	//ニア、ファーをセット
	void SetNearFar(float fNear, float fFar);

	//通常処理
	void Step();

	void Draw();

	//更新したデータを反映させる
	void Update();

	void SetPos(VECTOR set) { pos = set; }
	void SetRot(float set) { rot.y = set; }

	VECTOR GetPos() { return pos; }
};