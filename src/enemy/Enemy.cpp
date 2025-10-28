#include"Enemy.h"
#include"DxLib.h"
#include"../MyLib/MyLib.h"

void EnemyType1::Init(VECTOR vStartPos, VECTOR vGoalPos, float vStartRot, float vGoalRot)
{
	EnemyBase::Init();

	//座標、角度を設定
	m_vSpawnPos			= vStartPos;
	m_vMoveFinPos		= vGoalPos;
	m_vNextPos			= m_vSpawnPos;
	m_vPos				= m_vSpawnPos;
	m_vNextMovePos		= m_vMoveFinPos;

	//移動の角度を格納
	m_fStayRot[0]		= vStartRot;
	m_fStayRot[1]		= vGoalRot;

	//角度を設定
	m_vRot				= VGet(0.0f, vStartRot, 0.0f);

	//座標、角度を固定
	Updata();
	memset(&m_vSpeed, 0, sizeof(VECTOR));

	//移動速度を計算
	float sp			= (float)GetRand(MOVE_MAX_SPEED) + MOVE_MIN_SPEED;
	//速度の設定
	m_fSpeed			= sp * SPEED_SCALE;
	m_fStunMoveSpeed	= STUN_MOVE_SPEED;

	//状態の設定
	m_MovePoint			= true;
	m_State				= ENEMY_STATE_MOVE;
}

void EnemyType1::Step(Player& pl, ItemManager& itemMana, BackGround& block, bool gameOverFlag, bool clearFlag)
{
	//ブロックに連続でぶつかった時間を計算
	if (m_HitBlockFlag)
	{
		if (Math::MatchSpecifiedNum(m_fHitBlockCount, HIT_RESET_TIME, FRAME_TIME))
		{
			Respawn();
		}
	}
	m_HitBlockFlag = false;
	//フラグをリセット
	m_LookOnFlag = false;	

	//次の位置を更新
	m_vNextPos = m_vPos;

	//==============================================================================================

	//発見処理
	Discovery(pl, itemMana, block);
	//発見後のリアクション
	switch (m_FoundType)
	{
	case FOUND_NEUTRAL:	//未発見
		StopEffect();

		break;

	case FOUND_PLAYER:	//発見
	case FOUND_ITEM:
		//まだ再生されていなかったらエフェクトを再生
		if (m_iEffectHnadle[ENEMY_REACTION_FOUND]==-1)
		{
			//エフェクトを停止する
			StopEffect();

			m_iEffectHnadle[ENEMY_REACTION_FOUND] = CEffekseerCtrl::Request(EFFECT_ENEMY_REACTION1, m_vPos, true);
		}

		break;

	case ENEMY_LOSE:	//見失い
		//まだ再生されていなかったらエフェクトを再生
		if (m_iEffectHnadle[ENEMY_REACTION_LOST] == -1 &&
			GetAnimeID() == ANIMID_LOOK)
		{
			StopEffect();

			m_iEffectHnadle[ENEMY_REACTION_LOST] = CEffekseerCtrl::Request(EFFECT_ENEMY_REACTION2, m_vPos, true);
		}

		break;
	default:
		break;
	}

	//エフェクトの座標を更新
	for (int i = 0; i < ENEMY_REACTION_NUM; i++)
	{
		if (CEffekseerCtrl::IsActive(m_iEffectHnadle[i]))
		{
			CEffekseerCtrl::SetPosition(m_iEffectHnadle[i], m_vPos);
		}
	}

	//プレイヤーがゲームオーバーになったらエフェクトを停止
	if(gameOverFlag||clearFlag)
		StopEffect();

	//==============================================================================================

	//発見されていたら
	if (Data::GetFoundFlag())
	{
		Data::UpFoundGauge(FOUND_GAUGE_UP);
	}
		
	if(m_State!= ENEMY_STATE_MOVE)
	{		
		//状態更新時間, 未発見状態, 見渡しアニメーションの再生が終了
		if (Math::MatchSpecifiedNum(m_fCurrentStateTime, ENEMY_STATE_MAXTIME, FRAME_TIME) ||
			m_FoundType == ENEMY_LOSE && GetChecknd(ANIMID_LOOK))
		{
			if(m_fStunMoveSpeed == 0.01f)
			{
				m_iItemIndex = -1;

				//時間になったら未発見に戻す
				m_FoundType = FOUND_NEUTRAL;

				//発見フラグセット
				Data::SetFoundFlag(false);

				//時間をリセット
				m_fCurrentStateTime = 0.0f;

				//現在の状態が待機状態だったら
				if (m_State == ENEMY_STATE_NORMAL)
				{
					//移動に切り替え
					m_State = ENEMY_STATE_MOVE;
				}
				//待機じゃなかったら
				else
				{
					//待機に切り替え
					m_State = ENEMY_STATE_NORMAL;
				}

				//反転
				m_MovePoint = !m_MovePoint;

				if (m_MovePoint)
				{
					m_vNextMovePos = m_vMoveFinPos;
				}
				else
				{
					m_vNextMovePos = m_vSpawnPos;
				}
			}
		}
	}

	//気絶状態でなければ基本体制に戻す
	if (m_State != ENEMY_STATE_STUN)
	{
		Math::MatchSpecifiedNum(m_vRot.x, 0.0f, ROT_RETURN_SPEED);
		Math::MatchSpecifiedNum(m_vRot.z, 0.0f, ROT_RETURN_SPEED);
	}

	//各行動処理
	switch (m_State)
	{
	case ENEMY_STATE_NORMAL:		//待機
	{
		//その場で回転し固定
		float fNextRotY = m_vRot.y;
		//判定用値を使って回転するのかを確認
		if (Math::MatchSpecifiedNum(fNextRotY, m_fStayRot[m_MovePoint], NORMAL_ROT_SPEED))
		{
			//固定
			m_vRot.y = m_fStayRot[m_MovePoint];
		}
		else
		{
			//回転量を求める
			VECTOR speed = Math::GetMoveVec(m_fStayRot[m_MovePoint], -1.0f);
			VECTOR nextPos = VAdd(m_vPos, speed);
			//回転
			m_vRot = Math::GetMovingRot(m_vPos, m_vRot, nextPos, NORMAL_ROT_SPEED);
		}
		break;
	}

	case ENEMY_STATE_MOVE:			//移動
		//移動処理
		Move(pl.GetPos(), m_fSpeed);
		break;

	case ENEMY_STATE_DISCOVERY:		//発見時
		//移動処理
		EnemyBase::TrackingMove(pl, itemMana.GetItemPos(m_iItemIndex), ENEMY_TYPE1_DISCOVERY_SPEED);
		break;

	case ENEMY_STATE_STUN:			//気絶
	
		//Z軸回転
		if(m_StunMoveFlag[0])
		{
			if (Math::MatchSpecifiedNum(m_vRot.z, STUN_ROT_Z, m_fStunMoveSpeed))
			{
				m_StunMoveFlag[0] = false;
			}
		}
		else
		{
			if (Math::MatchSpecifiedNum(m_vRot.z, -STUN_ROT_Z, m_fStunMoveSpeed))
			{
				m_StunMoveFlag[0] = true;
			}
		}

		//X軸回転
		if (m_StunMoveFlag[1])
		{
			if (Math::MatchSpecifiedNum(m_vRot.x, STUN_ROT_X, m_fStunMoveSpeed))
			{
				m_StunMoveFlag[1] = false;
			}
		}
		else
		{
			if (Math::MatchSpecifiedNum(m_vRot.x, -STUN_ROT_X, m_fStunMoveSpeed))
			{
				m_StunMoveFlag[1] = true;
			}
		}

		//回転速度の変動
		if (m_StunMoveFlag[2])
		{
			if (Math::MatchSpecifiedNum(m_fStunMoveSpeed, ENEMY_STUN_MOVE_SPEED_MAX[0], ENEMY_STUN_MOVE_SPEED))
			{
				m_StunMoveFlag[2] = false;
			}
		}
		else
		{
			if (Math::MatchSpecifiedNum(m_fStunMoveSpeed, ENEMY_STUN_MOVE_SPEED_MAX[1], ENEMY_STUN_MOVE_SPEED))
			{
				m_StunMoveFlag[2] = true;
			}
		}

		break;

	default:
		break;
	}

	//重力をかける
	m_fGravityAdd += GRAVITY;
	m_vNextPos.y += m_fGravityAdd;
	
	//アニメ
	EnemyBase::Step();

	Updata();

	//一定以上落ちたら
	if (m_vNextPos.y <= DEATH_Y)
	{
		//リスポーン時間になったらリスポーンさせる
		if(Math::MatchSpecifiedNum(m_fRespawnTimeCount,ENEMY_RESPAWN_TIME,FRAME_TIME))
		{
			Respawn();
		}
	}
}

void EnemyType1::Move(VECTOR plPos, float speed)
{
	//移動量を計算
	m_vSpeed = Math::GetMoving(m_vPos, m_vNextMovePos, speed);
	//移動
	m_vNextPos = VAdd(m_vNextPos, m_vSpeed);

	//進行方向に回転
	m_vRot = Math::GetMovingRot(m_vPos, m_vRot, m_vNextPos, 0.03f);

	//移動先に到着
	if (Math::GetDistance(m_vPos, m_vNextMovePos) <= speed)
	{
		//初期位置か移動先だったら
		if (Math::GetDistance(m_vPos, m_vSpawnPos) <= speed|| Math::GetDistance(m_vPos, m_vMoveFinPos) <= speed)
		{
			//待機にする
			m_State = ENEMY_STATE_NORMAL;
			//時間をリセット
			m_fCurrentStateTime = 0.0f;
		}
		else
		{
			//現在位置と近い位置に設定
			if (Math::GetDistance(m_vPos, m_vSpawnPos) <= Math::GetDistance(m_vPos, m_vMoveFinPos))
			{
				m_vNextMovePos = m_vSpawnPos;
				m_MovePoint = false;
			}
			else
			{
				m_vNextMovePos = m_vMoveFinPos;
				m_MovePoint = true;
			}
		}
	}
}