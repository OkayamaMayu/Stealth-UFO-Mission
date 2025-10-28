#include"../Scene/SceneManager.h"

int GameOverScene::Loop()
{
	int iRet = 0;

	switch (m_SceneID)
	{
	case GAMEOVER_SCENE_INIT:
		Init();
		m_SceneID = GAMEOVER_SCENE_LOAD;

		break;

	case GAMEOVER_SCENE_LOAD:
		Sound::Play(BGM_GAMEOVER);
		Sound::SetVolume(BGM_GAMEOVER, 0);
		Sound::SetFadeVolume(BGM_GAMEOVER, 100);

		Load();
		m_SceneID = GAMEOVER_SCENE_LOOP;
		break;

	case GAMEOVER_SCENE_LOOP:
		Step();
		break;

	case GAMEOVER_SCENE_END:
		if(!Fade::GetInstance()->GetFadeNow())
		{
			Fin();
			m_SceneID = GAMEOVER_SCENE_INIT;
			iRet = -1;
		}
		break;

	default:
		break;
	}

	return iRet;
}

//===================================================================

void GameOverScene::Init()
{
	//ゲームオーバー背景初期化
	for (int i = 0; i < TEXT_NUM; i++)
	{
		m_iTextAlpha[i] = 0;
	}
	//画面を作るところに進行
	m_Program = SCREEN_MAKE;

	//画像を最大サイズに設定
	m_fSize = IMAGE_MAX_SIZE;
	
	//テキスト点滅フラグを設定
	m_TextFade = false;

	//爆破音再生
	Sound::Play(SE_GAMEOVER);
}

void GameOverScene::Load()
{
	for (int i = 0; i < IMAGE_MAX_NUM; i++)
	{
		m_iHandle[i] = LoadGraph(IMAGE_PATH[i].c_str());
	}
}

void GameOverScene::Step()
{
	if (Fade::GetInstance()->GetFadeNow())
		return;

	switch (m_Program)
	{
	case SCREEN_MAKE:	//画面を作る
		ImageMove();
		break;

	case WAIT_NOW:		//入力待ち
		//進行
		if (Data::GetNextScene() != SCENE_GAMEOVER)
		{
			m_SceneID = GAMEOVER_SCENE_END;
		}

		//リスポーン入力待ち
		ReSpawn();

		//文字の面滅
		if (m_TextFade)
		{
			if (Math::MatchSpecifiedNum(m_iTextAlpha[INPUT_WAITING], 0, TEXT_FADE_SPEED))
				m_TextFade = false;
		}
		else
		{
			if (Math::MatchSpecifiedNum(m_iTextAlpha[INPUT_WAITING], 100, TEXT_FADE_SPEED))
				m_TextFade = true;
		}
		break;

	default:
		break;
	}

	//テキストの表示
	Math::MatchSpecifiedNum(m_iTextAlpha[GAMEOVER], 100, TEXT_ALPHA_SPEED);
}

void GameOverScene::Draw()
{
	//グラデーション表示
	VECTOR vPos = {};
	Draw::DrawGraphAlpha(vPos, m_iAlpha, m_iHandle[BLACK_FADE], true);
	//黒帯表示
	vPos = { SCREEN_SIZE_X / 2.0f,SCREEN_SIZE_Y / 2.0f,0.0f };
	Draw::DrawRotaGraphV(vPos, m_fSize, 0.0f, m_iHandle[BLACK_LINE], true);


	//テキストの表示
	VECTOR textPos = TEXT_POS_BASE;
	for (int i = 0; i < TEXT_NUM; i++)
	{
		SetFontSize(FONT_SIZE[i]);
		Draw::DrawFormatStringCenterAlpha(textPos, m_iTextAlpha[i], COLOR[WHITE], GAMEOVER_TEXT[i].c_str());

		textPos.y += TEXT_POS_ADD;
	}
	//テキストサイズをもとに戻す
	SetFontSize(20);
}

void GameOverScene::Fin()
{
	//BGMを停止
	Sound::SetFadeVolume(BGM_GAMEOVER, 0);
}

//=================================================================================

void GameOverScene::ImageMove()
{
	bool nextProgramFlag = true;

	//どちらかの変更が完了していなかったらフラグを折る
	if (!Math::MatchSpecifiedNum(m_iAlpha, MAX_ALPHA, ALPHA_SPEED) ||
		!Math::MatchSpecifiedNum(m_fSize, 1.0f, SIZE_SPEED))
		nextProgramFlag = false;

	if (nextProgramFlag)
	{
		m_Program = WAIT_NOW;
	}

	//場を強制的に完成させる
	if (InputManager::CheckPush(INPUT_DETERMINATION))
	{
		for (int i = 0; i < TEXT_NUM; i++)
		{
			m_iTextAlpha[i] = 0;
		}

		//情報を完成状態に変更
		m_fSize = 1.0f;
		m_iAlpha = MAX_ALPHA;

		//待機状態にする
		m_Program = WAIT_NOW;
	}
}

void GameOverScene::ReSpawn()
{
	//何かしらが押されたら
	if (Input::CheckInputPush())
	{
		//次のシーンを設定
		Data::SetNextScene(SCENE_PLAY);
		//フェードアウト
		Fade::GetInstance()->RequestFade(Fade::FADE_OUT, FADEOUT_SPEED);

		//リスポーン地点からスタート
		Data::SetReSpawnFlag(true);
	}
}