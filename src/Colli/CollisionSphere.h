#pragma once
#include"CollisionBase.h"
#include"../MyLib/MyLib/Collision.h"

class CollisionSphere :public CollisionBase {
private:
	Sphere m_Sphere;	//コリジョン情報
public:
	CollisionSphere();									//コンストラクタ
	void SetCollision(Sphere set) { m_Sphere = set; }	//コリジョン情報を設定
	Sphere GetCollision() { return m_Sphere; }			//コリジョン情報を取得
};
