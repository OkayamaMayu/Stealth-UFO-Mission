#pragma once
#include"PlayCamera.h"
#include"DebugCamera.h"
#include"../LoadStageData/LoadStageData.h"

class CameraManager
{
public:
	//カメラID
	enum CAMERA_ID
	{
		CAMERA_ID_PLAY,				//プレイカメラ
		CAMERA_ID_DEBUG,			//デバックカメラ

		CAMERA_ID_NUM
	};

private:
	//基本となる視点
	const VECTOR DEFAULT_EYE_POS	= { 0.0f,CAMERA_OFFSET_Y + 50.0f,-CAMERA_LEMGTH };	
	//注視点
	const VECTOR DEFAULT_FORCUS_POS = { 0.0f,FORCUS_OFFSET_Y,0.0f };
	//アップベクトル
	const VECTOR DEFAULT_UP_VEC		= { 0.0f,1.0f,0.0f };
	//中央の十字の画像ハンドル
	const string IMAGE_PATH			= "data/system/centerCross.png" ;

private:
	PlayCamera	playCamera;			//プレイヤーカメラ
	DebugCamera debugCamera;		//プレイヤーカメラ
	CAMERA_ID	m_CurrentCameraID;	//現在のカメラID
	int			m_iHandle;			//中央の十字用画像ハンドル

public:
	CameraManager();
	~CameraManager();

	void Init(VECTOR plPos, float plRot, LoadStageData &data);
	void Step(VECTOR vForcus,float fRot, int plState, UFO ufo);
	void Load();
	void Updeta();
	void Draw();
	void Fin();

	//クリア演出用カメラ
	void		ClearCamera(VECTOR vForcus, float fRot) { playCamera.Step(vForcus, fRot); }
	//ゲームオーガー演出用カメラ
	void		GameoverCamera(VECTOR plPos, VECTOR vForcus, float fRot) { playCamera.Step(plPos, vForcus, fRot); }
	//カメラIDを取得
	CAMERA_ID	GetCameraId() { return m_CurrentCameraID; }
	//カメラIDを設定
	void		SetCameraId(CAMERA_ID set) { m_CurrentCameraID = set; }
	//カメラの座標を取得
	VECTOR		GetCameraPos();
	//カメラの注視点を取得
	VECTOR		GetCameraForcus() { return playCamera.GetForcus(); }
	//カメラの向きを取得
	float		GetRot() { return playCamera.GetRot(); }
	//一人称視点フラグ
	bool		GetPlVisionFlag() { return playCamera.GetPlVisionFlag(); }
	//上下の向きを取得
	float		GetForcusF() { return playCamera.GetForcusF(); }
	//一人称カメラの設定
	void		SetPlVision(bool set) { playCamera.SetPlVision(set); }
	//プレイカメラの取得
	PlayCamera& GetPlayCamera() { return playCamera; }

private:
	//カメラの種類を変える
	void		ChangeCameraMode(float fRot);
};