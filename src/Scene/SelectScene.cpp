#include"SceneManager.h"

int OpenStageNum;
SelectScene::SelectScene()
{
	m_SceneID = SELECT_SCEME_INIT;
}
SelectScene::~SelectScene()
{

}

int SelectScene::Loop()
{
	int iRet = 0;

	switch (m_SceneID)
	{
	case SELECT_SCEME_INIT:	//初期化
		//初期化
		Init();

		//読み込みへ
		m_SceneID = SELECT_SCEME_LOAD;
		break;

	case SELECT_SCEME_LOAD:	//読み込み
		//読み込み
		Load();

		//繰り返しへ
		m_SceneID = SELECT_SCEME_LOOP;

		//サウンドを再生
		Sound::Play(BGM_SELECT);
		Sound::SetVolume(BGM_SELECT, SOUND_MIN_VOLUME);
		Sound::SetFadeVolume(BGM_SELECT, SOUND_MAX_VOLUME);
		break;

	case SELECT_SCEME_LOOP:	//繰り返し
		//繰り返し
		Step();
		break;

	case SELECT_SCEME_END:	//終了
		if (!Fade::GetInstance()->GetFadeNow())
		{
			//終了
			Fin();

			//初期化に戻す
			m_SceneID = SELECT_SCEME_INIT;

			iRet = -1;
		}
		break;

	default:
		break;
	}

	return iRet;
}

//======================================================

//初期化
void SelectScene::Init()
{
	//フェードインする
	Fade::GetInstance()->RequestFade(Fade::FADE_IN, FADE_IN_SPEED);

	//選択中のステージに最後に入ったステージを入れる
	m_SelectStage = Data::GetLastPlayStage();

	//ステージ選択した時の動きフラグを初期化
	m_StageMove = false;

	//キーの入力間隔を初期化
	m_fKeyInterval = SELECT_KEY_INTERVAL;

	//選択中ステージを基準に最大値と最小値に割り振り
	for (int i = 0; i < STAGE_NUM; i++)
	{
		//左端
		if (m_SelectStage > i)
		{
			m_vPos[i] = SELECT_STAGE_IMAGE_POS[DRAW_ICON_LEFT_END];
			m_iFontSize[i] = SELECT_FONT_SIZE[DRAW_ICON_LEFT_END];
			m_fImageSize[i] = SELECT_ICON_SIZE_MAGNIFICATION[DRAW_ICON_LEFT_END];
		}
		//右端
		if (m_SelectStage < i)
		{
			m_vPos[i] = SELECT_STAGE_IMAGE_POS[DRAW_ICON_RIGHT_END];
			m_iFontSize[i] = SELECT_FONT_SIZE[DRAW_ICON_RIGHT_END];
			m_fImageSize[i] = SELECT_ICON_SIZE_MAGNIFICATION[DRAW_ICON_RIGHT_END];
		}
	}

	//表示ステージの情報を格納
	int stageNum = m_SelectStage;
	//アイコンの周囲計算用
	int stageDrawIconCount = SELECT_ICON_DRAW_MIN_COUNT;
	for (int i = 0;i < DRAW_ICON_NUM;i++)
	{
		//ステージの数内だったら実行
		if (stageNum + stageDrawIconCount >= 0 && stageNum + stageDrawIconCount < STAGE_NUM)
		{
			//座標
			m_vPos[stageNum + stageDrawIconCount]		= SELECT_STAGE_IMAGE_POS[i];
			//フォントサイズ
			m_iFontSize[stageNum + stageDrawIconCount]	= SELECT_FONT_SIZE[i];
			//アイコンサイズ
			m_fImageSize[stageNum + stageDrawIconCount] = SELECT_ICON_SIZE_MAGNIFICATION[i];
		}

		stageDrawIconCount++;
	}

	//開放中のステージ数を記録
	OpenStageNum = Data::GetClearStageNum() + STAGE_MAX_NUM_COUNT;
	//全ステージ出ていたらステージ数を固定
	if (OpenStageNum > STAGE_NUM)
	{
		OpenStageNum = STAGE_NUM;
	}

	//矢印フラグの初期化
	m_SelectArrowMoveFlag = SELECT_ARROW_MOVE_FLAG_INIT;
	//矢印の位置を初期化
	m_fArrowMove = SELECT_ARROW_MOVE_MIN;
	//矢印の透明度を初期化
	for(int i=0;i< ARROW_SIDE_NUM;i++)
	{
		m_iArrowFade[i] = SELECT_ARROW_FADE_INIT;
	}

	//選択中フラグを初期化
	m_DecisionFlag = false;

	//選択中のアイコンのサイズ変化フラグ
	m_SelectSizeFlag = SELECT_ICON_SIZE_MOVE_FLAG_INIT;

	//選択処理の初期化
	Select::Init();

	//入力情報の初期化
	for (int i = 0; i < PUTH_IMAGE_NUM; i++)
	{
		m_PushButtonFlag[i] = false;
	}
}

//読み込み
void SelectScene::Load()
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		//入力情報画像読み込み
		if (i >= PUSH_BUTTON_A)
		{
			LoadDivGraph(SELECTSTAGE_BACK_IMAGE_PATH[i].c_str(),
				PUTH_IMAGE_MAX_NUM,
				PUTH_IMAGE_NUM_X, PUTH_IMAGE_NUM_Y,
				PUTH_IMAGE_SIZE_X, PUTH_IMAGE_SIZE_Y,
				m_iImageHandle[i]);
		}
		//一枚画像読み込み
		else
		{
			m_iImageHandle[i][0] = LoadGraph(SELECTSTAGE_BACK_IMAGE_PATH[i].c_str());
		}
	}
}

//通常処理
void SelectScene::Step()
{
	//フェードが明けたら
	if (!Fade::GetInstance()->GetFadeNow())
	{
		//ステージ選択
		Select();

		//画像の移動処理
		Move();

		//入力情報の確認
		InputCheck();
	}

	//背景の通常処理
	BackGroundStep();

	//矢印の通常処理
	ArrowStep();

	//アイコンのサイズ変更
	SelectIconSizeChange();
}

//描画処理
void SelectScene::Draw()
{
	//背景描画
	for (int i = 0; i < BACK_GROUND_REQUIRED_NUM; i++)
	{
		//座標を受け取る
		VECTOR vPos = m_vBackPos;
		//背景の大きさをかけて画像を並べる
		vPos.x += SCREEN_SIZE_X * i;
		//描画
		Draw::DrawGraphV(vPos, m_iImageHandle[STAGE_BACK][0], true);
	}

	//現在の選択中ステージ
	int stageNum = m_SelectStage;
	//アイコンの周囲計算用
	int stageDrawIconCount = SELECT_ICON_DRAW_CONST;
	//ステージアイコンの移動完了個数
	int stageIconMoveFinCount = 0;
	for (int i = 0;i < DRAW_ICON_NUM + SELECT_ICON_DRAW_MAX_COUNT;i++)
	{
		//カウントが0(選択中)なら表示しない
		if (stageDrawIconCount == 0)
		{
			//カウントを進める
			stageDrawIconCount++;
			continue;
		}

		//ステージの数内だったら実行
		if (stageNum + stageDrawIconCount >= 0 && stageNum + stageDrawIconCount < OpenStageNum)
		{
			//描画画像IDを取得
			int drawImageID = SELECT_ICON_STAGE;
			//ステージの指定がエンドロールだったら画像を変更
			if (stageNum + stageDrawIconCount == STAGE_NUM - 1)
			{
				//エンドロールアイコンにする
				drawImageID = SELECT_ICON_END_ROLL;
			}

			//アイコンの描画
			Draw::DrawRotaGraphV(m_vPos[stageNum + stageDrawIconCount], m_fImageSize[stageNum + stageDrawIconCount], 0.0f, m_iImageHandle[drawImageID][0], true);
		}

		//カウントを加算
		stageDrawIconCount++;
	}

	//黒線と背景テキスト
	for (int i = STAGE_BLACK_LINE; i < IMAGE_NUM; i++)
	{
		//コントローラーが接続されている
		if (Input::Controller::CheckControllerConnected())
		{
			//入力情報_コントローラーを描画
			if (i == PUSH_BUTTON_A)
			{
				DrawGraph(0, 0, m_iImageHandle[i][m_PushButtonFlag[PUTH_IMAGE_CONTROLLER]], true);
			}
		}
		//コントローラーが接続されていない
		else
		{
			//入力情報_キーボードを描画
			if (i == PUSH_ENTER)
			{
				DrawGraph(0, 0, m_iImageHandle[i][m_PushButtonFlag[PUTH_IMAGE_KEY]], true);
			}
			//入力情報_マウスを描画
			if (i == PUSH_MOUSE)
			{
				DrawGraph(0, 0, m_iImageHandle[i][m_PushButtonFlag[PUTH_IMAGE_MOUSE]], true);
			}
		}

		//入力情報以前は通常描画
		if (i < PUSH_BUTTON_A)
		{
			DrawGraph(0, 0, m_iImageHandle[i][0], true);
		}
	}

	//選択中
	//描画画像IDを取得
	int drawImageID = SELECT_ICON_STAGE;
	//ステージの指定がエンドロールだったら画像を変更
	if (m_SelectStage == STAGE_NUM - 1)
	{
		//エンドロールアイコンにする
		drawImageID = SELECT_ICON_END_ROLL;
	}
	//選択中のアイコンは他よりも前面に表示
	Draw::DrawRotaGraphV(m_vPos[m_SelectStage], m_fImageSize[m_SelectStage], 0.0f, m_iImageHandle[drawImageID][0], true);

	//ステージ数の描画
	if (m_SelectStage != STAGE_END)
	{
		//選択中のステージ番号を文字列に変換
		string st = to_string(m_SelectStage);
		//フォントサイズを変更
		SetFontSize(m_iFontSize[m_SelectStage]);
		//描画
		Draw::DrawFormatStringCenter(m_vPos[m_SelectStage], COLOR[WHITE], st.c_str());
	}

	//ステージ評価
	VECTOR vPos = m_vPos[m_SelectStage];
	//位置を変更変数を計算
	VECTOR coordinateAdjustment = VScale(STAGE_EVALUATION_ADD_POS, m_fImageSize[m_SelectStage]);
	//変更
	vPos = VAdd(vPos, coordinateAdjustment);

	//評価を表示
	int imageNum = Data::GetStageEvaluation(m_SelectStage) - 1;
	if (imageNum != -1)
	{
		//エンドロールじゃなかったら
		if (m_SelectStage != STAGE_END)
		{
			//評価を描画
			Draw::DrawRotaGraphV(
				vPos,
				STAGE_EVALUATION_SIZE* m_fImageSize[m_SelectStage], 0.0f,
				m_iImageHandle[STAGE_EVALUATION_ICON_S + imageNum][0], true);
		}
	}
	
	//フォントサイズを戻す
	SetFontSize(BASIC_FONT_SIZE);

	//選択ヤジルシ
	for (int i = 0; i < ARROW_SIDE_NUM; i++)
	{
		//矢印の移動量を保存
		float speed = m_fArrowMove;
		//画像の上下反転フラグ
		bool turnFlag = false;

		//←なら
		if (i == SIDE_LEFT)
		{
			//移動を反転(逆に進む)
			speed *= -1;
			//画像を反転
			turnFlag = true;
		}

		//矢印の座標を取得
		VECTOR pos = SELECT_STAGE_ARROW_POS[i];

		//移動
		pos.x += speed;

		//ステージ左端の状態で←矢印
		//ステージ右端の状態で→矢印の場合
		//描画せず戻る
		if (m_SelectStage == STAGE_0 && i == SIDE_LEFT ||
			m_SelectStage == OpenStageNum - 1 && i == SIDE_RIGHT)
			continue;

		//矢印の描画
		Draw::DrawRotaGraphAlpha(pos, 1.0f, 0.0f, m_iArrowFade[i], m_iImageHandle[STAGE_SELECT_ARROW][0], true, turnFlag);
	}

	//長押し中の表示
	Select::Draw(m_vPos[m_SelectStage]);

	//マウスカーソル
	Mouse::GetInstance()->Draw();
}

//終了処理
void SelectScene::Fin()
{
	//BGMを0にする
	Sound::SetFadeVolume(BGM_SELECT, SOUND_MIN_VOLUME);
}

//================================================

//タイトルに戻る
void SelectScene::TitleBack()
{
	//戻る
	if (InputManager::CheckPush(INPUT_BACK))
	{
		//タイトルに戻る
		Data::SetNextScene(SCENE_TITLE);

		//終了処理へ
		m_SceneID = SELECT_SCEME_END;
		//フェードアウト
		Fade::GetInstance()->RequestFade(Fade::FADE_OUT, FADE_OUT_SPEED);

		//選択中のステージを格納
		Data::SetSelectStage((STAGE)m_SelectStage);

		//BGMを止める
		Sound::StopSound(BGM_SELECT);
	}
}

//入力情報の確認
void SelectScene::InputCheck()
{
	//入力情報表示の初期化
	for (int i = 0; i < PUTH_IMAGE_NUM; i++)
	{
		m_PushButtonFlag[i] = false;
	}

	//入力情報の確認
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

void SelectScene::Select()
{
	//ステージ移動中は実行しない
	if (m_StageMove)
		return;

	//戻る
	TitleBack();

	//入力間隔が指定の数なら
	if(Math::MatchSpecifiedNum(m_fKeyInterval, FRAME_TIME, SELECT_KEY_INTERVAL))
	{
		//マウス
		//矢印の選択
		bool selectFlag[ARROW_SIDE_NUM] = { false,false };	//選択しているか（エンター込み）
		for (int i = 0; i < ARROW_SIDE_NUM; i++)
		{
			//いどうりょうをけいさん
			float speed = m_fArrowMove;

			//←矢印
			if (i == SIDE_LEFT)
			{
				//移動量を反転
				speed *= -1;
			}

			//矢印の座標を取得
			VECTOR pos = SELECT_STAGE_ARROW_POS[i];
			//移動
			pos.x += speed;

			//矢印とマウスカーソルが当たっているか
			if (Mouse::GetInstance()->Collision(pos, SELECT_ARROW_SIZE))
			{
				//マウスで選択していたら
				if (InputManager::CheckPush(INPUT_DETERMINATION))
				{
					//選択
					selectFlag[i] = true;

					break;
				}
			}
		}

		//以下入力があったら==================================================

		//←へ
		if (InputManager::CheckSelectLeft() || selectFlag[SIDE_LEFT])
		{
			//左端だったら
			if (m_SelectStage <= STAGE_0)
				return;

			//←に移動
			m_SelectStage = (STAGE)m_SelectStage - 1;

			//矢印選択
			SelectArrow();
		}
		//→へ
		if (InputManager::CheckSelectRight() || selectFlag[SIDE_RIGHT])
		{
			if (m_SelectStage >= OpenStageNum - 1)
				return;

			//→に移動
			m_SelectStage = (STAGE)m_SelectStage + 1;

			//矢印選択
			SelectArrow();
		}
	}

	//マウス決定
	VECTOR mpuseSelectStagePos = SELECT_STAGE_IMAGE_POS[DRAW_ICON_SELECT_NOW];
	//アイコンがマウスで選択されていなかったら
	if (!Select::SelectMouse(mpuseSelectStagePos, SELECT_ICON_SIZE) && Select::GetDecisionToMouseFlag())
	{
		//長押しをリセット
		Select::SelectReset();
	}

	//決定長押し進行
	if (Select::SelectStep())
	{
		//選択中のステージを格納
		Data::SetSelectStage((STAGE)m_SelectStage);

		//終了処理へ
		m_SceneID = SELECT_SCEME_END;
		//フェードアウト
		Fade::GetInstance()->RequestFade(Fade::FADE_OUT, FADE_OUT_SPEED);

		//効果音を鳴らす
		Sound::Play(SE_ENTER);

		//次のシーンを設定(プレイ画面へ)
		Data::SetNextScene(SCENE_PLAY);

		//エンドロールを選択時
		if (m_SelectStage == STAGE_END)
		{
			//次のシーンを設定(エンドロール画面へ)
			Data::SetNextScene(SCENE_END);
		}

		//最後に遊んだステージデータを保存
		Data::SetLastPlayStage();
	}
}

//矢印選択
void SelectScene::SelectArrow()
{
	//入力間隔をリセット
	m_fKeyInterval = 0.0f;

	//動かすようにする
	m_StageMove = true;

	//矢印を内側に入れる
	m_fArrowMove = -50.0f;

	//矢印の動きを統一
	m_SelectArrowMoveFlag = true;

	//効果音を鳴らす
	Sound::Play(SE_CURSOR);
}

//背景の通常処理
void  SelectScene::BackGroundStep()
{
	//画面一枚分動いたら
	if (Math::MatchSpecifiedNum(m_vBackPos.x, -SCREEN_SIZE_X, BACK_GROUND_MOVE_SPEED))
	{
		//位置をリセット
		m_vBackPos.x = 0.0f;
	}
}

/*====================
	選択アイコン
====================*/
//アイコンの移動
void SelectScene::Move()
{
	//ステージ移動していないときは以下実行しない
	if (!m_StageMove)
		return;

	//長押し選択をリセット
	Select::SelectReset();

	//現在の選択中ステージ
	int stageNum = m_SelectStage;
	//アイコンの周囲計算用
	int stageDrawIconCount = SELECT_ICON_DRAW_MIN_COUNT;
	//ステージアイコンの移動完了個数
	int stageIconMoveFinCount = 0;

	for (int i = 0;i < DRAW_ICON_NUM;i++)
	{
		//ステージアイコンの移動が完了したかのフラグ
		bool movefin = false;

		//ステージの数内だったら実行
		if (stageNum + stageDrawIconCount >= 0 && stageNum + stageDrawIconCount < OpenStageNum)
		{
			//アイコンを移動
			movefin = SelectIconMove(stageNum + stageDrawIconCount, i);
		}
		else
		{
			//範囲外だったらそもそも完了
			movefin = true;
		}

		//アイコンの位置カウントを加算
		stageDrawIconCount++;

		//移動が完了していたら完了数を加算
		if (movefin)
		{
			//アイコン移動完了数を加算
			stageIconMoveFinCount++;
		}
	}

	//表示分の移動が全て完了したら
	if (stageIconMoveFinCount== DRAW_ICON_NUM)
	{
		//ステージ移動フラグをきる
		m_StageMove = false;
		//アイコンのサイズ変更フラグを初期化
		m_SelectSizeFlag = SELECT_ICON_SIZE_MOVE_FLAG_INIT;
	}
}

//アイコンの移動
bool SelectScene::SelectIconMove(int numID, int ID)
{
	//現在位置から移動完了か調べる
	if (Math::GetDistance(m_vPos[numID], SELECT_STAGE_IMAGE_POS[ID]) <= SELECT_ICON_MOVE_SPEED &&
		Math::GetDistance(m_vPos[numID], SELECT_STAGE_IMAGE_POS[ID]) >= -SELECT_ICON_MOVE_SPEED)
	{
		//指定の座標に固定する
		m_vPos[numID] = SELECT_STAGE_IMAGE_POS[ID];

		//移動完了
		return true;
	}
	else
	{
		//アイコンを移動
		m_vPos[numID] = VAdd(m_vPos[numID], Math::GetMoving(m_vPos[numID], SELECT_STAGE_IMAGE_POS[ID], SELECT_ICON_MOVE_SPEED));

		//アイコンのサイズを変更
		Math::MatchSpecifiedNum(m_fImageSize[numID], SELECT_ICON_SIZE_MAGNIFICATION[ID], SELECT_ICON_SIZE_MOVE_SPEED);

		//フォントサイズを変更
		Math::MatchSpecifiedNum(m_iFontSize[numID], SELECT_FONT_SIZE[ID], STAGE_FONT_SIZE_SPEED);
	}
	
	//移動中
	return false;
}

//アイコンのサイズ変更
void SelectScene::SelectIconSizeChange()
{
	//選択中のアイコンの動き
	if (m_SelectSizeFlag)
	{
		//大きくする
		if (Math::MatchSpecifiedNum(m_fImageSize[m_SelectStage], SELECT_ICON_SIZE_MAGNIFICATION[DRAW_ICON_SELECT_NOW] + SELECT_ICON_SIZE_ADD_MAX, SELECT_ICON_SIZE_SPEED))
		{
			m_SelectSizeFlag = false;
		}
	}
	else
	{
		//小さくする
		if (Math::MatchSpecifiedNum(m_fImageSize[m_SelectStage], SELECT_ICON_SIZE_MAGNIFICATION[DRAW_ICON_SELECT_NOW] + SELECT_ICON_SIZE_ADD_MIN, SELECT_ICON_SIZE_SPEED))
		{
			m_SelectSizeFlag = true;
		}
	}
}

/*====================
		選択矢印
====================*/
//矢印のステップ
void SelectScene::ArrowStep()
{
	//矢印の移動
	ArrowMove();
	
	//矢印の透明度変化
	ArrowFade();
}

//矢印の移動
void SelectScene::ArrowMove()
{
	//矢印の移動
	if (m_SelectArrowMoveFlag)
	{
		//移動最大値 + 余裕分から現在の移動量を引いて指定の数で割る
		float speed = (SELECT_ARROW_MOVE_MAX + SELECT_ARROW_MOVE_ADD - m_fArrowMove) / SELECT_ARROW_MOVE_SPEED_DIVISOR;
		//移動量が最大になったら
		if (Math::MatchSpecifiedNum(m_fArrowMove, SELECT_ARROW_MOVE_MAX, speed))
		{
			m_SelectArrowMoveFlag = false;
		}
	}
	else
	{
		//現在の移動量 + 余裕分を指定の数で割る
		float speed = (m_fArrowMove + SELECT_ARROW_MOVE_ADD) / SELECT_ARROW_MOVE_SPEED_DIVISOR;
		//移動量が最小になったら
		if (Math::MatchSpecifiedNum(m_fArrowMove, SELECT_ARROW_MOVE_MIN, speed))
		{
			m_SelectArrowMoveFlag = true;
		}
	}
}

//矢印の透明度変化
void SelectScene::ArrowFade()
{
	//矢印の表示（アイコン移動中は表示しない）
	for (int i = 0; i < 2; i++)
	{
		//ステージが移動していない
		if (!m_StageMove)
		{
			//透明度を最大にする
			Math::MatchSpecifiedNum(m_iArrowFade[i], SELECT_ARROW_FADE_MAX, SELECT_ARROW_FADE_SPEED);
		}
		//ステージが移動している
		else
		{
			//透明度を最小にする
			Math::MatchSpecifiedNum(m_iArrowFade[i], SELECT_ARROW_FADE_MIN, SELECT_ARROW_FADE_SPEED);
		}
	}
}