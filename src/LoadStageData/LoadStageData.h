#pragma once
#include "DxLib.h"

const int MAP_CHIP_SIZE = 32;								//マップチップのサイズ
const int MAP_NUM_X		= (1280 * 3) / MAP_CHIP_SIZE;		//横の数
const int MAP_NUM_Y		= (720 * 5) / MAP_CHIP_SIZE;		//縦の数
const int MAP_NUM_Z		= 7;								//奥行の数

class LoadStageData
{
public:
	struct SaveData
	{
		VECTOR	pos[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X];				//ブロック
		int		type[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X];				//ブロックの種類
		VECTOR	posOdject[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X];			//キャラやエネミー
		VECTOR	enemyPosMovrPoint[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X];	//エネミーの移動先
		float	enemyRot[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X][2];		//エネミーの向き（識別とポイント1,ポイント2）
		int		typeOdject[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X];		//物体の種類
		int		blockNum;											//ブロックの数
		int		enemyNum;											//エネミーの数(総数)
		int		enemy1Num;											//エネミーの数(総数)
		int		enemy2Num;											//エネミーの数(総数)
		int		itemNum;											//アイテムの数
		int		safetyAreaNum;										//影(安置)の数
		VECTOR	playerRot;											//プレイヤーの角度
		int		enemyPointNum;										//エネミーの通過ポイント数
		int		laserNum;											//レーザーの数
		float	odjectRot[MAP_NUM_Z][MAP_NUM_Y][MAP_NUM_X];			//レーザーの向き
		int		checkPointNum;										//チェックポイントの数
	};
	//設置するアイテム
	enum SET_BLOCK
	{
		BLOCK_NORMAL,		//通常ブロック
		BLOCK_AIR,			//空気ブロック
		BLOCK_WALL,			//壁
		BLOCK_4,

		BLOCK_PL,			//プレイヤー初期位置
		BLOCK_ENEMY1,		//エネミータイプ１
		BLOCK_ENEMY2,		//エネミータイプ２
		BLOCK_ITEM,			//アイテム
		BLOCK_SAFETYAREA,		//安置
		BLOCK_GOAL,			//ゴール

		BLOCK_ENEMY_POINT,	//エネミーの通過地点

		BLOCK_LASER,		//レーザー

		BLOCK_CAMERA_POINT,	//カメラ通貨ポイント

		BLOCK_CHECK_POINT,	//チェックポイント

		BLOCK_NUM
	};
	//少し浮かせてZファイティングを回避
	const float SAFETYAREA_UP_HEIGHT = 0.1f;
	//チェックポイントをブロックにぶつからないように配置する
	const float CHECKPOINT_UP_HEIGHT = 0.5f;

private:
	SaveData	setData;					//読み書きデータ
	VECTOR*		m_vBlockPos;				//ブロックの座標
	VECTOR		m_vPlPos;					//プレイヤーの位置
	VECTOR		m_vPlRot;					//プレイヤーの回転
	VECTOR*		m_vEnemyType1Pos;			//エネミーの座標
	VECTOR*		m_vEnemyType1MovePos;		//移動先
	VECTOR*		m_vItemPos;					//アイテムの座標
	VECTOR*		m_vSafetyAreaPos;			//安置の座標
	VECTOR		m_vGoalPos;					//ゴール位置
	VECTOR*		m_vEenemyPointPos;			//通過ポイント
	VECTOR*		m_vLaserPos;				//レーザー座標
	VECTOR		m_vCameraPoint[10];			//カメラの通過ポイント
	VECTOR*		m_vCheckPointPos;			//チェックポイントの座標
	float*		m_fEnemyType1Rot[2];		//エネミーの方向
	float*		m_fLaserRot;				//レーザーの向き
	float*		m_fCheckPointRot;			//チェックポイントの向き
	int			m_iSetNumCount[BLOCK_NUM];	//設定用カウント変数
	int			m_iBlockNum;				//ブロックの数
	int*		m_iBlockType;				//ブロックのタイプ
	int			m_iEnemyNum;				//エネミーの数
	int			m_iEnemyType1Num;			//エネミーの数
	int			m_iItemNum;					//アイテムの数
	int			m_iSafetyAreaNum;			//安置の数
	int			m_iEnemyPointNum;			//通過ポイントの数
	int			m_iLaserNum;				//レーザーの数
	int			m_iCameraPointNum;			//カメラ通過ポイントの数
	int			m_iCheckPointNum;			//チェックポイントの数

public:
	//初期化
	void		Init();
	//読み込み
	void		Load();
	//終了処理
	void		Fin();

	//ブロックの数を取得
	int			GetBlockNum() { return m_iBlockNum; }
	//ブロックのタイプを取得
	int			GetBlockType(int ID) { return m_iBlockType[ID]; }
	//ブロックの座標を取得
	VECTOR		GetBlockPos(int ID) { return m_vBlockPos[ID]; }
	//読み取ったプレイヤーのスポーン位置を取得
	VECTOR		GetPlayerSpawnPos() { return m_vPlPos; }
	//読み取ったプレイヤーのスポーン向きを取得
	VECTOR		GetPlayerSpawnRot() { return m_vPlRot; }
	//読み取ったエネミーの数を取得
	int			GetEnemyType1Num() { return m_iEnemyType1Num; }
	//読み取ったエネミーのスポーン位置
	VECTOR		GetEnemyType1SpawnPos(int ID) { return m_vEnemyType1Pos[ID]; }
	//読み取ったエネミーの移動先
	VECTOR		GetEnemyType1MovePos(int ID) { return m_vEnemyType1MovePos[ID]; }
	//読み取ったエネミーの待機角度
	float		GetEnemyType1Rot(int ID, int stGo) { return m_fEnemyType1Rot[stGo][ID]; }
	//読み取ったアイテムの数を取得
	int			GetItemNum() { return m_iItemNum; }
	//読み取ったアイテムのスポーン位置
	VECTOR		GetItemSpawnPos(int ID) { return m_vItemPos[ID]; }
	//読み取った安置の数を取得
	int			GetSafetyAreaNum() { return m_iSafetyAreaNum; }
	//読み取った安置のスポーン位置
	VECTOR		GetSafetyAreaSpawnPos(int ID) { return m_vSafetyAreaPos[ID]; }
	//読み取ったゴールの位置
	VECTOR		GetGoalPos() { return m_vGoalPos; }
	//セットデータを確保
	SaveData	GetSetData() { return setData; }
	//エネミー通過ポイントの数を取得
	int			GetEnemyPointNum() { return m_iEnemyPointNum; }
	//エネミー通過ポイントの座標を取得
	VECTOR		GetEnemyPointPos(int ID) { return m_vEenemyPointPos[ID]; }
	//レーザーの数を取得
	int			GetLaserNum() { return m_iLaserNum; }
	//レーザーの座標を取得
	VECTOR		GetLaserPos(int ID) { return m_vLaserPos[ID]; }
	//レーザーの向きを取得
	float		GetLaserRot(int ID) { return m_fLaserRot[ID]; }
	//カメラ通過ポイントの座標を取得
	VECTOR		GetCametaPoint(int ID) { return m_vCameraPoint[ID]; }
	//カメラ通過ポイントの数を取得
	int			GetCameraPointNum() { return m_iCameraPointNum; }
	//チェックポイントの数を取得
	int			GetCheckPointNum() { return m_iCheckPointNum; }
	//チェックポイントの座標を取得
	VECTOR		GetCheckPointPos(int ID) { return m_vCheckPointPos[ID]; }
	//チェックポイントの向きを取得
	float		GetCheckPointRot(int ID) { return m_fCheckPointRot[ID]; }

private:
	//数値の初期化
	void		NumericInit();
	//動的確保
	void		DynamicAllocation();
	//ステージ読み込み
	void		StageLoad();
	//ブロックの読み込み
	void		BlockLoad();
	//オブジェクトの読み込み
	void		ObjectLoad(int iX, int iY, int iZ);
};