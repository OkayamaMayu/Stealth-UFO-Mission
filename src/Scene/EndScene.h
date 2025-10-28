#pragma once
#include"../MyLib/MyLib.h"

class EndScene
{
private:
	enum END_SCENE
	{
		END_SCENE_INIT,	//初期化
		END_SCENE_LOAD,	//読み込み
		END_SCENE_LOOP,	//ループ
		END_SCENE_END,	//終了処理
	
		END_SCENE_NUM
	};

	//エンディング画像種類
	enum ENDING_ID
	{
		ENDING_NORMAL,		//通常
		ENDING_SPECIAL,		//特殊

		ENDING_NUM
	};

	//画像パス
	const string IMAGE_PATH[ENDING_NUM] =
	{
		"data/end/end_1.png",	//通常エンディング
		"data/end/end_2.png"	//特殊エンディング
	};
	const float ENDING_SCROLL_SPEED			= 0.7f;						//エンディングのスクロール速度
	const float SCROLL_SPEED_SCALE			= 1.3f;						//スクロール速度の上昇倍率
	const float	ENDING_LENGTH				= -SCREEN_SIZE_Y * 4.0f;	//エンディングの長さ
	const float	ENDING_ACCELERATION_LIMIT	= -SCREEN_SIZE_Y * 3.5f;	//エンディングの加速できなくなる制限
	const int	FADEIN_SPEED				= 10;						//フェードイン速度
	const int	FADEOUT_SPEED				= 2;						//フェードアウト速度
	const int	SOUND_FADEIN_SPEED			= 5;						//サウンドのフェードイン速度
	const int	SOUND_FADEOUT_SPEED			= 5;						//サウンドのフェードアウト速度
	
private:
	END_SCENE	m_SceneID;		//シーン保存
	float		m_fPosY;		//縦座標
	int			m_iHandle;		//画像ハンドル
	bool		m_NextFlag;		//進行フラグ

public:
	int	 Loop();
	void Draw();

private:
	void Init();
	void Load();
	void Step();
	void Fin();
};