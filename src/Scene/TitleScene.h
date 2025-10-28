#pragma once
#include"DxLib.h"
#include"../TitleModel/TitleModel.h"

class TitleScene
{
private:
	/*====================
		シーン管理
	====================*/
	//進行度
	enum TITLE_SCENE
	{
		TITLE_SCENE_INIT,	//初期化
		TITLE_SCENE_LOAD,	//読み込み
		TITLE_SCENE_LOOP,	//繰り返し
		TITLE_SCENE_END,	//終了
		
		TITLE_SCENE_NUM,
	};

	//繰り返し内の進行度
	enum TITLE_PROGRESS
	{
		TITLE_MAKE,			//画面の作成
		TITLE_STAY,			//操作待機
		TITLE_SELECT,		//ゲームモード選択
		GAME_START			//ゲームスタート
	};

	const int FADE_IN_SPEED		= 5;	//フェードインの速度
	const int FADE_OUT_SPEED	= 3;	//フェードアウトの速度

	/*====================
			画像
	====================*/
	//画像ID
	enum IMAGE_ID	
	{
		SELECT_WINDOW,		//選択肢

		CHECK_TEXT,			//はじめから確認用テキスト

		PUSH_BUTTON_A,		//決定説明/コントローラー_A
		PUSH_ENTER,			//決定説明/キーボード_エンターキー
		PUSH_MOUSE,			//決定説明/マウス_左クリック

		IMAGE_NUM
	};
	//画像パス
	const string		IMAGE_PATH[IMAGE_NUM] =
	{
		"data/text/selectWindow459×62.png",	//選択肢

		"data/title/CheckText_216,88.png",		//はじめから確認用テキスト

		"data/system/controller/title/a.png",	//決定説明/コントローラー_A
		"data/system/key/title/enter.png",		//決定説明/キーボード_エンターキー
		"data/system/key/title/mouse_l.png",	//決定説明/マウス_左クリック
	};
	//PVパス
	const string		PLAY_MOVIE_PATH			= "data/PV.mp4";
	//画像の最大差分数
	static const int	DIFFERENCIAL_MAX_NUM	= 2;	

	const float			PV_START_TIME			= 30.0f;		//PV再生時間
	const int			IMAGE_INIT_FADE			= 0;			//画像の初期透明度
	const int			IMAGE_MAX_FADE			= 100;			//画像の最大透明度
	const int			IMAGE_MIN_FADE			= 0;			//画像の最小透明度
	const int			IMAGE_FADE_SPEED		= 20;			//画像の透明度変更速度

	/*====================
			モデル
	====================*/
	//モデルID
	enum MODEL_ID
	{
		MODEL_PLAYER,	//プレイヤーの宇宙船
		MODEL_PLANET,	//星々
		MODEL_UFO,		//UFO
		MODEL_TEXT,		//タイトル文字
		MODEL_SKY,		//スカイモデル

		MODEL_NUM
	};
	//モデルパス
	const string MODEL_PATH[MODEL_NUM] =
	{
		"data/title/model/title_pl.x",		//プレイヤーの宇宙船
		"data/title/model/title_planet.x",	//星々
		"data/title/model/title_UFO.x",		//UFO
		"data/title/model/text.x",			//タイトル文字
		"data/map/sky/sky2.x"				//スカイモデル
	};

	//モデルの座標
	const VECTOR MODEL_POS[MODEL_NUM] =
	{
		{ -10.0f, -2.0f, 0.0f },			//プレイヤーの宇宙船の座標
		{ 100.0f, 0.0f, -250.0f },			//星々の座標
		{ 100.0f, 0.0f, -250.0f },			//UFOの座標
		{ -7.5f, 13.5f, -55.0f },			//タイトル文字の座標
		{ -10.0f,10.0f,-80.0f },			//スカイモデルの座標
	};
	//モデルサイズ
	const float MODEL_SCALE[MODEL_NUM] =
	{
		0.052f ,		//プレイヤーの宇宙船のサイズ
		0.1f ,			//星々のサイズ
		0.1f ,			//UFOのサイズ
		0.065f, 		//タイトル文字のサイズ
		0.3f,			//スカイモデルのサイズ
	};
	const VECTOR	SKY_ROTATION_SPEED			= { 0.00005f,0.0001f,0.0f };//スカイモデルの回転量
	const float		TEXT_ANIME_SPEED			= 0.7f;						//タイトルテキストアニメの再生速度

	/*====================
			サウンド
	====================*/
	const int		SOUND_MAX_VOLUME			= 100;						//最大音量
	const int		SOUND_MIN_VOLUME			= 0;						//最小音量
	const int		SOUND_VOLUME_CHANGE_SPEED	= 10;						//音量変化速度

	/*====================
			カメラ
	====================*/
	const VECTOR	CAMERA_POS					= { -10.0f,10.0f,-80.0f };	//座標
	const VECTOR	CAMERA_FOCUS				= { 20.0f,20.0f,0.0f };		//視点
	const VECTOR	CAMERA_UP					= { 0.0f,1.0f,0.0f };		//アップ
	const float		CAMERA_INIT_SPEED			= -1.0f;					//カメラの初期速度
	const float		CAMERA_NEAR					= 1.0f;						//ニア
	const float		CAMERA_FAR					= 2000.0f;					//ファー
	const float		CAMERA_MOVE_SPEED_ADD		= 0.05f;					//カメラの移動加速度
	const float		CAMERA_MAX_SPEED			= 3.0f;						//カメラの最大移動速度

	/*====================
			選択肢
	====================*/
	//選択肢
	enum GAME_MODE	
	{
		SELECT_NEUTRAL = -1,	//未選択

		SELECT_BEGINNING,		//はじめから
		SELECT_CONTINUATION,	//つづきから

		SELECT_NUM
	};

	//画像種類
	enum SELECT_DRAW_ID
	{
		UNSELECTED,	//未選択
		SELECTING,	//選択中
	};

	//選択肢の画像パス
	const int		SELECT_WINDOW_IMAGE_SIZE_X	= 459;						//選択肢の画像サイズ_X
	const int		SELECT_WINDOW_IMAGE_SIZE_Y	= 62;						//選択肢の画像サイズ_Y
	const VECTOR	SELECT_WINDOW_IMAGE_SIZE	= { 459.0f,62.0f,0.0f };	//選択肢の画像サイズ
	const int		SELECT_WINDOW_IMAGE_MAX_NUM = 2;						//選択肢画像数
	const int		SELECT_WINDOW_IMAGE_NUM_X	= 1;						//選択肢画像数_X
	const int		SELECT_WINDOW_IMAGE_NUM_Y	= 2;						//選択肢画像数_Y
	const int		SELECT_WINDOW_INIT_FADE		= 0;						//初期透明度
	const int		SELECT_WINDOW_MAX_FADE		= 100;						//最大透明度
	const int		SELECT_WINDOW_NIM_FADE		= 0;						//最小透明度
	const int		SELECT_WINDOW_FADE_SPEED	= 20;						//透明度の変化速度

	//選択肢の位置
	const VECTOR WINDOW_POS[SELECT_NUM] =
	{
		{ SCREEN_SIZE_X / 2 + 350.0f,SCREEN_SIZE_Y / 2 + 150.0f,0.0f } ,	//はじめから
		{ SCREEN_SIZE_X / 2 + 350.0f,SCREEN_SIZE_Y / 2 + 250.0f,0.0f }		//つづきから
	};
	//選択肢テキスト
	const string TITLE_SELECT_TEXT[SELECT_NUM] =
	{
		"はじめから",
		"つづきから",
	};
	const int	SELECT_FONT_SIZE		= 30;		//選択肢のフォントサイズ
	const int	SELECT_WINDOW_FADE_ADD	= 2;		//透明度の加算値
	const float SELECT_DRAW_POS_ADD		= -50.0f;	//選択中の選択肢画像をずらす

	//選択時注意事項画像サイズ
	const VECTOR CHECK_TEXT_SIZE = { 216.0f,88.0f,0.0f };
	//注意事項の描画位置
	const VECTOR CHECK_TEXT_DRAW_POS = 
	{
		CHECK_TEXT_SIZE.x * 0.3f,
		CHECK_TEXT_SIZE.y + 35.0f,
		0.0f 
	};

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
	const int PUTH_IMAGE_MAX_NUM = 2;				//入力説明画像数
	const int PUTH_IMAGE_NUM_X = 1;					//入力説明画像数_X
	const int PUTH_IMAGE_NUM_Y = 2;					//入力説明画像数_Y
	const int PUTH_IMAGE_SIZE_X = SCREEN_SIZE_X;	//入力情報サイズ_X
	const int PUTH_IMAGE_SIZE_Y = SCREEN_SIZE_Y;	//入力情報サイズ_Y

	//選択時の入力情報の位置
	const VECTOR SELECT_KEEP_POS[SELECT_NUM] =
	{
		//はじめからの位置
		{	WINDOW_POS[SELECT_BEGINNING].x + SELECT_DRAW_POS_ADD,
			WINDOW_POS[SELECT_BEGINNING].y,
			WINDOW_POS[SELECT_BEGINNING].z	},

		//つづきからの位置
		{	WINDOW_POS[SELECT_CONTINUATION].x + SELECT_DRAW_POS_ADD,
			WINDOW_POS[SELECT_CONTINUATION].y,
			WINDOW_POS[SELECT_CONTINUATION].z		},
	};

private:
	TitleModel		m_TitleModel[MODEL_NUM];						//タイトルモデル
	TITLE_PROGRESS	m_Progress;										//進行度
	TITLE_SCENE		m_SceneID;										//流れ
	GAME_MODE		m_SelectGameMode;								//ゲームモード選択
	VECTOR			m_vCameraPos;									//カメラ座標
	VECTOR			m_vCameraFor;									//カメラ視点
	VECTOR			m_vCameraUp;									//カメラアップ
	VECTOR			m_vWindowPos[SELECT_NUM];						//選択肢画像座標
	float			m_fCameraSpeed;									//カメラスピード
	float			m_fPvStartCount;								//PV再生カウント
	bool			m_ImageDrawFlag[IMAGE_NUM];						//画像表示フラグ
	bool			m_SelectableFlag;								//選択可能になる	
	bool			m_ContinuationFlag;								//既存データがある
	bool			m_PushButtonFlag[PUTH_IMAGE_NUM];				//ボタン押してるフラグ
	bool			m_PVPlayFlag;									//PV再生フラグ
	int				m_iSelectWindowFade;							//UIの透明度
	int				m_iWindowHandle[SELECT_NUM];					//セレクトウィンドウのハンドル
	int				m_iHandle[IMAGE_NUM][DIFFERENCIAL_MAX_NUM];		//2D画像ハンドル
	int				m_iImageFade[IMAGE_NUM];						//画像の透明度
	int				m_iPvHandle;									//PVのハンドル

public:
	TitleScene();
	~TitleScene();

	int Loop();
	void Draw();

private:
	void Init();
	void Fin();
	void Load();
	void Step();

private:
	//タイトル画面の作成
	void ScreenMake();
	//操作待機
	void ScreenStay();
	//選択
	void SelectState();
	//注意を出す
	void OutPutAttention();
	//ゲームをスタート
	void GameStart();
	//透明度の変更
	void ImageFade();
	//入力の確認
	void InputCheck();
	//選択肢の描画
	void DrawSelect();
	//注意事項の描画
	void DrawCheckText();
	//入力状態の描画
	void DrawPushButton();
};

