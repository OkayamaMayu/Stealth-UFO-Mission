#pragma once
#include"TitleScene.h"
#include"SelectScene.h"
#include"PlayScene.h"
#include"ResultScene.h"
#include"GameOverScene.h"
#include"EndScene.h"

#include"../MyLib/MyLib.h"

class SceneManager
{
private:

	TitleScene		titleScene;		//タイトルシーン
	SelectScene		selectScene;	//セレクトシーン
	PlayScene		playScene;		//プレイシーン
	ResultScene		resuleScene;	//リザルトシーン
	GameOverScene	gameOverScene;	//ゲームオーバーシーン
	EndScene		endScene;		//エンドシーン

	STAGE			m_StageNum;		//現在のステージ数

public:
	SceneManager();
	~SceneManager();

	void Loop();
	void Draw();
};