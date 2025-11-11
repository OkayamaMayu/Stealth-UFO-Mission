#include"UFO.h"
#include"../MyLib/MyLib.h"

void UFO::Init()
{
	CModel::Init();
	m_IsUse			= false;
	m_PLFound		= false;
	m_State			= STATE_NEUTRAL;
	m_fScale		= MODEL_SCALE;
	m_fUpPos		= 0.0f;
	m_vDrawPos		= m_vPos;
	m_MoveState		= MOVE_STAY;
	m_HitCameraFlag = false;
	m_CatchPlayer	= false;
	m_fAlpha			= 1.0f;
	m_fSpc			= 1.0f;
	m_iQTECount		= 0;
	m_iButtonCount	= 0;
	m_fMoveCount	= 0.0f;
	memset(&m_vKeepLStick, 0, sizeof(VECTOR));
}

void UFO::Load()
{
	LoadModel(UFO_MODEL_PATH.c_str());
}

void UFO::Start()
{
	//アニメの再生
	RequestLoop(0, ANIME_SPEED);
	//スケールの設定
	m_vScale = Vector::MakeVec(UFO_SCALE);
	//一回更新
	Updata();
}

void UFO::Step(Player& pl)
{
	if (!m_IsUse)
	{
		//出現していなかったらサウンドを停止して終了
		Sound::StopSound(SE_LOOP_UFO);
		Sound::StopSound(SE_LOOP_UFO_CATCH);
		return;
	}
	else
	{
		//出現中サウンドの再生
		Sound::Play(SE_LOOP_UFO);
	}

	switch (m_State)
	{
	case STATE_NEUTRAL:
		//何もしない
		break;

	case STATE_IN:			//登場
		//場に出す
		Math::MatchSpecifiedNum(m_fScale, UFO_SCALE, INOUT_SIZE_SPEED);
		Math::MatchSpecifiedNum(m_fUpPos, 0.0f, UFO_INOUT_SPEED);
		Math::MatchSpecifiedNum(m_fSpc, 0.0f, INOUT_SPC_SPEED);

		//指定の値になったら進行
		if (m_fScale == UFO_SCALE && m_fUpPos == 0.0f)
		{
			m_State = STATE_MOVE;
		}
		
		break;

	case STATE_MOVE:		//移動
		//サーチ判定の設定
		Math::MatchSpecifiedNum(m_fConeBottom, UFO_HEIGHT);
		Math::MatchSpecifiedNum(m_fConeRadius, UFO_RADIUS, SEARCH_RADIUS_SPEED);

		//行動
		switch (m_MoveState)
		{
		case MOVE_STAY:		//待機行動
			Stay(pl.GetPos(), pl.GetSafetyAreaInFlag());
			break;
		case MOVE_CHASE:	//移動処理
			Chase();
			break;
		default:
			break;
		}

		//探索
		Search(pl);

		//注目度が一定に下がったら退散
		if (Data::GetFoundGauge() <= PULLOUT_RANGE)
		{
			//退散に変更
			m_State = STATE_OUT;
		}

		break;

	case STATE_DISCOVERY:	//プレイヤーを捕獲
		//ゲームオーバーに移行するように
		QuickTimerEvent(pl);

		break;

	case STATE_OUT:			//退場
		Math::MatchSpecifiedNum(m_fConeBottom, 0.0f, SEARCH_RADIUS_SPEED);
		Math::MatchSpecifiedNum(m_fConeRadius, 0.0f);

		//場を去る
		Math::MatchSpecifiedNum(m_fScale, 0.0f, INOUT_SIZE_SPEED);
		Math::MatchSpecifiedNum(m_fUpPos, PULLOUT_HEIGHT, UFO_INOUT_SPEED / 2);
		Math::MatchSpecifiedNum(m_fSpc, 1.0f, INOUT_SPC_SPEED);

		//指定の値になったら進行
		if (m_fScale == 0.0f && m_fUpPos == PULLOUT_HEIGHT && m_fSpc == 1.0f)
		{
			m_State = STATE_NEUTRAL;
			m_IsUse = false;

			//サウンドを再生
			Sound::Play(SE_UFO_SPAWN);
		}
		break;

	default:
		break;
	}

	//描画用の高さ
	m_vDrawPos		= m_vPos;
	m_vDrawPos.y	+= m_fUpPos;

	//高さを適応
	m_vScale		= Vector::MakeVec(m_fScale);

	//半透明にする処理
	if (!m_HitCameraFlag)
	{
		Math::MatchSpecifiedNum(m_fAlpha, 1.0f, ALPHA_SPEED);
	}
	else
	{
		Math::MatchSpecifiedNum(m_fAlpha, ALPHA_MIN, ALPHA_SPEED);
	}
	//透明度の適応
	MV1SetOpacityRate(m_iHandle, m_fAlpha);

	Updata();
	UpdateAnim();

	//標準ライトのスペキュラカラーを変更
	SetLightSpcColor(GetColorF(1.0f, m_fSpc, 0.0f, 0.0f));

	//サウンド
	if (m_State == STATE_DISCOVERY && !m_CatchPlayer)
	{
		Sound::Play(SE_LOOP_UFO_CATCH);
	}
	else
	{
		Sound::StopSound(SE_LOOP_UFO_CATCH);
	}
}

void UFO::Draw()
{
	if (m_IsUse)
	{
		//UFOを描画
		CModel::Draw();

		VECTOR bottomPos = { m_vPos.x,m_vPos.y - m_fConeBottom,m_vPos.z };
		//透明度の変更
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255 * SEARCH_ALPHA / 100);
		//円錐を描画
		DrawCone3D(m_vPos, bottomPos, m_fConeRadius, SEARCH_POLYGON_NUM, COLOR[RED], COLOR[YELLOW], true);
		Draw::DrawBlendModeReset();
	}
}

void UFO::DrawUI()
{
	//発見状態じゃないかプレイヤーを確保していたら実行しない
	if (m_State != STATE_DISCOVERY || m_CatchPlayer)
		return;

	float x = SCREEN_SIZE_X / 2;
	float y = SCREEN_SIZE_Y / 2 + BARRAGE_HEIGHT;

	//本体用の半径サイズ
	float percent = (float)m_iButtonCount / ((float)QTE_BUTTON_NUM * (float)m_iQTECount) * BARRAGE_RADIUS;

	//背景
	Draw::DrawBlendModeAlpha(BARRAGE_BACK_ALPHA);
	DrawCircleAA(x, y, BARRAGE_RADIUS * BARRAGE_RADIUS_SCALE + 1, BARRAGE_VERTEX_NUM, COLOR[RED], true);
	Draw::DrawBlendModeReset();

	//本体
	Draw::DrawBlendModeAlpha(BARRAGE_MAIN_ALPHA);
	DrawCircleAA(x, y, percent * BARRAGE_RADIUS_SCALE, BARRAGE_VERTEX_NUM, COLOR[CYAN], true);
	Draw::DrawBlendModeReset();

	//枠
	DrawCircleAA(x, y, BARRAGE_RADIUS * BARRAGE_RADIUS_SCALE + 1, BARRAGE_VERTEX_NUM, COLOR[YELLOW], false);
	//枠
	DrawCircleAA(x, y, BARRAGE_RADIUS * BARRAGE_RADIUS_SCALE + 2, BARRAGE_VERTEX_NUM, COLOR[BLACK], false, BARRAGE_LINE_SIZE);
	
}

void UFO::Fin()
{
	//サウンドを停止する
	Sound::StopSound(SE_LOOP_UFO);
	Sound::StopSound(SE_LOOP_UFO_CATCH);

	CModel::Fin();
}

//==========================================================================

void UFO::RequestUFO(VECTOR plPos)
{
	if (m_IsUse)
		return;

	//出現回数を加算
	Data::GetEvaluationData().m_iUfoSpawningNum++;

	//変数の設定
	m_vPos				= GetSearchPos(plPos);
	m_fUpPos			= PULLOUT_HEIGHT;
	m_fCurrentMoveTime	= 0.0f;
	m_vScale			= VECTOR_ZERO;
	m_IsUse				= true;
	m_State				= STATE_IN;
	m_MoveState			= MOVE_STAY;
	m_fConeBottom		= 0.0f;
	m_fConeRadius		= 0.0f;

	//サウンドの再生
	Sound::Play(SE_UFO_SPAWN);
}
VECTOR UFO::GetSearchPos(VECTOR vPos)
{
	VECTOR ret = vPos;

	//プレイヤーの付近に出現させる
	//X方向
	if (RandPercent(MOVE_PROBABILITY))
	{
		ret.x += GetRand(UFO_SPAWN_RAND);
		ret.x += MOVE_LIMIT;
	}
	else
	{
		ret.x -= GetRand(UFO_SPAWN_RAND);
		ret.x -= MOVE_LIMIT;
	}

	//Z方向
	if (RandPercent(MOVE_PROBABILITY))
	{
		ret.z += GetRand(UFO_SPAWN_RAND);
		ret.z += MOVE_LIMIT;
	}
	else
	{
		ret.z -= GetRand(UFO_SPAWN_RAND);
		ret.z -= MOVE_LIMIT;
	}

	//高さを調節
	ret.y += UFO_HEIGHT;

	return ret;
}

void UFO::Updata()
{
	if (m_iHandle == -1)	return;

	MV1SetPosition(m_iHandle, m_vDrawPos);
	MV1SetRotationXYZ(m_iHandle, m_vRot);
	MV1SetScale(m_iHandle, m_vScale);
}
//待機
void UFO::Stay(VECTOR plPos, bool safetyAreaFlag)
{
	m_fCurrentMoveTime += FRAME_TIME;
	if (m_fCurrentMoveTime >= UFO_STATE_TIME)
	{
		m_fCurrentMoveTime	= 0.0f;

		//追跡状態にする
		m_MoveState			= MOVE_CHASE;
		m_vSpeed			= Math::GetMoving(m_vPos, m_vNextPos, 1.0f);
		m_fMoveCount		= 0.0f;
		m_fSpeedAdd			= 0.0f;

		//サウンドを再生
		Sound::Play(SE_UFO_MOVE);

		//次の位置を選択
		m_vNextPos = plPos;
		
		//影内||奈落落下中の場合正しい場所を認知できない
		if (safetyAreaFlag|| plPos.y <= DEATH_Y / 2)
		{
			//プレイヤーの付近に出現する
			//X方向
			if (RandPercent(MOVE_PROBABILITY))
			{
				m_vNextPos.x += GetRand(UFO_SPAWN_RAND);
			}
			else
			{
				m_vNextPos.x -= GetRand(UFO_SPAWN_RAND);
			}

			//Z方向
			if (RandPercent(MOVE_PROBABILITY))
			{
				m_vNextPos.z += GetRand(UFO_SPAWN_RAND);
			}
			else
			{
				m_vNextPos.z -= GetRand(UFO_SPAWN_RAND);
			}
		}

		//高さの調節
		m_vNextPos.y += UFO_HEIGHT;
	}
}
//追跡
void UFO::Chase()
{
	//加速していく
	m_fSpeedAdd += SPEED_ADD;

	//移動量の計算
	m_vSpeed	= Math::GetMoving(m_vPos, m_vNextPos, m_fSpeedAdd);
	//移動先を伸ばす
	m_vNextPos	=VAdd(m_vNextPos, m_vSpeed);
	//移動
	m_vPos		= VAdd(m_vPos, m_vSpeed);

	//一定時間移動したら
	if (Math::MatchSpecifiedNum(m_fMoveCount, CHASE_TIME, FRAME_TIME))
	{
		//待機に変更
		m_MoveState = MOVE_STAY;
	}
}

void UFO::Search(Player& pl)
{
	VECTOR vPos = m_vPos;
	vPos.y		+= m_fUpPos;

	//プレイヤーが安全地帯に入っている, 無敵状態, 落下状態, UFOより上にいる状態なら実行しない
	if (pl.GetSafetyAreaInFlag() || 
		pl.GetFeverTimeFlag() || 
		pl.GetPos().y < DEATH_Y / 2 || 
		pl.GetPos().y >= m_vDrawPos.y)
		return;

	VECTOR plPos = pl.GetPos();
	//高さを合わせる
	plPos.y = m_vPos.y;
	if (Math::GetDistance(m_vPos, plPos) <= m_fConeRadius / SEARCH_RADIUS_RATIO)
	{
		//確保状態
		m_iQTECount++;
		m_PLFound	= true;
		m_State		= STATE_DISCOVERY;
		pl.SetState(PLAYER_STATE_QTE);
	}
}

//ボタンガチャ押し
void UFO::QuickTimerEvent(Player& pl)
{
	if (!m_PLFound|| m_CatchPlayer)
		return;

	//プレイヤーがUFOに接近したら
	if (Math::GetDistance(m_vPos, pl.GetPos()) < CATCH_DISTANCE)
	{
		//ゲームオーバー
		m_CatchPlayer	= true;
		m_PLFound		= false;
		//一瞬暗転
		Fade::GetInstance()->RequestFlash(Fade::BLACK);
	}

	//左スティックの傾き加減
	VECTOR stickleft = {};

	//連打数を取得
	int inputNum = InputManager::CheckResistanceToUFO(stickleft);

	//スティックの傾きから連打数を取得
	if (fabs(stickleft.x) == 1.0f || fabs(stickleft.z) == 1.0f)
	{
		//前回の入力と違っていたら加算
		if (m_vKeepLStick.x != stickleft.x && m_vKeepLStick.z != stickleft.z)
		{
			inputNum++;
			//サウンドを再生
			Sound::Play(SE_UFO_QTE);
		}
	}

	//連打数を加算する
	if (inputNum != 0)
	{
		m_iButtonCount += inputNum;
		//サウンドを再生
		Sound::Play(SE_UFO_QTE);
	}

	//前回と比べるための保存
	m_vKeepLStick.x = stickleft.x;
	m_vKeepLStick.z = stickleft.z;

	//ボタンを指定以上押した（QTEの度に必要数が増える）
	if (m_iButtonCount >= QTE_BUTTON_NUM * m_iQTECount)
	{
		//変数を初期化
		m_iButtonCount	= 0;
		m_PLFound		= false;

		//プレイヤーを解放する
		pl.SetState(PLAYER_STATE_LIBERATION);
		pl.RequestFeverTime();

		m_State = STATE_MOVE;

		//=========================================================
		//次回は移動から開始
		m_fCurrentMoveTime = 0.0f;

		//追跡状態にする
		m_MoveState = MOVE_CHASE;

		//次の位置を選択
		m_vNextPos = pl.GetPos();

		//影内の場合正しい場所を認知できない
		if (!pl.GetSafetyAreaInFlag())
		{
			//プレイヤーの付近に出現する
			//X方向
			if (RandPercent(MOVE_PROBABILITY))
			{
				m_vNextPos.x += GetRand(UFO_SPAWN_RAND);
			}
			else
			{
				m_vNextPos.x -= GetRand(UFO_SPAWN_RAND);
			}

			//Z方向
			if (RandPercent(MOVE_PROBABILITY))
			{
				m_vNextPos.z += GetRand(UFO_SPAWN_RAND);
			}
			else
			{
				m_vNextPos.z -= GetRand(UFO_SPAWN_RAND);
			}
		}

		//高さの調節
		m_vNextPos.y	+= UFO_HEIGHT;

		m_fSpeedAdd		= 0.0f;
	}
}

//UFOを距離で透かす
void UFO::CheckUfoToCamera(VECTOR cameraPos,bool plVisionFlag, float discoveryRange) {
	m_HitCameraFlag = false;
	//一定の距離内に存在していたら発見状態に
	if (plVisionFlag || Math::GetDistance(m_vPos, cameraPos) > discoveryRange) return;

	m_HitCameraFlag = true;
}