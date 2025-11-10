#include"CollisionAABB.h"

CollisionAABB::CollisionAABB()
{
	//コリジョンタイプを設定
	m_CollisionType = TYPE_AABB; 
	//コリジョン情報を初期化
	m_AABB = {};
}