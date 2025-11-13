#include"DxLib.h"
#include"CameraManager.h"
#include"../MyLib/MyLib.h"

CameraManager::CameraManager()
{
	m_CurrentCameraID = CAMERA_ID_PLAY;
}

CameraManager::~CameraManager()
{

}

void CameraManager::Init(VECTOR plPos, float plRot, LoadStageData & data)
{
	//プレイカメラの設定
	playCamera.Init(DEFAULT_EYE_POS, DEFAULT_UP_VEC, plPos,plRot, data);
	playCamera.SetNearFar(1.0f, DEFAULT_FAR);

	//デバックカメラの設定
	debugCamera.Init(DEFAULT_EYE_POS);
	debugCamera.SetNearFar(1.0f, DEFAULT_FAR);
}

void CameraManager::Load()
{
	m_iHandle = LoadGraph(IMAGE_PATH.c_str());
}

void CameraManager::Step(VECTOR vForcus, float fRot,int plState, UFO ufo)
{
	switch (m_CurrentCameraID)
	{
	case CAMERA_ID_PLAY:	//プレイヤーカメラ

		playCamera.Step(vForcus, fRot,plState, ufo);

		break;

	case CAMERA_ID_DEBUG:	//デバックカメラ

		debugCamera.Step();

		break;
	default:
		break;
	}

	//情報の更新
	Updeta();
}
void CameraManager::Updeta()
{
	switch (m_CurrentCameraID)
	{
	case CAMERA_ID_PLAY:	//プレイヤーカメラ

		playCamera.Update();

		break;

	case CAMERA_ID_DEBUG:	//デバックカメラ

		debugCamera.Update();

		break;
	default:
		break;
	}
}

void CameraManager::Draw()
{
	switch (m_CurrentCameraID)
	{
	case CAMERA_ID_PLAY:	//プレイヤーカメラ

		break;

	case CAMERA_ID_DEBUG:	//デバックカメラ

		debugCamera.Draw();

		break;
	default:
		break;
	}

	//一人称カメラの場合中央の十字を描画
	if (GetPlVisionFlag())
	{
		//画面の中央に描画
		VECTOR pos = { SCREEN_SIZE_X / 2 ,SCREEN_SIZE_Y / 2 ,0.0f };
		Draw::DrawRotaGraphV(pos, 1.0f, 0.0f, m_iHandle, true);
	}

	playCamera.Draw();
}

void CameraManager::Fin()
{
}

//カメラの座標を取得
VECTOR CameraManager::GetCameraPos()
{
	//カメラの種類で返す座標を変える
	if (m_CurrentCameraID == CAMERA_ID_PLAY) 
	{ 
		return playCamera.GetPos(); 
	}
	else 
	{ 
		return debugCamera.GetPos();
	}
}

//カメラの種類を変える
void CameraManager::ChangeCameraMode(float fRot)
{
	if (Input::Key::Push(KEY_INPUT_C))
	{
		m_CurrentCameraID = CAMERA_ID_DEBUG;
		debugCamera.Init(playCamera.GetPos(), fRot);
	}
	if (Input::Key::Push(KEY_INPUT_V))
	{
		m_CurrentCameraID = CAMERA_ID_PLAY;
	}
}