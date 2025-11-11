#pragma once
#include"../MyLib/MyLib.h"

class UIManager
{
private:
	enum IMAGE_ID
	{
		SYSTEM_CATCH_ITEM,	//アイテム所持アイコン

		//キーボード＆マウス---------------------

		SYSTEM_MOVE1,
		SYSTEM_MOVE2,
		SYSTEM_MOVE3,
		SYSTEM_MOVE4,
		SYSTEM_JUMP,
		SYSTEM_CAMERA_MOVE1,
		SYSTEM_CAMERA_MOVE2,
		SYSTEM_CAMERA_ZOOM1_KEY,
		SYSTEM_CAMERA_ZOOM2_KEY,
		SYSTEM_CAMERA_ZOOM_MOUSE,
		SYSTEM_BLOCKMODE_KEY,
		SYSTEM_BLOCKMODE_MOUSE,
		SYSTEM_ITEMMODE,
		SYSTEM_PAUSE,

		SYSTEM_PLAY_TEXT,

		SYSTEM_PLAY_IMAGE_NUM,

		//---------------------------------------

		SYSTEM_ITEM_THROW_MOUSE = SYSTEM_PLAY_IMAGE_NUM,

		SYSTEM_ITEM_TEXT,

		SYSTEM_ITEM_IMAGE_NUM,

		//---------------------------------------

		SYSTEM_BLOCK_PUT_MOUSE = SYSTEM_ITEM_IMAGE_NUM,

		SYSTEM_BLOCK_TEXT,

		SYSTEM_BLOCK_IMAGE_NUM,

		//---------------------------------------

		SYSTEM_QTE = SYSTEM_BLOCK_IMAGE_NUM,

		SYSTEM_KEY_AND_MOUSE_IMAGE_NUM,

		//コントローラー-------------------------

		SYSTEM_CONTROLLER_MOVE = SYSTEM_KEY_AND_MOUSE_IMAGE_NUM,
		SYSTEM_CONTROLLER_JUMP,
		SYSTEM_CONTROLLER_CAMERA,
		SYSTEM_CONTROLLER_BLOCKMODE,
		SYSTEM_CONTROLLER_ITEMMODE,
		SYSTEM_CONTROLLER_PAUSE1,
		SYSTEM_CONTROLLER_PAUSE2,

		SYSTEM_CONTROLLER_PLAY_TEXT,

		SYSTEM_CONTROLLER_PLAY_IMAGE_NUM,

		//---------------------------------------

		SYSTEM_CONTROLLER_ITEM_THROW = SYSTEM_CONTROLLER_PLAY_IMAGE_NUM,

		SYSTEM_CONTROLLER_ITEM_TEXT,

		SYSTEM_CONTROLLER_ITEM_IMAGE_NUM,

		//---------------------------------------

		SYSTEM_CONTROLLER_BLOCK_PUT = SYSTEM_CONTROLLER_ITEM_IMAGE_NUM,

		SYSTEM_CONTROLLER_BLOCK_TEXT,

		SYSTEM_CONTROLLER_BLOCK_IMAGE_NUM,

		//---------------------------------------

		SYSTEM_CONTROLLER_QTE = SYSTEM_CONTROLLER_BLOCK_IMAGE_NUM,

		SYSTEM_CONTROLLER_IMAGE_NUM,

		//ブロック配置ゲージ---------------------

		SYSTEM_BLOCK_GAUGE_UNDER = SYSTEM_CONTROLLER_IMAGE_NUM,
		SYSTEM_BLOCK_GAUGE_MAIN,
		SYSTEM_BLOCK_GAUGE_COVER,
		SYSTEM_BLOCK_GAUGE_TEXT,
		SYSTEM_BLOCK_GAUGE_BLOCKNUM,

		//---------------------------------------

		IMAGE_NUM
	};
	const string UI_IMAGE_PATH[UIManager::IMAGE_NUM] =
	{
		"data/system/item.png",

		//キーボード＆マウス---------------------

		"data/system/play/w.png",
		"data/system/play/a.png",
		"data/system/play/s.png",
		"data/system/play/d.png",
		"data/system/play/spacs.png",
		"data/system/play/left.png",
		"data/system/play/right.png",
		"data/system/play/up.png",
		"data/system/play/down.png",
		"data/system/play/mouseWheel.png",
		"data/system/play/e.png",
		"data/system/play/mouseRight.png",
		"data/system/play/q.png",
		"data/system/play/ctrl.png",
		"data/system/play/text.png",

		//---------------------------------------

		"data/system/itemMode/mouseLeft.png",
		"data/system/itemMode/text.png",

		//---------------------------------------

		"data/system/blockMode/mouseLeft.png",
		"data/system/blockMode/text.png",

		//---------------------------------------

		"data/system/QTE/qte.png",

		//コントローラー-------------------------

		"data/system/controller/play/lStick.png",
		"data/system/controller/play/a.png",
		"data/system/controller/play/rStick.png",
		"data/system/controller/play/l.png",
		"data/system/controller/play/r.png",
		"data/system/controller/play/start.png",
		"data/system/controller/play/back.png",
		"data/system/controller/play/text.png",

		//---------------------------------------

		"data/system/controller/itemMode/a.png",
		"data/system/controller/itemMode/text.png",

		//---------------------------------------

		"data/system/controller/blockMode/a.png",
		"data/system/controller/blockMode/text.png",

		//---------------------------------------

		"data/system/controller/QTE/qte.png",

		//ブロック配置ゲージ---------------------

		"data/system/blockMode/gauge/under.png",
		"data/system/blockMode/gauge/main.png",
		"data/system/blockMode/gauge/cover.png",
		"data/system/blockMode/gauge/text.png",
		"data/system/blockMode/gauge/blockNum.png",
	};
	enum SCREEN_TYPE
	{
		TYPE_PLAY,
		TYPE_ITEM,
		TYPE_BLOCK,
		TYPE_UFO,
		TYPE_PASE,
	};

	enum TUTORIAL_ID
	{
		TUTORIAL_FOUND,				//発見ゲージ
		TUTORIAL_INVINCIBLE_AREA,	//無敵エリア

		TUTORIAL_ITEM1_CON,			//アイテムで注意をそらす_コントローラー
		TUTORIAL_ITEM2_CON,			//アイテムをあてる_コントローラー
		TUTORIAL_BLOCK_CON,			//ブロック配置_コントローラー
		TUTORIAL_BLOCK2_CON,		//レーザー_コントローラー

		TUTORIAL_ITEM1_KEY,			//アイテムで注意をそらす_キーボード
		TUTORIAL_ITEM2_KEY,			//アイテムをあてる_キーボード
		TUTORIAL_BLOCK_KEY,			//ブロック配置_キーボード
		TUTORIAL_BLOCK2_KEY,		//レーザー_キーボード

		TUTORIAL_IMAGE_NUM,
	};
	const string TUTORIAL_IMAGE_PATH[TUTORIAL_IMAGE_NUM] =
	{
		"data/system/tutorial/t1.png",
		"data/system/tutorial/t4.png",

		"data/system/tutorial/t3.png",
		"data/system/tutorial/t2.png",
		"data/system/tutorial/t6_1.png",
		"data/system/tutorial/t6_2.png",

		"data/system/tutorial/key/t3.png",
		"data/system/tutorial/key/t2.png",
		"data/system/tutorial/key/t6_1.png",
		"data/system/tutorial/key/t6_2.png",
	};
	
	const VECTOR TUTORIAL_POS[TUTORIAL_IMAGE_NUM] =
	{
		{-470.0f,-30.0f,-450.0f},	//発見ゲージ
		{-410.0f,-30.0f,-590.0f},	//無敵エリア

		{-310.0f,-30.0f,-810.0f},	//アイテムで注意をそらす_コントローラー
		{-470.0f,-30.0f,-810.0f},	//アイテムをあてる_コントローラー
		{-270.0f,-20.0f,-590.0f},	//ブロック配置_コントローラー
		{-130.0f,00.0f,-660.0f},	//レーザー_コントローラー

		{-310.0f,-30.0f,-810.0f},	//アイテムで注意をそらす_キーボード
		{-470.0f,-30.0f,-810.0f},	//アイテムをあてる_キーボード
		{-270.0f,-20.0f,-590.0f},	//ブロック配置_キーボード
		{-130.0f,00.0f,-660.0f},	//レーザー_キーボード
	};
	
	//ブロック配置ゲージ座標
	const VECTOR	BLOCK_GAUGE_BASE_POS		= { 170.0f,60.0f,0.0f };
	//配置ゲージの調節位置
	const VECTOR	BLOCK_GAUGE_ADJUSTMENT		= { 115.0f,60.0f,0.0f };
	//ブロック配置ゲージの中身座標
	const VECTOR	BLOCK_GAUGE_MAIN_POS		= 
	{
		BLOCK_GAUGE_BASE_POS.x - 115.0f,
		BLOCK_GAUGE_BASE_POS.y,
		0.0f
	};
	//ブロック配置ゲージの中身の半分の画像サイズ
	const VECTOR	BLOCK_GAUGE_MAIN_SIZE		= { 226.0f,77.0f,0.0f };
	//ブロック配置ゲージのテキスト座標
	const VECTOR	BLOCK_GAUGE_TEXT_POS		=
	{
		BLOCK_GAUGE_BASE_POS.x - 80.0f,
		BLOCK_GAUGE_BASE_POS.y - 30.0f,
		0.0f
	};
	//ブロック配置ゲージの配置可能数の座標
	const VECTOR	BLOCK_GAUGE_SET_NUM_POS		=
	{
		BLOCK_GAUGE_BASE_POS.x + 115.0f,
		BLOCK_GAUGE_BASE_POS.y + 16.0f,
		0.0f
	};
	//ブロック配置ゲージアイコン表示座標
	const VECTOR	BLOCK_GAUGE_ICON_POS		=
	{
		BLOCK_GAUGE_BASE_POS.x + 90.0f,
		BLOCK_GAUGE_BASE_POS.y + 30.0f,
		0.0f
	};
	//ブロック配置可能数のフォントサイズ
	const int			SET_BLOCK_NUM_FONT_SIZE	= 30;

	const float			ITEM_CATCH_POS			= 10.0f;	//アイテム所持アイコンの位置
	const float			CONTROLLER_STICK_MIN	= 0.2f;		//コントローラーの最小傾き
	const int			FOUND_GAUGE_MAX_SIZE	= 300;		//注目度ゲージの最大長さ
	const int			FOUND_GAUGE_POS_Y		= 50;		//注目度ゲージの高さ
	const int			ITEM_ICON_FADE_SPEED	= 20;		//アイテムのアイコンの表示速度
	const int			UI_FADEOUT				= 3;		//フェードアウト速度
	const int			UI_FADEIN				= 5;		//フェードアウト速度
	const int			BLOCK_MAX_NUM			= 3;		//ブロック配置ゲージの最大配置数
	const int			FOUND_GAUGE_LINE_SIZE	= 2;		//発見ゲージのラインの太さ
	static const int	INPUT_ANIME_NUM			= 9;		//入力情報アニメーションの最大値

	const float	TUTORIAL_MAX_SIZE				= 30.0f;	//チュートリアルの最大サイズ
	const float	TUTORIAL_POS_HEIGHT				= 10.0f;	//チュートリアルの高さ
	const int	TUTORIAL_FADE_MAX				= 100;		//チュートリアルの最大透明度
	const int	TUTORIAL_FADE_MIN				= 30;		//チュートリアルの最小透明度
	const int	TUTORIAL_FADE_SPEED				= 5;		//チュートリアルの透明度変異
	
private:
	SCREEN_TYPE m_ScreenType;
	float		m_fAnimeCount;								//アニメーションのカウント
	float		m_fTutorialSize[TUTORIAL_IMAGE_NUM];		//チュートリアルの画像サイズ
	bool		m_PushFlag[IMAGE_NUM];						//操作入力フラグ
	bool		m_ControllerFlag;							//コントローラーが接続されているかフラグ
	bool		m_TutorialFlag;								//チュートリアル実行フラグ
	bool		m_DrawTutorialFlag[TUTORIAL_IMAGE_NUM];		//チュートリアルの描画フラグ
	bool		m_TutorialFadeFlag[TUTORIAL_IMAGE_NUM];		//チュートリアルの透かしフラグ
	int			m_iHandle[IMAGE_NUM][INPUT_ANIME_NUM];		//画像ハンドル(アニメーション)
	int			m_iImageNum[2];								//0：Lスティック, 1：UFO
	int			m_iTutorialHandle[TUTORIAL_IMAGE_NUM];		//チュートリアル用ハンドル
	int			m_iFade[IMAGE_NUM];							//画像透明度
	int			m_iTutorialFade[TUTORIAL_IMAGE_NUM];		//チュートリアルの透明度

public:
	UIManager();
	~UIManager();

	void Init();
	void Load();
	void Step(bool itemMode, bool blockMode, bool pase, bool ufoFound,bool catchItemFlag,bool gameoverFlag,bool clearFlag, VECTOR plPos);
	void Draw(bool gameoverFlag);
	void Fin();

private:
	//通常カメラの入力チェック
	void CheckInputPlay();
	//アイテムモードの入力チェック
	void CheckInputItemMode();
	//ブロックモードの入力チェック
	void CheckInputBlockMode();

	//ブロック配置ゲージの描画
	void DrawBlockGauge();	
	//注目度ゲージの描画
	void DrawFoundGauge();	
	//チュートリアルの描画
	void DrawTutorial();

public:
	//チュートリアルの番号を取得
	int		GetTutorialNum() { return TUTORIAL_IMAGE_NUM; }
	//チュートリアルの座標を取得
	VECTOR	GetTutorialPos(int ID) { return TUTORIAL_POS[ID]; }
	//チュートリアルの透けるフラグを設定
	void	SetTutorialFadeFlag(int ID, bool set) { m_TutorialFadeFlag[ID] = set; }
	//チューリアルを距離で透かす
	void	CheckTutorialToPlayer(VECTOR playerPos, VECTOR playCameraPos);
};
