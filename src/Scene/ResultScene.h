#pragma once

#include"DxLib.h"

class ResultScene
{
private:
	enum RESULT_SCENE
	{
		RESULT_SCENE_INIT,		//初期化
		RESULT_SCENE_LOAD,		//読み込み
		RESULT_SCENE_LOOP,		//ループ
		RESULT_SCENE_END,		//終了
		
		RESULT_SCENE_NUM,
	};

	enum RESULT_PROGRESS
	{
		RESULT_MAKE,			//画面の作成
		RESULT_STAY,			//操作待機
		RESULT_SAVE,			//セーブ
		RESULT_SAVE_FINISH,		//セーブ完了
		NEXT_SCENE				//次のシーンへ
	};

	//画像ID
	enum IMAGE_ID
	{
		BACK_GROUND,	//背景
		BACK_WINDOW,	//背景ウィンドウ
		CLEAR_TEXT,		//クリア文字
		CIRCLE,			//評価背景
		EVALUATION_S,	//評価S
		EVALUATION_A,	//評価A
		EVALUATION_B,	//評価B
		EVALUATION_C,	//評価C
		BLACK_LINE,		//黒ライン

		IMAGE_NUM
	};
	//画像パス
	const string IMAGE_PATH[IMAGE_NUM] =
	{
		"data/system/baseBack.png",										//背景
		"data/result/backWindow_1280,720.png",							//背景ウィンドウ
		"data/result/clearText_797,187.png",							//クリア文字
		"data/result/circle_324,338.png",								//評価背景
		"data/result/evaluation/evaluationS_324,338.png",				//評価S
		"data/result/evaluation/evaluationA_324,338.png",				//評価A
		"data/result/evaluation/evaluationB_324,338.png",				//評価B
		"data/result/evaluation/evaluationC_324,338.png",				//評価C
		"data/system/black.png",										//黒ライン
	};
	//画像の座標
	const VECTOR IMAGE_POS[IMAGE_NUM] =
	{
		{0.0f,0.0f,0.0f},												//背景
		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f},						//背景ウィンドウ
		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f},						//クリア文字
		{SCREEN_SIZE_X / 2 + 294.5f,SCREEN_SIZE_Y / 2 + 26.5f,0.0f},	//評価背景
		{SCREEN_SIZE_X / 2 + 294.5f,SCREEN_SIZE_Y / 2 + 26.5f,0.0f},	//評価S
		{SCREEN_SIZE_X / 2 + 294.5f,SCREEN_SIZE_Y / 2 + 26.5f,0.0f},	//評価A
		{SCREEN_SIZE_X / 2 + 294.5f,SCREEN_SIZE_Y / 2 + 26.5f,0.0f},	//評価B
		{SCREEN_SIZE_X / 2 + 294.5f,SCREEN_SIZE_Y / 2 + 26.5f,0.0f},	//評価C
		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f},						//黒ライン
	};
	//文字の座標
	const VECTOR TEXT_POS[2] =
	{
		{ 250.0f,300.0f,0.0f },
		{ 250.0f,470.0f,0.0f }
	};

	//評価画面の進行
	enum RESULT_MAKE_PROGRESS
	{
		BASE,		//土台を表示
		TEXT,		//テキストを表示
		EVALUATION	//評価を表示
	};
	RESULT_MAKE_PROGRESS m_ResultMakeProgress;	//評価画面進行度の保存

	//選択肢_セーブするか
	enum OPTION_ID
	{
		YES,NO,			//はいいいえ
		OPTION_NUM
	};
	//選択肢テキスト
	const string	OPTION_TEXT[OPTION_NUM]		= { "はい","いいえ" };
	OPTION_ID m_Option;	//選択肢の保存
	//選択肢画像のパス
	const string	SELECT_WINDOW_PATH			= "data/text/selectWindow459×62.png";
	//画像数：総数, X方向の数, Y方向の数
	const int		SELECT_WINDOW_IMAGE_NUM[3]	= { 2,1,2 };
	//選択肢画像サイズ
	const VECTOR	SELECT_WINDOW_SIZE			= { 459.0f,62.0f,0.0f };
	//選択肢の位置
	const VECTOR RESULT_SELECT_IMAGE_POS[OPTION_NUM] =
	{
		{SCREEN_SIZE_X / 2 - 463.0f / 2 - 20.0f,SCREEN_SIZE_Y / 2 + 200.0f},	//選択肢1
		{SCREEN_SIZE_X / 2 + 463.0f / 2 + 20.0f,SCREEN_SIZE_Y / 2 + 200.0f},	//選択肢2
	};

	enum RESULT_TEXT_ID
	{
		ENEMY_FOUND,	//エネミーに見つかった数
		UFO_APPEARANCE,	//UFOの出現回数

		RESULT_TEXT_NUM
	};
	//リザルトの結果テキスト
	const string RESULT_TEXT[RESULT_TEXT_NUM] =
	{
		"テキに見つかった回数",
		"ＵＦＯが出現した回数",
	};
	//評価基準数
	const int	EVALUSTION_CRITERIA[4][RESULT_TEXT_NUM] =
	{
		{0,	0 },	//S評価
		{5,	2 },	//A評価
		{10,-1},	//B評価
		{-1,-1},	//C評価
	};

	//リザルトテキストの位置
	const VECTOR		RESULT_TEXT_POS					= { 300.0f,50.0f,0.0f };
	const float			BACKGROUND_SCROLL_SPEEED		= 0.5f;		//背景スクロールの速度
	const float			INIT_IMAGE_SIZE					= 2.0f;		//初期の画像サイズ
	const float			INIT_IMAGE_SCALE				= 1.0f;		//初期画像の追加スケール
	const float			INIT_TEXT_POS_BASE				= 100.0f;	//テキスト初期位置の基本
	const float			INIT_TEXT_POS_ADD				= 50.0f;	//テキスト初期位置の追加
	const float			EVALUSTION_ROT_NUM				= 5.5f;		//表示時の回転数
	const float			SPEED_RATIO						= 20.0f;	//速度割合
	const float			MAKE_ALPHA_MIN_SPEED			= 10.0f;	//画面作成の最低速度
	const float			MAKE_SIZE_MIN_SPEED				= 3.0f;		//サイズの画面生成最低速度
	const float			MAKE_TEXT_MIN_SPEED				= 1.5f;		//テキスト移動の最低速度
	const float			MAKE_TEXT_ALPHA_MIN_SPEED		= 5.0f;		//テキスト透明度の最低速度
	const float			MAKE_EVALUATION_SIZE_SPEED		= 0.5f;		//評価の最低速度
	const float			MAKE_EVALUATION_ALPHA_SPEED		= 1.0f;		//評価の透明度変更最低速度
	const float			MAKE_EVALUATION_ROT_SPEED		= 0.5f;		//評価の回転最低速度
	const float			FRY_MOVE_LIMIT					= 2.0f;		//アイコンの浮遊制限
	const float			FRY_MOVE_MIN_SPEED				= 0.5f;		//アイコン浮遊の最低速度
	const float			SAVE_TEXT_HEIGHT				= 100.0f;	//セーブテキストの高さ
	const float			SELECT_SAVE_TEXT_HEIGHT			= -20.0f;	//選択中のセーブ選択肢のテキストの高さ
	const float			SAVE_SELECT_WINDOW_POS			= -20.0f;	//セーブ選択肢の高さ
	const int			SOUND_FADEIN_SPEED				= 10;		//サウンドのフェードイン速度
	const int			FADEIN_SPEED					= 2;		//フェードインの速度
	const int			INIT_TEXT_ALPHA					= -10;		//テキスト初期透明度
	const int			SAVE_FADEIN_SPEED				= 10;		//セーブ画面へのフェードイン速度
	const int			SAVE_FADEOUT_SPEED				= 7;		//セーブ画面へのフェードアウト速度
	const int			SAVE_SOUND_VOLUME				= 70;		//セーブ画面のBGMボリューム
	const int			SAVE_SOUND_FADE_SPEED			= 5;		//セーブ画面BGMボリュームへの変更速度
	const int			SAVE_NOW_FADEIN_SPEED			= 7;		//セーブ中のフェードイン速度
	const int			SAVE_EXECUTION_FADEOUT_SPEED	= 10;		//セーブ実行時のフェードアウト速度
	const int			SAVE_SKIP_FADEOUT_SPEED			= 2;		//セーブをスキップしたときのフェードアウト速度
	const int			SCENE_CHANGE_FADEOUT_SPEED		= 2;		//シーンチェンジのフェードアウト速度
	const int			RESULT_FONT_SIZE				= 30;		//リザルトの文字列のフォントサイズ
	const int			SCORE_FONT_SIZE					= 40;		//スコア表示フォントサイズ
	const int			SAVE_TEXT_FONT_SIZE				= 50;		//セーブテキストのフォントサイズ
	const int			SAVE_SETECT_TEXT_FONT_SIZE		= 30;		//セーブ選択肢のフォントサイズ

private:
	RESULT_SCENE		m_SceneID;									//シーンを保存する
	RESULT_PROGRESS		m_Progress;									//進行度
	STAGE_EVALUATION	m_Evaluation;								//一時保存用評価
	VECTOR				m_vPos[IMAGE_NUM];							//画像の座標
	VECTOR				m_vTextPos[RESULT_TEXT_NUM];				//テキストの座標
	float				m_fSize[IMAGE_NUM];							//画像サイズ
	float				m_fFryPoiny;								//浮遊させる
	float				m_fEvaluationRot;							//評価の回転
	bool				m_FryFlag;									//浮くフラグ
	bool				m_SaveFlag;									//セーブするかのフラグ
	bool				m_ResultNextScene;							//次のシーンに向かうフラグ
	int					m_iHandle[IMAGE_NUM];						//画像フラグ
	int					m_iSelectImageHandle[OPTION_NUM];			//選択肢のハンドル
	int					m_iAlpha[IMAGE_NUM];						//透明度
	int					m_iTextAlpha[RESULT_TEXT_NUM];				//テキストの透明度
	int					m_iEnemyFoundNum[RESULT_TEXT_NUM];			//一時保存用発見値
	int					m_iResultImageNum;							//評価画像

public:
	ResultScene();
	~ResultScene();

	int Loop();
	void Draw();

private:
	void Init();
	void Fin();
	void Load();
	void Step();

	//場の作成
	void ResultMake();
	//場で待機
	void ResultStay();
	//セーブ画面
	void ResultSave();
	//セーブを完了させる
	void ResultSaveFinish();
};