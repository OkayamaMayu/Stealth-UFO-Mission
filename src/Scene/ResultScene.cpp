#include"SceneManager.h"
#include"../MyLib/MyLib.h"

ResultScene::ResultScene()
{
	m_SceneID = RESULT_SCENE_INIT;
}
ResultScene::~ResultScene()
{
}

int ResultScene::Loop()
{
	int iRet = 0;

	switch (m_SceneID)
	{
	case RESULT_SCENE_INIT:
		Init();
		m_SceneID = RESULT_SCENE_LOAD;
		break;

	case RESULT_SCENE_LOAD:
		//サウンドの再生
		Sound::Play(BGM_RESULT);
		Sound::SetVolume(BGM_RESULT, 0);
		Sound::SetFadeVolume(BGM_RESULT, 100, SOUND_FADEIN_SPEED);
		Load();
		m_SceneID = RESULT_SCENE_LOOP;
		break;

	case RESULT_SCENE_LOOP:
		if(!Fade::GetInstance()->GetFadeNow())
		Step();

		//背景スクロール
		if (Math::MatchSpecifiedNum(m_vPos[BACK_GROUND].x, -SCREEN_SIZE_X, BACKGROUND_SCROLL_SPEEED))
		{
			m_vPos[BACK_GROUND].x = 0.0f;
		}
		break;

	case RESULT_SCENE_END:
		if(!Fade::GetInstance()->GetFadeNow())
		{
			Fin();
			m_SceneID = RESULT_SCENE_INIT;
			iRet = -1;
		}
		break;

	default:
		break;
	}

	return iRet;
}

void ResultScene::Init()
{
	//変数の初期化
	m_ResultNextScene		= false;
	m_ResultMakeProgress	= BASE;
	m_Progress				= RESULT_MAKE;
	m_FryFlag				= true;
	m_fFryPoiny				= 0.0f;

	//画像情報の設定
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		m_vPos[i]	= IMAGE_POS[i];
		m_iAlpha[i] = 0;
		m_fSize[i]	= INIT_IMAGE_SIZE + INIT_IMAGE_SCALE * i;
	}
	m_iAlpha[BACK_GROUND]	= 100;
	m_fSize[BACK_GROUND]	= 1.0f;

	//評価サイズ
	for (int i = EVALUATION_S; i <= EVALUATION_C; i++)
	{
		m_iAlpha[i]			= 0;
		m_fSize[i]			= INIT_IMAGE_SIZE;
	}

	//テキスト情報の設定
	for (int i = 0; i < RESULT_TEXT_NUM; i++)
	{
		m_vTextPos[i]	= TEXT_POS[i];
		m_vTextPos[i].x -= INIT_TEXT_POS_BASE + INIT_TEXT_POS_ADD * i;
		m_iTextAlpha[i] = INIT_TEXT_ALPHA * i;
	}

	//表示時の回転数の設定
	m_fEvaluationRot = Math::ChangeDegToRad(360.0f * EVALUSTION_ROT_NUM);

	//評価作成
	m_iEnemyFoundNum[0] = Data::GetEvaluationData().m_iFoundNum;
	m_iEnemyFoundNum[1] = Data::GetEvaluationData().m_iUfoSpawningNum;

	//評価する
	if (m_iEnemyFoundNum[ENEMY_FOUND] == EVALUSTION_CRITERIA[0][ENEMY_FOUND] && m_iEnemyFoundNum[UFO_APPEARANCE] == 0)
	{
		//発見回数が0 + UFOの出現回数が0_S評価
		m_Evaluation = STAGE_EVALUATION_S;
	}
	else if (m_iEnemyFoundNum[ENEMY_FOUND] <= EVALUSTION_CRITERIA[1][ENEMY_FOUND] && m_iEnemyFoundNum[UFO_APPEARANCE] <= EVALUSTION_CRITERIA[1][UFO_APPEARANCE])
	{
		//発見回数とUFOの出現回数が指定の数以下_A評価
		m_Evaluation = STAGE_EVALUATION_A;
	}	
	else if (m_iEnemyFoundNum[ENEMY_FOUND] <= EVALUSTION_CRITERIA[2][ENEMY_FOUND])
	{
		//発見回数が指定の数以下_B評価
		m_Evaluation = STAGE_EVALUATION_B;
	}
	else
	{
		//上記に当てはまらない_C評価
		m_Evaluation = STAGE_EVALUATION_C;
	}

	//セーブしない状態
	m_SaveFlag = false;

	//表示する評価の画像番号を指定
	m_iResultImageNum = EVALUATION_S + m_Evaluation - 1;

	//フェードイン
	Fade::GetInstance()->RequestFade(Fade::FADE_IN, FADEIN_SPEED);
}

void ResultScene::Load()
{
	//初期化
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		m_iHandle[i] = LoadGraph(IMAGE_PATH[i].c_str());
	}

	//選択肢の読み込み
	LoadDivGraph(
		SELECT_WINDOW_PATH.c_str(),
		SELECT_WINDOW_IMAGE_NUM[0], SELECT_WINDOW_IMAGE_NUM[1], SELECT_WINDOW_IMAGE_NUM[2], 
		(int)SELECT_WINDOW_SIZE.x, (int)SELECT_WINDOW_SIZE.y,
		m_iSelectImageHandle);
}

void ResultScene::Step()
{
	//進行度
	switch (m_Progress)
	{
	case RESULT_MAKE:			//場の作成
		ResultMake();
		break;

	case RESULT_STAY:			//場の待機
		ResultStay();
		break;

	case RESULT_SAVE:			//セーブ画面
		ResultSave();
		break;

	case RESULT_SAVE_FINISH:	//セーブを完了させる
		ResultSaveFinish();
		break;

	case NEXT_SCENE:
		//ゲームを進行
		Data::SetNextScene(SCENE_SELECT);
		m_SceneID = RESULT_SCENE_END;

		break;

	default:
		break;
	}

	//浮遊させる
	if (m_FryFlag)
	{
		float speed = (m_fFryPoiny + FRY_MOVE_MIN_SPEED) / SPEED_RATIO;
		if (Math::MatchSpecifiedNum(m_fFryPoiny, -FRY_MOVE_LIMIT, speed))
		{
			m_FryFlag = false;
		}
	}
	else
	{
		float speed = (FRY_MOVE_LIMIT+ FRY_MOVE_MIN_SPEED - m_fFryPoiny) / SPEED_RATIO;
		if (Math::MatchSpecifiedNum(m_fFryPoiny, FRY_MOVE_LIMIT, speed))
		{
			m_FryFlag = true;
		}
	}

	//次に回ってきたら進行できる
	if (Data::GetNextScene() == SCENE_SELECT)
	{
		//終了処理へ
		m_SceneID = RESULT_SCENE_END;
	}
}


void ResultScene::Draw()
{
	switch (m_Progress)
	{
	case ResultScene::RESULT_MAKE:
	case ResultScene::RESULT_STAY:
	{
		VECTOR vPos = {};

		//背景描画(二枚分)
		for (int i = 0; i < 2; i++)
		{
			vPos = m_vPos[BACK_GROUND];
			vPos.x += SCREEN_SIZE_X * i;
			Draw::DrawGraphAlpha(vPos, m_iAlpha[BACK_GROUND], m_iHandle[BACK_GROUND], true);
		}
		
		//黒帯
		DrawGraph(0, 0, m_iHandle[BLACK_LINE], true);

		//クリア画面の描写
		for (int i = BACK_WINDOW; i <= CIRCLE; i++)
		{
			vPos = m_vPos[i];
			vPos.y += m_fFryPoiny + m_fFryPoiny / 2 * i;
			Draw::DrawRotaGraphAlpha(vPos, m_fSize[i], 0.0f, m_iAlpha[i], m_iHandle[i], true);
		}

		//テキスト
		for (int i = 0; i < RESULT_TEXT_NUM; i++)
		{
			vPos = m_vTextPos[i];
			vPos.y += m_fFryPoiny + m_fFryPoiny / 2 * (CIRCLE + 1 + i);

			//透明度の変更
			Draw::DrawBlendModeAlpha(m_iTextAlpha[i]);

			//フォントサイズを変更
			SetFontSize(RESULT_FONT_SIZE);
			//リザルト結果の描画
			DrawFormatStringF(vPos.x, vPos.y, COLOR[WHITE], RESULT_TEXT[i].c_str());

			//回数の表示
			SetFontSize(SCORE_FONT_SIZE);
			vPos = VAdd(vPos, RESULT_TEXT_POS);
			DrawFormatStringF(vPos.x, vPos.y, COLOR[WHITE], "%d回", m_iEnemyFoundNum[i]);

			//透明度のリセット
			Draw::DrawBlendModeReset();
		}
		SetFontSize(20);

		//評価の表示
		vPos = m_vPos[m_iResultImageNum];
		vPos.y += m_fFryPoiny + m_fFryPoiny / 2 * (EVALUATION_S + 2);
		Draw::DrawRotaGraphAlpha(
			vPos,
			m_fSize[m_iResultImageNum],
			m_fEvaluationRot,
			m_iAlpha[m_iResultImageNum],
			m_iHandle[m_iResultImageNum],
			true);
	}
		break;

	case ResultScene::RESULT_SAVE:
	{
		//質問の表示
		VECTOR vPos = { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2 - SAVE_TEXT_HEIGHT,0.0f };
		SetFontSize(SAVE_TEXT_FONT_SIZE);
		Draw::DrawFormatStringCenter(vPos, COLOR[WHITE], "セーブしますか？");

		//選択肢の表示
		SetFontSize(SAVE_SETECT_TEXT_FONT_SIZE);
		for (int i = 0; i < OPTION_NUM; i++)
		{
			VECTOR pos = RESULT_SELECT_IMAGE_POS[i];
			//選択されていたら座標をずらす
			if (m_Option == i)
			{
				pos.y += SELECT_SAVE_TEXT_HEIGHT;
			}

			//選択肢の表示
			if (m_Option == i)
			{
				Draw::DrawRotaGraphV(pos, 1.0f, 0.0f, m_iSelectImageHandle[1], true);
				Draw::DrawFormatStringCenter(pos, COLOR[WHITE], OPTION_TEXT[i].c_str());
			}
			else
			{
				Draw::DrawRotaGraphV(pos, 1.0f, 0.0f, m_iSelectImageHandle[0], true);
				Draw::DrawFormatStringCenter(pos, COLOR[GRAY], OPTION_TEXT[i].c_str());
			}	

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		SetFontSize(20);

		//選択中の長押しUIの位置調節
		vPos	= RESULT_SELECT_IMAGE_POS[m_Option];
		vPos.y += SELECT_SAVE_TEXT_HEIGHT;

		Mouse::GetInstance()->Draw();
		Select::Draw(vPos);
	}
		break;

	case ResultScene::RESULT_SAVE_FINISH:
	case ResultScene::NEXT_SCENE:
	
		//セーブされていたら
		if (m_SaveFlag)
		{
			//セーブ完了テキストを表示
			VECTOR vPos = { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f };
			Draw::DrawFormatStringCenter(vPos, COLOR[WHITE], "セーブが完了しました ▼");
		}
	
		break;

	default:
		break;
	}
	
}

void ResultScene::Fin()
{
	//BGM停止
	Sound::SetFadeVolume(BGM_RESULT, 0);
}

//----------------------------------------------------

void ResultScene::ResultMake()
{
	switch (m_ResultMakeProgress)
	{
	case ResultScene::BASE:			//土台の作成
	{
		//次の処理に進む
		bool nextStepFlag = true;
		for (int i = 0; i <= CIRCLE; i++)
		{
			//移動が完了していたら実行しない
			if (i != 0 &&
				m_fSize[i] == 1.0f && m_iAlpha[i] == 100 &&
				m_fSize[i - 1] != 1.0f && m_iAlpha[i - 1] != 100)
				continue;

			//透明度の変更速度の計算
			float speed = (100.0f - m_iAlpha[i]) / SPEED_RATIO + MAKE_ALPHA_MIN_SPEED;
			Math::MatchSpecifiedNum(m_iAlpha[i], 100, (int)speed);

			//サイズが1より大きいなら
			if (m_fSize[i] > 1.0f)
			{
				//サイズを小さくする
				speed = (m_fSize[i] + MAKE_SIZE_MIN_SPEED) / SPEED_RATIO;
				m_fSize[i] -= speed;
			}
			else
			{
				//固定する
				m_fSize[i] = 1.0f;

				//サウンドを再生する
				Sound::Play(SE_RESULT_1);
			}

			//土台を表示したら次へ
			if (m_iAlpha[i] != 100 || m_fSize[i] != 1.0f)
			{
				nextStepFlag = false;
			}
		}

		//次の工程へ
		if (nextStepFlag)
			m_ResultMakeProgress = TEXT;

		break;
	}
	case ResultScene::TEXT:			//テキストの表示
		for (int i = 0; i < RESULT_TEXT_NUM; i++)
		{
			//座標を移動
			float speed = (TEXT_POS[i].x + MAKE_TEXT_MIN_SPEED - m_vTextPos[i].x) / SPEED_RATIO;
			Math::MatchSpecifiedNum(m_vTextPos[i].x, TEXT_POS[i].x, speed);

			//透明度を変更
			speed = (100.0f - m_iTextAlpha[i]) / SPEED_RATIO + MAKE_TEXT_ALPHA_MIN_SPEED;
			Math::MatchSpecifiedNum(m_iTextAlpha[i], 100, (int)speed);
		}

		//文字を表示したら次の工程へ
		if (m_vTextPos[1].x == TEXT_POS[1].x && m_iTextAlpha[1] == 100)
		{
			m_ResultMakeProgress = EVALUATION;
		}

		break;

	case ResultScene::EVALUATION:	//評価の表示
	{
		//サイズを変更
		float speed = (m_fSize[m_iResultImageNum] + MAKE_EVALUATION_SIZE_SPEED) / SPEED_RATIO;
		Math::MatchSpecifiedNum(m_fSize[m_iResultImageNum], 1.0f, speed);

		//透明度を変更
		speed = (100.0f - m_iAlpha[m_iResultImageNum]) / SPEED_RATIO + MAKE_EVALUATION_ALPHA_SPEED;
		Math::MatchSpecifiedNum(m_iAlpha[m_iResultImageNum], 100, (int)speed);

		//回転しながら出現させる
		speed = (m_fEvaluationRot + MAKE_EVALUATION_ROT_SPEED) / SPEED_RATIO;
		Math::MatchSpecifiedNum(m_fEvaluationRot, 0.0f, speed);

		//移動が完了していたら
		if (m_fSize[m_iResultImageNum] == 1.0f &&
			m_iAlpha[m_iResultImageNum] == 100 &&
			m_fEvaluationRot == 0.0f)
		{
			//次の工程へ
			m_Progress = RESULT_STAY;

			//サウンドを再生する
			Sound::Play(SE_RESULT_2);
		}
	}
	break;
	default:
		break;
	}
}

void ResultScene::ResultStay()
{
	//進行フラグがたっていたら
	if (m_ResultNextScene)
	{
		//フェードイン
		Fade::GetInstance()->RequestFade(Fade::FADE_IN, SAVE_FADEIN_SPEED);
		//セーブ画面へ
		m_Progress = RESULT_SAVE;
	}

	//Enterキーが押されたなら
	if (InputManager::CheckPush(INPUT_DETERMINATION))
	{
		//SE
		Sound::Play(SE_ENTER);
		//BGM大きさ変更
		Sound::SetFadeVolume(BGM_RESULT, SAVE_SOUND_VOLUME, SAVE_SOUND_FADE_SPEED);

		//フェードアウト
		Fade::GetInstance()->RequestFade(Fade::FADE_OUT, SAVE_FADEOUT_SPEED);

		//次の工程へ
		m_ResultNextScene = true;

		//サウンドを再生
		Sound::Play(SE_ENTER);
	}
}

void ResultScene::ResultSave()
{
	//フラッシュが255になるのは「はい」選択時のみ
	if (m_SaveFlag)
	{
		//セーブ後のフェード明け
		Fade::GetInstance()->RequestFade(Fade::FADE_IN, SAVE_NOW_FADEIN_SPEED);
		//次の工程へ
		m_Progress = RESULT_SAVE_FINISH;
	}

	//スティックの入力を受け取る
	if (InputManager::CheckSelectLeft())
	{
		if (m_Option != m_Option - 1)
		{
			//選択をリセット
			Select::SelectReset();
			//サウンドを再生する
			Sound::Play(SE_CURSOR);
		}

		//左に行き過ぎないようにする
		m_Option = static_cast<OPTION_ID>(m_Option - 1);
		if (m_Option <= OPTION_NEUTRAL)
		{
			m_Option = YES;
		}
	}
	else if (InputManager::CheckSelectRight())
	{
		if (m_Option != m_Option + 1)
		{
			//選択をリセット
			Select::SelectReset();
			//サウンドを再生する
			Sound::Play(SE_CURSOR);
		}

		//右に行き過ぎないようにする
		m_Option = static_cast<OPTION_ID>(m_Option + 1);
		if (m_Option >= OPTION_NUM)
		{
			m_Option = NO;
		}
	}

	//マウス判定
	bool resetFlag = true;
	for (int i = 0; i < OPTION_NUM; i++)
	{
		//座標を調整
		VECTOR vPos		= RESULT_SELECT_IMAGE_POS[i];
		vPos.y			+= SAVE_SELECT_WINDOW_POS;

		//マウス選択
		if (Select::SelectMouse(vPos, SELECT_WINDOW_SIZE))
		{
			//未選択の物ならサウンドを再生する
			if (m_Option != i)
				Sound::Play(SE_CURSOR);

			//選択させる
			m_Option = (OPTION_ID)i;
			resetFlag = false;

			break;
		}
	}
	//選択をリセット
	if (resetFlag && Select::GetDecisionToMouseFlag())
	{
		Select::SelectReset();
	}

	//決定
	if (Select::SelectStep())
	{
		//サウンドを再生する
		Sound::Play(SE_ENTER);

		//評価を更新
		if ((int)Data::GetStageEvaluation(Data::GetSelectStage()) > (int)m_Evaluation ||
			Data::GetStageEvaluation(Data::GetSelectStage()) == STAGE_EVALUATION_NODATA)
			Data::StageEvaluation(Data::GetSelectStage(), m_Evaluation);

		//最後に遊んだステージを格納
		Data::SetLastPlayStage();
		//Clearしたステージ番号を格納
		Data::SetClearStageNum(Data::GetSelectStage());

		switch (m_Option)
		{
			//セーブ
		case YES:
			m_SaveFlag = true;

			//データを保存
			Data::SavePlayData();

			//画面を一瞬消す
			Fade::GetInstance()->RequestFade(Fade::FADE_OUT, SAVE_EXECUTION_FADEOUT_SPEED);

			break;

			//セーブしない
		case NO:
			Data::SetNextScene(SCENE_SELECT);
			Fade::GetInstance()->RequestFade(Fade::FADE_OUT, SAVE_SKIP_FADEOUT_SPEED);

			m_Progress = NEXT_SCENE;
			break;

		default:
			break;
		}
	}
}

void ResultScene::ResultSaveFinish()
{
	//演出実行中なら実行しない
	if (Fade::GetInstance()->GetFlashNow())
		return;

	//Enterキーが押されたなら
	if (InputManager::CheckPush(INPUT_DETERMINATION))
	{
		//進行する
		m_SaveFlag = true;
		m_Progress = NEXT_SCENE;

		//次のシーンへのフェードアウト
		Fade::GetInstance()->RequestFade(Fade::FADE_OUT, SCENE_CHANGE_FADEOUT_SPEED);

		//サウンドを再生する
		Sound::Play(SE_ENTER);
	}
}