#include"DxLib.h"
#include"UIManager.h"
#include"../MyLib/MyLib.h"

UIManager::UIManager()
{
	for (int i = 0; i < UIManager::IMAGE_NUM; i++)
	{
		for(int n=0;n<2;n++)
		{
			m_iHandle[i][n] = -1;
		}
	}
}
UIManager::~UIManager()
{
}

void UIManager::Init()
{
	for (int i = 0; i < UIManager::IMAGE_NUM; i++)
	{
		m_PushFlag[i] = false;
		m_iFade[i] = 0;
	}

	m_fAnimeCount = 0.0f;

	if (Input::Controller::CheckControllerConnected())
	{
		m_ControllerFlag = true;
	}
	else
	{
		m_ControllerFlag = false;
	}

	for (int i = 0; i < 2; i++)
	{
		m_iImageNum[i] = 0;
	}

	for (int i = 0; i < TUTORIAL_IMAGE_NUM; i++)
	{
		m_DrawTutorialFlag[i] = false;
		m_fTutorialSize[i] = 0.0f;
		m_iTutorialFade[i] = 100;
		m_TutorialFadeFlag[i] = true;
	}
}
void UIManager::Load()
{
	//コントローラーが接続されていたら
	if (m_ControllerFlag)
	{
		for (int i = 0; i < UIManager::SYSTEM_CONTROLLER_IMAGE_NUM; i++)
		{
			if (m_iHandle[i][0] != -1)
				continue;

			if (i == SYSTEM_CONTROLLER_PLAY_TEXT ||
				i == SYSTEM_CONTROLLER_ITEM_TEXT ||
				i == SYSTEM_CONTROLLER_BLOCK_TEXT||
				i == SYSTEM_CONTROLLER_CAMERA||
				i == SYSTEM_CATCH_ITEM)
			{
				m_iHandle[i][0] = LoadGraph(UI_IMAGE_PATH[i].c_str());
			}
			else
			{
				if(i== SYSTEM_CONTROLLER_MOVE)
					LoadDivGraph(UI_IMAGE_PATH[i].c_str(), 9, 3, 3, SCREEN_SIZE_X, SCREEN_SIZE_Y, m_iHandle[i]);

				else if(i== SYSTEM_CONTROLLER_QTE)
					LoadDivGraph(UI_IMAGE_PATH[i].c_str(), 4, 2, 2, SCREEN_SIZE_X, SCREEN_SIZE_Y, m_iHandle[i]);

				else
					LoadDivGraph(UI_IMAGE_PATH[i].c_str(), 2, 1, 2, SCREEN_SIZE_X, SCREEN_SIZE_Y, m_iHandle[i]);
			}
		}
	}
	//接続されていなかったら
	else
	{
		for (int i = 0; i < UIManager::SYSTEM_KEY_AND_MOUSE_IMAGE_NUM; i++)
		{
			if (m_iHandle[i][0] != -1)
				continue;

			if (i == SYSTEM_PLAY_TEXT ||
				i == SYSTEM_ITEM_TEXT ||
				i == SYSTEM_BLOCK_TEXT ||
				i == SYSTEM_CATCH_ITEM)
			{
				m_iHandle[i][0] = LoadGraph(UI_IMAGE_PATH[i].c_str());
			}
			else
			{
				LoadDivGraph(UI_IMAGE_PATH[i].c_str(), 2, 1, 2, SCREEN_SIZE_X, SCREEN_SIZE_Y, m_iHandle[i]);
			}
		}
	}

	//ブロックゲージを読み込み
	for (int i = SYSTEM_BLOCK_GAUGE_UNDER; i < UIManager::IMAGE_NUM; i++)
	{
		m_iHandle[i][0] = LoadGraph(UI_IMAGE_PATH[i].c_str());
	}

	if (Data::GetSelectStage() == STAGE_0)
	{
		m_TutorialFlag = true;
	}
	else
	{
		m_TutorialFlag = false;
	}

	/*====================
		チュートリアル
	====================*/
	//操作方法関係ないチュートリアルを読み込み
	for (int i = 0; i <= TUTORIAL_INVINCIBLE_AREA; i++)
	{
		m_iTutorialHandle[i] = LoadGraph(TUTORIAL_IMAGE_PATH[i].c_str());
	}

	//操作が分かれる部分--------------------------------------------------------
	
	//読み込み範囲：1,はじめ 2,おわり 
	//初期はコントローラー接続
	int tutorialImageNum[2] = { TUTORIAL_ITEM1_CON ,TUTORIAL_BLOCK2_CON };
	//コントローラーが接続されていなかったら
	if (!m_ControllerFlag)
	{
		//範囲をキーボード、マウスに切り替え
		tutorialImageNum[0] = TUTORIAL_ITEM1_KEY;
		tutorialImageNum[1] = TUTORIAL_BLOCK2_KEY;
	}

	//指定した範囲で読み込み
	for (int i = tutorialImageNum[0]; i <= tutorialImageNum[1]; i++)
	{
		m_iTutorialHandle[i] = LoadGraph(TUTORIAL_IMAGE_PATH[i].c_str());
	}
}
void UIManager::Step(bool itemMode, bool blockMode, bool pase, bool ufoFound, bool catchItemFlag, bool gameoverFlag, bool clearFlag, VECTOR plPos)
{
	//入力をリセット
	for (int i = 0; i < UIManager::IMAGE_NUM; i++)
	{
		if (i == SYSTEM_QTE)
			continue;
		
		m_PushFlag[i] = false;
	}

	//現在の画面を確認
	m_ScreenType = TYPE_PLAY;
	if (itemMode)
	{
		m_ScreenType = TYPE_ITEM;
	}
	if (blockMode)
	{
		m_ScreenType = TYPE_BLOCK;
	}
	if (pase)
	{
		m_ScreenType = TYPE_PASE;
	}
	if (ufoFound)
	{
		m_ScreenType = TYPE_UFO;
	}

	else
	{
		m_fAnimeCount = 0.0f;
		m_iImageNum[1] = 0;
	}

	//アイテムアイコンの表示非表示
	if(!catchItemFlag||Data::GetScene() == SCENE_GAMEOVER || gameoverFlag || clearFlag)
		Math::MatchSpecifiedNum(m_iFade[SYSTEM_CATCH_ITEM], 0, ITEM_ICON_FADE_SPEED);
	else if (catchItemFlag)
		Math::MatchSpecifiedNum(m_iFade[SYSTEM_CATCH_ITEM], 100, ITEM_ICON_FADE_SPEED);

	//入力判定
	switch (m_ScreenType)
	{
	case UIManager::TYPE_PLAY:
		CheckInputPlay();
		break;

	case UIManager::TYPE_ITEM:
		CheckInputItemMode();
		break;

	case UIManager::TYPE_BLOCK:
		CheckInputBlockMode();
		break;

	case UIManager::TYPE_UFO:
	{

		if (Math::MatchSpecifiedNum(m_fAnimeCount, 0.1f, FRAME_TIME))
		{
			m_fAnimeCount = 0.0f;
			m_PushFlag[SYSTEM_QTE] = !m_PushFlag[SYSTEM_QTE];

			m_iImageNum[1]++;
			if (m_iImageNum[1] >= 4)
			{
				m_iImageNum[1] = 0;
			}
		}

		break;
	}

	case UIManager::TYPE_PASE:
		break;

	default:
		break;
	}

	//UIの透明度
	if (Data::GetScene() == SCENE_GAMEOVER || gameoverFlag || clearFlag)
	{
		//UIをゆっくり消す
		for (int i = 0; i < UIManager::IMAGE_NUM; i++)
		{
			Math::MatchSpecifiedNum(m_iFade[i], 0, UI_FADEOUT);
		}
	}
	else
	{
		for (int i = 0; i < UIManager::IMAGE_NUM; i++)
		{
			if (i != SYSTEM_CATCH_ITEM)
				Math::MatchSpecifiedNum(m_iFade[i], 100, UI_FADEIN);
		}
	}

	//チュートリアル
	if (!m_TutorialFlag)
		return;

	for (int i = 0; i < TUTORIAL_IMAGE_NUM; i++)
	{
		if (m_ControllerFlag && i >= TUTORIAL_ITEM1_KEY)
		{
			break;
		}
		else if (!m_ControllerFlag && i == TUTORIAL_ITEM1_CON)
		{
			//コントローラーのチュートリアル分ずらす
			i += 4;
		}

		//チュートリアルを展開
		if (Math::GetDistance(plPos, TUTORIAL_POS[i]) <= 100.0f)
		{
			m_DrawTutorialFlag[i] = true;
		}
		else
		{
			m_DrawTutorialFlag[i] = false;
		}

		if (m_DrawTutorialFlag[i])
		{
			Math::MatchSpecifiedNum(m_fTutorialSize[i], TUTORIAL_MAX_SIZE);
		}
		else
		{
			Math::MatchSpecifiedNum(m_fTutorialSize[i], 0.0f);
		}

		//透明度を変化
		if (m_TutorialFadeFlag[i])
		{
			Math::MatchSpecifiedNum(m_iTutorialFade[i], TUTORIAL_FADE_MAX, TUTORIAL_FADE_SPEED);
		}
		else
		{
			Math::MatchSpecifiedNum(m_iTutorialFade[i], TUTORIAL_FADE_MIN, TUTORIAL_FADE_SPEED);
		}
	}
}
void UIManager::Draw(bool gameoverFlag)
{
	//ゲームオーバーの時は描画しない
	if(!gameoverFlag)
		//チュートリアル
		DrawTutorial();

	//ブロック配置ゲージ
	DrawBlockGauge();

	//注目度ゲージ
	DrawFoundGauge();

	//操作説明
	switch (m_ScreenType)
	{
	case UIManager::TYPE_PLAY:
		Draw::DrawRotaGraphAlpha(SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y / 2.0f, 1.0f, 0.0f, m_iFade[SYSTEM_CATCH_ITEM], m_iHandle[SYSTEM_CATCH_ITEM][0], true);

		if(m_ControllerFlag)	//コントローラー
			for (int i = SYSTEM_KEY_AND_MOUSE_IMAGE_NUM; i < UIManager::SYSTEM_CONTROLLER_PLAY_IMAGE_NUM; i++)
			{
				if(i == SYSTEM_CONTROLLER_MOVE)
				{
					Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_iImageNum[0]], true);
				}

				else
				{
					Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_PushFlag[i]], true);
				}
			}
		else	//キーボード
			for (int i = 0; i < UIManager::SYSTEM_PLAY_IMAGE_NUM; i++)
			{
				Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_PushFlag[i]], true);
			}
		break;

	case UIManager::TYPE_ITEM:
		Draw::DrawRotaGraphAlpha(SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y / 2.0f- ITEM_CATCH_POS, 1.0f, 0.0f, m_iFade[SYSTEM_CATCH_ITEM], m_iHandle[SYSTEM_CATCH_ITEM][0], true,true,true);

		if (m_ControllerFlag)	//コントローラー
			for (int i = UIManager::SYSTEM_CONTROLLER_PLAY_IMAGE_NUM; i < UIManager::SYSTEM_CONTROLLER_ITEM_IMAGE_NUM; i++)
			{
				Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_PushFlag[i]], true);
			}
		else	//キーボード
			for (int i = UIManager::SYSTEM_PLAY_IMAGE_NUM; i < UIManager::SYSTEM_ITEM_IMAGE_NUM; i++)
			{
				Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_PushFlag[i]], true);
			}
		break;

	case UIManager::TYPE_BLOCK:
		if (m_ControllerFlag)	//コントローラー
			for (int i = UIManager::SYSTEM_CONTROLLER_ITEM_IMAGE_NUM; i < UIManager::SYSTEM_CONTROLLER_BLOCK_IMAGE_NUM; i++)
			{
				Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_PushFlag[i]], true);
			}
		else	//キーボード
			for (int i = UIManager::SYSTEM_ITEM_IMAGE_NUM; i < UIManager::SYSTEM_BLOCK_IMAGE_NUM; i++)
			{
				Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[i], m_iHandle[i][m_PushFlag[i]], true);
			}
	
		break;

	case UIManager::TYPE_UFO:
		if (m_ControllerFlag)	//コントローラー
			Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[SYSTEM_CONTROLLER_QTE], m_iHandle[SYSTEM_CONTROLLER_QTE][m_iImageNum[1]], true);
		else	//キーボード
			Draw::DrawGraphAlpha(0.0f, 0.0f, m_iFade[SYSTEM_QTE], m_iHandle[SYSTEM_QTE][m_PushFlag[SYSTEM_QTE]], true);
		break;

	default:
		break;
	}
}
void UIManager::Fin()
{

}

//======================================================

void UIManager::DrawBlockGauge()
{
	//土台を表示
	Draw::DrawRotaGraphAlpha(BLOCK_GAUGE_BASE_POS, 1.0f, 0.0f, m_iFade[SYSTEM_BLOCK_GAUGE_UNDER], m_iHandle[SYSTEM_BLOCK_GAUGE_UNDER][0], true);

	//今のブロックの数値
	float blockGauge = Data::GetDrawBlockGauge();
	//配置可能ブロックの数を計算
	int blockNum = (int)(Data::GetBlockGauge() / SET_BLOCK_POINT);

	//現在の最新ブロックゲージ
	blockGauge = blockGauge - blockNum * SET_BLOCK_POINT;
	if (blockNum == BLOCK_MAX_NUM)
		blockGauge = SET_BLOCK_POINT;

	//割合でゲージの長さを決める
	float gaugeSize = 0.0f;
	if (blockGauge != 0)
		gaugeSize = blockGauge / SET_BLOCK_POINT;
	//ゲージのサイズを計算
	if (gaugeSize > 1.0f)
		gaugeSize = 1.0f;
	if (gaugeSize < 0)
		gaugeSize = 0;

	gaugeSize = BLOCK_GAUGE_MAIN_SIZE.x * gaugeSize;

	//中身の表示
	Draw::DrawBlendModeAlpha(m_iFade[SYSTEM_BLOCK_GAUGE_MAIN]);
	DrawModiGraphF(
		BLOCK_GAUGE_MAIN_POS.x,
		BLOCK_GAUGE_MAIN_POS.y - BLOCK_GAUGE_MAIN_SIZE.y / 2,
		BLOCK_GAUGE_MAIN_POS.x + gaugeSize,
		BLOCK_GAUGE_MAIN_POS.y - BLOCK_GAUGE_MAIN_SIZE.y / 2,
		BLOCK_GAUGE_MAIN_POS.x + gaugeSize,
		BLOCK_GAUGE_MAIN_POS.y + BLOCK_GAUGE_MAIN_SIZE.y / 2,
		BLOCK_GAUGE_MAIN_POS.x,
		BLOCK_GAUGE_MAIN_POS.y + BLOCK_GAUGE_MAIN_SIZE.y / 2,
		m_iHandle[SYSTEM_BLOCK_GAUGE_MAIN][0], true
	);
	Draw::DrawBlendModeReset();

	//ゲージのカバーを表示
	Draw::DrawRotaGraphAlpha(
		BLOCK_GAUGE_BASE_POS,
		1.0f,
		0.0f,
		m_iFade[SYSTEM_BLOCK_GAUGE_COVER], 
		m_iHandle[SYSTEM_BLOCK_GAUGE_COVER][0], 
		true
	);

	//文字を表示
	Draw::DrawRotaGraphAlpha(
		BLOCK_GAUGE_TEXT_POS,
		1.0f,
		0.0f, 
		m_iFade[SYSTEM_BLOCK_GAUGE_TEXT], 
		m_iHandle[SYSTEM_BLOCK_GAUGE_TEXT][0], 
		true
	);

	SetFontSize(SET_BLOCK_NUM_FONT_SIZE);
	//配置可能数
	Draw::DrawFormatStringAlpha(BLOCK_GAUGE_SET_NUM_POS, m_iFade[SYSTEM_BLOCK_GAUGE_BLOCKNUM], COLOR[WHITE], "%d", blockNum);
	SetFontSize(20);
	//ブロックアイコン
	Draw::DrawRotaGraphAlpha(
		BLOCK_GAUGE_ICON_POS,
		1.0f, 
		0.0f, 
		m_iFade[SYSTEM_BLOCK_GAUGE_BLOCKNUM], 
		m_iHandle[SYSTEM_BLOCK_GAUGE_BLOCKNUM][0], 
		true
	);
}

//注目度ゲージ
void UIManager::DrawFoundGauge()
{
	Draw::DrawBlendModeAlpha(m_iFade[SYSTEM_BLOCK_GAUGE_UNDER]);

	float lineSize = Data::GetFoundGauge() / REQUEST_UFO_COUNT;
	lineSize = FOUND_GAUGE_MAX_SIZE * lineSize;

	if (lineSize > FOUND_GAUGE_MAX_SIZE)
	{
		lineSize = (float)FOUND_GAUGE_MAX_SIZE;
	}

	//発見度ゲージの表示
	DrawLine(SCREEN_SIZE_X / 2 - FOUND_GAUGE_MAX_SIZE - 1, FOUND_GAUGE_POS_Y,
		SCREEN_SIZE_X / 2 + FOUND_GAUGE_MAX_SIZE + 1, FOUND_GAUGE_POS_Y,
		COLOR[BLACK], FOUND_GAUGE_LINE_SIZE * 2);

	DrawLine(SCREEN_SIZE_X / 2 - (int)lineSize, FOUND_GAUGE_POS_Y, 
		SCREEN_SIZE_X / 2 + (int)lineSize, FOUND_GAUGE_POS_Y, 
		COLOR[RED], FOUND_GAUGE_LINE_SIZE);

	Draw::DrawBlendModeReset();
}

void UIManager::DrawTutorial()
{
	for (int i = 0; i < TUTORIAL_IMAGE_NUM; i++)
	{		
		VECTOR vPos = TUTORIAL_POS[i];
		vPos.y += TUTORIAL_POS_HEIGHT;
			
		Draw::DrawBlendModeAlpha(m_iTutorialFade[i]);
		DrawBillboard3D(vPos, 0.5f, 0.0f, m_fTutorialSize[i], 0.0f, m_iTutorialHandle[i], true);
		Draw::DrawBlendModeReset();
	}
}

//======================================================

void UIManager::CheckInputPlay()
{
	//移動--------------------------------
	if (Input::Key::Keep(KEY_INPUT_W))
		m_PushFlag[SYSTEM_MOVE1] = true;

	if (Input::Key::Keep(KEY_INPUT_A))
		m_PushFlag[SYSTEM_MOVE2] = true;

	if (Input::Key::Keep(KEY_INPUT_S))
		m_PushFlag[SYSTEM_MOVE3] = true;

	if (Input::Key::Keep(KEY_INPUT_D))
		m_PushFlag[SYSTEM_MOVE4] = true;
	//------------------------------------

	//ジャンプ
	if (Input::Key::Keep(KEY_INPUT_SPACE))
		m_PushFlag[SYSTEM_JUMP] = true;

	//カメラ回転--------------------------
	if (Input::Key::Keep(KEY_INPUT_LEFT))
		m_PushFlag[SYSTEM_CAMERA_MOVE1] = true;

	if (Input::Key::Keep(KEY_INPUT_RIGHT))
		m_PushFlag[SYSTEM_CAMERA_MOVE2] = true;
	//------------------------------------

	//ズーム/ズームアウト-----------------
	if (Input::Key::Keep(KEY_INPUT_UP))
		m_PushFlag[SYSTEM_CAMERA_ZOOM1_KEY]		= true;

	if (Input::Key::Keep(KEY_INPUT_DOWN))
		m_PushFlag[SYSTEM_CAMERA_ZOOM2_KEY]		= true;

	if (Input::Mouse::Wheel(MOUSE_WHEEL_BACK) || Input::Mouse::Wheel(MOUSE_WHEEL_FRONT))
		m_PushFlag[SYSTEM_CAMERA_ZOOM_MOUSE]	= true;
	//------------------------------------

	//アイテムモード----------------------
	if (Input::Key::Keep(KEY_INPUT_E))
		m_PushFlag[SYSTEM_BLOCKMODE_KEY]	= true;

	if (Input::Mouse::Keep(MOUSE_INPUT_RIGHT))
		m_PushFlag[SYSTEM_BLOCKMODE_MOUSE]	= true;
	//------------------------------------

	//ブロックモード
	if (Input::Key::Keep(KEY_INPUT_Q))
		m_PushFlag[SYSTEM_ITEMMODE] = true;

	//メニュー
	if (Input::Key::Keep(KEY_INPUT_LCONTROL) || Input::Key::Keep(KEY_INPUT_RCONTROL))
		m_PushFlag[SYSTEM_PAUSE]	= true;

	//コントローラー==============================================================

	if (Input::Controller::Keep(XINPUT_BUTTON_A))
		m_PushFlag[SYSTEM_CONTROLLER_JUMP]		= true;

	if (Input::Controller::Keep(XINPUT_BUTTON_LEFT_SHOULDER))
		m_PushFlag[SYSTEM_CONTROLLER_BLOCKMODE] = true;

	if (Input::Controller::Keep(XINPUT_BUTTON_RIGHT_SHOULDER))
		m_PushFlag[SYSTEM_CONTROLLER_ITEMMODE]	= true;

	if (Input::Controller::Keep(XINPUT_BUTTON_START))
		m_PushFlag[SYSTEM_CONTROLLER_PAUSE1]	= true;

	if (Input::Controller::Keep(XINPUT_BUTTON_BACK))
		m_PushFlag[SYSTEM_CONTROLLER_PAUSE2]	= true;

	float sX, sY;
	Input::Controller::LStickIncline(sX,sY);
	m_iImageNum[0] = 0;
	//傾いている
	if (fabs(sX) > 0 || fabs(sY) > 0)
	{
		//↑
		if (sY > 0 && fabs(sX) < CONTROLLER_STICK_MIN)
		{
			m_iImageNum[0] = 1;
		}
		else if (sY > 0 && sX > 0)
		{
			m_iImageNum[0] = 2;
		}
		//→
		else if (fabs(sY) < CONTROLLER_STICK_MIN && sX > 0)
		{
			m_iImageNum[0] = 3;
		}
		else if (sY < 0 && sX > 0)
		{
			m_iImageNum[0] = 4;
		}
		//↓
		else if (sY < 0 && fabs(sX) < CONTROLLER_STICK_MIN)
		{
			m_iImageNum[0] = 5;
		}
		else if (sY < 0 && sX < 0)
		{
			m_iImageNum[0] = 6;
		}
		//←
		else if (fabs(sY) < CONTROLLER_STICK_MIN && sX < 0)
		{
			m_iImageNum[0] = 7;
		}
		else if (sY > 0 && sX < 0)
		{
			m_iImageNum[0] = 8;
		}
	}
}

void UIManager::CheckInputItemMode()
{

	if (Input::Mouse::Keep(MOUSE_INPUT_LEFT))
		m_PushFlag[SYSTEM_ITEM_THROW_MOUSE] = true;

	//コントローラー==============================================================

	if (Input::Controller::Keep(XINPUT_BUTTON_A))
		m_PushFlag[SYSTEM_CONTROLLER_ITEM_THROW] = true;
}

void UIManager::CheckInputBlockMode()
{
	if (Input::Mouse::Keep(MOUSE_INPUT_LEFT))
		m_PushFlag[SYSTEM_BLOCK_PUT_MOUSE] = true;

	//コントローラー==============================================================

	if (Input::Controller::Keep(XINPUT_BUTTON_A))
		m_PushFlag[SYSTEM_CONTROLLER_BLOCK_PUT] = true;
}

