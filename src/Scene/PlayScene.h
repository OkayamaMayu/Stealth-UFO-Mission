#pragma once
#include"DxLib.h"

#include"../Camera/CameraManager.h"
#include"../Player/Player.h"
#include"../BackGround/BackGround.h"
#include"../BackGround/sky.h"
#include"../enemy/EnemyManager.h"
#include"../Item/ItemManager.h"
#include"../Pause/Pause.h"
#include"../Goal/Goal.h"
#include"../SafetyArea/SafetyAreaManager.h"
#include"../enemy/UFO.h"
#include"../Laser/LaserManager.h"
#include"../Block/BlockManager.h"
#include"../UI/UIManager.h"
#include"../CheckPoint/CheckPointManager.h"
#include"../LoadStageData/LoadStageData.h"

class PlayScene
{
public:
	enum PLAY_SCENE
	{
		PLAY_SCENE_INIT,
		PLAY_SCENE_LOAD,
		PLAY_SCENE_LOOP,
		PLAY_SCENE_END,

		PLAY_SCENE_NUM
	};
private:
	enum IMAGE_ID
	{
		FOUND_FILTER,		//発見時のフィルター
		SET_BLOCK_MODE,		//ブロック配置モードのフィルター
		SET_BLOCK_GAGE_1,	//ブロック配置ゲージ1
		SET_BLOCK_GAGE_2,	//ブロック配置ゲージ2
		SET_BLOCK_GAGE_3,	//ブロック配置ゲージ3
		BLACK_FILTER,		//通常環境フィルター

		IMAGE_NUM
	};

	enum PLAY_PROGRESS
	{
		PLAY_MAKE,		//画面の作成
		PLAY_GAME,		//操作待機
		//PLAY_SELECT,	//ゲームモード選択
	};

	const string IMAGE_PATH[IMAGE_NUM] =
	{
		"data/system/foundFilter.png",
		"data/system/setBlockMode.png",
		"data/system/blockGage_289,105.png",
		"data/system/blockGage2_289,105.png",
		"data/system/blockGage3_188,50.png",
		"data/system/blackFilter.png",
	};

	const COLOR_F		INIT_LIGHT_COLOR				= { 1.0f,1.0f,1.0f,0.0f };	//標準ライトのスペキュラカラー
	const VECTOR		BLOCK_GAGE_IMAGE_SIZE			= { 188.0f,50.0f,0.0f };	//ブロック配置ゲージの画像サイズ
	const float			SCENE_WAIT_TIME					= 0.7f;						//ゲームオーバーの待機
	const float			SET_BLOCKMODE_FILTER_MAX_SIZE	= 2.0f;						//ブロック設置モードのフィルター最大サイズ
	const float			SKY_FILTER_SIZE					= 500.0f;					//天球をなじませるフィルターのサイズ
	const float			SPEED_RATIO						= 20.0f;					//速度の割合
	const float			SETBLOCK_SIZE_MIN_SPEED			= 0.5f;						//配置ブロックフィルターサイズの変更最低速度
	const float			SETBLOCK_SIZE_MAX_SPEED			= 2.0f;						//配置ブロックフィルターサイズの変更最高速度
	const float			AUTO_CLOSE_FPSCAMERA_TIME		= 0.2f;						//一人称カメラの自動的に閉じるまでの時間
	const int			FADEIN_SPEED					= 5;						//フェードインの速度
	const int			SOUND_FADEOUT_SPEED				= 10;						//サウンドのフェードアウトの速度
	const int			SOUND_FADEIN_SPEED				= 10;						//サウンドのフェードインの速度
	const int			BLOCK_GAGE_IMAGE_NUM[3]			= { 10,2,5 };				//ブロック配置ゲージの画像数：総数、X方向の数、Y方向の数
	const int			SKY_FILTER_ALPHA				= 30;						//天球をなじませるフィルターの透明度
	const int			SKY_FILTER_POLYGON_NUM			= 16;						//天球をなじませるフィルターのポリゴン数
	const int			SOUND_GAMEOVER_FADEOUT_SPEED	= 5;						//ゲームオーバー時のサウンドフェードアウト速度
	const int			SOUND_PAUSE_VOLUME = 70;	//ポーズ画面時の音量
	
private:
	BackGround			backGraund;				//背景
	Sky					sky;					//天球
	BlockManager		block;					//配置ブロック
	CameraManager		cameraMan;				//カメラマネージャー
	CheckPointManager	checkPoint;				//チェックポイント
	EnemyManager		enemy;					//エネミー
	UFO					ufo;					//UFO
	Goal				goal;					//ゴール
	ItemManager			item;					//アイテム
	LaserManager		laser;					//レーザー
	LoadStageData		loadStageData;			//ステージデータ
	Pause				pause;					//ポーズ
	Player				player;					//プレイヤー処理
	SafetyAreaManager	safetyArea;				//安置影
	UIManager			UI;						//UI

	PLAY_SCENE			m_SceneID;				//シーン保存
	PLAY_PROGRESS		m_Progress;				//進行度
	float				m_fSceneWaitCount;		//場面以降の一旦停止
	float				m_fSetBlockModeSize;	//ブロック配置モードのフィルターサイズ
	float				m_fCloseCount;			//一人称カメラを閉じるカウント
	bool				m_ClearFlag;			//クリア(true)　ゲームオーバー(false)
	bool				m_NextFlag;				//次の画面へ
	bool				m_CloseFlag;			//一人称カメラを閉じるフラグ
	bool				m_PauseFlag;			//ポーズ画面になっているフラグ
	int					m_iHandle[IMAGE_NUM];	//システムの画像ハンドル
	int					m_iSetBlockGage[10];	//ブロック配置ゲージ
	int					m_iEffectHandle;		//常時設定用エフェクトハンドル
	int					m_iBlockNumSub;			//ブロック配置可能数の保存

public:
	PlayScene();
	~PlayScene();

	int Loop();
	void Draw();
	void Fin();

	//クリア分岐フラグを取得
	bool GetClearFlag() { return m_ClearFlag; }
	//現在のシーンを設定
	void SetScene(PLAY_SCENE set) { m_SceneID = set; }

private:
	void Init();
	void Load();
	void Start();
	void Step();
	void play();

	//-------------------

	//ポーズ画面
	bool PauseStep();
	//アイテムをつかむ処理
	void ItemCatch();
	//当たり判定
	void Collision();

};