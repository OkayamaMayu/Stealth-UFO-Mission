#pragma once
#include"../MyLib/MyLib.h"
#include<string.h>

//選択肢
enum OPTION
{
	OPTION_NEUTRAL = -1,						//未選択

	OPTION_FROM_FIRST,							//ステージをやり直す
	OPTION_STAGE_SELECTION,						//ステージ選択に戻る
	OPTION_OPERATION,							//操作説明
	OPTION_BACK_GAME,							//ゲームに戻る

	OPTION_NUM
};

class Pause
{
private:
	//構成するもの
	enum IMAGE
	{
		IMAGE_BACK,								//背景

		IMAGE_SELECT_WINDOW1,					//選択ウィンドウ
		IMAGE_SELECT_WINDOW2,					//選択ウィンドウ
		IMAGE_SELECT_WINDOW3,					//選択ウィンドウ
		IMAGE_SELECT_WINDOW4,					//選択ウィンドウ

		IMAGE_OPERATION_BACK,					//操作説明の背景

		IMAGE_NUM
	};

	//進行度
	enum PROGRAM
	{
		IMAGE_MOVE,								//画像の移動
		SELECT_NOW,								//選択中
		CLOSE_WINDOW,							//ウィンドウを閉じる
	};

	//画像ID
	enum PAUSE_IMAGE_ID
	{
		PAUSE_IMAGE_BACK_GROUND,				//背景
		PAUSE_IMAGE_SELECT_WINDOW,				//選択肢
		PAUSE_IMAGE_TEXT_TRIANGLE,				//選択三角
		PAUSE_IMAGE_DESCRIPTION_CONTROLLER,		//操作説明_コントローラー
		PAUSE_IMAGE_DESCRIPTION_KEY,			//操作説明_キーボード

		PAUSE_IMAGE_NUM
	};

	const VECTOR	SELECT_WINDOW_SIZE		= { 459.0f,62.0f,0.0f };	//セレクトウィンドウのサイズ
	const VECTOR	PAUSE_IMAGE_POS[5]		=
	{
		{0.0f,0.0f,0.0f},															//背景

		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2 - SELECT_WINDOW_SIZE.y - 124,0.0f},	//選択肢1
		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2 - SELECT_WINDOW_SIZE.y - 0,0.0f},		//選択肢2
		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2 + SELECT_WINDOW_SIZE.y + 0,0.0f},		//選択肢3
		{SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2 + SELECT_WINDOW_SIZE.y + 124,0.0f},	//選択肢4
	};
	const float		PAUSE_WINDOW_ADD_SPEED	= 1.0f;									//ウィンドウの加速度
	const float		PAUSE_WINDOW_MAX_SPEED	= 50.0f;								//ウィンドウの最高速度
	const float		PAUSE_KEY_INTERVAL		= 0.2f;									//キー入力の感覚
	const float		INIT_SELECT_POS			= 463.0f;								//ウィンドウの初期位置
	const float		SELECT_WINDOW_POS		= -20.0f;								//選択状態のウィンドウ位置
	const float		INIT_SPEED[IMAGE_NUM]	=										//速度の初期値
	{
		0.0f,
		-10.0f,
		-20.0f,
		-30.0f
	};
	const string	PAUSE_IMAGE_PATH[PAUSE_IMAGE_NUM] =								//画像パス
	{
		"data/text/suspendBack.png",
		"data/text/selectWindow459×62.png",
		"data/text/textTriangle45×45_2.png",
		"data/option/1240x680.png",
		"data/option/1240x680_2.png",
	};
	const string	OPTION_TEXT[OPTION_NUM] =										//選択肢のテキスト
	{
		"ステージをやり直す",
		"ステージ選択に戻る",
		"操作説明",
		"ゲームに戻る",
	};
	const int		SELECT_WINDOW_MAX_NUM		= 2;								//選択肢ウィンドウの画像数
	const int		SELECT_WINDOW_XY_NUM[2]		= { 1,2 };							//選択肢ウィンドウのX数とY数
	const int		OPERATION_ALPHA_SPEED		= 10;								//操作説明の透明度変化速度
	const int		FROM_FIRST_FADEOUT			= 10;								//ステージをやり直すフェードアウト
	const int		STAGE_SELECTION_FADEOUT		= 5;								//ステージをやり直すフェードアウト
	const int		CLOSE_WINDOW_ALPHA_SPEED	= 10;								//閉じるときの透明度の変更速度
	const int		ALPHA_SPEED					= 2;								//画像の透明度の変更速度
	const int		SELECT_TEXT_FONT_SIZE		= 30;								//選択文字もフォントサイズ
	const int		DEFAULT_TEXT_FONT_SIZE		= 20;								//デフォルトフォンサイズ

private:
	PROGRAM m_Program;							//進行
	VECTOR	m_vPos[IMAGE_NUM];					//画像の座標
	OPTION	m_Option;							//選択中の項目
	float	m_fWindowSpeed[OPTION_NUM];			//ウィンドウの加減速
	float	m_fKeyInterval;						//前回選択時間
	bool	m_IsPause;							//ポーズ画面になっているか
	bool	m_IsSelect;							//選択完了フラグ
	bool	m_IsOperation;						//操作説明
	bool	m_TraianglMoveL;					//三角
	int		m_iBackHandle;						//背景
	int		m_iWindowHandle[2];					//選択ウィンドウ
	int		m_iTraiangleHandle;					//選択三角
	int		m_iOperationHandle;					//操作説明のハンドル
	int		m_iAlpha[IMAGE_NUM];				//透明度
	int		m_iOperationAlpha;					//操作説明の透明度

public:
	void	Init();
	void	InitVariable();
	void	Load();
	void	Step();
	void	Draw();
	void	Fin();

	//ポーズ画面を開いているか取得
	bool	GetIsPause() { return m_IsPause; }
	//ポーズ画面を開いているか設定
	void	SetIsPause(bool set) { m_IsPause = set; }
	//選択が完了しているか取得
	bool	GetIsSelect() { return m_IsSelect; }
	//選択が完了しているか設定
	void	SetIsSelect(bool set) { m_IsSelect = set; }
	//選択中の項目を取得
	OPTION	GetOption() { return m_Option; }
	//選択中の項目を設定
	void	SetOption(OPTION set) { m_Option = set; }
};