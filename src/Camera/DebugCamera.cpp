#include"DxLib.h"
#include"DebugCamera.h"
#include"CameraManager.h"
#include"../MyLib/MyLib.h"
#include"math.h"

DebugCamera::DebugCamera()
{
	memset(&pos, 0, sizeof(VECTOR));
	memset(&rot, 0, sizeof(VECTOR));
}

DebugCamera::~DebugCamera() {}

//初期化
void DebugCamera::Init()
{
	pos = VGet(0.0f, 0.0f, 1.0f);
	rot = VGet(0.0f, 0.0f, 1.0f);
}

//初期化：視点, 注視点, 上方向,
void DebugCamera::Init(VECTOR vPos, float fRot)
{
	pos		= vPos;
	rot.y	= fRot + DX_PI_F;
}

//ニア、ファーをセット
void DebugCamera::SetNearFar(float fNear, float fFar)
{
	SetCameraNearFar(fNear, fFar);
}

//通常処理
void DebugCamera::Step()
{
	float fSpd = 0.0f;
	//移動
	if (Input::Key::Keep(KEY_INPUT_W))
	{
		fSpd = MOVE_SPEED;
	}
	if (Input::Key::Keep(KEY_INPUT_S))
	{
		fSpd = -MOVE_SPEED;
	}

	float fRot = 0.0f;
	if (Input::Key::Keep(KEY_INPUT_A))
	{
		fSpd = -MOVE_SPEED;
		fRot = Math::ChangeDegToRad(90);
	}
	if (Input::Key::Keep(KEY_INPUT_D))
	{
		fSpd = MOVE_SPEED;
		fRot = Math::ChangeDegToRad(90);
	}

	//注視点移動
	if (Input::Key::Keep(KEY_INPUT_UP))
	{
		rot.x -= ROTATE_SPEED;
	}
	if (Input::Key::Keep(KEY_INPUT_DOWN))
	{
		rot.x += ROTATE_SPEED;
	}
	
	//回転移動
	if (Input::Key::Keep(KEY_INPUT_LEFT))
	{
		rot.y -= ROTATE_SPEED;
	}
	if (Input::Key::Keep(KEY_INPUT_RIGHT))
	{
		rot.y += ROTATE_SPEED;
	}

	pos.x += sinf(rot.y + fRot) * fSpd;
	pos.z += cosf(rot.y + fRot) * fSpd;
}

void DebugCamera::Draw()
{
	DrawFormatString(0, 0, GetColor(255, 0, 0), "デバックカメラモード");
}

//更新したデータを反映させる
void DebugCamera::Updata()
{
	//カメラ設定
	SetCameraPositionAndAngle(pos, rot.x, rot.y, rot.z);
}
