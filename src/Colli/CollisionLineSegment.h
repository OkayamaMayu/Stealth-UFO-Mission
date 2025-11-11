#pragma once
#include"CollisionBase.h"
#include"../MyLib/MyLib/Collision.h"

class CollisionLineSegment :public CollisionBase {
private:
	LineSegment m_LineSegment;	//コリジョン情報
public:
	CollisionLineSegment();										//コンストラクタ
	void SetCollision(LineSegment set) { m_LineSegment = set; }	//コリジョン情報を設定
};
