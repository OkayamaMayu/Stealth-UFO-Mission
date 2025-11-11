#pragma once
#include"DxLib.h"

//-------------------------------------------

#include"MyLib/FrameRate.h"
#include"MyLib/Fade.h"
#include"MyLib/Input.h"
#include"../InputManager/InputManager.h"
#include"MyLib/MyMath.h"
#include"MyLib/Vector.h"
//#include"MyLib/Collision.h"
#include"MyLib/Sound.h"
#include"MyLib/Draw.h"
#include"MyLib/SaveLoad.h"
#include"MyLib/DebugString.h"
#include"../Lib/Effekseer/Effekseer.h"
#include"../Colli/CollisionManager.h"

#include"../Mouse/Mouse.h"
#include"../Select/Select.h"

//作成中限定---------------------------------

//モデルの縮尺
const float		MODEL_SCALE		= 0.05f;
const VECTOR	MODEL_SCALE_V	= { MODEL_SCALE,MODEL_SCALE,MODEL_SCALE };
const float GRAVITY				= -0.05f;	//重力
const float DEATH_Y				= -100.0f;	//死ぬ高さ

//シーンID
enum SCENE
{
	SCENE_TITLE,	//タイトル
	SCENE_SELECT,	//ステージ選択
	SCENE_PLAY,		//プレイ
	SCENE_GAMEOVER,	//ゲームオーバー
	SCENE_RESUTLT,	//リザルト
	SCENE_END,		//クレジット

	SCENE_NUM		//シーンの数
};

//ステージID
enum STAGE
{
	STAGE_0,		//チュートリアル

	STAGE_1,		//ステージ1
	STAGE_2,		//ステージ2
	STAGE_3,		//ステージ3

	STAGE_END,		//クレジット

	STAGE_NUM,		//ステージ数
};
//ステージデータのファイルパス
const string STAGE_FILE_PATH[STAGE_NUM] =
{
	"data/map/stageData/mapData_NoTutorial.bin",
	"data/map/stageData/mapData_No1.bin",
	"data/map/stageData/mapData_No2.bin",
	"data/map/stageData/mapData_No3.bin",
};

const float SET_BLOCK_POINT		= 100.0f;		//ブロック配置ポイント
const int	FOUND_MAX_GAUGE		= 300;			//発見ゲージの最大値
const int	REQUEST_UFO_COUNT	= 100;			//UFOがくる時間
const int	BASIC_FONT_SIZE		= 20;			//フォントの基本サイズ

//エフェクト
enum EFFECT_ID
{
	EFFECT_ENVIRONMENT,							//常に表示させるプレイ中エフェクト
	EFFECT_BOUND_RING1,							//地面にバウンドした時のエフェクト
	EFFECT_BOUND_RING2,							//地面にバウンドした時のエフェクト
	EFFECT_GOAL,								//ゴール時に発生
	EFFECT_GOAL_POINT,							//ゴールオブジェクトに常時発生
	EFFECT_ARROW,								//安置を示すエフェクト
	EFFECT_LASER,								//レーザーの衝突位置
	EFFECT_ENEMY_REACTION1,						//エネミーの発見時リアクション
	EFFECT_ENEMY_REACTION2,						//エネミーの見失った時のリアクション
	EFFECT_ENEMY_REACTION3,						//エネミーにボールが当たった時
	EFFECT_CREATE_BLOCK,						//ブロック配置時

	TUTORIAL_GOLE,								//チュートリアル_ゴールについて
	TUTORIAL_ENEMY,								//チュートリアル_敵について
	TUTORIAL_ITEM1,								//チュートリアル_アイテムについて_投げ
	TUTORIAL_ITEM2,								//チュートリアル_アイテムについて_当て
	TUTORIAL_ZONE,								//チュートリアル_安置
	TUTORIAL_CREATEBLOCK,						//チュートリアル_ブロックの配置
	TUTORIAL_BLOCKGAUGE,						//チュートリアル_ブロックの使い道

	EFFECT_ID_MAX_NUM
};
const string EFFECT_PATH[EFFECT_ID_MAX_NUM] =
{
	"data/effekseer/environment.efk",			//常に表示させるプレイ中エフェクト
	"data/effekseer/ring1.efk",					//地面にバウンドした時のエフェクト
	"data/effekseer/ring2.efk",					//地面にバウンドした時のエフェクト
	"data/effekseer/goalFlash.efk",				//ゴール時に発生
	"data/effekseer/goalPoint.efk",				//ゴールオブジェクトに常時発生
	"data/effekseer/safetyArea.efk",				//安置を示すエフェクト
	"data/effekseer/laser.efk",					//レーザーの衝突位置
	"data/effekseer/reaction1.efk",				//エネミーの発見時リアクション
	"data/effekseer/reaction2.efk",				//エネミーの見失った時のリアクション
	"data/effekseer/itemHit.efk",				//エネミーにボールが当たった時
	"data/effekseer/CreateBlock.efk",			//ブロック配置時

	"data/effekseer/tutorial/tutorial7.efk",	//チュートリアル_ゴールについて
	"data/effekseer/tutorial/tutorial1.efk",	//チュートリアル_敵について
	"data/effekseer/tutorial/tutorial3.efk",	//チュートリアル_アイテムについて_投げ
	"data/effekseer/tutorial/tutorial2.efk",	//チュートリアル_アイテムについて_当て
	"data/effekseer/tutorial/tutorial4.efk",	//チュートリアル_安置
	"data/effekseer/tutorial/tutorial6.efk",	//チュートリアル_ブロックの配置
	"data/effekseer/tutorial/tutorial5.efk",	//チュートリアル_ブロックの使い道
};

//リザルト用評価
struct EvaluationData
{
	void Init() 
	{
		m_iFoundNum			= 0;
		m_iUfoSpawningNum	= 0;
	}
	int						m_iFoundNum;					//発見された数
	int						m_iUfoSpawningNum;				//UFOの出現数
};

class Data
{	
private:
	static EvaluationData	m_EvaluationData;				//リザルト評価
	static SaveLoad			m_saveData;						//セーブデータ
	static SaveLoad			m_saveDataBeginning;			//セーブデータ_はじめから
	static STAGE			m_SelectStage;					//選択中のステージ
	static SCENE			m_Scene;						//現在のシーン
	static SCENE			m_NextScene;					//次のシーン
	static float			m_fFoundGauge;					//発見ゲージ
	static float			m_fBlockGauge;					//ブロック生成ゲージ
	static float			m_fDrawBlockGauge;				//ブロック生成ゲージ(表示用)
	static bool				m_FoundFlag;					//発見されてるフラグ
	static bool				m_ReSpawnFlag;					//リスポーンしたフラグ

public:
	static int				m_EffectID[EFFECT_ID_MAX_NUM];	//エフェクシア用ID

	static void Step();

	//セーブデータ関係
	//クリアしたステージを記録
	static void		SetClearStageNum(STAGE set){ if (m_saveData.data.m_ClearStage < set)m_saveData.data.m_ClearStage = set; }
	//クリアしている最新ステージを取得
	static STAGE	GetClearStageNum(){ return (STAGE)m_saveData.data.m_ClearStage; }
	//プレイデータを初期化
	static void		ResetPlayData(){ m_saveData = m_saveDataBeginning; }
	//最後にプレイしたステージを取得
	static STAGE	GetLastPlayStage(){ return (STAGE)m_saveData.data.m_LastPlayStage; }
	//最後にプレイしたステージを設定
	static void		SetLastPlayStage(){ m_saveData.data.m_LastPlayStage = m_SelectStage; }
	//データを記録
	static void		SavePlayData(){ m_saveData.Save(); }
	//データを読み込み
	static void		LoadPlayData(){ m_saveData.Load(); }
	
	//発見度を取得
	static float	GetFoundGauge(){ return m_fFoundGauge; }
	//発見度を指定
	static void		SetFoundGauge(float set=0.0f){ m_fFoundGauge =set;}
	//発見度上昇
	static void		UpFoundGauge(float upPoint){ m_fFoundGauge += upPoint; if (m_fFoundGauge >= FOUND_MAX_GAUGE)m_fFoundGauge = FOUND_MAX_GAUGE; }
	//発見度下降
	static void		DownFoundGauge(float downPoint){ m_fFoundGauge -= downPoint; if (m_fFoundGauge <= 0.0f)m_fFoundGauge = 0.0f; }

	//ブロック生成ゲージを取得
	static float	GetBlockGauge(){ return m_fBlockGauge; }
	//ブロック生成ゲージの描画用取得
	static float	GetDrawBlockGauge(){ return m_fDrawBlockGauge; }
	//ブロック生成ゲージを上昇
	static void		AddBlockGauge(float addGauge){ m_fBlockGauge += addGauge; if (m_fBlockGauge >= SET_BLOCK_POINT * 3)m_fBlockGauge = SET_BLOCK_POINT * 3; }
	//ブロック生成ゲージを設定
	static void		SetBlockGauge(float set = 0.0f){ m_fBlockGauge = set; }
	//ブロック生成ゲージの描画用変数設定
	static void		SetDrawBlockGauge(float set = 0.0f){ m_fDrawBlockGauge = set; }

	//発見フラグ取得
	static bool		GetFoundFlag(){ return m_FoundFlag; }
	//発見フラグ設定
	static void		SetFoundFlag(bool set){ m_FoundFlag = set; }

	//選択中のステージを取得
	static STAGE	GetSelectStage(){ return m_SelectStage; }
	//ステージを設定
	static void		SetSelectStage(STAGE set){ m_SelectStage = set; }

	//次のシーンを取得
	static SCENE	GetNextScene(){ return m_NextScene; }
	//次のシーンを設定
	static void		SetNextScene(SCENE set){ m_NextScene = set; }
	//現在のシーンを取得
	static SCENE	GetScene(){ return m_Scene; }
	//現在のシーンを設定
	static void		SetScene(SCENE set = m_NextScene){ m_Scene = set; }

	//リスポーンフラグを取得
	static bool		GetReSpawnFlag(){ return m_ReSpawnFlag; }
	//リスポーンフラグを設定
	static void		SetReSpawnFlag(bool set){ m_ReSpawnFlag = set; }

	//ステージ評価を設定
	static void		StageEvaluation	(int stageNum, STAGE_EVALUATION set){ m_saveData.data.m_StageEvaluation[stageNum] = set; }
	//ステージ評価を取得
	static STAGE_EVALUATION	GetStageEvaluation(int stageNum){ return m_saveData.data.m_StageEvaluation[stageNum]; }
	//評価基準を取得
	static EvaluationData&	GetEvaluationData(){ return m_EvaluationData; }
};

//-------------------------------------------

const int SCREEN_SIZE_X = 1280;	// X方向の画面サイズを指定
const int SCREEN_SIZE_Y = 720;	// Y方向の画面サイズを指定

#define VECTOR_ZERO VGet(0.0f,0.0f,0.0f)

//色ID
enum COLOR_ID
{
	WHITE,		//白
	BLACK,		//黒
	RED,		//赤
	GREEN,		//緑
	BLUE,		//青
	YELLOW,		//イエロー
	CYAN,		//シアン
	MAGENTA,	//マゼンタ
	GRAY,		//グレー

	COLOR_NUM,
};
//よく使う色まとめ
unsigned const int COLOR[COLOR_NUM] =
{
	GetColor(255, 255, 255),
	GetColor(0, 0, 0),
	GetColor(255, 0, 0),
	GetColor(0, 255, 0),
	GetColor(0, 0, 255),
	GetColor(255, 255, 0),
	GetColor(0, 255, 255),
	GetColor(255, 0, 255),
	GetColor(150, 150, 150),
};

//関数====================================================================================================================

//確率出します：int ％
bool RandPercent(int iPercent);