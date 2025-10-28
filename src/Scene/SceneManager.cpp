#include"SceneManager.h"
#include"../Mouse/Mouse.h"

SceneManager::SceneManager()
{
	//実行して初めの画面を設定
	Data::SetScene(SCENE_TITLE);

	Fade::GetInstance()->Init();
}

SceneManager::~SceneManager()
{
	Sound::Fin();

	Mouse::GetInstance()->Fin();
}

void SceneManager::Loop()
{
	Mouse::GetInstance()->Step();

	switch (Data::GetScene())
	{
	case SCENE_TITLE:		//タイトル
		if (titleScene.Loop() != 0)
			Data::SetScene();
		break;

	case SCENE_SELECT:		//ステージ選択
		if (selectScene.Loop() != 0)
			Data::SetScene();
		break;

	case SCENE_PLAY:		//プレイ
	case SCENE_GAMEOVER:	//ゲームオーバー
		while (true)
		{
			if (playScene.Loop() != 0)
				Data::SetScene();

			//ゲームオーバーはプレイ画面の上から表示する
			if (Data::GetScene() == SCENE_GAMEOVER)
				if (gameOverScene.Loop() != 0)
				{
					Data::SetScene();
					//プレイシーンの終了処理をはさむ
					playScene.Fin();
				}
			break;
		}
		break;

	case SCENE_RESUTLT:		//リザルト
		if (resuleScene.Loop() != 0)
			Data::SetScene();
		break;

	case SCENE_END:			//エンディング
		if (endScene.Loop() != 0)
			Data::SetScene();
		break;

	default:
		break;
	}

	Fade::GetInstance()->Step();
}

void SceneManager::Draw()
{
	switch (Data::GetScene())
	{
	case SCENE_TITLE:		//タイトル
		titleScene.Draw();
		break;

	case SCENE_SELECT:		//ステージ選択
		selectScene.Draw();
		break;

	case SCENE_PLAY:		//プレイ
	case SCENE_GAMEOVER:	//ゲームオーバー
		playScene.Draw();
		//ゲームオーバーの場合はプレイ画面の上から描画する
		if(Data::GetScene() ==SCENE_GAMEOVER)
			gameOverScene.Draw();
		break;

	case SCENE_RESUTLT:		//リザルト
		resuleScene.Draw();
		break;

	case SCENE_END:			//エンディング
		endScene.Draw();
		break;

	default:
		break;
	}

	Fade::GetInstance()->Draw();
}