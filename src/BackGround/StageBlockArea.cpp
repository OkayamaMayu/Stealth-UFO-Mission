#include"StageBlockArea.h"

void StageBlockArea::Init(AABB set) {
	Init();

	//コリジョン情報の設定
	m_Collision.SetCollision(set);
	m_Collision.SetKind(KIND_AREA);
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase* hitCollision) {Hit(hitCollision); });
	m_Collision.SetOwner(this);
	//情報を登録
	CollisionManager::GetInstance()->RegisterCollision(&m_Collision);
}
void StageBlockArea::Init() {
	m_StageBlock.clear();
	m_StageBlock.shrink_to_fit();
}

void StageBlockArea::Load(int originHandle[]) {
	for (auto& stageBlock : m_StageBlock) {
		//空気ブロックなら実行しない
		if (stageBlock.GetBlockType() == StageBlock::BLOCK_AIR) continue;
		//オリジナルモデルをコピー
		stageBlock.Load(originHandle[stageBlock.GetBlockType()]);
	}
}

void StageBlockArea::Start() {
	for (auto& stageBlock : m_StageBlock) {
		stageBlock.Start();
	}
}

void StageBlockArea::Step() {
	for (auto& stageBlock : m_StageBlock) {
		stageBlock.Step();
	}
}

void StageBlockArea::Draw() {
	for (auto& stageBlock : m_StageBlock) {
		//空気ブロックは実行しない
		if (stageBlock.GetBlockType() == StageBlock::BLOCK_AIR)continue;
		stageBlock.Draw();
	}
}

void StageBlockArea::Fin() {
	for (auto& stageBlock : m_StageBlock) {
		//空気ブロックは実行しない
		if (stageBlock.GetBlockType() == StageBlock::BLOCK_AIR)continue;
		stageBlock.Fin();
	}

	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

//当たった処理
void StageBlockArea::Hit(CollisionBase* hitCollision) {
	for (auto& stageBlock : m_StageBlock) {
		//区画内のブロックの当たり判定
		AABB stageBlockCollison = stageBlock.GetCollision().GetCollision();
		//当たり判定を実行しない
		if (!stageBlock.GetCollision().IsCollision())continue;

		//当たっていなければ実行しない
		if (!HitBlock(stageBlockCollison, hitCollision))continue;
		//CollisionAABB stageBlockIt = stageBlock.GetCollision();
		CollisionBase* stageCollison = &stageBlock.GetCollision();
		//当たっていたら
		hitCollision->HitCollision(stageCollison);
	}
}

//当たり判定
bool StageBlockArea::HitBlock(AABB stageBlockCollison, CollisionBase* hitCollision) {
	switch (hitCollision->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* sub = static_cast<CollisionAABB*>(hitCollision);
		AABB collision = sub->GetCollision();
		return Collision::IsCollidingAABBToAABB(stageBlockCollison, collision);
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* sub = static_cast<CollisionSphere*>(hitCollision);
		Sphere collision = sub->GetCollision();
		return Collision::IsCollidingAABBToSphere(stageBlockCollison, collision);
		break;
	}
	case TYPE_LINE: {
		CollisionLineSegment* sub = static_cast<CollisionLineSegment*>(hitCollision);
		LineSegment collision = sub->GetCollision();
		return Collision::IsCollidingAABBToLineSegment(stageBlockCollison, collision);
		break;
	}
	default:
		break;
	}

	return false;
}