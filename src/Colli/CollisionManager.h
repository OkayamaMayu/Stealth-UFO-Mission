#pragma once
#include<vector>
using namespace std;

#include"../Player/Player.h"
#include"../enemy/EnemyManager.h"
#include"../BackGround/BackGround.h"
#include"../Item/ItemManager.h"
#include"../Goal/Goal.h"
#include"../Camera/CameraManager.h"
#include"../enemy/UFO.h"
#include"../Laser/LaserManager.h"
#include"../Block/BlockManager.h"
#include"../CheckPoint/CheckPointManager.h"
#include"../UI/UIManager.h"

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
	static CollisionManager* m_Instance;		//インスタンス

public:
	static void					Create();		//インスタンスの生成
	static void					Destroy();		//インスタンスの削除
	static CollisionManager*	GetInstance();	//インスタンスの取得

	//--------------------------------

	//ブロックとプレイヤー
	void CheckStageBlockToPlayer(Player& player, BackGround& block);
	//ブロックとエネミー1
	void CheckStageBlockToEnemyType1(EnemyManager& enemyManager, BackGround& block);
	//ブロックとアイテム
	bool CheckStageBlockToItem(ItemManager& itemManager, BackGround& block);

	//ブロックとカメラ
	void CheckStageBlockToCamera(CameraManager& cameraManager, BackGround& block);
	//ブロックとリング
	void CheckStageBlockToPlRing(Player& player, BackGround& block, BlockManager& createBlock);
	//ブロックとレーザー
	void CheckStageBlockToLaser(LaserManager& laserManager, BackGround& block);

	//ゴールとプレイヤー
	void CheckGoalToPayer(Player& player, Goal& goal);
	//チェックポイントとプレイヤー
	void CheckCheckPointToPayer(CheckPointManager& checkPointManager, Player& player);

	//エネミーとプレイヤー
	void CheckEnemyToPlayer(Player& player, EnemyManager& enemyManager);
	//エネミーとエネミー
	void CheckEnemyToEnemy(EnemyManager& enemyManager);
	//エネミー1とアイテム
	bool CheckEnemyType1ToItem(EnemyManager& enemyManager, ItemManager& itemManager);

	//レーザーとプレイヤー
	void CheckLaserToPlayer(Player& player, LaserManager& laserManager, EnemyManager& enemyManager);

	//カメラとUFO
	void CheckUfoToCamera(UFO& cUfo, CameraManager& cameraMan);
	//カメラとチェックポイント
	void CheckCheckPointToCamera(CheckPointManager& checkPointManager, CameraManager& camera, Player& player);

	//ブロック配置可能位置
	void CheckSetBlockPos(CameraManager& cameraManager, BlockManager& createBlock, BackGround& block, bool setMode);
	//配置ブロックとレーザー
	void CheckSetBlockToLaser(LaserManager& laserManager, BlockManager& createBlock);
	//配置ブロックとプレイヤー
	void CheckSetBlockToPlayer(Player& player, BlockManager& createBlock);
	//配置ブロックとエネミー1
	void CheckSetBlockToEnemyType1(EnemyManager& enemyManager, BlockManager& createBlock);
	//配置ブロックとアイテム
	void CheckSetBlockToItem(ItemManager& itemManager, BlockManager& createBlock);

	//チュートリアルとプレイヤー
	void CheckTutorialToPlayer(UIManager& Ui,Player& player, CameraManager& cameraManager);
};