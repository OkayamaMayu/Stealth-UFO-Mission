#pragma once
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
public:
	//ブロックとプレイヤー
	static void CheckStageBlockToPlayer(Player& player, BackGround& block);
	//ブロックとエネミー1
	static void CheckStageBlockToEnemyType1(EnemyManager& enemyManager, BackGround& block);
	//ブロックとアイテム
	static bool CheckStageBlockToItem(ItemManager& itemManager, BackGround& block);

	//ブロックとカメラ
	static void CheckStageBlockToCamera(CameraManager& cameraManager, BackGround& block);
	//ブロックとリング
	static void CheckStageBlockToPlRing(Player& player, BackGround& block, BlockManager& createBlock);
	//ブロックとレーザー
	static void CheckStageBlockToLaser(LaserManager& laserManager, BackGround& block);

	//ゴールとプレイヤー
	static void CheckGoalToPayer(Player& player, Goal& goal);
	//チェックポイントとプレイヤー
	static void CheckCheckPointToPayer(CheckPointManager& checkPointManager, Player& player);

	//エネミーとプレイヤー
	static void CheckEnemyToPlayer(Player& player, EnemyManager& enemyManager);
	//エネミーとエネミー
	static void CheckEnemyToEnemy(EnemyManager& enemyManager);
	//エネミー1とアイテム
	static bool CheckEnemyType1ToItem(EnemyManager& enemyManager, ItemManager& itemManager);

	//レーザーとプレイヤー
	static void CheckLaserToPlayer(Player& player, LaserManager& laserManager, EnemyManager& enemyManager);

	//カメラとUFO
	static void CheckUfoToCamera(UFO& cUfo, CameraManager& cameraMan);
	//カメラとチェックポイント
	static void CheckCheckPointToCamera(CheckPointManager& checkPointManager, CameraManager& camera, Player& player);

	//ブロック配置可能位置
	static void CheckSetBlockPos(CameraManager& cameraManager, BlockManager& createBlock, BackGround& block, bool setMode);
	//配置ブロックとレーザー
	static void CheckSetBlockToLaser(LaserManager& laserManager, BlockManager& createBlock);
	//配置ブロックとプレイヤー
	static void CheckSetBlockToPlayer(Player& player, BlockManager& createBlock);
	//配置ブロックとエネミー1
	static void CheckSetBlockToEnemyType1(EnemyManager& enemyManager, BlockManager& createBlock);
	//配置ブロックとアイテム
	static void CheckSetBlockToItem(ItemManager& itemManager, BlockManager& createBlock);

	//チュートリアルとプレイヤー
	static void CheckTutorialToPlayer(UIManager& Ui,Player& player, CameraManager& cameraManager);
};