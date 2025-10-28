#include"SceneManager.h"
#include"../MyLib/MyLib.h"

TitleScene::TitleScene()
{
	m_SceneID = TITLE_SCENE_INIT;
}
TitleScene::~TitleScene()
{
}

int TitleScene::Loop()
{
	int iRet = 0;

	switch (m_SceneID)
	{
	case TITLE_SCENE_INIT:	//初期化
		//初期化
		Init();

		//読み込みへ
		m_SceneID = TITLE_SCENE_LOAD;

		//フェードインする
		Fade::GetInstance()->RequestFade(Fade::FADE_IN, FADE_IN_SPEED);
		break;

	case TITLE_SCENE_LOAD:	//読み込み
		//サウンドの初期化
		Sound::Init();
		Sound::Play(BGM_TITLE);
		Sound::SetVolume(BGM_TITLE, SOUND_MAX_VOLUME);

		//読み込み
		Load();

		//繰り返しへ
		m_SceneID = TITLE_SCENE_LOOP;
		break;

	case TITLE_SCENE_LOOP:	//繰り返し
		//繰り返し
		Step();
		break;

	case TITLE_SCENE_END:	//終了
		if(!Fade::GetInstance()->GetFadeNow())
		{
			//終了
			Fin();

			//初期化に戻す
			m_SceneID = TITLE_SCENE_INIT;

			iRet = -1;
		}
		break;

	default:
		break;
	}

	return iRet;
}

void TitleScene::Init()
{
	//カメラ設定
	m_vCameraPos	= CAMERA_POS;
	m_vCameraFor	= CAMERA_FOCUS;
	m_vCameraUp		= CAMERA_UP;
	SetCameraPositionAndTargetAndUpVec(m_vCameraPos, m_vCameraFor, m_vCameraUp);
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	//タイトルに表示するモデルの初期化
	for (int i = 0; i < MODEL_NUM; i++)
	{
		m_TitleModel[i].Init(MODEL_POS[i]);
	}

	//進行度設定
	m_Progress			= TITLE_MAKE;
	//はじめの選択肢
	m_SelectGameMode	= SELECT_NEUTRAL;

	//選択肢画像初期化
	for (int i = 0; i < SELECT_NUM; i++)
	{
		m_iWindowHandle[i]	= -1;
		m_vWindowPos[i]		= WINDOW_POS[i];
	}

	//選択肢の透明度
	m_iSelectWindowFade = SELECT_WINDOW_INIT_FADE;

	//選択可能フラグの初期化
	m_SelectableFlag	= false;

	//UIの押されているフラグを初期化
	for (int i = 0; i < PUTH_IMAGE_NUM; i++)
	{
		m_PushButtonFlag[i] = false;
	}

	//カメラの移動速度の初期化
	m_fCameraSpeed = CAMERA_INIT_SPEED;

	//画像の初期化
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		m_ImageDrawFlag[i] = true;
		m_iImageFade[i] = IMAGE_INIT_FADE;
	}

	//セーブデータ上書き確認画像だけ非表示にする
	m_ImageDrawFlag[CHECK_TEXT] = false;

	//選択処理の初期化
	Select::Init();

	//マウス関係処理の初期化
	Mouse::GetInstance()->Init();

	//PV再生変数の初期化
	m_fPvStartCount = 0.0f;
	m_PVPlayFlag			= false;

	//BGMの設定
	Sound::SetFadeVolume(BGM_TITLE, SOUND_MAX_VOLUME, SOUND_VOLUME_CHANGE_SPEED);
	Sound::Play(BGM_TITLE);
}

void TitleScene::Load()
{
	//モデル読み込み
	for (int i = 0; i < MODEL_NUM; i++)
	{
		m_TitleModel[i].Load(MODEL_PATH[i].c_str(), MODEL_SCALE[i]);
	}
	//スカイモデルに回転速度を設定
	m_TitleModel[MODEL_SKY].SetSpeed(SKY_ROTATION_SPEED);
	
	//テキストモデルのアニメーションを再生
	m_TitleModel[MODEL_TEXT].RequestAnime(TitleModel::ANIME_GO, TEXT_ANIME_SPEED);

	//2D画像読み込み
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		//選択肢ウィンドウ
		if (i == SELECT_WINDOW)
		{
			LoadDivGraph(IMAGE_PATH[i].c_str(),
				SELECT_WINDOW_IMAGE_MAX_NUM,
				SELECT_WINDOW_IMAGE_NUM_X, SELECT_WINDOW_IMAGE_NUM_Y,
				(int)SELECT_WINDOW_IMAGE_SIZE.x, (int)SELECT_WINDOW_IMAGE_SIZE.y,
				m_iWindowHandle);
		}
		//入力情報
		else if (i >= PUSH_BUTTON_A)
		{
			LoadDivGraph(IMAGE_PATH[i].c_str(),
				PUTH_IMAGE_MAX_NUM,
				PUTH_IMAGE_NUM_X, PUTH_IMAGE_NUM_Y,
				PUTH_IMAGE_SIZE_X, PUTH_IMAGE_SIZE_Y,
				m_iHandle[i]);
		}
		//それ以外
		else
		{
			m_iHandle[i][0] = LoadGraph(IMAGE_PATH[i].c_str());
		}
	}

	//マウス読み込み
	Mouse::GetInstance()->Load();

	//PV
	m_iPvHandle = LoadGraph(PLAY_MOVIE_PATH.c_str());
}

void TitleScene::Step()
{
	//PV--------------------------------------------------

	//停止
	if (GetMovieStateToGraph(m_iPvHandle))
	{
		if (Input::CheckInputPush())
		{
			m_PVPlayFlag = false;
			PauseMovieToGraph(m_iPvHandle);
			Fade::GetInstance()->RequestFlash();

			Sound::Play(BGM_TITLE);

			//カウントを0に戻す
			m_fPvStartCount = 0.0f;
		}
	}

	//再生時間になったら
	if(Math::MatchSpecifiedNum(m_fPvStartCount,PV_START_TIME,FRAME_TIME))
	{
		//はじめの一回だけ実行
		if(!m_PVPlayFlag)
		{
			Fade::GetInstance()->RequestFlash();
			Sound::StopAll();

			//動画再生
			PlayMovieToGraph(m_iPvHandle);
			SeekMovieToGraph(m_iPvHandle, 0);
		}

		//再生
		m_PVPlayFlag = true;
	}

	//通常のタイトル画面に戻す
	if (m_PVPlayFlag && !GetMovieStateToGraph(m_iPvHandle))
	{
		//PV再生待機時間をリセット
		m_fPvStartCount = 0.0f;

		//PVを再生
		m_PVPlayFlag = false;
		PauseMovieToGraph(m_iPvHandle);
		Fade::GetInstance()->RequestFlash();

		//BGMを再生
		Sound::Play(BGM_TITLE);
	}

	//何かしらで選択されている
	//マウスが動いている
	if (Select::GetDecisionFlag() || Select::GetDecisionToMouseFlag() ||
		Mouse::GetInstance()->GetMouseMove())
	{
		//PV再生待機時間をリセット
		m_fPvStartCount = 0.0f;
	}

	//----------------------------------------------------

	switch (m_Progress)
	{
	case TitleScene::TITLE_MAKE:	//画面の作成
		
		//画面を作る
		ScreenMake();

		break;

	case TitleScene::TITLE_STAY:	//操作待機
		
		//待機処理
		ScreenStay();

		break;

	case TitleScene::TITLE_SELECT:	//ゲームモード選択

		//選択
		SelectState();

		//注意を出す
		OutPutAttention();

		break;

	case TitleScene::GAME_START:	//ゲームスタート
		
		//ゲームを開始する
		GameStart();

		break;

	default:
		break;
	}

	//3Dモデルの通常処理
	for (int i = 0; i < MODEL_NUM; i++)
	{
		m_TitleModel[i].Step();
	}

	//透明度の変化
	ImageFade();
	
	//キー入力の確認
	InputCheck();
}

void TitleScene::Draw()
{

	//タイトルモデル描画
	for (int i = 0; i < MODEL_NUM; i++)
	{
		m_TitleModel[i].Draw();
	}

	//選択肢の描画
	DrawSelect();
	
	//確認用テキスト
	DrawCheckText();

	//入力状態の描画
	DrawPushButton();
	
	//長押し選択の描画
	Select::Draw(SELECT_KEEP_POS[m_SelectGameMode]);

	//マウスカーソル
	Mouse::GetInstance()->Draw();

	//PV
	if (GetMovieStateToGraph(m_iPvHandle))
	{
		DrawExtendGraph(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, m_iPvHandle, true);
	}
}

void TitleScene::Fin()
{
	//モデルの終了処理
	for (int i = 0; i < MODEL_NUM; i++)
	{
		m_TitleModel[i].Fin();
	}

	//ボリュームを落とす
	Sound::SetFadeVolume(BGM_TITLE, SOUND_MIN_VOLUME);
}

//==================================================================

//タイトル画面の作成
void TitleScene::ScreenMake()
{
	//開始時のテキストアニメーションが終わっていたら
	if (m_TitleModel[MODEL_TEXT].GetChecknd(TitleModel::ANIME_GO))
	{
		//テキストを停止させる
		m_TitleModel[MODEL_TEXT].ChangeAnimation(TitleModel::ANIME_DEF);

		//待機状態へ
		m_Progress = TITLE_STAY;
	}
}

//操作待機
void TitleScene::ScreenStay()
{
	if (!Fade::GetInstance()->GetFadeNow())
	{
		//場が完成している かつ 何かしらが押されたなら
		if(Input::CheckInputPush())
		{
			//SE再生
			Sound::Play(SE_ENTER);

			//選択へ
			m_Progress = TITLE_SELECT;
		}
	}
}

//------------------------------------

//選択
void TitleScene::SelectState()
{
	//選択肢が完全に表示されていたら
	if (Math::MatchSpecifiedNum(m_iSelectWindowFade, SELECT_WINDOW_MAX_FADE, SELECT_WINDOW_FADE_SPEED))
	{
		//はじめの一回だけ入る
		if (!m_SelectableFlag)
		{
			//セーブデータがあるかの確認
			SaveLoad data;
			data.Load();

			//最初の選択肢を入れる
			//セーブデータがない(クリアステージが-1)
			if (data.data.m_ClearStage == -1)
			{
				//はじめから
				m_SelectGameMode = SELECT_BEGINNING;
				//既存データがない
				m_ContinuationFlag = false;
			}
			//セーブデータがある
			else
			{
				//つづきから
				m_SelectGameMode = SELECT_CONTINUATION;
				//m_SelectGameMode = SELECT_BEGINNING;
				//既存データがある
				m_ContinuationFlag = true;
			}

			//選択可能にする
			m_SelectableFlag = true;
		}
	}

	//選択肢の上下_上へ
	if (InputManager::CheckSelectUp())
	{
		//はじめから（↑）じゃなかったら
		if(m_SelectGameMode!=SELECT_BEGINNING)
		{
			//SE再生
			Sound::Play(SE_CURSOR);
			//はじめからにする
			m_SelectGameMode = SELECT_BEGINNING;
		}

		//長押し選択をリセット
		Select::SelectReset();

		//PV再生待機時間をリセット
		m_fPvStartCount = 0.0f;
	}

	//既存データがあったらつづきからを選択できるようにする
	if(m_ContinuationFlag)
	{
		//選択肢の上下_下へ
		if (InputManager::CheckSelectDown())
		{
			//つづきから（↓）じゃなかったら
			if (m_SelectGameMode != SELECT_CONTINUATION)
			{
				//SE再生
				Sound::Play(SE_CURSOR);
				//つづきからにする
				m_SelectGameMode = SELECT_CONTINUATION;
			}

			//長押し選択をリセット
			Select::SelectReset();

			//PV再生待機時間をリセット
			m_fPvStartCount = 0.0f;
		}
	}

	//マウス判定をリセットするフラグ
	bool resetFlag = true;

	for (int i = 0; i < SELECT_NUM; i++)
	{
		//座標セット
		VECTOR selectPos = m_vWindowPos[i];

		//選択できるか判定
		if (Select::SelectMouse(selectPos, SELECT_WINDOW_IMAGE_SIZE))
		{
			//現在選択していない
			if (m_SelectGameMode != i)
			{
				//選択する
				m_SelectGameMode = (GAME_MODE)i;
				//SE再生
				Sound::Play(SE_CURSOR);
				//長押し選択をリセット
				Select::SelectReset();
			}

			//選択をリセット
			resetFlag = false;

			//当たっていたら抜ける
			break;
		}
	}

	//リセットフラグがtrueかつマウスで選択していたら
	if (resetFlag&&Select::GetDecisionToMouseFlag())
	{
		//選択をリセット
		Select::SelectReset();
	}

	//決定
	if(Select::SelectStep())
	{
		//SE再生
		Sound::Play(SE_ENTER);

		//テキストのアニメーションを再生
		m_TitleModel[MODEL_TEXT].RequestAnime(m_TitleModel[MODEL_TEXT].ANIME_GO);

		//続きからだったら
		if (m_SelectGameMode == SELECT_CONTINUATION)
		{
			//データを読み込む
			Data::LoadPlayData();
		}
		//はじめからなら
		else
		{
			//データをリセット
			Data::ResetPlayData();
		}

		//表示している画像を消す
		for (int i = 0; i < IMAGE_NUM; i++)
		{
			m_ImageDrawFlag[i] = false;
		}

		//ゲームスタートへ
		m_Progress = GAME_START;
	}
}

//注意を出す
void TitleScene::OutPutAttention()
{
	//注意喚起を非表示にする
	m_ImageDrawFlag[CHECK_TEXT] = false;
	//データがある状態ではじめからを選択
	if (m_ContinuationFlag && m_SelectGameMode == SELECT_BEGINNING && 
		m_SceneID == TITLE_SELECT)
	{
		//注意喚起を出す
		m_ImageDrawFlag[CHECK_TEXT] = true;
	}
}

//------------------------------------

//ゲームをスタート
void TitleScene::GameStart()
{
	//注意喚起を消す
	m_ImageDrawFlag[CHECK_TEXT] = false;
	//選択肢の透明を0へ
	Math::MatchSpecifiedNum(m_iSelectWindowFade, SELECT_WINDOW_NIM_FADE, SELECT_WINDOW_FADE_ADD);

	//カメラを加速
	m_fCameraSpeed += CAMERA_MOVE_SPEED_ADD;

	//カメラの回転
	float cameraRot;

	//カメラの回転を求める
	cameraRot = Math::GetAngle(m_vCameraPos, m_vCameraFor);
	//移動方向を求める
	VECTOR speed = Math::GetMoveVec(cameraRot, -m_fCameraSpeed);
	//移動量を加算
	m_vCameraPos = VAdd(m_vCameraPos, speed);

	//移動速度が指定以上 かつ 次のシーンが指定されている
	if (m_fCameraSpeed >= CAMERA_MAX_SPEED &&
		Data::GetNextScene() != SCENE_SELECT)
	{
		Data::SetNextScene(SCENE_SELECT);
		Fade::GetInstance()->RequestFade(Fade::FADE_OUT, FADE_OUT_SPEED);
	}

	if (!Fade::GetInstance()->GetFadeNow() && Data::GetNextScene() == SCENE_SELECT)
	{
		m_SceneID = TITLE_SCENE_END;
	}

	//カメラの更新
	SetCameraPositionAndTargetAndUpVec(m_vCameraPos, m_vCameraFor, m_vCameraUp);
}

//------------------------------------

//透明度の変更
void TitleScene::ImageFade()
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		if (m_ImageDrawFlag[i])
		{
			//フェードイン
			Math::MatchSpecifiedNum(m_iImageFade[i], IMAGE_MAX_FADE, IMAGE_FADE_SPEED);
		}
		else
		{
			//フェードアウト
			Math::MatchSpecifiedNum(m_iImageFade[i], IMAGE_MIN_FADE, IMAGE_FADE_SPEED);
		}
	}
}

//入力の確認
void TitleScene::InputCheck()
{
	for (int i = 0; i < PUTH_IMAGE_NUM; i++)
	{
		m_PushButtonFlag[i]						= false;
	}

	if (Input::Controller::Keep(XINPUT_BUTTON_A))
	{
		m_PushButtonFlag[PUTH_IMAGE_CONTROLLER] = true;
	}
	if (Input::Key::Keep(KEY_INPUT_RETURN))
	{
		m_PushButtonFlag[PUTH_IMAGE_KEY]		= true;
	}
	if (Input::Mouse::Keep(MOUSE_INPUT_LEFT))
	{
		m_PushButtonFlag[PUTH_IMAGE_MOUSE]		= true;
	}
}

//====================================

//選択肢の描画
void TitleScene::DrawSelect()
{
	SetFontSize(SELECT_FONT_SIZE);
	for (int i = 0; i < SELECT_NUM; i++)
	{
		//既存データがないかつ、つづきからの番号だと描画なし
		if (!m_ContinuationFlag && i == SELECT_CONTINUATION)
			continue;

		//未選択時の値をセット
		//描画座標
		VECTOR windowPos = m_vWindowPos[i];
		//テキストの色
		unsigned int textColor = COLOR[GRAY];
		//描画画像ID
		int drawImageNum = UNSELECTED;

		//選択中だったら
		if (m_SelectGameMode == i)
		{
			//座標をずらす
			windowPos.x += SELECT_DRAW_POS_ADD;
			//色を変更
			textColor = COLOR[WHITE];
			//描画画像IDを変更
			drawImageNum = SELECTING;
		}

		//画像の描画
		Draw::DrawRotaGraphAlpha(windowPos, 1.0f, 0.0f, m_iSelectWindowFade, m_iWindowHandle[drawImageNum], true);
		//テキストの描画
		Draw::DrawFormatStringCenterAlpha(windowPos, m_iSelectWindowFade, textColor, TITLE_SELECT_TEXT[i].c_str());
	}
	SetFontSize(BASIC_FONT_SIZE);
}

//注意事項の描画
void TitleScene::DrawCheckText()
{
	//既存データがない か 選択肢がはじめから以外だったら
	if (!m_ContinuationFlag ||
		m_SelectGameMode != SELECT_BEGINNING)
		return;

	//はじめからの位置
	VECTOR pos = m_vWindowPos[SELECT_BEGINNING];
	pos = VSub(pos, CHECK_TEXT_DRAW_POS);
	Draw::DrawGraphAlpha(pos, m_iImageFade[CHECK_TEXT], m_iHandle[CHECK_TEXT][0], true);
}

//入力状態の描画
void TitleScene::DrawPushButton()
{
	//進行が待機状態以降(選択肢表示状態)
	if (m_Progress > TITLE_STAY)
	{
		//コントローラーが接続されている
		if (Input::Controller::CheckControllerConnected())
		{
			//入力状態(コントローラー)を描画
			Draw::DrawGraphAlpha(0.0f, 0.0f, m_iImageFade[PUSH_BUTTON_A], m_iHandle[PUSH_BUTTON_A][m_PushButtonFlag[PUTH_IMAGE_CONTROLLER]], true);
		}
		//コントローラーが接続されていない
		else
		{
			//入力状態(キーボード)を描画
			Draw::DrawGraphAlpha(0.0f, 0.0f, m_iImageFade[PUSH_ENTER], m_iHandle[PUSH_ENTER][m_PushButtonFlag[PUTH_IMAGE_KEY]], true);
			//入力状態(マウス)を描画
			Draw::DrawGraphAlpha(0.0f, 0.0f, m_iImageFade[PUSH_MOUSE], m_iHandle[PUSH_MOUSE][m_PushButtonFlag[PUTH_IMAGE_MOUSE]], true);
		}
	}
}