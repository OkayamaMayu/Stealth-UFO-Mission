#include "EnemyBase.h"

void EnemyBase::Init(VECTOR vPos, VECTOR vRot)
{
	//座標、角度を設定
	m_vSpawnPos		= vPos;
	m_vNextPos		= m_vSpawnPos;
	m_vPos			= m_vNextPos;
	m_vRot			= vRot;
	//座標、角度の適用
	Update();

	memset(&m_vSpeed, (int)0.0f, sizeof(VECTOR));

	Init();
}

void EnemyBase::Init()
{
	//変数の初期化
	m_State						= ENEMY_STATE_NORMAL;
	m_fGravityAdd				= 0.0f;
	m_fSpeed					= 0.0f;
	m_fStunMoveSpeed			= 0.0f;
	m_fPlFoundCound				= 0.0f;
	m_fRespawnTimeCount			= 0.0f;
	m_fNextMoveRot				= 0.0f;
	m_fHitBlockCount			= 0.0f;
	m_IsUse						= true;
	m_InitPosFlag				= false;
	m_LookOnFlag				= false;
	m_HitBlockFlag				= false;
	m_ProgressImpossibleFlag	= false;
	m_iItemIndex				= -1;

	//行動の持続時間の設定
	m_fStateTime			= GetRand((int)ENEMY_STATE_MAXTIME) + 1.0f;

	for (int i = 0; i < ENEMY_REACTION_NUM; i++)
		m_iEffectHnadle[i] = -1;

	//はじめの行動を選択
	if (RandPercent(ACT_PROBABILITY))
	{
		m_fCurrentStateTime = 0.0f;
	}
	else
	{
		m_fCurrentStateTime = ENEMY_STATE_MAXTIME;
	}
}

void EnemyBase::Load(int originHandle)
{
	//ブロックをコピー
	m_iHandle = MV1DuplicateModel(originHandle);
}

void EnemyBase::Start()
{
	//大きさを変える
	m_vScale = MODEL_SCALE_V;
	MV1SetScale(m_iHandle, m_vScale);

	//アニメーションの再生
	RequestLoop(ANIMID_DEFFAULT, ANIMATION_SPEED[ANIMID_DEFFAULT]);

	Update();
}

void EnemyBase::Draw()
{
	Update();
	CModel::Draw();
}

void EnemyBase::Fin()
{

}

//地面/天井に当たった場合の重力のリセット
void EnemyBase::HitGravityReset()
{
	m_fGravityAdd = 0.0f;
}
//配置ブロックと当たった
void EnemyBase::HitSetBlock()
{
	//待機にする
	m_State				= ENEMY_STATE_NORMAL;
	//時間をリセット
	m_fCurrentStateTime = 0.0f;
}

//移動処理
void EnemyBase::Move(VECTOR plPos, float speed)
{
	//目的地に向かっていく
	if (!m_InitPosFlag)
	{
		//移動量を計算
		m_vSpeed	= Math::GetMoveVec(m_fNextMoveRot, speed);
		//移動
		m_vNextPos	= VAdd(m_vNextPos, m_vSpeed);

		//進行方向に回転
		m_vRot		= Math::GetMovingRot(m_vPos, m_vRot, m_vNextPos, MOVE_ROT_SPEED);

		//移動限界地点に到達したら
		float dist	= Math::GetDistance(m_vSpawnPos, m_vNextPos);
		if (dist >= ENEMY_MOVE_MAX)
		{
			//初期位置に向かうフラグ
			m_InitPosFlag		= true;
			//時間の設定
			m_fCurrentStateTime = m_fStateTime;
		}
	}
	else //初期位置に向かうフラグ
	{
		//回転
		m_vRot		= Math::GetMovingRot(m_vPos, m_vRot, m_vSpawnPos, RESET_ROT_SPEED);
		m_vSpeed	= Math::GetMoveVec(m_vRot.y, speed);
		//移動
		m_vSpeed	= VScale(m_vSpeed, RESET_MOVE_SCALE);
		m_vNextPos	= VAdd(m_vNextPos, m_vSpeed);

		//初期位置の一定距離に近づいたら
		if (Math::GetDistance(m_vNextPos, m_vSpawnPos) <= RESET_ACCEPTABLE)
		{
			//移動先へ向かうフラグ
			m_InitPosFlag = false;
			//時間の設定
			m_fCurrentStateTime = m_fStateTime;
		}
		else
		{
			//時間の設定
			m_fCurrentStateTime = 0.0f;
		}
	}
}

//発見時移動関数
void EnemyBase::TrackingMove(Player& player, VECTOR itemPos, float speed)
{
	//見失っている状態は実行しない
	if (m_FoundType == ENEMY_LOSE)return;

	//変数の設定
	VECTOR vPos		= {};
	m_vSpeed		= VECTOR_ZERO;
	float fSpeed	= speed;

	//発見時の速度計算
	if (m_FoundType == FOUND_PLAYER)
	{
		//プレイヤー
		vPos	= player.GetPos();

		//プレイヤーの追跡速度を作成
		//距離から速度を作成する
		fSpeed	= Math::GetDistance(m_vPos, vPos) * TRACKING_MOVE_SCALE;
	}
	else if (m_FoundType == FOUND_ITEM)
	{
		//アイテム

		vPos	= itemPos;
		fSpeed /= 2.0f;
	}

	//アイテムを発見していない, プレイヤーが安全地帯に入っていない, プレイヤーが無敵状態になっていない
	if (m_FoundType == FOUND_ITEM || !player.GetSafetyAreaInFlag() && !player.GetFeverTimeFlag())
	{
		//進行方向に回転
		float fRotY		= m_vRot.y;
		float fPlEneRot = Math::GetAngle(m_vNextPos, vPos);

		//方向を合わせる
		if (Math::MatchSpecifiedNum(fRotY, fPlEneRot, TRACKING_ROT_SPEED))
		{
			//固定する
			m_vRot.y	= fPlEneRot;
		}
		else
		{
			//回転する
			m_vRot		= Math::GetMovingRot(m_vPos, m_vRot, vPos, 0.1f);
		}

		//移動量を計算
		m_vSpeed = Math::GetMoveVec(m_vRot.y, fSpeed);
		//移動
		m_vSpeed = VScale(m_vSpeed, -1.0f);
	}

	//一定距離に入っていなかったら
	if (Math::GetDistance(m_vPos, vPos) >= TRACKING_CERTAIN)
	{
		//移動
		m_vNextPos = VAdd(m_vNextPos, m_vSpeed);
	}
}

//発見
void EnemyBase::Discovery(Player& player, ItemManager& itemMana, StageBlockManager& block)
{
	//気絶状態なら未発見状態にして終了
	if (m_State == ENEMY_STATE_STUN)
	{
		m_FoundType = FOUND_NEUTRAL;
		return;
	}

	//対象との間にブロックが存在するか
	float distance = 0.0f;

	//視界の最大と最小
	LineSegment MaxLine = {};
	LineSegment MinLine = {};
	MaxLine.startPos = MinLine.startPos = m_vPos;
	//視点の終点を求める
	VECTOR moveVec = Math::GetMoveVec(m_vRot.y + Math::ChangeDegToRad(45.0f), ENEMY_FOUNDSIZE);
	MaxLine.endPos = VAdd(m_vPos, moveVec);
	moveVec = Math::GetMoveVec(m_vRot.y - Math::ChangeDegToRad(45.0f), ENEMY_FOUNDSIZE);
	MinLine.endPos = VAdd(m_vPos, moveVec);

	for (int i = 0; i < MAP_AREA_NUM; i++) {
		AABB areaCollison = block.GetStageBlockArea(i).GetCollision().GetCollision();
		//エリアに当たっていなければ実行しない
		if (!Collision::IsCollidingAABBToLineSegment(areaCollison, MinLine)&&
			!Collision::IsCollidingAABBToLineSegment(areaCollison, MaxLine))continue;

		for (auto stageBlock : block.GetStageBlock(i)) {
			//空気ブロックなら間に入っていても気にしない
			if (stageBlock.GetBlockType() == stageBlock.BLOCK_AIR)continue;

			VECTOR blockPos = stageBlock.GetPos();

			//ブロックが視界内にあるか判定
			if (!Math::CheckVision(blockPos, m_vRot.y, m_vPos, ENEMY_FOUNDSIZE))continue;

			//Yを比べる
			if (m_vPos.y > blockPos.y)continue;
			//距離を計算
			float checkDistance = Math::GetDistance(m_vPos, blockPos);

			if (checkDistance > distance && distance != 0.0f)continue;
			//格納中の値より小さかったら更新
			distance = checkDistance;
		}
	}

	//視界にプレイヤーが入ったら（発見の最優先）
	//プレイヤーが小さい場合はプレイヤーとの間にブロックがない
	//ブロックとの距離が0.0fの場合は視界にブロックがない
	VECTOR plPos = player.GetPos();
	float plDistance = Math::GetDistance(m_vPos, plPos);
	bool loseFlag = true;//見失いフラグ
	if (plDistance <= distance || distance == 0.0f)
	{
		bool discoveryFlag = false;//発見フラグ
		//視界内かつ影に入っていない
		if (m_FoundType == FOUND_PLAYER && !player.GetSafetyAreaInFlag())
		{
			//すでにプレイヤーを発見している状態では一定距離に入るだけで発見状態にする
			if (Math::GetDistance(m_vPos, plPos) <= ENEMY_FOUNDSIZE)
			{
				discoveryFlag = true;
			}
		}
		else if (!player.GetSafetyAreaInFlag())
		{
			//プレイヤーを発見していない状態
			if (Math::CheckVision(plPos, m_vRot.y, m_vPos, ENEMY_FOUNDSIZE))
			{
				discoveryFlag = true;
			}
		}

		//発見されている
		if (discoveryFlag)
		{
			m_fPlFoundCound = 0.0f;
			PlayerDiscovery();
			loseFlag = false;
		}
	}

	//見失う
	if (loseFlag && Math::MatchSpecifiedNum(m_fPlFoundCound, 2.0f, FRAME_TIME))
	{
		m_FoundType = ENEMY_LOSE;
	}

	//プレイヤー発見中はアイテムの判定はしない
	if (m_FoundType == FOUND_PLAYER)
		return;

	for (int i = 0; i < itemMana.GetItemMaxNum(); i++)
	{
		Item item = itemMana.GetItem(i);
		float itemDistance = Math::GetDistance(m_vPos, item.GetPos());

		//使われていないアイテムは認識しない
		if (!item.GetIsUse())
			continue;

		if (item.GetFoundFlag())
		{
			//視界内
			if (Math::GetDistance(m_vPos, item.GetPos()) <= ITEM_CERTAIN)
			{
				//発見状態にする
				m_State = ENEMY_STATE_DISCOVERY;
				//経過時間をリセット
				m_fCurrentStateTime = 0.0f;
				//発見時間の最低時間を設定
				m_fStateTime = ITEM_DISCOVERY_TIME;

				//発見したアイテムの番号を入れる
				m_iItemIndex = i;

				//まだ発見していない
				if (m_FoundType != FOUND_ITEM)
				{
					//一定距離内にプレイヤーがいるならサウンドを再生する
					if (Math::GetDistance(plPos, m_vPos) <= ITEM_FOUND_SOUND_AREA)
						Sound::Play(SE_ENEMY_FOUND);
				}

				//アイテム発見状態にする
				m_FoundType = FOUND_ITEM;
			}
		}
	}
}

void EnemyBase::PlayerDiscovery(bool type)
{
	//発見状態にする
	m_State = ENEMY_STATE_DISCOVERY;
	//経過時間をリセット
	m_fCurrentStateTime = 0.0f;
	//発見時間の最低時間を設定
	m_fStateTime = FOUND_MIN_TIME;

	if (type)
		if (m_FoundType != FOUND_PLAYER)
		{
			//プレイヤーが発見されたときに回数を加算
			Data::GetEvaluationData().m_iFoundNum++;

			Sound::Play(SE_ENEMY_FOUND);
		}

	//発見フラグセット
	Data::SetFoundFlag(true);
	Data::UpFoundGauge(FOUND_GAUGE_UP);

	//プレイヤー発見状態にする
	m_FoundType = FOUND_PLAYER;
}

void EnemyBase::Respawn()
{
	m_fRespawnTimeCount = 0.0f;
	m_fGravityAdd		= RESPAWN_GRAVITY;
	m_vNextPos			= m_vSpawnPos;
	m_vPos				= m_vNextPos;
	m_fHitBlockCount	= 0.0f;
	m_HitBlockFlag		= false;
	m_State				= ENEMY_STATE_NORMAL;
	m_FoundType			= FOUND_NEUTRAL;
	m_vNextPos.y		+= RESPAWN_POS_Y;
	Update();
}

//エフェクトの停止
void EnemyBase::StopEffect() 
{
	for (int i = 0; i < ENEMY_REACTION_NUM; i++)
	{
		if (m_iEffectHnadle[i] == -1)
			continue;

		//エフェクトを停止する
		CEffekseerCtrl::Stop(m_iEffectHnadle[i]);
		m_iEffectHnadle[i] = -1;
	}
}

//プレイヤーに当たった時の処理
void EnemyBase::HitPlayer(CollisionBase* hitBase) {
	//プレイヤーに当たっていなかったら終了
	if (hitBase->GetKind() != KIND_PLAYER)return;
	//気絶状態だったら
	if (m_State == EnemyBase::ENEMY_STATE_STUN)return;
	
	//発見状態にする
	PlayerDiscovery();
}

/*--------------------------------
		  アニメーション
----------------------------------*/

void EnemyBase::Step()
{
	switch (m_AnimData.m_iID)
	{
	case ANIMID_DEFFAULT:
		ExecDafault();
		break;

	case ANIMID_DEF_TO_SEARCH:
		ChangeDafToSearch();
		break;

	case ANIMID_SEARCH:
		ExecSearch();
		break;

	case ANIMID_LOOK:
		ExecLook();
		break;

	case ANIMID_SEARCH_TO_DEF:
		ChangeSearchToDef();
		break;

	default:
		break;
	}
	UpdateAnim();
}

//何もしてない
void EnemyBase::ExecDafault()
{
	//発見状態になった
	if (m_State == ENEMY_STATE_DISCOVERY || m_State == ENEMY_STATE_STUN)
	{
		//次のアニメへ
		RequestEndLoop(ANIMID_DEF_TO_SEARCH, ANIMATION_SPEED[ANIMID_DEF_TO_SEARCH]);
	}
}
//デフォ→サーチ
void EnemyBase::ChangeDafToSearch()
{
	//アニメ終了後
	if (GetChecknd(ANIMID_DEF_TO_SEARCH))
	{
		if (m_State != ENEMY_STATE_STUN)
		{
			//次のアニメへ
			RequestLoop(ANIMID_SEARCH, ANIMATION_SPEED[ANIMID_SEARCH]);
		}
		//スタン状態だときょろつく
		else
		{
			//次のアニメへ
			RequestEndLoop(ANIMID_LOOK, ANIMATION_SPEED[ANIMID_LOOK]);
		}
	}
}
//サーチ
void EnemyBase::ExecSearch()
{
	//見失った
	if (m_FoundType == ENEMY_LOSE)
	{
		//次のアニメへ
		RequestEndLoop(ANIMID_LOOK, ANIMATION_SPEED[ANIMID_LOOK]);
	}

	//発見状態じゃなかったら
	if (m_State != ENEMY_STATE_DISCOVERY)
	{
		//次のアニメへ
		RequestEndLoop(ANIMID_SEARCH_TO_DEF, ANIMATION_SPEED[ANIMID_SEARCH_TO_DEF]);
	}
}
//見渡し
void EnemyBase::ExecLook()
{
	switch (m_FoundType)
	{
	case FOUND_PLAYER:
	case FOUND_ITEM:
		//追跡へ
		RequestLoop(ANIMID_SEARCH, ANIMATION_SPEED[ANIMID_SEARCH] / 2);
		break;

	case ENEMY_LOSE:
		if (GetChecknd() && GetAnimeID() == ANIMID_LOOK)
		{
			//終了時にまだ未発見なら再び見渡す
			RequestEndLoop(ANIMID_LOOK, ANIMATION_SPEED[ANIMID_LOOK]);
		}

		break;

	default:
		break;
	}

	if (m_State == ENEMY_STATE_STUN)
	{
		//終了時にまだスタン状態なら再び見渡す
		RequestEndLoop(ANIMID_LOOK, ANIMATION_SPEED[ANIMID_LOOK]);
	}

	//発見状態じゃなかったら
	if (m_State != ENEMY_STATE_DISCOVERY && GetChecknd(ANIMID_LOOK))
	{
		//次のアニメへ
		RequestEndLoop(ANIMID_SEARCH_TO_DEF, ANIMATION_SPEED[ANIMID_SEARCH_TO_DEF]);

		CEffekseerCtrl::Stop(m_iEffectHnadle[ENEMY_REACTION_LOST]);
	}

}
//サーチ→デフォ
void EnemyBase::ChangeSearchToDef()
{
	//アニメ終了後
	if (GetChecknd() && GetAnimeID() == ANIMID_SEARCH_TO_DEF)
	{
		//次のアニメへ
		RequestLoop(ANIMID_DEFFAULT, ANIMATION_SPEED[ANIMID_DEFFAULT]);
	}
}