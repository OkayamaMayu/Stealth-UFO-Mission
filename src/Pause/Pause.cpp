#include"Pause.h"

void Pause::Init()
{
	//変数の初期化
	InitVariable();

	//ハンドルの初期化
	m_iBackHandle							= -1;
	m_iTraiangleHandle						= -1;
	m_iOperationHandle						= -1;
	memset(&m_iWindowHandle, -1, sizeof(int));
}

void Pause::InitVariable()
{
	//変数の初期化
	m_IsPause								= false;
	m_IsSelect								= false;
	m_IsOperation							= false;
	m_TraianglMoveL							= true;
	m_Program								= IMAGE_MOVE;
	m_iOperationAlpha						= 0;
	m_fKeyInterval							= PAUSE_KEY_INTERVAL;
	m_fWindowSpeed[OPTION_FROM_FIRST]		= INIT_SPEED[OPTION_FROM_FIRST];
	m_fWindowSpeed[OPTION_STAGE_SELECTION]	= INIT_SPEED[OPTION_STAGE_SELECTION];
	m_fWindowSpeed[OPTION_OPERATION]		= INIT_SPEED[OPTION_OPERATION];
	m_fWindowSpeed[OPTION_BACK_GAME]		= INIT_SPEED[OPTION_BACK_GAME];
	memset(&m_iAlpha, 0, sizeof(int));

	//初期位置の設定
	m_vPos[IMAGE_BACK]						= PAUSE_IMAGE_POS[IMAGE_BACK];
	m_vPos[IMAGE_SELECT_WINDOW1]			= { SCREEN_SIZE_X + INIT_SELECT_POS / 2,	PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW1].y,	0.0f };
	m_vPos[IMAGE_SELECT_WINDOW2]			= { -INIT_SELECT_POS / 2,					PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW2].y,	0.0f };
	m_vPos[IMAGE_SELECT_WINDOW3]			= { SCREEN_SIZE_X + INIT_SELECT_POS / 2,	PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW3].y,	0.0f };
	m_vPos[IMAGE_SELECT_WINDOW4]			= { -INIT_SELECT_POS / 2,					PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW4].y,	0.0f };

	//選択中設定
	m_Option = OPTION_NEUTRAL;
}

void Pause::Load()
{
	if(m_iBackHandle==-1)
	{
		m_iBackHandle = LoadGraph(PAUSE_IMAGE_PATH[PAUSE_IMAGE_BACK_GROUND].c_str());
	}

	//選択肢読み込み
	if(m_iWindowHandle[0]==-1)
	{
		LoadDivGraph(
			PAUSE_IMAGE_PATH[PAUSE_IMAGE_SELECT_WINDOW].c_str(),
			SELECT_WINDOW_MAX_NUM, SELECT_WINDOW_XY_NUM[0], SELECT_WINDOW_XY_NUM[1],
			(int)SELECT_WINDOW_SIZE.x, (int)SELECT_WINDOW_SIZE.y, 
			m_iWindowHandle);
	}

	if(m_iTraiangleHandle==-1)
	{
		m_iTraiangleHandle = LoadGraph(PAUSE_IMAGE_PATH[PAUSE_IMAGE_TEXT_TRIANGLE].c_str());
	}

	if (m_iOperationHandle == -1)
	{
		//操作説明_コントローラー
		if(Input::Controller::CheckControllerConnected())
		{
			m_iOperationHandle = LoadGraph(PAUSE_IMAGE_PATH[PAUSE_IMAGE_DESCRIPTION_CONTROLLER].c_str());
		}
		//操作説明_キーボード
		else
		{
			m_iOperationHandle = LoadGraph(PAUSE_IMAGE_PATH[PAUSE_IMAGE_DESCRIPTION_KEY].c_str());
		}
	}
}

void Pause::Step()
{	
	if (!m_IsPause)
	{
		//コントロールが押されたら
		if (InputManager::CheckPush(INPUT_OPTION_LEFT)|| InputManager::CheckPush(INPUT_OPTION_RIGHT))
		{
			//初期化
			InitVariable();

			//ポーズ画面を開く
			m_IsPause = true;

			//サウンドを再生
			Sound::Play(SE_PLAY_PAUSE);
		}
		else
			return;
	}

	//操作説明
	if (m_IsOperation)
	{
		switch (m_Program)
		{
			case IMAGE_MOVE:	//画像の移動
				Math::MatchSpecifiedNum(m_iOperationAlpha, 100, OPERATION_ALPHA_SPEED);
				if (m_iOperationAlpha >= 100)
				{
					m_Program		= SELECT_NOW;
				}
				break;

			case SELECT_NOW:	//選択中
				if (InputManager::CheckPush(INPUT_DETERMINATION))
				{
					m_Program		= CLOSE_WINDOW;
				}
				break;

			case CLOSE_WINDOW:	//画面を閉じる
				Math::MatchSpecifiedNum(m_iOperationAlpha, 0, OPERATION_ALPHA_SPEED);
				if (m_iOperationAlpha <= 0)
				{
					//選択画面に戻す
					m_Program		= SELECT_NOW;
					//操作説明を閉じる
					m_IsOperation	= false;

					//サウンドを再生する
					Sound::Play(SE_PLAY_PAUSE);
				}
				break;

		default:
			break;
		}
	}
	else //選択
	{
		switch (m_Program)
		{
		case IMAGE_MOVE:

			for (int i = 0; i < OPTION_NUM; i++)
			{
				//選択を指定の位置に移動する
				Math::MatchSpecifiedNum(m_fWindowSpeed[i], PAUSE_WINDOW_MAX_SPEED, PAUSE_WINDOW_ADD_SPEED);
				Math::MatchSpecifiedNum(m_vPos[IMAGE_SELECT_WINDOW1 + i].x, PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW1].x, m_fWindowSpeed[i]);
			}

			//最後の選択肢が移動を完了したら進行
			if (m_vPos[IMAGE_SELECT_WINDOW1 + OPTION_NUM - 1].x == PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW1 + OPTION_NUM - 1].x)
			{
				m_Option		= OPTION_FROM_FIRST;
				m_fKeyInterval	= PAUSE_KEY_INTERVAL;
				m_Program		= SELECT_NOW;
			}

			break;

		case SELECT_NOW:
		{
			if (Math::MatchSpecifiedNum(m_fKeyInterval, PAUSE_KEY_INTERVAL, FRAME_TIME))
			{
				//上へ移動
				if (InputManager::CheckSelectUp())
				{
					//入力間隔をリセット
					m_fKeyInterval = 0.0f;

					if (m_Option != m_Option - 1)
					{
						//サウンドを再生
						Sound::Play(SE_CURSOR);
					}

					//選択肢を移動
					m_Option = static_cast<OPTION>(m_Option - 1);
					if (m_Option <= OPTION_NEUTRAL)
					{
						//一番上に固定する
						m_Option = OPTION_FROM_FIRST;
					}
				}
				else if (InputManager::CheckSelectDown())
				{
					//入力間隔をリセット
					m_fKeyInterval = 0.0f;

					if (m_Option != m_Option + 1)
					{
						//サウンドを再生
						Sound::Play(SE_CURSOR);
					}

					//選択肢を移動
					m_Option = static_cast<OPTION>(m_Option + 1);
					if (m_Option >= OPTION_NUM)
					{
						//一番下に固定する
						m_Option = OPTION_BACK_GAME;
					}
				}
			}

			//選択肢を移動する
			if (!InputManager::CheckSelectUp() && !InputManager::CheckSelectDown())
			{
				m_fKeyInterval = PAUSE_KEY_INTERVAL;			
			}

			//移動したら選択をリセット
			if (InputManager::CheckSelectUp() || InputManager::CheckSelectDown())
			{
				Select::SelectReset();
			}

			//マウス判定
			bool selectResetFlag = true;
			for (int i = 0; i < OPTION_NUM; i++)
			{
				//座標を取得
				VECTOR pos = PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW1 + i];

				//マウスで選択されていたら
				if (Select::SelectMouse(pos, SELECT_WINDOW_SIZE))
				{
					if (m_Option != i)
					{
						//サウンドを再生
						Sound::Play(SE_CURSOR);
					}

					//選択しているものを変更
					m_Option		= static_cast<OPTION>(i);
					selectResetFlag = false;

					break;
				}
			}

			//変更したら選択中をリセット
			if (selectResetFlag && Select::GetDecisionToMouseFlag())
			{
				Select::SelectReset();
			}

			//決定
			bool closeFlag = false;
			switch (m_Option)
			{
			case OPTION_FROM_FIRST:			//ステージをやり直す
				if (Select::SelectStep())
				{
					Fade::GetInstance()->RequestFade(Fade::FADE_OUT, FROM_FIRST_FADEOUT);
					closeFlag = true;
				}
				break;

			case OPTION_STAGE_SELECTION:	//ステージ選択に戻る
				if (Select::SelectStep())
				{
					Fade::GetInstance()->RequestFade(Fade::FADE_OUT, STAGE_SELECTION_FADEOUT);
					closeFlag = true;
				}
				break;

			case OPTION_OPERATION:			//操作説明
			{
				bool Flag = Select::GetSelectMouseFlag();
				if (!Mouse::GetInstance()->GetMouseCursorDrawFlag() && InputManager::CheckPush(INPUT_DETERMINATION) ||
					Mouse::GetInstance()->GetMouseCursorDrawFlag() && Flag && InputManager::CheckPush(INPUT_DETERMINATION))
				{
					//操作説明を表示する
					m_IsOperation	= true;
					//画面の作成工程に変更
					m_Program		= IMAGE_MOVE;

					//サウンドを再生する
					Sound::Play(SE_ENTER);
				}
				break;
			}

			case OPTION_BACK_GAME:			//ゲームに戻る
			{
				bool Flag = Select::GetSelectMouseFlag();
				if (!Mouse::GetInstance()->GetMouseCursorDrawFlag() && InputManager::CheckPush(INPUT_DETERMINATION) ||
					Mouse::GetInstance()->GetMouseCursorDrawFlag() && Flag && InputManager::CheckPush(INPUT_DETERMINATION))
				{
					closeFlag = true;
				}
				break;
			}

			default:
				break;
			}

			//直接ポーズをとく
			if (InputManager::CheckPush(INPUT_OPTION_LEFT)|| InputManager::CheckPush(INPUT_OPTION_RIGHT))
			{
				m_Option = OPTION_BACK_GAME;
				closeFlag = true;
			}

			//ウィンドウを閉じる
			if (closeFlag)
			{
				m_Program = CLOSE_WINDOW;

				//サウンドを再生する
				Sound::Play(SE_ENTER);
				Sound::Play(SE_PLAY_PAUSE);
			}

			break;
		}
		case CLOSE_WINDOW:					//ウィンドウを閉じる

			for (int i = 0; i < IMAGE_NUM; i++)
			{
				Math::MatchSpecifiedNum(m_iAlpha[i], 0, CLOSE_WINDOW_ALPHA_SPEED);
			}

			if (m_iAlpha[IMAGE_OPERATION_BACK] <= 0)
			{
				m_IsSelect = true;
			}

			break;

		default:
			break;
		}

		for (int i = 0; i < IMAGE_NUM; i++)
		{
			Math::MatchSpecifiedNum(m_iAlpha[i], 100, ALPHA_SPEED);
		}		
	}
}

void Pause::Draw()
{
	if (!m_IsPause)
		return;

	//透明度付き背景描画
	Draw::DrawGraphAlpha(m_vPos[IMAGE_BACK], m_iAlpha[IMAGE_BACK], m_iBackHandle, true);

	SetFontSize(SELECT_TEXT_FONT_SIZE);
	for (int i = 0; i < OPTION_NUM; i++)
	{
		VECTOR pos = m_vPos[IMAGE_SELECT_WINDOW1 + i];
		if (m_Option == static_cast<OPTION>(i))
		{
			pos.x += SELECT_WINDOW_POS;
			//選択中
			Draw::DrawRotaGraphAlpha(
				pos, 1.0f, 0.0f,
				m_iAlpha[IMAGE_SELECT_WINDOW1 + 1], m_iWindowHandle[1], true);

			Draw::DrawFormatStringCenterAlpha(pos, m_iAlpha[IMAGE_SELECT_WINDOW1 + 1], COLOR[WHITE], "%s", OPTION_TEXT[i].c_str());
		}
		else
		{
			//未選択
			Draw::DrawRotaGraphAlpha(
				pos, 1.0f, 0.0f,
				m_iAlpha[IMAGE_SELECT_WINDOW1 + 1], m_iWindowHandle[0], true);

			Draw::DrawFormatStringCenterAlpha(pos, m_iAlpha[IMAGE_SELECT_WINDOW1 + 1], COLOR[GRAY], "%s", OPTION_TEXT[i].c_str());

		}
	}
	SetFontSize(DEFAULT_TEXT_FONT_SIZE);

	//操作説明が出ていたら
	if (m_IsOperation)
	{
		Draw::DrawRotaGraphAlpha(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, 1.0f, 0.0f, m_iOperationAlpha, m_iOperationHandle, true);
	}

	//座標を変更
	VECTOR pos	= PAUSE_IMAGE_POS[IMAGE_SELECT_WINDOW1 + m_Option];
	pos.x		+= SELECT_WINDOW_POS;
	//選択ウィンドウの描画
	Select::Draw(pos);

	//マウスカーソルの表示
	Mouse::GetInstance()->Draw();
}

void Pause::Fin()
{

}