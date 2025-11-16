#include"Enemy.h"
#include"DxLib.h"
#include"../MyLib/MyLib.h"
#include"../BackGround/StageBlockManager.h"

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
	Update();
	memset(&m_vSpeed, 0, sizeof(VECTOR));

	//移動速度を計算
	float sp			= (float)GetRand(MOVE_MAX_SPEED) + MOVE_MIN_SPEED;
	//速度の設定
	m_fSpeed			= sp * SPEED_SCALE;
	m_fStunMoveSpeed	= STUN_MOVE_SPEED;

	//状態の設定
	m_MovePoint			= true;
	m_State				= ENEMY_STATE_MOVE;

	//モデルサイズを設定
	m_vSize = ENEMY_TYPE1_SIZE;
	m_vSize.y /= 2.0f;

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	//構造体の設定
	UpdateCollision();
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase* hitCollision) {Hit(hitCollision); });
}

void EnemyType1::Step(Player& pl, ItemManager& itemMana, StageBlockManager& block, bool gameOverFlag, bool clearFlag)
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

	//コリジョン情報の更新
	UpdateCollision();
	
	//アニメ
	EnemyBase::Step();

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

void EnemyType1::Fin() {
	EnemyBase::Fin();

	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
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

//当たった処理
void EnemyType1::Hit(CollisionBase* hitCollision) {
	//ステージの区画は実行しない
	if (hitCollision->GetKind() == KIND_AREA)return;
	//プレイヤー以降は実行しない
	if (hitCollision->GetKind() >= KIND_PLAYER_RING&& hitCollision->GetKind() < KIND_ENEMY)return;
	if (hitCollision->GetKind() >= KIND_ITEM)return;

	//修正可能軸を設定する
	SetEditAxisFlag();
	COLLISION_AXIS collisionAxis = CollisionManager::GetInstance()->SelectModifyingAxis(m_EditAxisFlag, &m_Collision, hitCollision);

	//当たった先の情報
	VECTOR hitPos = {};
	VECTOR hitSize = {};
	switch (hitCollision->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* hitAABB = static_cast<CollisionAABB*>(hitCollision);
		hitPos = hitAABB->GetCollision().centerPos;
		hitSize = hitAABB->GetCollision().size;
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* hitSphere = static_cast<CollisionSphere*>(hitCollision);
		hitPos = hitSphere->GetCollision().centerPos;
		hitSize.y = hitSphere->GetCollision().radius;
		break;
	}
	default:
		break;
	}

	//発見状態判定
	HitPlayer(hitCollision);

	//軸を修正
	switch (collisionAxis)
	{
	case AXIS_Y:
		HitY(hitPos, hitSize);
		break;

	case AXIS_X:
		HitX(hitPos, hitSize);
		HitStageBlock(hitCollision);
		break;

	case AXIS_Z:
		HitZ(hitPos, hitSize);
		HitStageBlock(hitCollision);
		break;

	default:
		break;
	}
}

//X軸の当たった処理
void EnemyType1::HitX(VECTOR hitPos, VECTOR hitSize) {
	if (m_vPos.x == m_vNextPos.x)return;

	//情報を取得
	AABB MyCollision = m_Collision.GetCollision();

	VECTOR enemyPos = MyCollision.centerPos;
	VECTOR enemySize = MyCollision.size;
	if (enemyPos.x < hitPos.x) {
		//←側にあった
		enemyPos.x -= (enemyPos.x + enemySize.x) - (hitPos.x - hitSize.x);
	}
	if (enemyPos.x > hitPos.x) {
		//→側に当たった
		enemyPos.x += (hitPos.x + hitSize.x) - (enemyPos.x - enemySize.x);
	}

	//適用
	m_vNextPos.x = enemyPos.x;
	//コリジョン情報の更新
	UpdateCollision();
}
//Y軸の当たった処理
void EnemyType1::HitY(VECTOR hitPos, VECTOR hitSize) {
	if (m_vPos.y == m_vNextPos.y)return;

	//情報を取得
	AABB MyCollision = m_Collision.GetCollision();

	VECTOR enemyPos = MyCollision.centerPos;
	VECTOR enemySize = MyCollision.size;
	if (enemyPos.y > hitPos.y) {
		//床に当たった
		enemyPos.y += (hitPos.y + hitSize.y) - (enemyPos.y - enemySize.y);
	}
	if (enemyPos.y < hitPos.y) {
		//天井に当たった
		enemyPos.y -= (enemyPos.y + enemySize.y) - (hitPos.y - hitSize.y);
	}

	//重力を初期化
	HitGravityReset();

	//適用
	m_vNextPos.y = enemyPos.y;
	//座標を足元に移動
	m_vNextPos.y -= enemySize.y;
	//コリジョン情報の更新
	UpdateCollision();
}
//Z軸の当たった処理
void EnemyType1::HitZ(VECTOR hitPos, VECTOR hitSize) {
	if (m_vPos.z == m_vNextPos.z)return;

	//情報を取得
	AABB MyCollision = m_Collision.GetCollision();
	VECTOR enemyPos = MyCollision.centerPos;
	VECTOR enemySize = MyCollision.size;

	//座標の修正
	if (enemyPos.z < hitPos.z) {
		enemyPos.z -= (enemyPos.z + enemySize.z) - (hitPos.z - hitSize.z);
	}
	if (enemyPos.z > hitPos.z) {
		enemyPos.z += (hitPos.z + hitSize.z) - (enemyPos.z - enemySize.z);
	}

	//適用
	m_vNextPos.z = enemyPos.z;
	//コリジョン情報の更新
	UpdateCollision();
}

//コリジョン情報の更新
void EnemyType1::UpdateCollision() {
	AABB setCollision = m_Collision.GetCollision();
	//サイズを設定
	setCollision.size = m_vSize;
	//中心座標を設定
	setCollision.centerPos = m_vNextPos;
	setCollision.centerPos.y += setCollision.size.y;
	//情報を更新
	m_Collision.SetCollision(setCollision);
}

//ステージブロックとの衝突
void EnemyType1::HitStageBlock(CollisionBase* hitCollision) {
	//ブロックに当たっていなかったら終了
	if (hitCollision->GetKind() != KIND_BLOCK)return;
	//ブロック情報を受け取る
	StageBlock* stageBlock = static_cast<StageBlock*>(hitCollision->GetOwner());
	//衝突したブロックが空気ブロックなら終了
	if (stageBlock->GetBlockType() == StageBlock::BLOCK_AIR)return;

	//壁に接触しているフラグの設定
	m_HitBlockFlag = true;

	//床に衝突していたらジャンプさせる
	if (stageBlock->GetBlockType() <= StageBlock::BLOCK_NORMAL_2) {
		//ジャンプさせる
		SetGravity(ENEMY_JUMP_POWER);
	}

	//移動状態か壁ブロックでなけば終了
	if (m_State != EnemyBase::ENEMY_STATE_MOVE || stageBlock->GetBlockType() != StageBlock::BLOCK_WALL)return;

	m_ProgressImpossibleFlag = true;
}