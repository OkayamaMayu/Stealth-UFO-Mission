#include "EndScene.h"
#include "DxLib.h"
#include "../MyLib/MyLib.h"


int EndScene::Loop()
{
	int iRet = 0;

	switch (m_SceneID)
	{
	case EndScene::END_SCENE_INIT:
		Init();
		m_SceneID = END_SCENE_LOAD;
		break;

	case EndScene::END_SCENE_LOAD:
		Load();
		m_SceneID = END_SCENE_LOOP;
		break;

	case EndScene::END_SCENE_LOOP:
		if (!Fade::GetInstance()->GetFadeNow())
			Step();
		break;

	case EndScene::END_SCENE_END:
		if (!Fade::GetInstance()->GetFadeNow())
		{
			Fin();
			m_SceneID = END_SCENE_INIT;
			iRet = -1;
		}
		break;

	default:
		break;
	}

	return iRet;
}

void EndScene::Init()
{
	//初期化
	m_fPosY		= 0.0f;
	m_NextFlag	= false;

	//フェードイン
	Fade::GetInstance()->RequestFade(Fade::FADE_IN, FADEIN_SPEED);
}

void EndScene::Load()
{
	int Snum = 0;
	//S評価の数を取得
	for (int i = 0; i < STAGE_END; i++)
	{
		if (Data::GetStageEvaluation(i) == STAGE_EVALUATION_S)
			Snum++;
	}

	//全ステージSなら特殊エンディングへ
	if (Snum >= STAGE_END)
	{
		m_iHandle = LoadGraph(IMAGE_PATH[ENDING_SPECIAL].c_str());
		Sound::Play(BGM_END_2);
		Sound::SetVolume(BGM_END_2, 0);
		Sound::SetFadeVolume(BGM_END_2, 100, SOUND_FADEIN_SPEED);
	}
	//上記以外なら通常エンディングへ
	else
	{
		m_iHandle = LoadGraph(IMAGE_PATH[ENDING_NORMAL].c_str());
		Sound::Play(BGM_END_1);
		Sound::SetVolume(BGM_END_1, 0);
		Sound::SetFadeVolume(BGM_END_1, 100, SOUND_FADEIN_SPEED);
	}
}

void EndScene::Step()
{
	if(!m_NextFlag)
	{
		//速度を設定
		float sp = ENDING_SCROLL_SPEED;

		//キーを押していると加速する
		if (InputManager::CheckKeep(INPUT_DETERMINATION))
			sp *= SCROLL_SPEED_SCALE;

		//一定以上スクロールしていると加速させない
		if (m_fPosY <= ENDING_ACCELERATION_LIMIT)
		{
			sp = ENDING_SCROLL_SPEED;
		}

		//最後までスクロールしたら次の工程へ
		if (Math::MatchSpecifiedNum(m_fPosY, ENDING_LENGTH, sp))
		{
			m_NextFlag = true;
		}
	}
	else
	{
		//完了していたら入力を待つ
		if (InputManager::CheckPush(INPUT_DETERMINATION))
		{
			//終了処理へ
			m_SceneID = END_SCENE_END;

			//次のシーンを設定する
			Data::SetNextScene(SCENE_SELECT);

			//サウンドをフェードアウトする
			Sound::SetFadeVolume(BGM_END_1, 0, SOUND_FADEOUT_SPEED);
			Sound::SetFadeVolume(BGM_END_2, 0, SOUND_FADEOUT_SPEED);

			//サウンドを再生
			Sound::Play(SE_ENTER);

			//画面をフェードアウト
			Fade::GetInstance()->RequestFade(Fade::FADE_OUT, FADEOUT_SPEED);
		}
	}
}

void EndScene::Draw()
{
	DrawGraphF(0.0f,m_fPosY, m_iHandle, true);
}

void EndScene::Fin()
{

}
