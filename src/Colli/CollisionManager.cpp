#include"DxLib.h"
#include"CollisionManager.h"
#include"../MyLib/MyLib.h"

CollisionManager* CollisionManager::m_Instance = nullptr;

//インスタンスの生成
void CollisionManager::Create(){
	if (m_Instance == nullptr){
		m_Instance = new CollisionManager();
	}
}

//インスタンスの削除
void CollisionManager::Destroy(){
	if (m_Instance != nullptr){
		delete m_Instance;
		m_Instance = nullptr;
	}
}

//インスタンスの所得
CollisionManager* CollisionManager::GetInstance(){
	if (m_Instance == nullptr){
		//インスタンスがない場合は生成
		Create();
	}

	return m_Instance;
}

//========================================

//ブロックとプレイヤー
void CollisionManager::CheckStageBlockToPlayer
(Player& player, BackGround& block)
{
	//UFOに捕まっている状態なら当たり判定をとらない
	if (player.GetState() == PLAYER_STATE_QTE)
	{
		player.SetPos(player.GetNextPos());
		player.Updata();
		return;
	}

	//プレイヤーの座標
	VECTOR playerPos = player.GetPos();
	VECTOR playerSize = PLAYER_SIZE;

	//点をモデルの中央に移動する
	VECTOR checkPlayerPos = playerPos;
	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	//縦高さが大きくなり過ぎたので戻す
	checkPlayerSize.y /= 2.0f;
	//次の座標
	VECTOR playerNextPos = player.GetNextPos();

	//ブロックのサイズ
	VECTOR blockSize = VGet(5.0f, 5.0f, 5.0f);
	//直径にする
	VECTOR checkBlockSize = VScale(blockSize, 2.0f);

	//床/天井
	checkPlayerPos.y = playerNextPos.y;
	for (int i = 0; i < block.GetBlockNum(); i++)
	{
		VECTOR blockPos = block.GetPos(i);

		//一定距離の外側は以下計算させない
		if (Math::GetDistance(blockPos, playerNextPos) >= COLLISION_DISANCE)
			continue;

		//当たっているかチェック
		if (Collision::Rect3D(
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize, blockPos, checkBlockSize))
		{
			if (checkPlayerPos.y < blockPos.y){
				//天井
				checkPlayerPos.y += (blockPos.y - blockSize.y) - (checkPlayerPos.y + playerSize.y);
				player.HitCeiling();
			}else if (checkPlayerPos.y > blockPos.y){
				//床
				checkPlayerPos.y += (blockPos.y + blockSize.y) - checkPlayerPos.y;
				player.HitGround();
			}

			break;
		}
	}

	//壁X
	checkPlayerPos.x = playerNextPos.x;
	for (int i = 0; i < block.GetBlockNum(); i++)
	{
		VECTOR blockPos = block.GetPos(i);

		//一定距離の外側は以下計算させない
		if (Math::GetDistance(blockPos, playerNextPos) >= COLLISION_DISANCE)
			continue;
		
		//当たったら
		if (Collision::Rect3D(
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize, blockPos, checkBlockSize))
		{
			//横の壁
			if (checkPlayerPos.x < blockPos.x)
			{
				checkPlayerPos.x += (blockPos.x - blockSize.x) - (checkPlayerPos.x + playerSize.x);
			}
			else if (checkPlayerPos.x > blockPos.x)
			{
				checkPlayerPos.x += (blockPos.x + blockSize.x) - (checkPlayerPos.x - playerSize.x);
			}

			break;
		}
	}

	//壁Z
	checkPlayerPos.z = playerNextPos.z;
	for (int i = 0; i < block.GetBlockNum(); i++)
	{
		VECTOR blockPos = block.GetPos(i);

		//一定距離の外側は以下計算させない
		if (Math::GetDistance(blockPos, playerNextPos) >= COLLISION_DISANCE)
			continue;

		//当たったら
		if (Collision::Rect3D(
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize, blockPos, checkBlockSize))
		{
			//手前と奥の壁
			if (checkPlayerPos.z < blockPos.z)
			{
				checkPlayerPos.z += (blockPos.z - blockSize.z) - (checkPlayerPos.z + playerSize.z);
			}
			else if (checkPlayerPos.z > blockPos.z)
			{
				checkPlayerPos.z += (blockPos.z + blockSize.z) - (checkPlayerPos.z - playerSize.z);

			}

			break;
		}
	}

	//座標を適応
	player.SetPos(checkPlayerPos);
	player.Updata();
}
//エネミー1とブロック
void CollisionManager::CheckStageBlockToEnemyType1
(EnemyManager& enemyManager, BackGround& block)
{
	for (int enemyNum = 0; enemyNum < enemyManager.GetEnemyType1MaxNum(); enemyNum++)
	{
		EnemyType1& enemy = enemyManager.GetEnemyType1(enemyNum);

		//座標を取得
		VECTOR checkEnemyPos = enemy.GetPos();
		//次の座標
		VECTOR enemyNextPos = enemy.GetNextPos();

		//サイズを取得
		VECTOR enemySize = enemy.GetSize();
		//直径にする
		VECTOR checkEnemySize = VScale(enemySize, 2.0f);
		//縦の高さが大きくなり過ぎたので戻す
		checkEnemySize.y /= 2.0f;

		//ブロックのサイズ
		VECTOR blockSize = Vector::MakeVec(BLOCK_SIZE);
		//直径にする
		VECTOR checkBlockSize = VScale(blockSize, 2.0f);

		//床/天井
		checkEnemyPos.y = enemyNextPos.y;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkEnemyPos) >= COLLISION_DISANCE)
				continue;

			//当たったら
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, blockPos, checkBlockSize))
			{
				//天井と床
				if (checkEnemyPos.y < blockPos.y)
				{
					checkEnemyPos.y += (blockPos.y - blockSize.y) - (checkEnemyPos.y + enemySize.y);
					enemy.HitGravityReset();
				}
				else if (checkEnemyPos.y > blockPos.y)
				{
					checkEnemyPos.y += (blockPos.y + blockSize.y) - checkEnemyPos.y;
					enemy.HitGravityReset();
				}

				break;
			}
		}

		//壁X
		checkEnemyPos.x = enemyNextPos.x;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkEnemyPos) >= COLLISION_DISANCE)
				continue;

			//当たったら
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, blockPos, checkBlockSize))
			{
				//ブロックに当たっている
				if (block.GetBlockType(i) <= BackGround::BLOCK_WALL)
				{
					enemy.SetHitBlockFlag(true);
				}

				//横の壁
				if (checkEnemyPos.x < blockPos.x)
				{
					checkEnemyPos.x += (blockPos.x - blockSize.x) - (checkEnemyPos.x + enemySize.x);
				}
				else if (checkEnemyPos.x > blockPos.x)
				{
					checkEnemyPos.x += (blockPos.x + blockSize.x) - (checkEnemyPos.x - enemySize.x);
				}

				//床に衝突したらジャンプ
				if (block.GetBlockType(i) == BackGround::BLOCK_NORMAL_1 ||
					block.GetBlockType(i) == BackGround::BLOCK_NORMAL_2)
				{
					enemyManager.HitJump(enemyNum);
				}

				//移動状態かつ壁に衝突したら中継ポイントへ
				if (enemy.GetState() == EnemyBase::ENEMY_STATE_MOVE && block.GetBlockType(i) == BackGround::BLOCK_WALL)
				{
					enemy.SetEnemyPointPos(enemyManager.HitWall(enemyNum));
				}

				break;
			}
		}

		//壁Z
		checkEnemyPos.z = enemyNextPos.z;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkEnemyPos) >= COLLISION_DISANCE)
				continue;

			//当たったら
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, blockPos, checkBlockSize))
			{
				//ブロックに当たっている
				if (block.GetBlockType(i) <= BackGround::BLOCK_WALL)
				{
					enemy.SetHitBlockFlag(true);
				}

				//手前と奥の壁
				if (checkEnemyPos.z < blockPos.z)
				{
					checkEnemyPos.z += (blockPos.z - blockSize.z) - (checkEnemyPos.z + enemySize.z);
				}
				else if (checkEnemyPos.z > blockPos.z)
				{
					checkEnemyPos.z += (blockPos.z + blockSize.z) - (checkEnemyPos.z - enemySize.z);
				}

				//床に衝突したらジャンプ
				if (block.GetBlockType(i) == BackGround::BLOCK_NORMAL_1 ||
					block.GetBlockType(i) == BackGround::BLOCK_NORMAL_2)
				{
					enemyManager.HitJump(enemyNum);
				}

				//移動状態かつ壁に衝突したら中継ポイントへ
				if (enemy.GetState() == EnemyBase::ENEMY_STATE_MOVE&& block.GetBlockType(i)== BackGround::BLOCK_WALL)
				{
					enemy.SetEnemyPointPos(enemyManager.HitWall(enemyNum));
				}

				break;
			}
		}

		enemy.SetPos(checkEnemyPos);
		enemy.Updata();
	}
}
//ブロックとアイテム
bool CollisionManager::CheckStageBlockToItem(ItemManager& itemManager, BackGround& block)
{
	//地面に当たったかのフラグ
	bool hitGroundFlag = false;
	for (int itemNum = 0; itemNum < itemManager.GetItemMaxNum(); itemNum++)
	{
		Item& item = itemManager.GetItem(itemNum);
		if (!item.GetIsUse()||item.GetCatchFlag())
			continue;

		//座標を取得
		VECTOR checkItemPos		= item.GetPos();
		//次の座標
		VECTOR itemNextPos		= item.GetNextPos();
		//サイズを取得
		VECTOR itemSize			= item.ITEM_SIZE;
		//直径にする
		VECTOR checkItemSize	= VScale(itemSize, 2.0f);
		//ブロックの大きさ
		VECTOR blockSize		= Vector::MakeVec(BLOCK_SIZE);
		//直径にする
		VECTOR checkBgSize		= VScale(blockSize, 2.0f);

		//床/天井
		checkItemPos.y = itemNextPos.y;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkItemPos) >= COLLISION_DISANCE||
				block.GetBlockType(i) == block.BLOCK_AIR)
				continue;

			//当たったら
			if (Collision::Rect3D(
				VGet(checkItemPos.x, checkItemPos.y, checkItemPos.z),
				checkItemSize, blockPos, checkBgSize))
			{
				//天井と床
				if (checkItemPos.y < blockPos.y)
				{
					checkItemPos.y += (blockPos.y - blockSize.y) - (checkItemPos.y + itemSize.y);
					item.HitCeiling();
				}
				else if (checkItemPos.y > blockPos.y)
				{
					checkItemPos.y += (blockPos.y + blockSize.y) - (checkItemPos.y - itemSize.y);
					
					bool isHit = item.HitGround(checkItemPos.y);
				
					if (isHit && !hitGroundFlag)
					{
						hitGroundFlag = true;
					}
				}

				break;
			}
		}

		//壁X
		checkItemPos.x = itemNextPos.x;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkItemPos) >= COLLISION_DISANCE ||
				block.GetBlockType(i) == block.BLOCK_AIR)
				continue;

			//当たったら
			if (Collision::Rect3D(
				VGet(checkItemPos.x, checkItemPos.y, checkItemPos.z),
				checkItemSize, blockPos, checkBgSize))
			{
				//横の壁
				if (checkItemPos.x < blockPos.x)
				{
					checkItemPos.x += (blockPos.x - blockSize.x) - (checkItemPos.x + itemSize.x);
					
					item.Reflection();
				}
				else if (checkItemPos.x > blockPos.x)
				{
					checkItemPos.x += (blockPos.x + blockSize.x) - (checkItemPos.x - itemSize.x);
					
					item.Reflection(-1);
				}

				break;
			}
		}

		//壁Z
		checkItemPos.z = itemNextPos.z;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkItemPos) >= COLLISION_DISANCE ||
				block.GetBlockType(i) == block.BLOCK_AIR)
				continue;

			//当たったら
			if (Collision::Rect3D(
				VGet(checkItemPos.x, checkItemPos.y, checkItemPos.z),
				checkItemSize, blockPos, checkBgSize))
			{
				//手前と奥の壁
				if (checkItemPos.z < blockPos.z)
				{
					checkItemPos.z += (blockPos.z - blockSize.z) - (checkItemPos.z + itemSize.z);

					item.Reflection();
				}
				else if (checkItemPos.z > blockPos.z)
				{
					checkItemPos.z += (blockPos.z + blockSize.z) - (checkItemPos.z - itemSize.z);

					item.Reflection(-1);
				}

				break;
			}
		}

		item.SetPos(checkItemPos);
		item.Updata();
	}

	return hitGroundFlag;
}
//ブロックとカメラ
void CollisionManager::CheckStageBlockToCamera(CameraManager& cameraManager, BackGround& block)
{
	PlayCamera &camera = cameraManager.GetPlayCamera();

	VECTOR cameraPos = camera.GetPos();
	VECTOR cameraSize = { 1.0f,1.0f,1.0f };

	//フォーカス（プレイヤーの位置）
	VECTOR cameraFocusPos = camera.GetForcus();
	cameraFocusPos.y += -FORCUS_OFFSET_Y - camera.GetForcusF();

	//直径にする
	VECTOR checkCameraSize = VScale(cameraSize, 2.0f);

	//ブロックのサイズ
	VECTOR blockSize = VGet(5.0f, 5.0f, 5.0f);
	//直径にする
	VECTOR checkblockSize = VScale(blockSize, 2.0f);

	for (int i = 0; i < block.GetBlockNum(); i++)
	{
		VECTOR blockPos = block.GetPos(i);
		block.SetIsDraw(i, true);

		//ブロック/プレイヤー座標とカメラの座標の距離をそれぞれ計算する
		float distanceBlock = Math::GetDistance(blockPos, cameraPos);
		float distancePlayerPos = Math::GetDistance(cameraFocusPos, cameraPos);

		//ブロックがプレイヤーの下(地面)にあるか、ブロックがｐｌの奥にあった場合は透かせない
		if (cameraFocusPos.y >= blockPos.y || distanceBlock >= distancePlayerPos)
			continue;

		if(!camera.GetUfoFlag())
		{
			if (!camera.GetPlVisionFlag() &&
				Math::GetDistance(blockPos, cameraPos) <= CAMERA_LEMGTH)
			{
				block.SetIsDraw(i, false);
			}
		}
		else
		{
			if (!camera.GetPlVisionFlag() &&
				Math::GetDistance(blockPos, cameraPos) <= CAMERA_LEMGTH+ CAMERA_LEMGTH_UFO)
			{
				block.SetIsDraw(i, false);
			}
		}
	}
}
//ブロックとリング
void CollisionManager::CheckStageBlockToPlRing(Player& player, BackGround& block, BlockManager& createBlock)
{
	player.SetDrawRingFlag(true);

	VECTOR playerSize = PLAYER_SIZE;

	//点をモデルの中央に移動する
	VECTOR checkPlayerPos = player.GetNextPos();
	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	//縦高さが大きくなり過ぎたので戻す
	checkPlayerSize.y /= 2.0f;

	while (true)
	{
		//落とす
		checkPlayerPos.y -= BLOCK_SIZE;

		//リングが一定まで落ちた
		if (checkPlayerPos.y <= DEATH_Y / 2)
		{
			//座標を適応
			player.SetRingPos(checkPlayerPos);
			//リングを非表示
			player.SetDrawRingFlag(false);

			return;
		}

		bool breakFlag = false;
		for (int i = 0; i < block.GetBlockNum(); i++)
		{
			VECTOR blockPos = block.GetPos(i);

			//一定距離の外側は以下計算させない
			if (Math::GetDistance(blockPos, checkPlayerPos) >= BLOCK_SIZE*1.5f)
				continue;
			else
			{
				//ブロックの上辺
				checkPlayerPos.y = blockPos.y + BLOCK_SIZE + 0.5f;
				//リングの位置を更新
				player.SetRingPos(checkPlayerPos);

				breakFlag = true;

				break;
			}
		}

		//当たっていたら抜ける
		if (breakFlag)
			break;
	}
	
	//配置ブロックの上にリングを出す
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		if (!createBlock.GetBlock(i).GetIsUse())
			continue;

		Block& block = createBlock.GetBlock(i);
		if (Math::GetDistance(block.GetPos(), checkPlayerPos) >= COLLISION_DISANCE)
			continue;

		VECTOR blockPos = block.GetPos();

		if (Math::GetDistance(blockPos, checkPlayerPos) <= BLOCK_SIZE * 1.5f)
		{
			//ブロックの上辺
			checkPlayerPos.y = blockPos.y + BLOCK_SIZE;
			//リングの位置を更新
			player.SetRingPos(checkPlayerPos);
		}
	}
}
//ブロックとレーザー
void CollisionManager::CheckStageBlockToLaser(LaserManager& laserManager, BackGround& block)
{
	//それぞれの数を取得
	int laserMaxNum = laserManager.GetLaserNum();
	int blockNum = block.GetBlockNum();

	for (int laserNum = 0; laserNum < laserMaxNum; laserNum++)
	{
		Laser& laser = laserManager.GetLaser(laserNum);
		//レーザーの座標
		//基準となる座標と伸ばした後の座標
		VECTOR laserPos[2] = { laser.GetPos(),laser.GetPos() };
		//レーザーの向き
		VECTOR laserRot = laser.GetRot();

		float laserMovePoint = laser.GetLaserLength();
		while (true)
		{
			//伸ばす
			Math::MatchSpecifiedNum(laserMovePoint, laser.LASER_MAX_LENGTH + 1.0f, BLOCK_SIZE/2);
			VECTOR laserVec = Math::GetMoveVec(laserRot, laserMovePoint);
			laserPos[1] = VAdd(laserPos[0], laserVec);

			//レーザーが一定まで伸びた
			if (laserMovePoint > laser.LASER_MAX_LENGTH)
			{
				//座標を適応
				laser.SetNextPos(laserPos[1]);
				laser.SetLaserLength(laserMovePoint);

				break;
			}

			bool laserFlag = false;
			for (int i = 0; i < blockNum; i++)
			{
				VECTOR blockPos = block.GetPos(i);

				//一定距離の外側は以下計算させない
				if (Math::GetDistance(blockPos, laserPos[1]) >= BLOCK_SIZE)
					continue;
				else
				{
					laserFlag = true;

					//座標を適応
					laserPos[1] = blockPos;
					VECTOR nextPos = laserPos[1];
					VECTOR laserVec = Math::GetMoveVec(laserRot.y, BLOCK_SIZE);
					nextPos = VSub(nextPos, laserVec);
					laser.SetNextPos(nextPos);

					VECTOR lPos = laserPos[0];
					laserMovePoint = Math::GetDistance(blockPos, lPos);

					laser.SetLaserLength(laserMovePoint);

					break;
				}
			}

			if (laserFlag)
				break;
		}
	}
}

//ゴールとプレイヤー
void CollisionManager::CheckGoalToPayer(Player& player, Goal& goal)
{
	//プレイヤー
	VECTOR playerPos = player.GetPos();
	VECTOR playerSize = PLAYER_SIZE;

	//点をモデルの中央に移動する
	VECTOR checkPlayerPos = playerPos;
	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	//縦高さが大きくなり過ぎたので戻す
	checkPlayerSize.y /= 2.0f;
	//次の座標
	VECTOR playerNextPos = player.GetNextPos();

	//ゴール
	VECTOR goalPos = goal.GetPos();
	VECTOR goalSize = goal.GOAL_SIZE;

	//点をモデルの中央に移動する
	VECTOR checkgGoalPos = goalPos;
	checkgGoalPos.y += goalSize.y / 2;
	//直径にする
	VECTOR checkGoalSize = VScale(goalSize, 2.0f);
	//縦高さが大きくなり過ぎたので戻す
	checkGoalSize.y /= 2.0f;

	//一定距離の外側は以下計算させない
	if (Math::GetDistance(checkPlayerPos, checkgGoalPos) >= COLLISION_DISANCE)
		return;

	bool goalFlag = false;
	//Y
	checkPlayerPos.y = playerNextPos.y;
	//当たったら
	if (Collision::Rect3D(
		VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
		checkPlayerSize, checkgGoalPos, checkGoalSize))
	{
		goalFlag = true;

		//天井と床
		if (checkPlayerPos.y < checkgGoalPos.y)
		{
			checkPlayerPos.y += checkgGoalPos.y - (checkPlayerPos.y + playerSize.y);
			player.HitCeiling();
		}

		else if (checkPlayerPos.y > checkgGoalPos.y)
		{
			checkPlayerPos.y += (checkgGoalPos.y + goalSize.y / 2) - checkPlayerPos.y;
			player.HitGround();
		}
	}

	//Z
	checkPlayerPos.z = playerNextPos.z;
	//当たったら
	if (Collision::Rect3D(
		VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
		checkPlayerSize, checkgGoalPos, checkGoalSize))
	{
		goalFlag = true;

		//手前と奥の壁
		if (checkPlayerPos.z < checkgGoalPos.z)
		{
			checkPlayerPos.z += (checkgGoalPos.z - goalSize.z) - (checkPlayerPos.z + playerSize.z);
		}

		else if (checkPlayerPos.z > checkgGoalPos.z)
		{
			checkPlayerPos.z += (checkgGoalPos.z + goalSize.z) - (checkPlayerPos.z - playerSize.z);
		}
	}

	//X
	checkPlayerPos.x = playerNextPos.x;
	//当たったら
	if (Collision::Rect3D(
		VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
		checkPlayerSize, checkgGoalPos, checkGoalSize))
	{
		goalFlag = true;

		//横の壁
		if (checkPlayerPos.x < checkgGoalPos.x)
		{
			checkPlayerPos.x += (checkgGoalPos.x - goalSize.x) - (checkPlayerPos.x + playerSize.x);
		}

		else if (checkPlayerPos.x > checkgGoalPos.x)
		{
			checkPlayerPos.x += (checkgGoalPos.x + goalSize.x) - (checkPlayerPos.x - playerSize.x);
		}
	}

	//当たっていたら（ゴールしていたら）
	if (goalFlag)
	{
		goal.HitGoal();

		CEffekseerCtrl::Request(EFFECT_GOAL, goalPos, false);

		Sound::Play(SE_GOLA_HIT);
	}

	//NextPosを更新して最終は地面で判定
	player.SetNextPos(checkPlayerPos);
}
//チェックポイントとプレイヤー
void CollisionManager::CheckCheckPointToPayer(CheckPointManager& checkPointManager, Player& player)
{
	//プレイヤー座標を取得
	VECTOR playerPos = player.GetPos();
	//プレイヤーの向きを取得
	VECTOR playerRot = player.GetRot();

	//チェックポイントの数を取得
	int checkPointNum = checkPointManager.GetCheckPointNum();
	//チェックポイントのサイズを取得
	float checkPointSize = checkPointManager.GetModelSize() * CHECKPOINT_HIT_SCALE;

	for (int i = 0; i < checkPointNum; i++)
	{
		//起動済みなら実行しない
		if (checkPointManager.GetCheckPoint(i).GetIsActive())
			continue;

		//チェックポイントを取得
		CheckPoint checkPoint = checkPointManager.GetCheckPoint(i);
		//チェックポイントの座標を取得
		VECTOR checkPointPos = checkPoint.GetPos();

		//当たっていたら
		if (Math::GetDistance(checkPointPos, playerPos) <= checkPointSize)
			if (checkPointManager.IsHit(i, playerPos, playerRot.y))
				return;
	}
}
//エネミーとプレイヤー
void CollisionManager::CheckEnemyToPlayer(Player& player, EnemyManager& enemyManager)
{
	//プレイヤー
	VECTOR playerPos = player.GetPos();
	VECTOR playerNextPos = player.GetNextPos();
	VECTOR playerSize = PLAYER_SIZE;

	//点をモデルの中央に移動する
	VECTOR checkPlayerPos = playerPos;
	
	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	checkPlayerPos.y /= 2.0f;

	for (int enemyNum = 0; enemyNum < enemyManager.GetEnemyType1MaxNum(); enemyNum++)
	{
		bool foundFlag = false;	//プレイヤー発見フラグ

		EnemyType1& enemy = enemyManager.GetEnemyType1(enemyNum);

		//ブロック生成ゲージを上昇
		if (Math::GetDistance(checkPlayerPos, enemy.GetPos()) <= COLLISION_DISANCE * 1.5f)
			Data::AddBlockGauge(0.25f);

		//一定距離までは判定しない
		if (Math::GetDistance(checkPlayerPos, enemy.GetPos()) >= COLLISION_DISANCE)
			continue;

		VECTOR enemyPos = enemy.GetPos();
		//座標を取得
		VECTOR checkEnemyPos = enemyPos;
		//次の座標
		VECTOR enemyNextPos = enemy.GetNextPos();

		//サイズを取得
		VECTOR enemySize = enemy.GetSize();
		//直径にする
		VECTOR checkEnemySize = VScale(enemySize, 2.0f);
		//縦の高さが大きくなり過ぎたので戻す
		checkEnemySize.y /= 2.0f;

		//床/天井
		checkEnemyPos.y = enemyNextPos.y;
		checkPlayerPos.y = playerNextPos.y;
		//当たったら
		if (Collision::Rect3D(
			VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
			checkEnemySize,
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize))
		{
			//天井と床
			if (checkEnemyPos.y < checkPlayerPos.y)
			{
				//エネミーが下
				player.HitGround();
			}	
			else if (checkEnemyPos.y > checkPlayerPos.y)
			{
				//プレイヤーが下
				enemy.HitGravityReset();
			}

			foundFlag = true;
		}

		//壁X
		checkEnemyPos.x = enemyNextPos.x;
		checkPlayerPos.x = playerNextPos.x;
		//当たったら
		if (Collision::Rect3D(
			VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
			checkEnemySize,
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize))
		{
			float num = 0.0f;	//差分

			//横の壁
			if (checkEnemyPos.x < checkPlayerPos.x)
			{
				num += (checkPlayerPos.x - playerSize.x) - (checkEnemyPos.x + enemySize.x);
			}
			else if (checkEnemyPos.x > checkPlayerPos.x)
			{
				num += (checkPlayerPos.x + playerSize.x) - (checkEnemyPos.x - enemySize.x);
			}

			checkEnemyPos.x += num;

			foundFlag = true;
		}
		
		//壁Z
		checkEnemyPos.z = enemyNextPos.z;
		checkPlayerPos.z = playerNextPos.z;
		//当たったら
		if (Collision::Rect3D(
			VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
			checkEnemySize,
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize))
		{
			float num = 0.0f;	//差分

			//手前と奥の壁
			if (checkEnemyPos.z < checkPlayerPos.z)
			{
				num += (checkPlayerPos.z - playerSize.z) - (checkEnemyPos.z + enemySize.z);
			}
			else if (checkEnemyPos.z > checkPlayerPos.z)
			{
				num += (checkPlayerPos.z + playerSize.z) - (checkEnemyPos.z - enemySize.z);
			}

			checkEnemyPos.z += num;

			foundFlag = true;
		}

		//当たっていて気絶状態でなかったら発見状態にする
		if (foundFlag&& enemy.GetState() != EnemyBase::ENEMY_STATE_STUN)
		{
			enemy.PlayerDiscovery();
		}

		enemy.SetNextPos(checkEnemyPos);
		player.SetNextPos(checkPlayerPos);
	}
}
//レーザーとプレイヤー
void CollisionManager::CheckLaserToPlayer(Player& player, LaserManager& laserManager, EnemyManager& enemyManager)
{
	//レーザーとプレイヤー
	bool laserHitFlag = false;

	//プレイヤー
	VECTOR playerPos = player.GetPos();
	playerPos.y += PLAYER_SIZE.y / 2;
	VECTOR playerSize = PLAYER_SIZE;

	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	checkPlayerSize.y /= 2;

	int laserNum = laserManager.GetLaserNum();
	bool posSetFlag = false;
	for (int i = 0; i < laserNum; i++)
	{
		//現在のレーザーの長さ以上に距離があったら実行しない
		if (Math::GetDistance(playerPos, laserManager.GetLaser(i).GetPos()) >= laserManager.GetLaser(i).GetLaserLength())
			continue;
	
		//レーザーの情報を設定
		Laser& laser = laserManager.GetLaser(i);
		VECTOR laserSize = MODEL_SCALE_V;
		//基準となる座標と伸ばした後の座標
		VECTOR laserPos[2] = { laser.GetPos(),laser.GetPos() };
		VECTOR laserRot = laser.GetRot();
		float laserLength = laser.GetLaserLength();

		VECTOR laserVec = {};
		
		//伸ばした長さ
		float laserMovePoint = 0.0f;

		while (true)
		{
			//伸ばす
			Math::MatchSpecifiedNum(laserMovePoint, laserLength + 1.0f, 1.0f);
			laserVec = Math::GetMoveVec(laserRot, laserMovePoint);
			laserPos[1] = VAdd(laserPos[0], laserVec);

			//レーザーが一定まで伸びた
			if (laserMovePoint > laserLength)
				break;
			
			//レーザーがプレイヤーに当たった
			if (Collision::Rect3D(
				laserPos[1], laserSize,
				playerPos,checkPlayerSize))
			{
				DebugString::GetInstance()->AddFormatString(0, 100, "●");

				Data::SetFoundFlag(true);

				posSetFlag = true;

				VECTOR hitPos = playerPos;
				hitPos.y = laserPos[1].y;
				hitPos = laserPos[1];
				CEffekseerCtrl::Request(EFFECT_LASER, hitPos, false);

				break;
			}
			else
			{
				DebugString::GetInstance()->AddFormatString(0, 100, "〇");
			}
		}

		if (posSetFlag)
			break;
	}

	//プレイヤーとエネミー：プレイヤーが反応していたら
	if (!posSetFlag)
	{
		laserHitFlag = false;
		return;
	}

	int enemyMaxNum = enemyManager.GetEnemyType1MaxNum();
	for (int i = 0; i < enemyMaxNum; i++)
	{
		//一定距離までは判定しない
		if (Math::GetDistance(playerPos, enemyManager.GetEnemyType1(i).GetPos()) >= ENEMY_FOUNDSIZE * 2)
			continue;

		EnemyType1& enemy = enemyManager.GetEnemyType1(i);
		enemy.PlayerDiscovery(false);
	}

	//プレイヤーが発見されたときに回数を加算
	if(!laserHitFlag)
	{
		laserHitFlag = true;
		Data::GetEvaluationData().m_iFoundNum++;
		Data::UpFoundGauge(20);
	}
}

//エネミー同士
void CollisionManager::CheckEnemyToEnemy(EnemyManager& enemyManager)
{
	//ファースト
	for (int firstEnemyNum = 0; firstEnemyNum < enemyManager.GetEnemyType1MaxNum(); firstEnemyNum++)
	{
		//一番目のエネミー
		EnemyType1& firstEnemy = enemyManager.GetEnemyType1(firstEnemyNum);	

		//座標を取得
		VECTOR firstEnemyPos = firstEnemy.GetPos();
		VECTOR checkFirstEnemyPos = firstEnemyPos;
		//次の座標
		VECTOR firstEnemyNextPos = firstEnemy.GetNextPos();

		//サイズを取得
		VECTOR firstEnemySize = firstEnemy.GetSize();
		//直径にする
		VECTOR checkFirstEnemySize = VScale(firstEnemySize, 2.0f);
		//縦の高さが大きくなり過ぎたので戻す
		checkFirstEnemySize.y /= 2.0f;

		//セカンド
		for (int secondEnemyNum = 0; secondEnemyNum < enemyManager.GetEnemyType1MaxNum(); secondEnemyNum++)
		{
			//二番目のエネミー
			EnemyType1& secondEnemy = enemyManager.GetEnemyType1(secondEnemyNum);

			//一定距離までは判定しない
			if (Math::GetDistance(checkFirstEnemyPos, secondEnemy.GetPos()) >= COLLISION_DISANCE ||
				firstEnemyNum == secondEnemyNum)
				continue;

			//座標を取得
			VECTOR secondEnemyPos = secondEnemy.GetPos();
			VECTOR checkSecondEnemyPos = secondEnemyPos;
			//次の座標
			VECTOR secondEnemyNextPos = secondEnemy.GetNextPos();

			//サイズを取得
			VECTOR secondEnemySize = secondEnemy.GetSize();
			//直径にする
			VECTOR checkSecondEnemySize = VScale(secondEnemySize, 2.0f);
			//縦の高さが大きくなり過ぎたので戻す
			checkSecondEnemySize.y /= 2.0f;

			//床/天井
			checkSecondEnemyPos.y = secondEnemyNextPos.y;
			checkFirstEnemyPos.y = firstEnemyNextPos.y;
			//当たったら
			if (Collision::Rect3D(
				VGet(checkSecondEnemyPos.x, checkSecondEnemyPos.y + secondEnemySize.y / 2.0f, checkSecondEnemyPos.z),
				checkSecondEnemySize,
				VGet(checkFirstEnemyPos.x, checkFirstEnemyPos.y + firstEnemySize.y / 2.0f, checkFirstEnemyPos.z),
				checkFirstEnemySize))
			{
				//天井と床
				if (checkSecondEnemyPos.y < checkFirstEnemyPos.y)
				{
					//エネミーが下
					firstEnemy.HitGravityReset();
				}
				else if (checkSecondEnemyPos.y > checkFirstEnemyPos.y)
				{
					//プレイヤーが下
					secondEnemy.HitGravityReset();
				}
			}

			//壁のX側
			checkSecondEnemyPos.x = secondEnemyNextPos.x;
			checkFirstEnemyPos.x = firstEnemyNextPos.x;
			//当たったら
			if (Collision::Rect3D(
				VGet(checkSecondEnemyPos.x, checkSecondEnemyPos.y + secondEnemySize.y / 2.0f, checkSecondEnemyPos.z),
				checkSecondEnemySize,
				VGet(checkFirstEnemyPos.x, checkFirstEnemyPos.y + firstEnemySize.y / 2.0f, checkFirstEnemyPos.z),
				checkFirstEnemySize))
			{
				float num = 0.0f;	//差分

				//横の壁
				if (checkSecondEnemyPos.x < checkFirstEnemyPos.x)
				{
					num += (checkFirstEnemyPos.x - firstEnemySize.x) - (checkSecondEnemyPos.x + secondEnemySize.x);
				}
				else if (checkSecondEnemyPos.x > checkFirstEnemyPos.x)
				{
					num += (checkFirstEnemyPos.x + firstEnemySize.x) - (checkSecondEnemyPos.x - secondEnemySize.x);
				}

				checkSecondEnemyPos.x += num / 2;
				checkFirstEnemyPos.x -= num / 2;
			}

			//壁Z
			checkSecondEnemyPos.z = secondEnemyNextPos.z;
			checkFirstEnemyPos.z = firstEnemyNextPos.z;
			//当たったら
			if (Collision::Rect3D(
				VGet(checkSecondEnemyPos.x, checkSecondEnemyPos.y + secondEnemySize.y / 2.0f, checkSecondEnemyPos.z),
				checkSecondEnemySize,
				VGet(checkFirstEnemyPos.x, checkFirstEnemyPos.y + firstEnemySize.y / 2.0f, checkFirstEnemyPos.z),
				checkFirstEnemySize))
			{
				float num = 0.0f;	//差分

				//手前と奥の壁
				if (checkSecondEnemyPos.z < checkFirstEnemyPos.z)
				{
					num += (checkFirstEnemyPos.z - firstEnemySize.z) - (checkSecondEnemyPos.z + secondEnemySize.z);
				}

				else if (checkSecondEnemyPos.z > checkFirstEnemyPos.z)
				{
					num += (checkFirstEnemyPos.z + firstEnemySize.z) - (checkSecondEnemyPos.z - secondEnemySize.z);
				}

				checkSecondEnemyPos.z += num / 2;
				checkFirstEnemyPos.z -= num / 2;
			}

			secondEnemy.SetNextPos(checkSecondEnemyPos);
			firstEnemy.SetNextPos(checkFirstEnemyPos);
		}
	}
}
//エネミー1とアイテム
bool CollisionManager::CheckEnemyType1ToItem(EnemyManager& enemyManager, ItemManager& itemManager)
{
	int itemMaxNum = itemManager.GetItemMaxNum();
	int enemyMaxNum = enemyManager.GetEnemyType1MaxNum();

	//アイテムが衝突したかのフラグ
	bool hitItemToEnemyFlag = false;

	for (int i = 0; i < itemMaxNum; i++)
	{
		if (!itemManager.GetItem(i).GetEnemyHitFlag())
		{
			continue;
		}
		Item& item = itemManager.GetItem(i);

		//アイテムの情報
		VECTOR itemPos	= item.GetNextPos();
		VECTOR itemRot	= item.GetRot();
		VECTOR itemSize = item.ITEM_SIZE;

		for (int enemyNum = 0; enemyNum < enemyMaxNum; enemyNum++)
		{
			//一定距離までは判定しない
			if (Math::GetDistance(item.GetPos(), enemyManager.GetEnemyType1(enemyNum).GetPos()) >= COLLISION_DISANCE)
				continue;

			EnemyType1& enemy = enemyManager.GetEnemyType1(enemyNum);

			VECTOR enemyPos = enemy.GetPos();
			//座標を取得
			VECTOR checkEnemyPos = enemy.GetNextPos();
			enemy.GetNextPos();
			//サイズを取得
			VECTOR enemySize = enemy.GetSize();
			//直径にする
			VECTOR checkEnemySize = VScale(enemySize, 2.0f);
			//縦の高さが大きくなり過ぎたので戻す
			checkEnemySize.y /= 2.0f;

			//当たる
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, itemPos, itemSize))
			{
				Sound::Play(SE_ITEM_HIT);

				item.HitEnemy();
				enemy.SetState(EnemyBase::ENEMY_STATE_STUN);

				if (!hitItemToEnemyFlag)
				{
					hitItemToEnemyFlag = true;
				}
			}
		}
	}

	return hitItemToEnemyFlag;
}

//カメラとUFO
void CollisionManager::CheckUfoToCamera(UFO& cUfo, CameraManager& cameraMan)
{
	//情報
	PlayCamera& camera = cameraMan.GetPlayCamera();
	VECTOR cameraPos = camera.GetPos();
	UFO& ufo = cUfo;

	ufo.SetHitCameraFlag(false);
	//一定の距離内に存在していたら発見状態に
	if (!camera.GetPlVisionFlag() &&
		Math::GetDistance(ufo.GetPos(), cameraPos) <= CAMERA_LEMGTH+ CAMERA_LEMGTH_UFO)
	{
		ufo.SetHitCameraFlag(true);
	}
}
//カメラとチェックポイント
void CollisionManager::CheckCheckPointToCamera(CheckPointManager& checkPointManager, CameraManager& camera, Player& player)
{
	//チェックポイント数を確保
	int checkPointNum = checkPointManager.GetCheckPointNum();

	//カメラの座標
	VECTOR cameraPos = camera.GetPlayCamera().GetPos();

	for (int i = 0; i < checkPointNum; i++)
	{
		//チェックポイントの座標
		VECTOR checkPointPos = checkPointManager.GetCheckPoint(i).GetPos();
		//半透明フラグをきる
		checkPointManager.GetCheckPoint(i).SetSemitransparentFlag(false);

		//カメラとチェックポイントの距離
		float checkPointToCameraDistance = Math::GetDistance(checkPointPos, cameraPos);

		//カメラと一定範囲に入ったら
		if (checkPointToCameraDistance <= CAMERA_LEMGTH)
		{
			//プレイヤーの座標
			VECTOR playerPos = player.GetPos();

			//チェックポイントとプレイヤーのカメラまでの距離を見て
			//チェックポイント方が近い(プレイヤーより手前に来ている)場合は以下実行
			if (checkPointToCameraDistance <= Math::GetDistance(playerPos, cameraPos))
			{
				//半透明にする
				checkPointManager.GetCheckPoint(i).SetSemitransparentFlag(true);
			}
		}
	}
}

//ブロック配置可能位置
void CollisionManager::CheckSetBlockPos(CameraManager& cameraManager, BlockManager& createBlock, BackGround& block, bool setMode)
{
	createBlock.SetBlockSetFlag(false);

	//配置可能ブロックの数を計算
	int blockNum = (int)(Data::GetBlockGauge() / SET_BLOCK_POINT);
	if (!setMode || blockNum <= 0)
		return;

	//画面の中心
	VECTOR screenPos = { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f };
	//スクリーン座標をワールド座標にする
	screenPos = ConvScreenPosToWorldPos(screenPos);
	
	//カメラの座標
	VECTOR cameraPos1 = cameraManager.GetCameraPos();
	//カメラの視点座標
	VECTOR cameraPos2 = cameraManager.GetCameraForcus();

	//配置予定座標
	VECTOR createPos = screenPos;

	VECTOR blockSize = Vector::MakeVec(BLOCK_SIZE);

	for (int i = 0; i < block.GetBlockNum(); i++)
	{
		createPos = cameraManager.GetCameraForcus();
		//ブロックの中心に一人称十字を当てる
		createPos.y -= blockSize.y;

		//一定距離外もしくは指定のブロックの場合実行しない
		if (Math::GetDistance(createPos, block.GetPos(i)) >= COLLISION_DISANCE * CREATE_BLOCK_HIT_SCALE ||
			block.GetBlockType(i) == BackGround::BLOCK_AIR|| block.GetBlockType(i) == BackGround::BLOCK_WALL)
			continue;

		VECTOR blockPos = block.GetPos(i);

		//伸ばした長さ
		float movePoint = 0.0f;
		bool setFlag = false;
		while (true)
		{
			if (Math::MatchSpecifiedNum(movePoint, COLLISION_DISANCE* CREATE_BLOCK_HIT_SCALE, BLOCK_SIZE))
				break;

			//カメラの座標から視点の座標までのベクトルを生成
			VECTOR v = Vector::VecCreate(cameraPos2, screenPos);
			//現在の長さ分まで伸ばす
			v = Vector::VecScale(v, movePoint);
			//座標を伸ばしていく
			createPos = VAdd(createPos, v);

			//以下判定
			for (int b = 0; b < BLOCK_MAX_NUM; b++)
			{
				//既配置されたブロックとの判定
				//配置されていなかったら実行しない
				if (!createBlock.GetBlock(b).GetIsUse())
					continue;

				//一定範外なら実行しない
				Block& block = createBlock.GetBlock(b);
				if (Math::GetDistance(block.GetPos(), createPos) >= COLLISION_DISANCE)
					continue;

				VECTOR setBgPos = block.GetPos();

				//配置ブロックに当たっていたら
				if (Collision::Rect3D(setBgPos, blockSize, createPos, blockSize))
				{
					//ブロックの上辺
					createPos = setBgPos;
					createPos.y = setBgPos.y + BLOCK_SIZE * 2;

					setFlag = true;
				}
			}

			//ステージブロックと一定範囲外なら実行しない
			if (Math::GetDistance(createPos, block.GetPos(i)) >= COLLISION_DISANCE * CREATE_BLOCK_STAGE_HIT_SCALE)
				break;
			
			//当たっていたら
			if (Collision::Rect3D(createPos, blockSize, blockPos, blockSize))
			{
				//床にあげる
				createPos = blockPos;
				createPos.y += blockSize.y*2;

				//配置できるようにする
				setFlag = true;
			}

			//配置できるようになっていたら
			if (setFlag)
			{
				createBlock.SetBlockPos(createPos);
				createBlock.SetBlockSetFlag(true);

				return;
			}
		}
	}
}
//配置ブロックとレーザー
void CollisionManager::CheckSetBlockToLaser(LaserManager& laserManager, BlockManager& createBlock)
{
	//数情報
	int laserNum = laserManager.GetLaserNum();
	int blockNum = BLOCK_MAX_NUM;

	//配置していない状態では実行しないようにする
	bool blockFlag = false;
	for (int i = 0; i < blockNum; i++)
	{
		//配置されていたらフラグを変更
		if (createBlock.GetBlock(i).GetIsUse())
			blockFlag = true;
	}

	//配置されていなかったら実行しない
	if (!blockFlag)
		return;

	for (int laNum = 0; laNum < laserNum; laNum++)
	{
		Laser& laser = laserManager.GetLaser(laNum);
		//基準となる座標と伸ばした後の座標
		VECTOR laserPos[2] = { laser.GetPos(),laser.GetPos() };
		//レーザーの向き
		VECTOR laserRot = laser.GetRot();

		//レーザーの長さ
		float laserMovePoint = laser.LASER_SIZE.z;
		while (true)
		{
			//伸ばす
			Math::MatchSpecifiedNum(laserMovePoint, laser.GetLaserLength() + 1.0f, 1.0f);
			//向きから移動量を計算
			VECTOR laserVec = Math::GetMoveVec(laserRot, laserMovePoint);
			//伸ばす
			laserPos[1] = VAdd(laserPos[0], laserVec);

			//レーザーが一定まで伸びた
			if (laserMovePoint > laser.GetLaserLength())
			{
				//座標を適応
				laser.SetNextPos(laserPos[1]);
				laser.SetLaserLength(laserMovePoint);

				break;
			}

			bool posSetFlag = false;
			for (int i = 0; i < blockNum; i++)
			{
				//ブロックが配置されていなかったら実行しない
				if (!createBlock.GetBlock(i).GetIsUse())
					continue;

				VECTOR blockPos = createBlock.GetBlock(i).GetPos();

				//一定距離の外側は以下計算させない
				if (Math::GetDistance(blockPos, laserPos[1]) >= BLOCK_SIZE)
					continue;
				else
				{
					//座標を適応
					laser.SetNextPos(laserPos[1]);
					laser.SetLaserLength(laserMovePoint);

					posSetFlag = true;

					break;
				}
			}

			//配置できるなら抜ける
			if (posSetFlag)
				break;
		}
	}
}
//配置ブロックとプレイヤー
void CollisionManager::CheckSetBlockToPlayer(Player& player, BlockManager& createBlock)
{
	//座標の取得
	VECTOR playerPos = player.GetPos();
	VECTOR playerSize = PLAYER_SIZE;

	//点をモデルの中央に移動する
	VECTOR checkPlayerPos = playerPos;
	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	//縦高さが大きくなり過ぎたので戻す
	checkPlayerSize.y /= 2.0f;
	//次の座標
	VECTOR playerNextPos = player.GetNextPos();

	//ブロックのサイズを取得
	VECTOR blockSize = Vector::MakeVec(BLOCK_SIZE);
	//直径にする
	VECTOR checkBgSize = VScale(blockSize, 2.0f);

	//天井---------------------------------------------------------------------------------
	checkPlayerPos.y = playerNextPos.y;
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		//使用されていなかったら実行しない
		if (!createBlock.GetBlock(i).GetIsUse())
			continue;

		//一定範囲外なら実行しない
		Block& block = createBlock.GetBlock(i);
		if (Math::GetDistance(block.GetPos(), player.GetPos()) >= COLLISION_DISANCE)
			continue;

		VECTOR blockPos = block.GetPos();

		//当たったら
		if (Collision::Rect3D(
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize, blockPos, checkBgSize))
		{
			//天井
			checkPlayerPos.y += (blockPos.y + blockSize.y) - checkPlayerPos.y;
			player.HitGround();

			break;
		}
	}

	//壁X------------------------------------------------------------------------------------
	checkPlayerPos.x = playerNextPos.x;
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		//使用されていなかったら実行しない
		if (!createBlock.GetBlock(i).GetIsUse())
			continue;

		//一定範囲外なら実行しない
		Block& block = createBlock.GetBlock(i);
		if (Math::GetDistance(block.GetPos(), player.GetPos()) >= COLLISION_DISANCE)
			continue;

		VECTOR blockPos = block.GetPos();

		//当たったら
		if (Collision::Rect3D(
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize, blockPos, checkBgSize))
		{
			//横の壁
			if (checkPlayerPos.x < blockPos.x)
			{
				checkPlayerPos.x += (blockPos.x - blockSize.x) - (checkPlayerPos.x + playerSize.x);
			}
			else if (checkPlayerPos.x > blockPos.x)
			{
				checkPlayerPos.x += (blockPos.x + blockSize.x) - (checkPlayerPos.x - playerSize.x);

			}

			break;
		}
	}

	//壁Z------------------------------------------------------------------------------------
	checkPlayerPos.z = playerNextPos.z;
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		//使用されていなかったら実行しない
		if (!createBlock.GetBlock(i).GetIsUse())
			continue;

		//一定範囲外なら実行しない
		Block& block = createBlock.GetBlock(i);
		if (Math::GetDistance(block.GetPos(), player.GetPos()) >= COLLISION_DISANCE)
			continue;

		VECTOR blockPos = block.GetPos();

		//当たったら
		if (Collision::Rect3D(
			VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
			checkPlayerSize, blockPos, checkBgSize))
		{
			//手前と奥の壁
			if (checkPlayerPos.z < blockPos.z)
			{
				checkPlayerPos.z += (blockPos.z - blockSize.z) - (checkPlayerPos.z + playerSize.z);
			}
			else if (checkPlayerPos.z > blockPos.z)
			{
				checkPlayerPos.z += (blockPos.z + blockSize.z) - (checkPlayerPos.z - playerSize.z);

			}

			break;
		}
	}

	player.SetNextPos(checkPlayerPos);
}

//配置ブロックとエネミー1
void CollisionManager::CheckSetBlockToEnemyType1(EnemyManager& enemyManager, BlockManager& createBlock)
{
	//ブロックのサイズを取得
	VECTOR blockSize = Vector::MakeVec(BLOCK_SIZE);
	//直径にする
	VECTOR checkBgSize = VScale(blockSize, 2.0f);

	//サイズを取得
	VECTOR enemySize = enemyManager.GetEnemyType1Size();
	//直径にする
	VECTOR checkEnemySize = VScale(enemySize, 2.0f);
	//縦の高さが大きくなり過ぎたので戻す
	checkEnemySize.y /= 2.0f;

	for (int enemyNum = 0; enemyNum < enemyManager.GetEnemyType1MaxNum(); enemyNum++)
	{
		EnemyType1& enemy = enemyManager.GetEnemyType1(enemyNum);

		//座標を取得
		VECTOR checkEnemyPos = enemy.GetPos();
		//次の座標
		VECTOR enemyNextPos = enemy.GetNextPos();

		//床/天井----------------------------------------------------------------------------------
		checkEnemyPos.y = enemyNextPos.y;
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			//使用されていなかったら実行しない
			if (!createBlock.GetBlock(i).GetIsUse())
				continue;

			Block& block = createBlock.GetBlock(i);
			//一定範囲外なら実行しない
			if (Math::GetDistance(block.GetPos(), checkEnemyPos) >= COLLISION_DISANCE)
				continue;

			VECTOR blockPos = block.GetPos();

			//当たったら
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, blockPos, checkBgSize))
			{
				//天井と床
				checkEnemyPos.y += (blockPos.y + blockSize.y) - checkEnemyPos.y;
				enemy.HitGravityReset();

				break;
			}
		}

		//壁X------------------------------------------------------------------------------------
		checkEnemyPos.x = enemyNextPos.x;
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			//使用されていなかったら実行しない
			if (!createBlock.GetBlock(i).GetIsUse())
				continue;

			Block& block = createBlock.GetBlock(i);
			//一定範囲外なら実行しない
			if (Math::GetDistance(block.GetPos(), checkEnemyPos) >= COLLISION_DISANCE)
				continue;

			VECTOR blockPos = block.GetPos();

			//当たったら
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, blockPos, checkBgSize))
			{
				//横の壁
				if (checkEnemyPos.x < blockPos.x)
				{
					checkEnemyPos.x += (blockPos.x - blockSize.x) - (checkEnemyPos.x + enemySize.x);
					enemy.HitSetBlock();
				}
				else if (checkEnemyPos.x > blockPos.x)
				{
					checkEnemyPos.x += (blockPos.x + blockSize.x) - (checkEnemyPos.x - enemySize.x);
					enemy.HitSetBlock();
				}

				break;
			}
		}

		//壁Z------------------------------------------------------------------------------------
		checkEnemyPos.z = enemyNextPos.z;
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			//使用されていなかったら実行しない
			if (!createBlock.GetBlock(i).GetIsUse())
				continue;

			Block& block = createBlock.GetBlock(i);
			//一定範囲外なら実行しない
			if (Math::GetDistance(block.GetPos(), checkEnemyPos) >= COLLISION_DISANCE)
				continue;

			VECTOR blockPos = block.GetPos();

			//当たったら
			if (Collision::Rect3D(
				VGet(checkEnemyPos.x, checkEnemyPos.y + enemySize.y / 2.0f, checkEnemyPos.z),
				checkEnemySize, blockPos, checkBgSize))
			{
				//手前と奥の壁
				if (checkEnemyPos.z < blockPos.z)
				{
					checkEnemyPos.z += (blockPos.z - blockSize.z) - (checkEnemyPos.z + enemySize.z);
					enemy.HitSetBlock();
				}
				else if (checkEnemyPos.z > blockPos.z)
				{
					checkEnemyPos.z += (blockPos.z + blockSize.z) - (checkEnemyPos.z - enemySize.z);
					enemy.HitSetBlock();
				}

				break;
			}
		}

		enemy.SetNextPos(checkEnemyPos);
	}
}

//配置ブロックとアイテム
void CollisionManager::CheckSetBlockToItem(ItemManager& itemManager, BlockManager& createBlock)
{
	//ブロックのサイズを取得
	VECTOR blockSize		= Vector::MakeVec(BLOCK_SIZE);
	//直径にする
	VECTOR checkBgSize		= VScale(blockSize, 2.0f);

	//サイズを取得
	VECTOR itemSize			= itemManager.GetItem(0).ITEM_SIZE;
	//直径にする
	VECTOR checkItemSize	= VScale(itemSize, 2.0f);

	int itemMaxNum			= itemManager.GetItemMaxNum();
	for (int itemNum = 0; itemNum < itemMaxNum; itemNum++)
	{
		Item& item = itemManager.GetItem(itemNum);
		//使用されていないもしくはプレイヤーが持っていたら実行しない
		if (!item.GetIsUse() || item.GetCatchFlag())
			continue;

		//座標を取得
		VECTOR checkItemPos = item.GetPos();
		//次の座標
		VECTOR itemNextPos = item.GetNextPos();
		
		//床/天井
		checkItemPos.y = itemNextPos.y;
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			//配置されていなかったら実行しない
			if (!createBlock.GetBlock(i).GetIsUse())
				continue;

			Block& block = createBlock.GetBlock(i);
			//一定範囲外なら実行しない
			if (Math::GetDistance(block.GetPos(), checkItemPos) >= COLLISION_DISANCE)
				continue;

			VECTOR blockPos = block.GetPos();

			//当たったら
			if (Collision::Rect3D(
				VGet(checkItemPos.x, checkItemPos.y, checkItemPos.z),
				checkItemSize, blockPos, checkBgSize))
			{
				//天井と床
				if (checkItemPos.y < blockPos.y)
				{
					checkItemPos.y += (blockPos.y - blockSize.y) - (checkItemPos.y + itemSize.y);
					item.HitCeiling();
				}
				else if (checkItemPos.y > blockPos.y)
				{
					checkItemPos.y += (blockPos.y + blockSize.y) - (checkItemPos.y - itemSize.y);
					item.HitGround(checkItemPos.y);
				}

				break;
			}
		}

		//壁X
		checkItemPos.x = itemNextPos.x;
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			//配置されていなかったら実行しない
			if (!createBlock.GetBlock(i).GetIsUse())
				continue;

			Block& block = createBlock.GetBlock(i);
			//一定範囲外なら実行しない
			if (Math::GetDistance(block.GetPos(), checkItemPos) >= COLLISION_DISANCE)
				continue;

			VECTOR blockPos = block.GetPos();

			//当たったら
			if (Collision::Rect3D(
				VGet(checkItemPos.x, checkItemPos.y, checkItemPos.z),
				checkItemSize, blockPos, checkBgSize))
			{
				//横の壁
				if (checkItemPos.x < blockPos.x)
				{
					checkItemPos.x += (blockPos.x - blockSize.x) - (checkItemPos.x + itemSize.x);

					item.Reflection();
				}
				else if (checkItemPos.x > blockPos.x)
				{
					checkItemPos.x += (blockPos.x + blockSize.x) - (checkItemPos.x - itemSize.x);

					item.Reflection(-1);
				}

				break;
			}
		}

		//壁Z
		checkItemPos.z = itemNextPos.z;
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			//配置されていなかったら実行しない
			if (!createBlock.GetBlock(i).GetIsUse())
				continue;

			Block& block = createBlock.GetBlock(i);
			//一定範囲外なら実行しない
			if (Math::GetDistance(block.GetPos(), checkItemPos) >= COLLISION_DISANCE)
				continue;

			VECTOR blockPos = block.GetPos();

			//当たったら
			if (Collision::Rect3D(
				VGet(checkItemPos.x, checkItemPos.y, checkItemPos.z),
				checkItemSize, blockPos, checkBgSize))
			{
				//手前と奥の壁
				if (checkItemPos.z < blockPos.z)
				{
					checkItemPos.z += (blockPos.z - blockSize.z) - (checkItemPos.z + itemSize.z);

					item.Reflection();
				}
				else if (checkItemPos.z > blockPos.z)
				{
					checkItemPos.z += (blockPos.z + blockSize.z) - (checkItemPos.z - itemSize.z);

					item.Reflection(-1);
				}

				break;
			}
		}

		item.SetNextPos(checkItemPos);
	}
}

//チュートリアルとプレイヤー
void CollisionManager::CheckTutorialToPlayer(UIManager& Ui, Player& player, CameraManager& cameraManager)
{
	//プレイヤーの座標を取得
	VECTOR playerPos = player.GetPos();

	for (int i = 0; i < Ui.GetTutorialNum(); i++)
	{
		//通常の表示にする
		Ui.SetTutorialFadeFlag(i, true);

		//チュートリアルの座標を取得
		VECTOR tutorialPos = Ui.GetTutorialPos(i);

		//カメラの座標を取得（プレイヤーとチュートリアルの前後間を調べる用））
		VECTOR cameraPos = cameraManager.GetPlayCamera().GetPos();

		//チュートリアル/プレイヤー座標とカメラの座標の距離をそれぞれ計算する
		float distanceTutoPos = Math::GetDistance(tutorialPos, cameraPos);
		float distancePlayerPos = Math::GetDistance(playerPos, cameraPos);

		//プレイヤーに一定距離近付いたら
		if (Math::GetDistance(tutorialPos, playerPos) <= TUTORIAL_FADE_DISTANCE)
		{
			//チュートリアルがプレイヤーの奥にあった場合は透かせない
			if (distanceTutoPos >= distancePlayerPos)
				continue;

			//チュートリアルをすかす
			Ui.SetTutorialFadeFlag(i, false);
		}
	}
}