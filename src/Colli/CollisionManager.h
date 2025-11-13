#pragma once
#include<vector>
using namespace std;

//#include"../Player/Player.h"
//#include"../enemy/EnemyManager.h"
//#include"../BackGround/BackGround.h"
//#include"../Item/ItemManager.h"
//#include"../Goal/Goal.h"
//#include"../Camera/CameraManager.h"
//#include"../enemy/UFO.h"
//#include"../Laser/LaserManager.h"
//#include"../Block/BlockManager.h"
//#include"../CheckPoint/CheckPointManager.h"
//#include"../UI/UIManager.h"
#include"CollisionBase.h"
#include"CollisionAABB.h"
#include"CollisionSphere.h"
#include"CollisionLineSegment.h"

//エネミーにアイテムが当たった時の処理
//true：反射	false：消す
const bool	ENEMY_TO_ITEM_Flag				= false;
const float COLLISION_DISANCE				= 20.0f;	//この距離内だけ判定する
const float TUTORIAL_FADE_DISTANCE			= 50.0f;	//チュートリアルの透かし範囲
const float CHECKPOINT_HIT_SCALE			= 3.0f;		//チェックポイントの大きめ当たり判定のスケール
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
	//bool CheckHit(Sphere collisionA, LineSegment collisionB) { return Collision::IsCollidingAABBToAABB(collisionA, collisionB); }
	//bool CheckHit(LineSegment collisionA, Sphere collisionB) { return Collision::IsCollidingAABBToAABB(collisionA, collisionB); }

	//修正軸を選ぶ
	COLLISION_AXIS SelectModifyingAxis(CollisionBase* baseA, CollisionBase* baseB);

	//--------------------------------

	//ブロックとプレイヤー
	//void CheckStageBlockToPlayer(Player& player, BackGround& block);
	////ブロックとエネミー1
	//void CheckStageBlockToEnemyType1(EnemyManager& enemyManager, BackGround& block);
	////ブロックとアイテム
	//bool CheckStageBlockToItem(ItemManager& itemManager, BackGround& block);

	////ブロックとリング
	//void CheckStageBlockToPlRing(Player& player, BackGround& block, BlockManager& createBlock);
	////ブロックとレーザー
	//void CheckStageBlockToLaser(LaserManager& laserManager, BackGround& block);

	////ゴールとプレイヤー
	//void CheckGoalToPayer(Player& player, Goal& goal);
	////チェックポイントとプレイヤー
	//void CheckCheckPointToPayer(CheckPointManager& checkPointManager, Player& player);

	////エネミーとプレイヤー
	//void CheckEnemyToPlayer(Player& player, EnemyManager& enemyManager);
	////エネミーとエネミー
	//void CheckEnemyToEnemy(EnemyManager& enemyManager);
	////エネミー1とアイテム
	//bool CheckEnemyType1ToItem(EnemyManager& enemyManager, ItemManager& itemManager);

	////ブロック配置可能位置
	//void CheckSetBlockPos(CameraManager& cameraManager, BlockManager& createBlock, BackGround& block, bool setMode);
	////配置ブロックとレーザー
	//void CheckSetBlockToLaser(LaserManager& laserManager, BlockManager& createBlock);
	////配置ブロックとプレイヤー
	//void CheckSetBlockToPlayer(Player& player, BlockManager& createBlock);
	////配置ブロックとエネミー1
	//void CheckSetBlockToEnemyType1(EnemyManager& enemyManager, BlockManager& createBlock);
	////配置ブロックとアイテム
	//void CheckSetBlockToItem(ItemManager& itemManager, BlockManager& createBlock);
};