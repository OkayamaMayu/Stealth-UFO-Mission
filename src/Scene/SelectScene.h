#pragma once
#include"../MyLib/MyLib.h"

class SelectScene
{
private:
	/*====================
		シーン管理
	====================*/
	//進行度
	enum SELECT_SCENE
	{
		SELECT_SCEME_INIT,	//初期化
		SELECT_SCEME_LOAD,	//読み込み
		SELECT_SCEME_LOOP,	//繰り返し
		SELECT_SCEME_END,	//終了

		SELECT_SCEME_NUM,
	};

	const int FADE_IN_SPEED = 20;	//フェードインのスピード
	const int FADE_OUT_SPEED = 5;	//フェードアウトのスピード

	/*====================
			画像
	====================*/
	//画像ID
	enum IMAGE_ID
	{
		STAGE_BACK,					//背景

		SELECT_ICON_STAGE,			//通常ステージアイコン
		SELECT_ICON_END_ROLL,		//エンドロールアイコン

		STAGE_SELECT_ARROW,			//矢印

		STAGE_EVALUATION_ICON_S,	//ステージ評価_S
		STAGE_EVALUATION_ICON_A,	//ステージ評価_A
		STAGE_EVALUATION_ICON_B,	//ステージ評価_B
		STAGE_EVALUATION_ICON_C,	//ステージ評価_C

		STAGE_BLACK_LINE,			//上下の黒帯

		STAGE_SELECT_TEXT,			//ステージセレクトの文字

		PUSH_BUTTON_A,				//入力情報_A
		PUSH_ENTER,					//入力情報_エンター
		PUSH_MOUSE,					//入力情報_マウスクリック

		IMAGE_NUM,
	};
	//画像パス
	const string SELECTSTAGE_BACK_IMAGE_PATH[IMAGE_NUM] =
	{
		"data/system/baseBack.png",							//背景

		"data/stageSelect/150x150.png",						//通常ステージアイコン
		"data/stageSelect/end.png",							//エンドロールアイコン

		"data/system/arrow_74,69.png",						//矢印

		"data/result/evaluation/evaluationS_324,338.png",	//ステージ評価_S
		"data/result/evaluation/evaluationA_324,338.png",	//ステージ評価_A
		"data/result/evaluation/evaluationB_324,338.png",	//ステージ評価_B
		"data/result/evaluation/evaluationC_324,338.png",	//ステージ評価_C

		"data/system/black.png",							//上下の黒帯

		"data/stageSelect/text.png",						//ステージセレクトの文字

		"data/system/controller/select/a.png",				//入力情報_A
		"data/system/key/select/enter.png",					//入力情報_エンター
		"data/system/key/select/mouse_l.png",				//入力情報_マウスクリック
	};

	static const int UI_ANIME_IMAGE_MAX_NUM = 2;	//UIのアニメ数

	/*====================
		サウンド
	====================*/
	const int SOUND_MAX_VOLUME			= 100;	//最大音量
	const int SOUND_MIN_VOLUME			= 0;	//最小音量
	const int SOUND_VOLUME_CHANGE_SPEED = 10;	//音量変化速度

	/*====================
		選択アイコン
	====================*/
	//画面に表示するアイコン
	enum DRAW_ICON_ID
	{
		DRAW_ICON_LEFT_END,		//左端
		DRAW_ICON_LEFT,			//左
		DRAW_ICON_SELECT_NOW,	//真ん中(選択中)
		DRAW_ICON_RIGHT,		//右
		DRAW_ICON_RIGHT_END,	//右端

		DRAW_ICON_NUM			//一度に表示されるアイコンの数
	};
	//選択アイコンの位置
	const VECTOR SELECT_STAGE_IMAGE_POS[DRAW_ICON_NUM] =
	{
		{ 100,						SCREEN_SIZE_Y / 2 + 200 ,	0.0f },	//左端
		{ SCREEN_SIZE_X / 2 - 350,	SCREEN_SIZE_Y / 2 + 100,	0.0f },	//左
		{ SCREEN_SIZE_X / 2,		SCREEN_SIZE_Y / 2 + 25,		0.0f },	//真ん中(選択中)
		{ SCREEN_SIZE_X / 2 + 350,	SCREEN_SIZE_Y / 2 + 100,	0.0f },	//右
		{ SCREEN_SIZE_X - 100,		SCREEN_SIZE_Y / 2 + 200,	0.0f },	//右端
	};
	//ステージ解放時の最大値計算用
	//(+1が解放したステージ分,+2が最大数)
	const int STAGE_MAX_NUM_COUNT				= 2;
	const int	SELECT_ICON_DRAW_MIN_COUNT		= -2;					//表示アイコンの最小値_数値設定用
	const int	SELECT_ICON_DRAW_CONST			= -3;					//表示アイコンの最小値_描画用
	const int	SELECT_ICON_DRAW_MAX_COUNT		= 2;					//アイコン表示時の追加分
	const int	SELECT_FONT_SIZE_MIN			= 0;					//フォントサイズの最小値
	const float SELECT_KEY_INTERVAL				= 0.05f;				//キー入力の感覚
	const int	SELECT_FONT_SIZE[DRAW_ICON_NUM]	= { 0,100,150,100,0 };	//フォントサイズ

	//ステージアイコンのサイズ倍率
	const float SELECT_ICON_SIZE_MAGNIFICATION[DRAW_ICON_NUM] = 
	{ 
		0.2f,1.0f,1.8f,1.0f,0.2f 
	};	
	const float SELECT_ICON_SIZE				= 150.0f;					//アイコンのサイズ
	const float SELECT_ICON_SIZE_ADD_MAX		= 0.3f;						//サイズの最大値
	const float SELECT_ICON_SIZE_ADD_MIN		= -0.3f;					//サイズの最小値
	const float SELECT_ICON_SIZE_SPEED			= 0.005f;					//サイズの変化速度
	const bool	SELECT_ICON_SIZE_MOVE_FLAG_INIT	= true;						//選択アイコンのサイズの変更のフラグ
	const float SELECT_ICON_MOVE_SPEED			= 10.0f;					//アイコン移動速度
	const float SELECT_ICON_SIZE_MOVE_SPEED		= 0.05f;					//アイコンサイズの変更速度
	const int	STAGE_FONT_SIZE_SPEED			= 5;						//フォントサイズの変更速度

	/*====================
			評価
	====================*/
	const VECTOR	STAGE_EVALUATION_ADD_POS	= { 50.0f,60.0f,0.0f };		//ステージ評価の表示位置変更
	const float		STAGE_EVALUATION_SIZE		= 0.3f;						//ステージアイコンを基準にした評価のサイズ

	/*====================
		選択矢印
	====================*/
	//矢印の位置
	enum ARROW_SIDE
	{
		SIDE_LEFT,
		SIDE_RIGHT,

		ARROW_SIDE_NUM,
	};
	//移動矢印の位置
	const VECTOR SELECT_STAGE_ARROW_POS[ARROW_SIDE_NUM] =
	{
		{ SCREEN_SIZE_X / 2 - 250.0f, SCREEN_SIZE_Y / 2, 0.0f },
		{ SCREEN_SIZE_X / 2 + 250.0f, SCREEN_SIZE_Y / 2, 0.0f },
	};
	const VECTOR	SELECT_ARROW_SIZE				= { 82.0f,80.0f,0.0f };	//画像サイズ
	const bool		SELECT_ARROW_MOVE_FLAG_INIT		= false;				//選択矢印の初期化
	const int		SELECT_ARROW_FADE_INIT			= 0;					//矢印の透明度の初期値
	const int		SELECT_ARROW_FADE_MAX			= 100;					//矢印の透明度の最大値
	const int		SELECT_ARROW_FADE_MIN			= 0;					//矢印の透明度の最小値
	const int		SELECT_ARROW_FADE_SPEED			= 20;					//矢印の透明度の変化速度
	const float		SELECT_ARROW_MOVE_MAX			= 30.0f;				//矢印の最大移動値
	const float		SELECT_ARROW_MOVE_MIN			= 0.0f;					//矢印の最小移動値
	const float		SELECT_ARROW_MOVE_ADD			= 5.0f;					//矢印の移動量の幅
	const int		SELECT_ARROW_MOVE_SPEED_DIVISOR = 40;					//矢印のスピード計算用

	/*====================
			入力情報
	====================*/
	//キー入力説明画像
	enum PUSH_IMAGE_ID
	{
		PUTH_IMAGE_CONTROLLER,	//コントローラー
		PUTH_IMAGE_KEY,			//キーボード
		PUTH_IMAGE_MOUSE,		//マウス

		PUTH_IMAGE_NUM			//入力手段数
	};
	const int PUTH_IMAGE_MAX_NUM	= 2;					//入力情報画像数
	const int PUTH_IMAGE_NUM_X		= 1;					//入力情報画像数_X
	const int PUTH_IMAGE_NUM_Y		= 2;					//入力情報画像数_Y
	const int PUTH_IMAGE_SIZE_X		= SCREEN_SIZE_X;		//入力情報サイズ_X
	const int PUTH_IMAGE_SIZE_Y		= SCREEN_SIZE_Y;		//入力情報サイズ_Y

	/*====================
			背景
	====================*/
	const int	BACK_GROUND_REQUIRED_NUM	= 2;			//スクロール時の必要描画枚数
	const float BACK_GROUND_MOVE_SPEED		= 0.5f;			//背景の移動スピード

private:
	SELECT_SCENE	m_SceneID;											//進行度
	VECTOR			m_vBackPos;											//背景座標
	VECTOR			m_vPos[STAGE_NUM];									//ステージ選択の座標
	float			m_fImageSize[STAGE_NUM];							//ステージ選択の大きさ
	float			m_fKeyInterval;										//前回選択時間
	float			m_fArrowMove;										//ステージ選択矢印移動量
	float			m_fDecisionTime;									//選択時間の加算
	bool			m_StageMove;										//選択ステージ変更後の動き
	bool			m_SelectArrowMoveFlag;								//矢印の動きフラグ
	bool			m_DecisionFlag;										//選択中フラグ
	bool			m_SelectSizeFlag;									//選択中のアイコンの動きフラグ
	bool			m_PushButtonFlag[PUTH_IMAGE_NUM];					//ボタン押してるフラグ
	int				m_iImageHandle[IMAGE_NUM][UI_ANIME_IMAGE_MAX_NUM];	//画像ハンドル
	int				m_iFontSize[STAGE_NUM];								//フォントサイズ
	int				m_iArrowFade[ARROW_SIDE_NUM];						//矢印の透明度
	int				m_SelectStage;										//選択中のステージ

public:
	SelectScene();
	~SelectScene();
	int Loop();
	void Draw();

private:
	void Init();
	void Load();
	void Step();
	void Fin();

private:
	//タイトルに戻る
	void TitleBack();
	//入力情報の確認
	void InputCheck();
	//選択
	void Select();
	//矢印選択
	void SelectArrow();
	//背景の通常処理
	void BackGroundStep();
	//アイコンの移動
	void Move();
	//アイコンの移動
	bool SelectIconMove(int numID,int ID);
	//アイコンのサイズ変更
	void SelectIconSizeChange();
	//矢印のステップ
	void ArrowStep();
	//矢印の移動
	void ArrowMove();
	//矢印の透明度変化
	void ArrowFade();
};