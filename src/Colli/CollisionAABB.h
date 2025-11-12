#pragma once
#include"CollisionBase.h"
#include"../MyLib/MyLib/Collision.h"

class CollisionAABB :public CollisionBase {
private:
	AABB m_AABB;	//コリジョン情報
public:
	CollisionAABB();								//コンストラクタ
	void SetCollision(AABB set) { m_AABB = set; }	//コリジョン情報を設定
	AABB GetCollision() { return m_AABB; }			//コリジョン情報を取得
};
