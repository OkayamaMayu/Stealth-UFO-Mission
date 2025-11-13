#include"DxLib.h"
#include"PlayCamera.h"
#include"math.h"
#include"../MyLib/MyLib.h"

PlayCamera::PlayCamera()
{
	memset(&m_vPos, 0, sizeof(VECTOR));
	memset(&m_vCriteriaPos, 0, sizeof(VECTOR));
	memset(&m_vForcus, 0, sizeof(VECTOR));
	memset(&m_vUp, 0, sizeof(VECTOR));

	m_fRotAdd		= 0.0f;
	m_fRot			= 0.0f;
	m_fPlMoveTime	= 0.0f;
	m_fOffsetAdd	= 0.0f;
	m_fOffset		= 0.0f;
	m_fLemgthAdd	= 0.0f;
	m_fLemgth		= 0.0f;
	m_fCount		= 0.0f;
	m_PlVision		= false;
	m_fForcus		= 0.0f;
	m_fForcusAdd	= 0.0f;
	m_fLemgth_UFO	= 0.0f;
	m_fOffset_UFO	= 0.0f;
}

PlayCamera::~PlayCamera() {}

//初期化
void PlayCamera::Init()
{
	//変数の初期化
	m_vPos[MAIN]			= VGet(0.0f, 0.0f, 1.0f);
	m_vPos[SUB]				= VGet(0.0f, 0.0f, 1.0f);
	m_vCriteriaPos			= VGet(0.0f, 0.0f, 1.0f);
	m_vForcus[MAIN]			= VGet(0.0f, 0.0f, 0.0f);
	m_vForcus[SUB]			= VGet(0.0f, 0.0f, 0.0f);
	m_vUp					= VGet(0.0f, 1.0f, 0.0f);
	m_fRotAdd				= 0.0f;
	m_fRot					= 0.0f;
	m_fLemgth				= DEFAULT_LEMGTH;
	m_fLemgthAdd			= 0.0f;
	m_fOffset				= DEFAULT_OFFSET;
	m_fOffsetAdd			= 0.0f;
	m_fCount				= 0.0f;
	m_fPlMoveTime			= 0.0f;
	m_fForcus				= 0.0f;
	m_fForcusAdd			= 0.0f;
	m_PlVision				= false;
	m_HitStage				= false;
	m_UfoFoundFlag			= false;
	m_CameraMoveDirection	= false;
	m_fOffsetMain			= FORCUS_OFFSET_Y;
	m_fLemgth_UFO			= 0.0f;
	m_fOffset_UFO			= 0.0f;
	m_vMousePos[MAIN]		= Mouse::GetInstance()->GetMousePos();
	m_vMousePos[SUB]		= m_vMousePos[MAIN];
	m_fImageScale			= 1.0f;
}

void PlayCamera::Init(VECTOR vPos, VECTOR vUp,VECTOR plPos, float plRot, LoadStageData &data)
{
	//変数の初期化
	Init();

	//開始前演出のカメラ通過ポイントの数
	m_iCameraPointNum = data.GetCameraPointNum();

	//座標を設定
	m_vPos[MAIN]	= vPos;
	m_vPos[SUB]		= vPos;
	m_vCriteriaPos	= vPos;

	//視点を設定
	m_vForPos = data.GetCametaPoint(m_iCameraPointNum - 1);

	//移動先の座標
	VECTOR nextPos = {};
	if (m_iCameraPointNumNow + 1 >= m_iCameraPointNum)
	{
		//プレイヤーのスポーン位置を取得
		nextPos = data.GetPlayerSpawnPos();
	}
	else
	{
		//次のカメラの通過ポイントを取得
		nextPos = m_vStartPoint[m_iCameraPointNumNow + 1];
	}

	//進行方向を計算
	m_fStartStagingRot	= Math::GetAngle(nextPos, m_vForPos);
	//横にスクロールにする
	m_fStartStagingRot += Math::ChangeDegToRad(-90.0f);

	//視点を設定
	m_vForcus[MAIN]		= m_vForPos;
	m_vForcus[SUB]		= m_vForPos;

	//アップベクトルを設定
	m_vUp				= vUp;
	//回転を設定
	m_fRot				= plRot;

	//通過ポイント
	for (int i = 0; i < m_iCameraPointNum; i++)
	{
		//通過ポイントを取得
		m_vStartPoint[i] = data.GetCametaPoint(i);
	}
	m_iCameraPointNumNow = 0;

	//マウスをリセット
	ResetMouse();

	//リスポーン状態なら
	if (Data::GetReSpawnFlag())
	{
		//すぐに始められるようにする
		m_fStartStagingRot = m_fRot;
		m_fOffset = 0.0f;
		m_fLemgth = 0.0f;

		//マウスをリセット
		Mouse::GetInstance()->Reset();

		//視点を設定
		m_vForPos = plPos;
	}
}

void PlayCamera::Load()
{
	//演出用上下の黒画像読み込み
	m_iHandle = LoadGraph(IMAGE_PATH.c_str());
}

//ニア、ファーをセット
void PlayCamera::SetNearFar(float fNear, float fFar)
{
	SetCameraNearFar(fNear, fFar);
}

//通常処理：プレイヤーの座標, プレイヤーの回転角度
void PlayCamera::Step(VECTOR vForcus, float fRot, int plState, UFO ufo)
{
	//画像サイズを拡大していく
	Math::MatchSpecifiedNum(m_fImageScale, IMAGE_MAX_SCALE, IMAGE_SCALE_SPEED);

	//高さをカメラの状態から分ける
	if (m_PlVision)
	{
		m_fOffsetMain = FORCUS_OFFSET_PL;
	}
	else
	{
		m_fOffsetMain = FORCUS_OFFSET_Y;
	}

	//フラグ関係の取得
	m_UfoFlag = ufo.GetIsUse();
	m_UfoFoundFlag = ufo.GetPLFound();

	//マウスの座標を取得
	m_vMousePos[MAIN] = Mouse::GetInstance()->GetMousePos();

	//ステージ落下状態になっていなかったら
	if (m_vPos[MAIN].y >= DEATH_Y / 2)
	{
		//カメラを回転-----------------
		CameraTurn();

		//カメラズーム-----------------
		CameraZoom();
	}

	/*================================
		メイン視点(動かせる視点)
	==================================*/
	CameraMove(MAIN, vForcus, m_fRot, m_fLemgth, m_fOffset);
	m_vPos[MAIN]		= m_vPos[MAIN];

	/*============================
		サブ視点(常にプレイヤーを追尾)
	==============================*/
	CameraMove(SUB, vForcus, fRot);
	m_vPos[SUB]			= m_vPos[SUB];

	/*===================
		注視点をもとめる
	=====================*/
	m_vForcus[SUB]		= vForcus;
	m_vForcus[SUB].y   += m_fOffsetMain + m_fForcus;
	
	//プレイヤーを追いかける================================================
	VECTOR vForcusMain	= m_vForcus[MAIN];
	vForcusMain.y = 0.0f;
	VECTOR vForcusSub	= m_vForcus[SUB];
	vForcusSub.y = 0.0f;

	if(!m_PlVision)
	{
		//離れている距離から移動量を計算
		float speed = Math::GetDistance(vForcusMain, vForcusSub) * CAMERA_SPEED_SCALE;
		if (Math::GetDistance(vForcusMain, vForcusSub) > speed)
		{
			//移動予定量より離れていたら移動
			VECTOR vSpeed = Math::GetMoving(vForcusMain, vForcusSub, speed);
			vForcusMain = VAdd(vForcusMain, vSpeed);
		}
		else
		{
			//移動予定量より離れていなかったら固定する
			vForcusMain = vForcusSub;
		}
	}
	
	//適応
	m_vForcus[MAIN]		= m_vForcus[SUB];
	if (!m_PlVision)
	{
		//視点座標を適応
		m_vForcus[MAIN].x = vForcusMain.x;
		m_vForcus[MAIN].z = vForcusMain.z;
	}

	//-------------------------------------------

	//プレイヤーが移動中ならカメラを追尾
	if (plState == PLAYER_STATL_MOVE || plState == PLAYER_STATL_RUN)
	{
		//移動開始から1.5秒経った
		if (Math::MatchSpecifiedNum(m_fPlMoveTime, MOVE_CAMERA_FIX_COUNT, FRAME_TIME) && !m_PlVision)
		{
			if (m_fRot < fRot + TRACKING_REMAINDER && m_fRot > fRot - TRACKING_REMAINDER)
			{
				m_fRot		= fRot;
			}
			else
			{
				//プレイヤーの背後に配置
				VECTOR rot	= VGet(0.0f, m_fRot, 0.0f);
				rot			= Math::GetMovingRot(m_vPos[SUB], rot, m_vPos[MAIN], ROT_SPEED);
				m_fRot		= rot.y;
			}
		}
	}
	else
	{
		m_fPlMoveTime = 0.0f;
	}

	Update();

	//ステージ落下状態なら一人称カメラを病める
	if (m_vPos[MAIN].y <= DEATH_Y / 2)
	{
		m_PlVision = false;
		m_vPos[MAIN].y = DEATH_Y / 2;
	}

	DebugString::GetInstance()->AddFormatString(0, 200, "1：%f, %f", m_vMousePos[0].x, m_vMousePos[0].y);
	DebugString::GetInstance()->AddFormatString(0, 220, "2：%f, %f", m_vMousePos[1].x, m_vMousePos[1].y);

	//マウスをリセット
	ResetMouse();
}
//クリア演出用
void PlayCamera::Step(VECTOR vForcus,float fRot)
{
	//カメラの状態から高さを分ける
	if (m_PlVision)
	{
		m_fOffsetMain = FORCUS_OFFSET_PL;
	}
	else
	{
		m_fOffsetMain = FORCUS_OFFSET_Y;
	}

	m_fLemgth = 0;
	m_fOffset = 0;

	/*============================
		サブ視点(常にプレイヤーを追尾)
	==============================*/
	CameraMove(SUB, vForcus, fRot, m_fLemgth, m_fOffset);

	/*===================
		注視点をもとめる
	=====================*/
	m_vForcus[MAIN] = vForcus;
	m_vForcus[MAIN].y += m_fForcus;

	m_vPos[MAIN].x = m_vPos[SUB].x;
	m_vPos[MAIN].z = m_vPos[SUB].z;

	Update();
}
//ゲームオーガー演出用
void PlayCamera::Step(VECTOR plPos, VECTOR vForcus, float fRot)
{
	if (m_PlVision)
	{
		m_fOffsetMain = FORCUS_OFFSET_PL;
	}
	else
	{
		m_fOffsetMain = FORCUS_OFFSET_Y;
	}

	/*============================
		サブ視点(常にプレイヤーを追尾)
	==============================*/
	CameraMove(SUB, plPos, fRot);

	m_vPos[SUB].y += -CAMERA_OFFSET_Y - m_fOffset_UFO;

	/*===================
		注視点をもとめる
	=====================*/
	m_vForcus[MAIN] = vForcus;

	m_vPos[MAIN]	= m_vPos[SUB];

	Update();
}

bool PlayCamera::StartStep(VECTOR plPos, VECTOR goalPos)
{
	ResetMouse();

	VECTOR nextPos = {};
	if (m_iCameraPointNumNow + 1 >= m_iCameraPointNum)
	{
		nextPos = plPos;
	}
	else
	{
		nextPos = m_vStartPoint[m_iCameraPointNumNow+1];
	}

	//次のポイントに到着していなかったら移動する
	if (Math::GetDistance(m_vForPos, nextPos) > START_MOVE_DISTANCE)
	{
		VECTOR sp = Math::GetMoving(m_vForPos, nextPos, 1.0f);
		m_vForPos = VAdd(m_vForPos, sp);
	}
	else
	{
		m_vForPos	= nextPos;
	}
	CameraMove(SUB, m_vForPos, m_fStartStagingRot, m_fLemgth, m_fOffset);

	m_vPos[MAIN]	= m_vPos[SUB];
	m_vPos[MAIN].y += m_fOffset;

	VECTOR pos = m_vForPos;
	pos.y += FORCUS_OFFSET_Y;
	m_vForcus[MAIN] = m_vForcus[SUB] = pos;

	Update();

	if (Math::GetDistance(m_vForPos, nextPos) <= 1.0f)
	{
		m_iCameraPointNumNow++;
		if(m_iCameraPointNumNow > m_iCameraPointNum)
		{
			//角度の変更
			//値を合わせる
			Math::MatchSpecifiedNum(m_fStartStagingRot, m_fRot, FORCUS_RESET_SPEED);

			//速度を得る
			float speed = Math::GetDistance(m_vForPos, m_vPos[MAIN]);
			speed *= START_MOVE_SCALE;

			//変数をスタートに合わせる
			Math::MatchSpecifiedNum(m_fOffset, 0.0f, speed);
			Math::MatchSpecifiedNum(m_fLemgth, 0.0f, speed);

			if (m_fStartStagingRot == m_fRot && m_fOffset == 0.0f && m_fLemgth == 0.0f)
			{
				//最終地点に到着したら
				if (Math::MatchSpecifiedNum(m_fImageScale, IMAGE_MAX_SCALE, IMAGE_SCALE_SPEED))
				{
					//進行
					Mouse::GetInstance()->Reset();
					return true;
				}
			}
		}
	}

	//強制スタート
	if (InputManager::CheckPush(INPUT_DETERMINATION))
	{
		m_fStartStagingRot	= m_fRot;
		m_fOffset			= 0.0f;
		m_fLemgth			= 0.0f;
		m_fImageScale		= IMAGE_MAX_SCALE;

		Fade::GetInstance()->RequestFlash(Fade::BLACK);
		
		Mouse::GetInstance()->Reset();
		return true;
	}
	return false;
}

//更新したデータを反映させる
void PlayCamera::Update()
{
	//カメラ設定
	SetCameraPositionAndTargetAndUpVec(m_vPos[MAIN], m_vForcus[MAIN], m_vUp);
}

void PlayCamera::Draw()
{
	DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, m_fImageScale, 0.0f, m_iHandle, true);
}

//====================================================================================================

//カメラの行列計算
void PlayCamera::CameraMove(MAIN_AND_SUB ID, VECTOR vForcus, float fRot, float fLemgth, float fOffset)
{
	//カメラの座標をもとめる
	VECTOR cameraPos = vForcus;
	cameraPos.z		+= CAMERA_LEMGTH+ fLemgth+ m_fLemgth_UFO;
	cameraPos.y		+= CAMERA_OFFSET_Y+ fOffset+ m_fOffset_UFO;

	MATRIX pos1		= MGetTranslate(VScale(vForcus, -1.0f));	//プレイヤーを元に原点 に 戻す
	MATRIX pos2		= MGetTranslate(vForcus);					//プレイヤーを元に原点 から 戻す
	MATRIX rot		= MGetRotY(fRot);							//回転

	//行列の合成
	MATRIX matrix;
	matrix			= MMult(pos1, rot);
	matrix			= MMult(matrix, pos2);

	//座標の変換
	m_vPos[ID]		= VTransform(cameraPos, matrix);
}

//カメラの回転
void PlayCamera::CameraTurn()
{
	//二人称
		VECTOR stickRight = {};
	if (!m_PlVision)
	{
		if (InputManager::CheckCameraMove(stickRight))
		{
			//移動量を計算
			Math::MatchSpecifiedNum(m_fRotAdd, CAMERA_MOVE_SPEED * fabs(stickRight.x), CAMERA_MOVE_SPEED_ADD);

			//左右のどちらに回転するか判定
			if (stickRight.x < ROT_STICK_ACCEPTABLE)
			{
				m_CameraMoveDirection = false;
			}
			else if (stickRight.x > ROT_STICK_ACCEPTABLE)
			{
				m_CameraMoveDirection = true;
			}
		}

		//マウス
		bool cameraMouseMoveFlag = false;
		if (Math::GetDistance(m_vMousePos[MAIN], m_vMousePos[SUB]) > MOUSE_MOVE)
		{
			float cameraSp = m_vMousePos[SUB].x - m_vMousePos[MAIN].x;
			m_fRotAdd = cameraSp * MOUSE_MOVE_SCALE;
			m_fRotAdd *= -1.0f;

			cameraMouseMoveFlag = true;
		}

		//カメラをゆっくり止める
		if (!InputManager::CheckCameraMove(stickRight) &&
			!cameraMouseMoveFlag)
		{
			//減速
			Math::MatchSpecifiedNum(m_fRotAdd, 0.0f, CAMERA_MOVE_SPEED_ADD * CAMERA_STOP_SPEED_SCALE);
		}
		else
		{
			//カメラを移動すると自動追尾をリセット
			m_fPlMoveTime = 0.0f;
		}
	}
	//一人称
	else
	{
		VECTOR stickRight = {};
		if (InputManager::CheckCameraMove(stickRight))
		{
			//加速
			Math::MatchSpecifiedNum(m_fRotAdd, CAMERA_MOVE_SPEED / FPS_CAMERA_MOVE_SCALE * fabs(stickRight.x), CAMERA_MOVE_SPEED_ADD * FPS_CAMERA_SPEED_SCALE);

			//左右のどちらに回転するか判定
			if (stickRight.x < ROT_STICK_ACCEPTABLE)
			{
				m_CameraMoveDirection = false;
			}
			else if (stickRight.x > ROT_STICK_ACCEPTABLE)
			{
				m_CameraMoveDirection = true;
			}
		}

		//マウス
		bool cameraMouseMoveFlag = false;
		if (Math::GetDistance(m_vMousePos[MAIN], m_vMousePos[SUB]) > MOUSE_MOVE_PL)
		{
			//前フレームとの差を計算
			float cameraSp = m_vMousePos[SUB].x - m_vMousePos[MAIN].x;
			//移動量を計算
			m_fRotAdd = cameraSp * MOUSE_MOVE_SCALE_PL;
			m_fRotAdd *= -1.0f;

			cameraMouseMoveFlag = true;
		}

		//カメラをゆっくり止める
		if (!InputManager::CheckCameraMove(stickRight) &&
			!cameraMouseMoveFlag)
		{
			Math::MatchSpecifiedNum(m_fRotAdd, 0.0f, CAMERA_MOVE_SPEED_ADD * FPS_CAMERA_SPEED_SCALE);
		}
		else
		{
			//カメラを移動すると自動追尾をリセット
			m_fPlMoveTime = 0.0f;
		}
	}

	//回転させる
	if (!m_CameraMoveDirection)
	{
		m_fRot += m_fRotAdd;
	}
	else
	{
		m_fRot -= m_fRotAdd;
	}

	//回転が一周(180度)をこえると変数をリセット
	Math::RotReset(m_fRot);
}

//カメラ切り替え
void PlayCamera::OpenPlVision()
{
	//フラグをON
	m_PlVision = true;
	m_fCount = 0.0f;

	//カメラをリセット
	ResetMouse();

	//SEを再生
	Sound::Play(SE_PLAYER_VIEW_CHANGE);
}
void PlayCamera::ClosePlVision()
{
	//フラグを折る
	m_PlVision = false;
	//時間を固定
	m_fCount = CAMERA_FIX_COUNT;

	//カメラをリセット
	ResetMouse();

	//SEを再生
	Sound::Play(SE_PLAYER_VIEW_CHANGE);
}

//カメラのズーム
void PlayCamera::CameraZoom()
{
	//一人称視点中じゃなかったら
	if (!m_PlVision)
	{
		//一人称になる
		if (InputManager::CheckPush(INPUT_ITEM_MODE)&&
			!InputManager::CheckKeep(INPUT_BLOCK_MODE)&&
			!m_UfoFoundFlag)
		{
			//開いていなかったら一人称にする
			OpenPlVision();
		}

		//UFOが出現していたら
		if (m_UfoFlag)
		{
			//UFOに捕まっていない状態
			if(!m_UfoFoundFlag)
			{
				Math::MatchSpecifiedNum(m_fOffset_UFO, CAMERA_OFFSET_UFO_Y, UFO_CAMERA_ZOOM_SPEED);
				Math::MatchSpecifiedNum(m_fLemgth_UFO, CAMERA_LEMGTH_UFO, UFO_CAMERA_ZOOM_SPEED);
			}
			else
			{
				Math::MatchSpecifiedNum(m_fOffset_UFO, 0.0f, UFO_CATCH_ZOOM_SPEED);
				Math::MatchSpecifiedNum(m_fLemgth_UFO, 0.0f, UFO_CATCH_ZOOM_SPEED);
			}
		}
		else
		{
			Math::MatchSpecifiedNum(m_fOffset_UFO, 0.0f, UFO_CAMERA_ZOOM_SPEED);
			Math::MatchSpecifiedNum(m_fLemgth_UFO, 0.0f, UFO_CAMERA_ZOOM_SPEED);
		}
	}

	//一人称視点中
	if (m_PlVision)
	{
		Math::MatchSpecifiedNum(m_fOffset_UFO, 0.0f, UFO_FPS_OFFSET_SPEED);
		Math::MatchSpecifiedNum(m_fLemgth_UFO, 0.0f, UFO_FPS_LEMGTH_SPEED);

		//自動======================================================================
		//カメラ移動速度を設定
		Math::MatchSpecifiedNum(m_fLemgthAdd, -CAMERA_ZOOM_SPEED * FPS_CAMERA_ZOOM_SCALE, (CAMERA_ZOOM_SPEED * FPS_CAMERA_ZOOM_SCALE - m_fLemgthAdd)* FPS_CAMERA_ZOOM_SPEED);
		Math::MatchSpecifiedNum(m_fOffsetAdd, -CAMERA_ZOOM_SPEED * FPS_CAMERA_ZOOM_SCALE, (CAMERA_ZOOM_SPEED * FPS_CAMERA_ZOOM_SCALE - m_fLemgthAdd)* FPS_CAMERA_ZOOM_SPEED);

		//ズーム
		m_fLemgth += m_fLemgthAdd;
		m_fOffset += m_fOffsetAdd;

		//固定
		if (m_fLemgth <= -CAMERA_LEMGTH_PL)
		{
			m_fLemgth = -CAMERA_LEMGTH_PL;
		}

		if (m_fOffset <= -CAMERA_OFFSET_PL)
		{
			m_fOffset = -CAMERA_OFFSET_PL;
		}

		//操作======================================================================

		VECTOR stickRight = {};
		if (InputManager::CheckCameraUpDown(stickRight))
		{
			Math::MatchSpecifiedNum(m_fForcusAdd, 0.01f * stickRight.y, CAMERA_MOVE_SPEED_ADD * FPS_CAMERA_SPEED_SCALE);
		}

		//マウス
		bool cameraMouseMoveFlag = false;
		if (Math::GetDistance(m_vMousePos[MAIN], m_vMousePos[SUB]) > MOUSE_MOVE_PL)
		{
			//差を求める
			float cameraSp = m_vMousePos[SUB].y - m_vMousePos[MAIN].y;
			m_fForcusAdd = cameraSp * MOUSE_MOVE_SCALE_PL;

			cameraMouseMoveFlag = true;
		}

		//カメラをゆっくり止める
		if (!InputManager::CheckCameraUpDown(stickRight)&&
			!cameraMouseMoveFlag)
		{
			//移動速度を戻す
			Math::MatchSpecifiedNum(m_fForcusAdd, 0.0f, CAMERA_MOVE_SPEED_ADD * FPS_CAMERA_SPEED_SCALE);
		}

		//変更
		m_fForcus += m_fForcusAdd;

		if (m_fForcus <= -FPS_CAMERA_MOVE_SCALE)
		{
			m_fForcus = -FPS_CAMERA_MOVE_SCALE;
		}
		else if (m_fForcus >= FPS_CAMERA_ZOOM_SPEED)
		{
			//固定
			m_fForcus = FPS_CAMERA_ZOOM_SPEED;
		}

		//解除======================================================================
		//開始時と分ける
		if (m_fCount > FPS_CAMERA_CLOSE_TIME)
		{
			if ((InputManager::CheckPush(INPUT_ITEM_MODE) || InputManager::CheckRelease(INPUT_ITEM_MODE)) &&
				!InputManager::CheckKeep(INPUT_BLOCK_MODE))
			{
				//開いていたら一人称カメラを閉じる
				if(m_PlVision)
					ClosePlVision();
			}
		}
		m_fCount += FRAME_TIME;	//固定時間を加算
		Math::MatchSpecifiedNum(m_fCount, CAMERA_FIX_COUNT, FRAME_TIME);
	}

	//一人称視点中じゃなかったら
	if (!m_PlVision&& !m_HitStage)
	{
		//視点の上下を戻す
		Math::MatchSpecifiedNum(m_fForcus, 0.0f, FORCUS_RESET_SPEED);
		
		//カメラズーム操作のコントローラーのスティック
		VECTOR stickRight = {};
		if (InputManager::CheckCameraZoom(stickRight))
		{
			Math::MatchSpecifiedNum(m_fLemgthAdd, CAMERA_ZOOM_SPEED * stickRight.y, CAMERA_ZOOM_SPEED_ADD);
			Math::MatchSpecifiedNum(m_fOffsetAdd, CAMERA_ZOOM_SPEED * stickRight.y, CAMERA_ZOOM_SPEED_ADD);
		}

		//マウス
		if (Input::Mouse::Wheel(MOUSE_WHEEL_BACK))	//ズーム
		{
			Math::MatchSpecifiedNum(m_fLemgthAdd, -CAMERA_ZOOM_SPEED * (FPS_ZOOM_SCALE / 2), CAMERA_ZOOM_SPEED_ADD* FPS_ZOOM_SCALE);
			Math::MatchSpecifiedNum(m_fOffsetAdd, -CAMERA_ZOOM_SPEED * (FPS_ZOOM_SCALE / 2), CAMERA_ZOOM_SPEED_ADD* FPS_ZOOM_SCALE);
		}
		if (Input::Mouse::Wheel(MOUSE_WHEEL_FRONT))	//アウト
		{
			Math::MatchSpecifiedNum(m_fLemgthAdd, CAMERA_ZOOM_SPEED* (FPS_ZOOM_SCALE / 2), CAMERA_ZOOM_SPEED_ADD* FPS_ZOOM_SCALE);
			Math::MatchSpecifiedNum(m_fOffsetAdd, CAMERA_ZOOM_SPEED* (FPS_ZOOM_SCALE / 2), CAMERA_ZOOM_SPEED_ADD* FPS_ZOOM_SCALE);
		}

		//カメラをゆっくり止める
		if (!InputManager::CheckCameraZoom(stickRight) &&
			!Input::Mouse::Wheel(MOUSE_WHEEL_BACK) && !Input::Mouse::Wheel(MOUSE_WHEEL_FRONT))
		{
			//固定時間を過ぎたら
			if (Math::MatchSpecifiedNum(m_fCount, CAMERA_FIX_COUNT, FRAME_TIME))
			{
				//拡大率を戻す
				Math::MatchSpecifiedNum(m_fLemgth, 0.0f, FPS_CAMERA_ZOOM_SPEED);
				Math::MatchSpecifiedNum(m_fOffset, 0.0f, FPS_CAMERA_ZOOM_SPEED);
			}

			//移動速度を戻す
			Math::MatchSpecifiedNum(m_fLemgthAdd, 0.0f, CAMERA_ZOOM_SPEED_ADD);
			Math::MatchSpecifiedNum(m_fOffsetAdd, 0.0f, CAMERA_ZOOM_SPEED_ADD);
		}
		else
		{
			m_fCount = 0.0f;
		}

		//増加量の加算
		m_fLemgth += m_fLemgthAdd;
		m_fOffset += m_fOffsetAdd;

		if(!m_HitStage)
		{
			//距離の最大値最小値の固定
			if (m_fLemgth <= -CAMERA_LEMGTH / 2)
			{
				m_fLemgth = -CAMERA_LEMGTH / 2;
			}
			else if (m_fLemgth >= CAMERA_LEMGTH / 2)
			{
				m_fLemgth = +CAMERA_LEMGTH / 2;
			}

			//高さの最大値最小値の固定
			if (m_fOffset <= OFFSET_MIN_FIXED_VALUE)
			{
				m_fOffset = OFFSET_MIN_FIXED_VALUE;
			}
			else if (m_fOffset >= OFFSET_MAX_FIXED_VALUE)
			{
				m_fOffset = OFFSET_MAX_FIXED_VALUE;
			}
		}
	}
}

void PlayCamera::ResetMouse(int type)
{
	if(type==0)
	{
		//マウスカーソルを画面中央へ
		SetMousePoint(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2);
	}

	m_vMousePos[MAIN] = Mouse::GetInstance()->GetMousePos();
	m_vMousePos[SUB] = m_vMousePos[MAIN];
}