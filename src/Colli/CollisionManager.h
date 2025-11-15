#pragma once
#include<vector>
using namespace std;

#include"CollisionBase.h"
#include"CollisionAABB.h"
#include"CollisionSphere.h"
#include"CollisionLineSegment.h"

const float COLLISION_DISANCE				= 20.0f;	//この距離内だけ判定する
const float TUTORIAL_FADE_DISTANCE			= 50.0f;	//チュートリアルの透かし範囲
const float CREATE_BLOCK_HIT_SCALE			= 3.0f;		//チェックポイントの大きめ当たり判定のスケール
const float CREATE_BLOCK_STAGE_HIT_SCALE	= 5.0f;		//生成ブロックとステージブロックの当たるスケール

class CollisionManager
{
private:
	static CollisionManager*	m_Instance;			//インスタンス
	vector<CollisionBase*>		m_Collsion;			//コリジョン情報

public:
	static void					Create();			//インスタンスの生成
	static void					Destroy();			//インスタンスの削除
	static CollisionManager*	GetInstance();		//インスタンスの取得

	//--------------------------------

	void RegisterCollision(CollisionBase* base);	//コリジョンを登録
	void UnRegisterCollision(CollisionBase* base);	//コリジョンを解除

	void Update();									//接触処理

	//当たり判定の識別
	//baseAのコリジョンタイプを識別
	bool Collision(CollisionBase* baseA, CollisionBase* baseB);
	//baseBのコリジョンタイプを識別
	bool Collision(AABB collisionA, CollisionBase* baseB);
	bool Collision(Sphere collisionA, CollisionBase* baseB);
	bool Collision(LineSegment collisionA, CollisionBase* baseB);


	//当たり判定
	bool CheckHit(AABB collisionA, AABB collisionB) { return Collision::IsCollidingAABBToAABB(collisionA, collisionB) ; }
	bool CheckHit(AABB collisionA, Sphere collisionB) { return Collision::IsCollidingAABBToSphere(collisionA, collisionB); }
	bool CheckHit(Sphere collisionA, AABB collisionB) { return Collision::IsCollidingAABBToSphere(collisionB, collisionA); }
	bool CheckHit(AABB collisionA, LineSegment collisionB) { return Collision::IsCollidingAABBToLineSegment(collisionA, collisionB); }
	bool CheckHit(LineSegment collisionA, AABB collisionB) { return Collision::IsCollidingAABBToLineSegment(collisionB, collisionA); }
	bool CheckHit(Sphere collisionA, Sphere collisionB) { return Collision::IsCollidingSphereToSphere(collisionA, collisionB); }
	bool CheckHit(Sphere collisionA, LineSegment collisionB) { return Collision::IsCollidingSphereToLineSegment(collisionA, collisionB); }
	bool CheckHit(LineSegment collisionA, Sphere collisionB) { return Collision::IsCollidingSphereToLineSegment(collisionB, collisionA); }

	//修正軸を選ぶ
	//引数：修正可能軸フラグ構造体, 修正する対象, 当たった相手
	COLLISION_AXIS SelectModifyingAxis(EditAxisFlag editAxisFlag, CollisionBase* baseA, CollisionBase* baseB);
};