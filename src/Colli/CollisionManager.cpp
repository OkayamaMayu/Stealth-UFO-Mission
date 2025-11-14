#include"DxLib.h"
#include"CollisionManager.h"
#include"../MyLib/MyLib.h"
#include <algorithm>

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

//コリジョンを登録
void CollisionManager::RegisterCollision(CollisionBase* base) {
	//要素を探す
	auto it = find(m_Collsion.begin(), m_Collsion.end(), base);

	//要素が見つかっていたら終了
	if (it != m_Collsion.end()) return;

	//要素を登録
	m_Collsion.push_back(base);
}

//コリジョンを解除
void CollisionManager::UnRegisterCollision(CollisionBase* base) {
	//要素を探す
	auto it = find(m_Collsion.begin(), m_Collsion.end(), base);

	//要素が見つかっていなかったら終了
	if (it == m_Collsion.end()) return;

	//要素を解除
	m_Collsion.erase(it);
}

//接触処理
void CollisionManager::Update() {
	for (int hitMain = 0;hitMain < m_Collsion.size(); hitMain++) {
		//当たり判定を実行しない
		if (!m_Collsion[hitMain]->IsCollision())continue;
		if (m_Collsion[hitMain]->GetOwner() == nullptr)continue;

		for (int hitSub = hitMain + 1;hitSub < m_Collsion.size(); hitSub++) {
			if (m_Collsion[hitSub]->GetOwner() == nullptr)continue;
			//当たり本体とkindが同じなら実行しない
			if (m_Collsion[hitMain]->GetKind() == m_Collsion[hitSub]->GetKind())continue;
			//当たり判定を実行しない
			if (!m_Collsion[hitSub]->IsCollision())continue;
			//一定距離の外側は以下計算させない
			if (Math::GetDistance(m_Collsion[hitMain]->GetOwner()->GetPos(), m_Collsion[hitSub]->GetOwner()->GetPos()) >= COLLISION_DISANCE)continue;
			
			//当たっているか調べる
			if (Collision(m_Collsion[hitMain], m_Collsion[hitSub])){
				//当たっていたら
				m_Collsion[hitMain]->HitCollision(m_Collsion[hitSub]);
				m_Collsion[hitSub]->HitCollision(m_Collsion[hitMain]);
			}
		}
	}
}

//========================================
//baseAのコリジョンタイプを識別
bool CollisionManager::Collision(CollisionBase* baseA, CollisionBase* baseB) {
	switch (baseA->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* main = static_cast<CollisionAABB*>(baseA);
		AABB collision = main->GetCollision();
		return Collision(collision, baseB);
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* main = static_cast<CollisionSphere*>(baseA);
		Sphere collision = main->GetCollision();
		return Collision(collision, baseB);
		break;
	}
	case TYPE_LINE: {
		CollisionLineSegment* main = static_cast<CollisionLineSegment*>(baseA);
		LineSegment collision = main->GetCollision();
		return Collision(collision, baseB);
		break;
	}
	default:
		break;
	}

	return false;
}
//baseBのコリジョンタイプを識別
bool CollisionManager::Collision(AABB collisionA, CollisionBase* baseB) {
	switch (baseB->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* sub = static_cast<CollisionAABB*>(baseB);
		AABB collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* sub = static_cast<CollisionSphere*>(baseB);
		Sphere collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}
	case TYPE_LINE: {
		CollisionLineSegment* sub = static_cast<CollisionLineSegment*>(baseB);
		LineSegment collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}
	default:
		break;
	}

	return false;
}
bool CollisionManager::Collision(Sphere collisionA, CollisionBase* baseB) {
	switch (baseB->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* sub = static_cast<CollisionAABB*>(baseB);
		AABB collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* sub = static_cast<CollisionSphere*>(baseB);
		Sphere collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}
	/*case TYPE_LINE: {
		CollisionLineSegment* sub = static_cast<CollisionLineSegment*>(baseB);
		LineSegment collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}*/
	default:
		break;
	}

	return false;
}
bool CollisionManager::Collision(LineSegment collisionA, CollisionBase* baseB) {
	switch (baseB->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* sub = static_cast<CollisionAABB*>(baseB);
		AABB collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}
	/*case TYPE_SPHERE: {
		CollisionSphere* sub = static_cast<CollisionSphere*>(baseB);
		Sphere collisionB = sub->GetCollision();
		return CheckHit(collisionA, collisionB);
		break;
	}*/
	default:
		break;
	}

	return false;
}

//修正軸を選ぶ
COLLISION_AXIS CollisionManager::SelectModifyingAxis(EditAxisFlag editAxisFlag, CollisionBase* baseA, CollisionBase* baseB) {
	//情報を取得
	VECTOR baseAPos = {};
	VECTOR baseASize = {};
	switch (baseA->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* hitAABB = static_cast<CollisionAABB*>(baseA);
		baseAPos = hitAABB->GetCollision().centerPos;
		baseASize = hitAABB->GetCollision().size;
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* hitSphere = static_cast<CollisionSphere*>(baseA);
		baseAPos = hitSphere->GetCollision().centerPos;
		baseASize.y = hitSphere->GetCollision().radius;
		break;
	}
	default:
		break;
	}

	//当たった先の情報
	VECTOR baseBPos = {};
	VECTOR baseBSize = {};
	switch (baseA->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* hitAABB = static_cast<CollisionAABB*>(baseB);
		baseBPos = hitAABB->GetCollision().centerPos;
		baseBSize = hitAABB->GetCollision().size;
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* hitSphere = static_cast<CollisionSphere*>(baseB);
		baseBPos = hitSphere->GetCollision().centerPos;
		baseBSize.y = hitSphere->GetCollision().radius;
		break;
	}
	default:
		break;
	}

	//当たった後の処理を分岐
	float differenceX = 0.0f;
	float differenceY = 0.0f;
	float differenceZ = 0.0f;
	//X軸の差を求める
	if (baseAPos.x < baseBPos.x) {
		differenceX = (baseAPos.x + baseASize.x) - (baseBPos.x - baseBSize.x);
	}
	else if (baseAPos.x > baseBPos.x) {
		differenceX = (baseBPos.x + baseBSize.x) - (baseAPos.x - baseASize.x);
	}
	//Y軸の差を求める
	if (baseAPos.y > baseBPos.y) {
		//床に当たった
		differenceY = (baseBPos.y + baseBSize.y) - (baseAPos.y - baseASize.y);
	}
	else if (baseAPos.y < baseBPos.y) {
		//天井に当たった
		differenceY -= (baseAPos.y + baseASize.y) - (baseBPos.y - baseBSize.y);
	}
	//Z軸の差を求める
	if (baseAPos.z < baseBPos.z) {
		differenceZ = (baseAPos.z + baseASize.z) - (baseBPos.z - baseBSize.z);
	}
	else if (baseAPos.z > baseBPos.z) {
		differenceZ = (baseBPos.z + baseBSize.z) - (baseAPos.z - baseASize.z);
	}

	if (!editAxisFlag.x)differenceX = INFINITY;
	if (!editAxisFlag.y)differenceY = INFINITY;
	if (!editAxisFlag.z)differenceZ = INFINITY;

	//小さいほうを修正軸として返す
	if (differenceZ < differenceX && differenceZ < differenceY)return AXIS_Z;
	if (differenceY < differenceX && differenceY < differenceZ)return AXIS_Y;
	return AXIS_X;
}

//========================================

////ブロックとリング
//void CollisionManager::CheckStageBlockToPlRing(Player& player, BackGround& block, BlockManager& createBlock)
//{
//	player.SetDrawRingFlag(true);
//
//	VECTOR playerSize = PLAYER_SIZE;
//
//	//点をモデルの中央に移動する
//	VECTOR checkPlayerPos = player.GetNextPos();
//	//直径にする
//	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
//	//縦高さが大きくなり過ぎたので戻す
//	checkPlayerSize.y /= 2.0f;
//
//	while (true)
//	{
//		//落とす
//		checkPlayerPos.y -= BLOCK_SIZE;
//
//		//リングが一定まで落ちた
//		if (checkPlayerPos.y <= DEATH_Y / 2)
//		{
//			//座標を適用
//			player.SetRingPos(checkPlayerPos);
//			//リングを非表示
//			player.SetDrawRingFlag(false);
//
//			return;
//		}
//
//		bool breakFlag = false;
//		for (int i = 0; i < block.GetBlockNum(); i++)
//		{
//			VECTOR blockPos = block.GetPos(i);
//
//			//一定距離の外側は以下計算させない
//			if (Math::GetDistance(blockPos, checkPlayerPos) >= BLOCK_SIZE*1.5f)
//				continue;
//			else
//			{
//				//ブロックの上辺
//				checkPlayerPos.y = blockPos.y + BLOCK_SIZE + 0.5f;
//				//リングの位置を更新
//				player.SetRingPos(checkPlayerPos);
//
//				breakFlag = true;
//
//				break;
//			}
//		}
//
//		//当たっていたら抜ける
//		if (breakFlag)
//			break;
//	}
//	
//	//配置ブロックの上にリングを出す
//	for (int i = 0; i < BLOCK_MAX_NUM; i++)
//	{
//		if (!createBlock.GetBlock(i).GetIsUse())
//			continue;
//
//		Block& block = createBlock.GetBlock(i);
//		if (Math::GetDistance(block.GetPos(), checkPlayerPos) >= COLLISION_DISANCE)
//			continue;
//
//		VECTOR blockPos = block.GetPos();
//
//		if (Math::GetDistance(blockPos, checkPlayerPos) <= BLOCK_SIZE * 1.5f)
//		{
//			//ブロックの上辺
//			checkPlayerPos.y = blockPos.y + BLOCK_SIZE;
//			//リングの位置を更新
//			player.SetRingPos(checkPlayerPos);
//		}
//	}
//}

////ブロックとレーザー
//void CollisionManager::CheckStageBlockToLaser(LaserManager& laserManager, BackGround& block)
//{
//	//それぞれの数を取得
//	int laserMaxNum = laserManager.GetLaserNum();
//	int blockNum = block.GetBlockNum();
//
//	for (int laserNum = 0; laserNum < laserMaxNum; laserNum++)
//	{
//		Laser& laser = laserManager.GetLaser(laserNum);
//		//レーザーの座標
//		//基準となる座標と伸ばした後の座標
//		VECTOR laserPos[2] = { laser.GetPos(),laser.GetPos() };
//		//レーザーの向き
//		VECTOR laserRot = laser.GetRot();
//
//		float laserMovePoint = laser.GetLaserLength();
//		while (true)
//		{
//			//伸ばす
//			Math::MatchSpecifiedNum(laserMovePoint, laser.LASER_MAX_LENGTH + 1.0f, BLOCK_SIZE/2);
//			VECTOR laserVec = Math::GetMoveVec(laserRot, laserMovePoint);
//			laserPos[1] = VAdd(laserPos[0], laserVec);
//
//			//レーザーが一定まで伸びた
//			if (laserMovePoint > laser.LASER_MAX_LENGTH)
//			{
//				//座標を適用
//				laser.SetNextPos(laserPos[1]);
//				laser.SetLaserLength(laserMovePoint);
//
//				break;
//			}
//
//			bool laserFlag = false;
//			for (int i = 0; i < blockNum; i++)
//			{
//				VECTOR blockPos = block.GetPos(i);
//
//				//一定距離の外側は以下計算させない
//				if (Math::GetDistance(blockPos, laserPos[1]) >= BLOCK_SIZE)
//					continue;
//				else
//				{
//					laserFlag = true;
//
//					//座標を適用
//					laserPos[1] = blockPos;
//					VECTOR nextPos = laserPos[1];
//					VECTOR laserVec = Math::GetMoveVec(laserRot.y, BLOCK_SIZE);
//					nextPos = VSub(nextPos, laserVec);
//					laser.SetNextPos(nextPos);
//
//					VECTOR lPos = laserPos[0];
//					laserMovePoint = Math::GetDistance(blockPos, lPos);
//
//					laser.SetLaserLength(laserMovePoint);
//
//					break;
//				}
//			}
//
//			if (laserFlag)
//				break;
//		}
//	}
//}

////ゴールとプレイヤー
//void CollisionManager::CheckGoalToPayer(Player& player, Goal& goal)
//{
//	//プレイヤー
//	VECTOR playerPos = player.GetPos();
//	VECTOR playerSize = PLAYER_SIZE;
//
//	//点をモデルの中央に移動する
//	VECTOR checkPlayerPos = playerPos;
//	//直径にする
//	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
//	//縦高さが大きくなり過ぎたので戻す
//	checkPlayerSize.y /= 2.0f;
//	//次の座標
//	VECTOR playerNextPos = player.GetNextPos();
//
//	//ゴール
//	VECTOR goalPos = goal.GetPos();
//	VECTOR goalSize = goal.GOAL_SIZE;
//
//	//点をモデルの中央に移動する
//	VECTOR checkgGoalPos = goalPos;
//	checkgGoalPos.y += goalSize.y / 2;
//	//直径にする
//	VECTOR checkGoalSize = VScale(goalSize, 2.0f);
//	//縦高さが大きくなり過ぎたので戻す
//	checkGoalSize.y /= 2.0f;
//
//	//一定距離の外側は以下計算させない
//	if (Math::GetDistance(checkPlayerPos, checkgGoalPos) >= COLLISION_DISANCE)
//		return;
//
//	bool goalFlag = false;
//	//Y
//	checkPlayerPos.y = playerNextPos.y;
//	//当たったら
//	if (Collision::Rect3D(
//		VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
//		checkPlayerSize, checkgGoalPos, checkGoalSize))
//	{
//		goalFlag = true;
//
//		//天井と床
//		if (checkPlayerPos.y < checkgGoalPos.y)
//		{
//			checkPlayerPos.y += checkgGoalPos.y - (checkPlayerPos.y + playerSize.y);
//			player.HitCeiling();
//		}
//
//		else if (checkPlayerPos.y > checkgGoalPos.y)
//		{
//			checkPlayerPos.y += (checkgGoalPos.y + goalSize.y / 2) - checkPlayerPos.y;
//			player.HitGround();
//		}
//	}
//
//	//Z
//	checkPlayerPos.z = playerNextPos.z;
//	//当たったら
//	if (Collision::Rect3D(
//		VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
//		checkPlayerSize, checkgGoalPos, checkGoalSize))
//	{
//		goalFlag = true;
//
//		//手前と奥の壁
//		if (checkPlayerPos.z < checkgGoalPos.z)
//		{
//			checkPlayerPos.z += (checkgGoalPos.z - goalSize.z) - (checkPlayerPos.z + playerSize.z);
//		}
//
//		else if (checkPlayerPos.z > checkgGoalPos.z)
//		{
//			checkPlayerPos.z += (checkgGoalPos.z + goalSize.z) - (checkPlayerPos.z - playerSize.z);
//		}
//	}
//
//	//X
//	checkPlayerPos.x = playerNextPos.x;
//	//当たったら
//	if (Collision::Rect3D(
//		VGet(checkPlayerPos.x, checkPlayerPos.y + playerSize.y / 2.0f, checkPlayerPos.z),
//		checkPlayerSize, checkgGoalPos, checkGoalSize))
//	{
//		goalFlag = true;
//
//		//横の壁
//		if (checkPlayerPos.x < checkgGoalPos.x)
//		{
//			checkPlayerPos.x += (checkgGoalPos.x - goalSize.x) - (checkPlayerPos.x + playerSize.x);
//		}
//
//		else if (checkPlayerPos.x > checkgGoalPos.x)
//		{
//			checkPlayerPos.x += (checkgGoalPos.x + goalSize.x) - (checkPlayerPos.x - playerSize.x);
//		}
//	}
//
//	//当たっていたら（ゴールしていたら）
//	if (goalFlag)
//	{
//		goal.HitGoal();
//
//		CEffekseerCtrl::Request(EFFECT_GOAL, goalPos, false);
//
//		Sound::Play(SE_GOLA_HIT);
//	}
//
//	//NextPosを更新して最終は地面で判定
//	player.SetNextPos(checkPlayerPos);
//}
////チェックポイントとプレイヤー
//void CollisionManager::CheckCheckPointToPayer(CheckPointManager& checkPointManager, Player& player)
//{
//	//プレイヤー座標を取得
//	VECTOR playerPos = player.GetPos();
//	//プレイヤーの向きを取得
//	VECTOR playerRot = player.GetRot();
//
//	//チェックポイントの数を取得
//	int checkPointNum = checkPointManager.GetCheckPointNum();
//	//チェックポイントのサイズを取得
//	float checkPointSize = checkPointManager.GetModelSize() * CHECKPOINT_HIT_SCALE;
//
//	for (int i = 0; i < checkPointNum; i++)
//	{
//		//起動済みなら実行しない
//		if (checkPointManager.GetCheckPoint(i).GetIsActive())
//			continue;
//
//		//チェックポイントを取得
//		CheckPoint checkPoint = checkPointManager.GetCheckPoint(i);
//		//チェックポイントの座標を取得
//		VECTOR checkPointPos = checkPoint.GetPos();
//
//		//当たっていたら
//		if (Math::GetDistance(checkPointPos, playerPos) <= checkPointSize)
//			if (checkPointManager.IsHit(i, playerPos, playerRot.y))
//				return;
//	}
//}

////配置ブロックとレーザー
//void CollisionManager::CheckSetBlockToLaser(LaserManager& laserManager, BlockManager& createBlock)
//{
//	//数情報
//	int laserNum = laserManager.GetLaserNum();
//	int blockNum = BLOCK_MAX_NUM;
//
//	//配置していない状態では実行しないようにする
//	bool blockFlag = false;
//	for (int i = 0; i < blockNum; i++)
//	{
//		//配置されていたらフラグを変更
//		if (createBlock.GetBlock(i).GetIsUse())
//			blockFlag = true;
//	}
//
//	//配置されていなかったら実行しない
//	if (!blockFlag)
//		return;
//
//	for (int laNum = 0; laNum < laserNum; laNum++)
//	{
//		Laser& laser = laserManager.GetLaser(laNum);
//		//基準となる座標と伸ばした後の座標
//		VECTOR laserPos[2] = { laser.GetPos(),laser.GetPos() };
//		//レーザーの向き
//		VECTOR laserRot = laser.GetRot();
//
//		//レーザーの長さ
//		float laserMovePoint = laser.LASER_SIZE.z;
//		while (true)
//		{
//			//伸ばす
//			Math::MatchSpecifiedNum(laserMovePoint, laser.GetLaserLength() + 1.0f, 1.0f);
//			//向きから移動量を計算
//			VECTOR laserVec = Math::GetMoveVec(laserRot, laserMovePoint);
//			//伸ばす
//			laserPos[1] = VAdd(laserPos[0], laserVec);
//
//			//レーザーが一定まで伸びた
//			if (laserMovePoint > laser.GetLaserLength())
//			{
//				//座標を適用
//				laser.SetNextPos(laserPos[1]);
//				laser.SetLaserLength(laserMovePoint);
//
//				break;
//			}
//
//			bool posSetFlag = false;
//			for (int i = 0; i < blockNum; i++)
//			{
//				//ブロックが配置されていなかったら実行しない
//				if (!createBlock.GetBlock(i).GetIsUse())
//					continue;
//
//				VECTOR blockPos = createBlock.GetBlock(i).GetPos();
//
//				//一定距離の外側は以下計算させない
//				if (Math::GetDistance(blockPos, laserPos[1]) >= BLOCK_SIZE)
//					continue;
//				else
//				{
//					//座標を適用
//					laser.SetNextPos(laserPos[1]);
//					laser.SetLaserLength(laserMovePoint);
//
//					posSetFlag = true;
//
//					break;
//				}
//			}
//
//			//配置できるなら抜ける
//			if (posSetFlag)
//				break;
//		}
//	}
//}