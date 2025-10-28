#pragma once
#include"../MyLib/MyLib.h"
#include<string.h>

class GameOverScene
{
private:
	enum GAMEOVER_SCENE
	{
		GAMEOVER_SCENE_INIT,		//初期化
		GAMEOVER_SCENE_LOAD,		//読み込み
		GAMEOVER_SCENE_LOOP,		//ループ
		GAMEOVER_SCENE_END,			//終了処理

		GAMEOVER_SCENE_NUM
	};

	//-------------------------------------------------------
	//画像の種類
	enum GAMEOVER_IMAGE_ID
	{
		BLACK_LINE,					//黒帯画像
		BLACK_FADE,					//黒グラデーション画像

		IMAGE_MAX_NUM
	};

	//進行度
	enum PROGRAM
	{
		SCREEN_MAKE,				//画面を作る
		WAIT_NOW,					//入力待ち
	};

	//テキストの種類
	enum TEXT_ID
	{
		GAMEOVER,					//ゲームオーバーの文字
		INPUT_WAITING,				//入力待ち

		TEXT_NUM
	};

	//ゲームオーバー背景画像のパス
	const string IMAGE_PATH[IMAGE_MAX_NUM] =
	{
		"data/system/black.png",
		"data/system/suspendBack.png",
	};

	//ゲームオーバーに表示するテキスト
	const string GAMEOVER_TEXT[TEXT_NUM] =
	{
		"- ゲームオーバー -",
		"ボタンを押してリスタート"
	};

	//テキスト表示位置のベース
	const VECTOR	TEXT_POS_BASE		= { SCREEN_SIZE_X / 2.0f,150.0f,0.0f };
	const float		IMAGE_MAX_SIZE		= 1.5f;			//画像の最大サイズ
	const float		TEXT_POS_ADD		= 400.0f;		//テキスト位置の間隔
	const float		SIZE_SPEED			= 0.03f;		//画像サイズの変更速度
	const int		TEXT_FADE_SPEED		= 2;			//テキストの点滅速度
	const int		TEXT_ALPHA_SPEED	= 2;			//テキストの表示速度
	const int		FONT_SIZE[TEXT_NUM] = { 60,40 };	//テキストサイズ
	const int		MAX_ALPHA			= 60;			//最大の透明度
	const int		ALPHA_SPEED			= 10;			//透明度の変更速度
	const int		FADEOUT_SPEED		= 10;			//フェードアウト速度

private:	
	GAMEOVER_SCENE	m_SceneID;							//シーン保存
	PROGRAM			m_Program;							//進行
	float			m_fSize;							//黒帯画像のサイズ
	bool			m_IsSelect;							//選択
	bool			m_TextFade;							//文字の点滅
	int				m_iHandle[IMAGE_MAX_NUM];			//画像ハンドル
	int				m_iAlpha;							//画像の透明度
	int				m_iTextAlpha[TEXT_NUM];				//テキストの透明度

public:
	int		Loop();
	void	Draw();

private:
	void	Init();
	void	Load();
	void	Step();	
	void	Fin();

	//場を整える
	void	ImageMove();
	//選択肢
	void	ReSpawn();
};