#pragma once
#include"DxLib.h"
#include"../enemy/UFO.h"
#include"../LoadStageData/LoadStageData.h"

const float CAMERA_LEMGTH				= 40.0f;						//通常カメラの距離
const float CAMERA_LEMGTH_UFO			= 40.0f;						//UFO出現時カメラの距離
const float CAMERA_LEMGTH_PL			= CAMERA_LEMGTH - 0.5f;			//一人称カメラの距離
const float CAMERA_OFFSET_Y				= 25.0f;						//通常カメラの高さ
const float CAMERA_OFFSET_UFO_Y			= 40.0f;						//UFO出現時カメラの高さ
const float CAMERA_OFFSET_PL			= CAMERA_OFFSET_Y - 15.0f;		//一人称カメラの高さ
const float FORCUS_OFFSET_Y				= 13.0f;						//通常カメラの注視点の高さ
const float FORCUS_OFFSET_PL			= 15.0f;						//一人称カメラの注視点の高さ
const float CAMERA_MOVE_SPEED			= 0.04f;						//カメラの回転スピードの最大
const float CAMERA_MOVE_SPEED_ADD		= 0.001f;						//回転スピード増加量
const float CAMERA_ZOOM_SPEED			= 0.5f;							//ズームスピードの速度
const float CAMERA_ZOOM_SPEED_ADD		= 0.2f;							//ズームスピードの増加量
const float CAMERA_FIX_COUNT			= 10.0f;						//カメラの固定時間
const float MOVE_CAMERA_FIX_COUNT		= 1.5f;							//プレイヤー移動時の自動追尾開始時間
const float MOUSE_MOVE					= 0.0f;							//これ以上動かすと視点が動く(一人称)
const float MOUSE_MOVE_SCALE			= 0.0025f;						//カメラ移動倍率(一人称)
const float MOUSE_MOVE_PL				= 2.0f;							//これ以上動かすと視点が動く(二人称)
const float MOUSE_MOVE_SCALE_PL			= 0.001f;						//カメラ移動倍率(二人称)
const int	CAMERA_START_POINT_NUM		= 10;							//開始時演出のカメラ通過ポイントの数

class PlayCamera
{
public:
	//変数の二個セット
	enum MAIN_AND_SUB
	{
		MAIN,	//メイン使用
		SUB,	//バックアップ、サブ

		MAB_SET	//変数の数
	};

	const string	IMAGE_PATH				=  "data/system/black.png" ;	//演出用上下の黒画像パス
	const float		DEFAULT_LEMGTH			= 20.0f;						//距離の初期値
	const float		DEFAULT_OFFSET			= 150.0f;						//高さの初期値
	const float		CAMERA_SPEED_SCALE		= 0.09f;						//カメラの移動倍率
	const float		TRACKING_REMAINDER		= 0.02f;						//カメラの回転の余り幅
	const float		ROT_SPEED				= 0.015f;						//回転スピード
	const float		START_MOVE_DISTANCE		= 1.1f;							//開始前の演出のカメラ移動ポイントの許容範囲
	const float		START_MOVE_SCALE		= 0.005f;						//演出用移動速度の倍率
	const float		ROT_STICK_ACCEPTABLE	= 0.3f;							//回転時のスティックの許容範囲
	const float		IMAGE_MAX_SCALE			= 1.5f;							//イメージの最大サイズ
	const float		IMAGE_SCALE_SPEED		= 0.01f;						//画像の拡大スピード
	const float		FPS_CAMERA_MOVE_SCALE	= 2.5f;							//一人称時の
	const float		UFO_CAMERA_ZOOM_SPEED	= 0.5f;							//UFO出現状態のカメラ位置移動速度
	const float		UFO_CATCH_ZOOM_SPEED	= 0.05f;						//UFOに捕まっている時のカメラズームスピード
	const float		UFO_FPS_OFFSET_SPEED	= 2.0f;							//UFO出現時の一人称カメラのズーム速度
	const float		UFO_FPS_LEMGTH_SPEED	= 1.0f;							//UFO出現時の一人称カメラの距離変更速度
	const float		FPS_CAMERA_ZOOM_SCALE	= 3.0f;							//一人称時のカメラの倍率
	const float		FPS_CAMERA_ZOOM_SPEED	= 0.3f;							//一人称時のカメラのズーム速度
	const float		FPS_CAMERA_CLOSE_TIME	= 0.5f;							//一人称カメラを閉じれるようになる時間
	const float		FPS_ZOOM_SCALE			= 5.0f;							//ズームズームアウトの速度倍率
	const float		FORCUS_RESET_SPEED		= 0.01f;						//カメラの上下のリセット速度
	const float		OFFSET_MIN_FIXED_VALUE	= -5.0f;						//高さの最小固定値
	const float		OFFSET_MAX_FIXED_VALUE	= 13.0f;						//高さの最大固定値
	const int		CAMERA_STOP_SPEED_SCALE = 4;							//カメラ回転停止のスケール
	const int		FPS_CAMERA_SPEED_SCALE	= 5;							//一人称時のカメラ速度の倍率

private:
	VECTOR			m_vPos[MAB_SET];										//座標
	VECTOR			m_vCriteriaPos;											//視点目安
	VECTOR			m_vForcus[MAB_SET];										//注視点
	VECTOR			m_vUp;													//上方向
	VECTOR			m_vMousePos[MAB_SET];									//マウスの座標(0が現在, 1が過去)
	VECTOR			m_vStartPoint[CAMERA_START_POINT_NUM];					//スタート時のカメラ通過ポイント
	VECTOR			m_vForPos;												//開始前の視点
	float			m_fRotAdd;												//回転率加算
	float			m_fRot;													//回転角度
	float			m_fStartStagingRot;										//ゲーム開始時の演出用角度
	float			m_fLemgth;												//通常カメラの距離調節用
	float			m_fLemgth_UFO;											//UFO出現時カメラの距離調節用
	float			m_fLemgthAdd;											//距離調節用加算
	float			m_fOffsetMain;											//高さのメイン（ベースの視点の高さ）
	float			m_fOffset;												//通常カメラの高さ調節用
	float			m_fOffset_UFO;											//UFO出現時カメラの高さ調節用
	float			m_fOffsetAdd;											//高さ調節用加算
	float			m_fForcus;												//視点の上下
	float			m_fForcusAdd;											//視点の上下加算
	float			m_fCount;												//カメラの固定時間
	float			m_fPlMoveTime;											//プレイヤーの移動時間
	float			m_fImageScale;											//画像サイズ
	bool			m_PlVision;												//一人称視点フラグ
	bool			m_HitStage;												//ステージに当たってる判定
	bool			m_UfoFlag;												//UFOが出てるフラグ
	bool			m_UfoFoundFlag;											//プレイヤーを吸収中
	bool			m_CameraMoveDirection;									//カメラ回転の左右フラグ
	int				m_iHandle;												//画像ハンドル
	int				m_iCameraPointNum;										//カメラ通過ポイントの数
	int				m_iCameraPointNumNow;									//今の通過ポイントの場所

public:
	PlayCamera();
	~PlayCamera();

	//初期化
	void	Init();
	//初期化：視点, 注視点, 上方向,
	void	Init(VECTOR vPos, VECTOR vUp, VECTOR plPos, float plRot, LoadStageData &data);
	//画像の読み込み
	void	Load();
	//ニア、ファーをセット
	void	SetNearFar(float fNear, float fFar);
	//通常処理：プレイヤーの座標, プレイヤーの回転角度
	void	Step(VECTOR vForcus, float fRot,int plState, UFO ufo);
	//ゴール演出用
	void	Step(VECTOR vForcus,float fRot);
	//ゲームオーガー演出用
	void	Step(VECTOR plPos, VECTOR vForcus, float fRot);
	//描画
	void	Draw();
	//開始時の演出
	bool	StartStep(VECTOR plPos, VECTOR goalPos);
	//更新したデータを反映させる
	void	Update();
	//一人称視点フラグ
	bool	GetPlVisionFlag() { return m_PlVision; }
	//一人称カメラにする
	void	OpenPlVision();
	//一人称カメラをやめる
	void	ClosePlVision();
	//向きの取得
	float	GetRot() { return m_fRot; }
	//一人称カメラの設定
	void	SetPlVision(bool set) { m_PlVision = set; }
	//UFOが出現しているかフラグ
	bool	GetUfoFlag() { return m_UfoFlag; }
	//マウスをリセット
	void	ResetMouse(int type = 0);
	//座標を取得
	VECTOR	GetPos() { return m_vPos[MAIN]; }
	//注視点を取得
	VECTOR	GetForcus() { return m_vForcus[SUB]; }
	//視点の上下を取得
	float	GetForcusF() { return m_fForcus; };

private:
	//カメラの行列計算
	void	CameraMove(MAIN_AND_SUB ID, VECTOR vForcus, float fRot, float fLemgth = 0.0f, float fOffset = 0.0f);
	//カメラの回転
	void	CameraTurn();
	//カメラのズーム
	void	CameraZoom();
};