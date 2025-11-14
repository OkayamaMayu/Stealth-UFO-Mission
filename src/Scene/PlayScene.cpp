#include"../Scene/SceneManager.h"
#include"../Colli/CollisionManager.h"


PlayScene::PlayScene()
{
	m_SceneID			= PLAY_SCENE_INIT;
	m_NextFlag		= false;
	m_ClearFlag	= false;

	Data::SetReSpawnFlag(false);
}

PlayScene::~PlayScene()
{
	Fin();
}

int PlayScene::Loop()
{
	int iRet = 0;

	switch (m_SceneID)
	{
	case PLAY_SCENE_INIT:
	case PLAY_SCENE_LOAD:

		loadStageData.Load();	//ステージデータの読み込み
		Init();					//初期化
		Load();					//読み込み
		Start();				//開始前準備

		//フェードイン
		Fade::GetInstance()->RequestFade(Fade::FADE_IN, FADEIN_SPEED);

		//サウンドの設定
		Sound::SetVolume(BGM_PLAY, 0);
		Sound::SetVolume(SE_LOOP_PLAY, 0);
		Sound::SetVolume(SE_LOOP_ENEMY_FOUND, 0);
		Sound::SetVolume(SE_LOOP_UFO, 0);
		Sound::SetVolume(SE_LOOP_UFO_CATCH, 0);
		Sound::Play(BGM_PLAY);
		Sound::Play(SE_LOOP_PLAY);

		//マウス操作カメラを初期化する
		cameraMan.GetPlayCamera().ResetMouse();
		//進行
		m_SceneID = PLAY_SCENE_LOOP;
		break;

	case PLAY_SCENE_LOOP:
		Step();
		break;

	case PLAY_SCENE_END:
		if (!Fade::GetInstance()->GetFadeNow())
		{
			//BGMをフェードアウトする
			Sound::SetFadeVolume(BGM_PLAY, 0, SOUND_FADEOUT_SPEED);
			Sound::SetFadeVolume(SE_LOOP_PLAY, 0, SOUND_FADEOUT_SPEED);

			Fin();
			m_SceneID = PLAY_SCENE_INIT;
			iRet = -1;
		}
		break;

	default:
		break;
	}

	return iRet;
}

//================================================================

void PlayScene::Init()
{
	//工程を設定
	m_Progress		= PLAY_MAKE;
	//ブロックの配置可能数の初期化
	m_iBlockNumSub	= 0;

	//標準ライトのスペキュラカラー
	SetLightSpcColor(INIT_LIGHT_COLOR);

	//次のシーンを設定
	Data::SetNextScene(SCENE_PLAY);

	//発見度セット
	Data::SetFoundGauge();
	//発見フラグセット
	Data::SetFoundFlag(false);
	//ブロック生成ゲージを初期化
	Data::SetBlockGauge();
	Data::SetDrawBlockGauge();

	//評価をリセット
	//初期位置の時は初期化
	if (!Data::GetReSpawnFlag())
		Data::GetEvaluationData().Init();

	//プレイヤーの初期情報
	VECTOR plPos = loadStageData.GetPlayerSpawnPos();
	VECTOR plRot = loadStageData.GetPlayerSpawnRot();
	//リスポーンする場合
	if (Data::GetReSpawnFlag())
	{
		//リスポーン位置
		plPos	= checkPoint.GetReSpawnPos();
		plRot.y = checkPoint.GetReSpawnRot();
	}
	//プレイヤーの初期化
	player.Init(plPos, plRot);

	//初期位置の時は初期化
	if (!Data::GetReSpawnFlag())
		checkPoint.Init(plPos, plRot.y, loadStageData);

	//カメラの初期化
	cameraMan.Init(player.GetPos(), plRot.y, loadStageData);

	//エフェクトの初期化
	CEffekseerCtrl::UpdateAutoCamera();

	//変数の初期化
	m_ClearFlag			= false;
	m_NextFlag			= false;
	m_fSceneWaitCount	= 0.0f;
	m_fSetBlockModeSize = SET_BLOCKMODE_FILTER_MAX_SIZE;
	m_CloseFlag			= false;
	m_fCloseCount		= 0.0f;
	m_PauseFlag			= false;

	backGraund	.Init(loadStageData);				//ステージの初期化
	sky			.Init(cameraMan.GetCameraPos());	//天球の初期化
	enemy		.Init(loadStageData);				//エネミーの初期化
	ufo			.Init();							//UFOの初期化
	item		.Init(loadStageData);				//アイテムの初期化
	goal		.Init(loadStageData.GetGoalPos());	//ゴールの初期化
	safetyArea	.Init(loadStageData);				//安全地帯の初期化
	pause		.Init();							//ポーズ画面の初期化
	laser		.Init(loadStageData);				//レーザーの初期化
	block		.Init();							//配置ブロックの初期化
	UI			.Init();							//UIの初期化
}

void PlayScene::Load()
{
	//エフェクトの読み込み
	for (int i = 0; i < EFFECT_ID_MAX_NUM; i++)
	{
		Data::m_EffectID[i] = CEffekseerCtrl::LoadData(EFFECT_PATH[i].c_str());
	}
	//画像の読み込み
	for(int i=0;i<IMAGE_NUM;i++)
	{
		m_iHandle[i] = LoadGraph(IMAGE_PATH[i].c_str());
	}
	//ブロック配置ゲージの読み込み
	LoadDivGraph(
		IMAGE_PATH[SET_BLOCK_GAGE_3].c_str(),
		BLOCK_GAGE_IMAGE_NUM[0], BLOCK_GAGE_IMAGE_NUM[1], BLOCK_GAGE_IMAGE_NUM[2],
		(int)BLOCK_GAGE_IMAGE_SIZE.x, (int)BLOCK_GAGE_IMAGE_SIZE.y,
		m_iSetBlockGage);

	cameraMan	.Load();	//カメラの読み込み
	backGraund	.Load();	//ステージの読み込み
	sky			.Load();	//天球の読み込み
	player		.Load();	//プレイヤーの読み込み
	enemy		.Load();	//エネミーの読み込み
	ufo			.Load();	//UFOの読み込み
	item		.Load();	//アイテムの読み込み
	goal		.Load();	//ゴールの読み込み
	laser		.Load();	//レーザーの読み込み
	safetyArea	.Load();	//安全地帯の読み込み
	pause		.Load();	//ポーズの読み込み
	block		.Load();	//配置ブロックの読み込み
	checkPoint	.Load();	//チェックポイントの読み込み
	UI			.Load();	//UIの読み込み
	//プレイカメラの初期化
	cameraMan.GetPlayCamera().Load();
}

void PlayScene::Start()
{
	//常時発生エフェクトの再生
	m_iEffectHandle = CEffekseerCtrl::Request(EFFECT_ENVIRONMENT, cameraMan.GetCameraPos(), true);

	backGraund	.Start();	//ステージの追加設定処理
	sky			.Start();	//天球の追加設定処理
	block		.Start();	//配置ブロックの追加設定処理
	checkPoint	.Start();	//チェックポイントの追加設定処理
	enemy		.Start();	//エネミーの追加設定処理
	ufo			.Start();	//UFOの追加設定処理
	goal		.Start();	//ゴールの追加設定処理
	item		.Start();	//アイテムの追加設定処理
	laser		.Start();	//レーザーの追加設定処理
	player		.Start();	//プレイヤーの追加設定処理
	safetyArea	.Start();	//安全地帯の追加設定処理

	//レーザーの終了地点を設定
	//CollisionManager::GetInstance()->CheckStageBlockToLaser(laser, backGraund);
	////プレイヤーの足元のリングの位置を設定
	//CollisionManager::GetInstance()->CheckStageBlockToPlRing(player, backGraund, block);
}

void PlayScene::Step()
{
	switch (m_Progress)
	{
	case PlayScene::PLAY_MAKE:	//画面の作成

		//カメラの移動
		if (cameraMan.GetPlayCamera().StartStep(player.GetPos(), goal.GetPos()))
		{
			//カメラの移動が完了したらゲーム本編に移行する
			m_Progress = PLAY_GAME;
		}

		//サウンドを大きくしていく
		Sound::SetFadeVolume(BGM_PLAY, 100, SOUND_FADEIN_SPEED);
		Sound::SetFadeVolume(SE_LOOP_PLAY, 100, SOUND_FADEIN_SPEED);

		break;
	case PlayScene::PLAY_GAME:	//ゲーム本編
		//メイン処理
		play();

		//UIの処理
		UI.Step(
			cameraMan.GetPlVisionFlag(), player.GetSetBlockFlag(), pause.GetIsPause(),
			ufo.GetPLFound(), player.GetItemCatch(), m_PauseFlag, goal.GetIsGoalFlag(),
			player.GetPos());

		break;

	default:
		break;
	}

	sky		.Step(cameraMan.GetCameraPos());	//天球の通常処理
	laser	.Step();							//レーザーの処理

	//常に表示しているエフェクトの更新
	CEffekseerCtrl::SetPosition(m_iEffectHandle, cameraMan.GetCameraPos());
	CEffekseerCtrl::UpdateAutoCamera();
}

void PlayScene::Draw()
{
	sky					.Draw();	//天球の描画

	//ゴールしていないかつプレイヤーがUFOに捕まっていない場合
	if (!goal.GetIsGoalFlag() && !ufo.GetCatchPlayer())
	{
		enemy			.Draw();	//エネミーの描画
		item			.Draw();	//アイテムの描画
		laser			.Draw();	//レーザーの描画
	}

	//プレイヤーがUFOに捕まっていない場合
	if (!ufo.GetCatchPlayer())
	{
		goal			.Draw();	//ゴールの描画
		block			.Draw();	//配置ブロックの描画
		safetyArea		.Draw();	//安全地帯の描画
	}

	//ゴールしていない場合
	if (!goal.GetIsGoalFlag())
		player			.Draw();	//プレイヤーの描画

	//プレイヤーがUFOに捕まっていない場合
	if (!ufo.GetCatchPlayer())
		backGraund		.Draw();	//ステージの描画

	//ゴールしていない場合
	if (!goal.GetIsGoalFlag())
	{
		//プレイヤーがUFOに捕まっていない場合
		if (!ufo.GetCatchPlayer())
		{
			checkPoint	.Draw();	//チェックポイントの描画
		}
		ufo				.Draw();	//UFOの描画
	}

	//環境フィルター：天球をなじませる
	Draw::DrawBlendModeAlpha(SKY_FILTER_ALPHA);
	DrawSphere3D(player.GetPos(), SKY_FILTER_SIZE, SKY_FILTER_POLYGON_NUM, COLOR[CYAN], COLOR[RED], true);
	Draw::DrawBlendModeReset();

	//エフェクトの描画
	CEffekseerCtrl::Draw();

	//環境フィルター：画面端を少し暗くする
	DrawGraph(0, 0, m_iHandle[BLACK_FILTER],true);

	if (!goal.GetIsGoalFlag() && Data::GetScene() != SCENE_GAMEOVER)
	{
		//ブロック配置モードフィルター
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, m_fSetBlockModeSize, 0.0f, m_iHandle[SET_BLOCK_MODE], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	ufo			.DrawUI();						//UFOの連打UIの表示
	cameraMan	.Draw();						//カメラの表示
	UI			.Draw(ufo.GetCatchPlayer());	//UI描画
	
	//=========================
	pause		.Draw();	//ポーズ画面の表示

	DebugString::GetInstance()->Draw();
}

void PlayScene::Fin()
{
	backGraund		.Fin();	//ステージの終了処理
	sky				.Fin();	//天球の終了処理
	cameraMan		.Fin();	//カメラの終了処理
	player			.Fin();	//プレイヤーの終了処理
	enemy			.Fin();	//エネミーの終了処理
	ufo				.Fin();	//UFOの終了処理
	item			.Fin();	//アイテムの終了処理
	pause			.Fin();	//ポーズ画面の終了処理
	goal			.Fin();	//ゴールの終了処理
	safetyArea		.Fin();	//安全地帯の終了処理
	laser			.Fin();	//レーザーの終了処理
	block			.Fin();	//配置ブロックの終了処理
	checkPoint		.Fin();	//チェックポイントの終了処理
	UI				.Fin();	//UIの終了処理
	loadStageData	.Fin();	//ステージ読み込みデータの終了処理

	//シーンIDを元に戻す
	m_SceneID = PLAY_SCENE_INIT;
	//エフェクトを全停止する
	CEffekseerCtrl::StopAll();
}

//==============================================================================================

void PlayScene::play()
{
	//UIの処理
	//ブロック配置モードフィルターのサイズ変更
	if (player.GetSetBlockFlag())
	{
		float speed = (m_fSetBlockModeSize + SETBLOCK_SIZE_MIN_SPEED) / SPEED_RATIO;
		Math::MatchSpecifiedNum(m_fSetBlockModeSize, 1.0f, speed);
	}
	else
	{
		float speed = (SETBLOCK_SIZE_MAX_SPEED + SETBLOCK_SIZE_MIN_SPEED - m_fSetBlockModeSize) / SPEED_RATIO;
		Math::MatchSpecifiedNum(m_fSetBlockModeSize, SET_BLOCKMODE_FILTER_MAX_SIZE, speed);
	}

	//ブロック配置可能数を取得
	int blockNum = (int)(Data::GetBlockGauge() / SET_BLOCK_POINT);
	//保存しているブロック数より大きくなったら
	if (m_iBlockNumSub < blockNum)
	{
		//サウンドを再生する
		Sound::Play(SE_BLOCK_GET);
	}
	//ブロック数を保存する
	m_iBlockNumSub = blockNum;

	//ゲームオーバーになっていたら以下実行しない
	if (Data::GetScene() == SCENE_GAMEOVER)
		return;

	Data::	Step();	//データの処理
	goal.	Step();	//ゴール判定

	//次のシーンがゲームクリア
	if (goal.GetNextSceneFlag() && !Fade::GetInstance()->GetFadeNow())
	{
		//終了処理
		m_SceneID = PLAY_SCENE_END;
		//リザルト画面へ
		Data::SetNextScene(SCENE_RESUTLT);

		//次回初期位置にする
		Data::SetReSpawnFlag(false);
	}

	//次のシーンがゲームオーバー
	if (Data::GetNextScene() == SCENE_GAMEOVER)
	{
		if (m_fSceneWaitCount != -1.0f)
		{
			//一時停止
			if (Math::MatchSpecifiedNum(m_fSceneWaitCount, SCENE_WAIT_TIME, FRAME_TIME))
			{
				m_fSceneWaitCount	= -1.0f;
			}
		}
		else
		{
			//シーンを進行
			Data::SetScene(SCENE_GAMEOVER);
			//BGMをゆっくり消す
			Sound::SetFadeVolume(BGM_PLAY, 0, SOUND_GAMEOVER_FADEOUT_SPEED);
		}
	}

	//クリアしたら
	if (goal.GetIsGoalFlag())
	{
		//通常カメラに戻す
		cameraMan.SetPlVision(false);

		//プレイヤーの向き
		VECTOR plRot = player.GetRot();
		//クリア演出カメラの実行
		cameraMan.ClearCamera(goal.GetPos(), plRot.y);

		//プレイヤーを非表示にする
		player.SetIsDraw(false);

		//エネミーのエフェクトを停止する
		enemy.StopEffect();

		return;
	}

	m_PauseFlag = false;
	//ポーズ画面に何か選択したらそのまま処理を終了する
	if (PauseStep())
		return;

	//ポーズ画面の場合
	if (pause.GetIsPause())
	{
		//音量を変更する
		Sound::SetFadeVolume(BGM_PLAY,				SOUND_PAUSE_VOLUME, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_PLAY,			SOUND_PAUSE_VOLUME, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_ENEMY_FOUND,	SOUND_PAUSE_VOLUME, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_UFO,			SOUND_PAUSE_VOLUME, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_UFO_CATCH,		SOUND_PAUSE_VOLUME, SOUND_FADEOUT_SPEED);
		cameraMan.GetPlayCamera().ResetMouse(1);
		return;
	}
	else if(!m_PauseFlag)
	{
		//音量を変更する
		Sound::SetFadeVolume(BGM_PLAY,				100,				SOUND_FADEIN_SPEED);
		Sound::SetFadeVolume(SE_LOOP_PLAY,			100,				SOUND_FADEIN_SPEED);
		Sound::SetFadeVolume(SE_LOOP_ENEMY_FOUND,	100,				SOUND_FADEIN_SPEED);
		Sound::SetFadeVolume(SE_LOOP_UFO,			100,				SOUND_FADEIN_SPEED);
		Sound::SetFadeVolume(SE_LOOP_UFO_CATCH,		100,				SOUND_FADEIN_SPEED);
	}

	if (!Fade::GetInstance()->GetFadeNow())
	{
		//画面を閉じる確認
		if (!m_CloseFlag)m_CloseFlag = item.GetModeChangeFlag();

		//一人称を閉じるフラグがたっている
		if (m_CloseFlag)
		{
			if (Math::MatchSpecifiedNum(m_fCloseCount, AUTO_CLOSE_FPSCAMERA_TIME, FRAME_TIME))
			{
				//時間とフラグを初期化
				m_fCloseCount	= 0.0f;
				m_CloseFlag		= false;

				//ブロック配置モードになっていたら
				if (player.GetSetBlockFlag())
				{
					//閉じる
					player.SetSetBlockFlag(false);
					//サウンドを再生
					Sound::Play(SE_BLOCK_MODE_OUT);
				}

				//一人称カメラになっていたら閉じる
				if(cameraMan.GetPlVisionFlag())
					cameraMan.GetPlayCamera().ClosePlVision();
			}
		}

		//デバックモード（カメラ参照）じゃない
		if (cameraMan.GetCameraId() == CameraManager::CAMERA_ID_PLAY)
		{
			//次がゲームオーバーになっていないなら実行
			if (Data::GetNextScene() != SCENE_GAMEOVER)
			{
				//プレイヤーの通常処理
				player.Step(ufo.GetPos(), cameraMan.GetCameraForcus(), cameraMan.GetRot(), cameraMan.GetPlVisionFlag(), ufo.GetCatchPlayer());

				//ブロック配置モードの切り替え
				if (InputManager::CheckPush(INPUT_BLOCK_MODE) && !ufo.GetPLFound())
				{
					player.SetSetBlockFlag(true);
					//サウンドを再生
					Sound::Play(SE_BLOCK_MODE_IN);
					//一人称カメラにする
					cameraMan.GetPlayCamera().OpenPlVision();
				}
				if (InputManager::CheckRelease(INPUT_BLOCK_MODE) && !ufo.GetPLFound())
				{
					player.SetSetBlockFlag(false);
					//サウンドを再生
					Sound::Play(SE_BLOCK_MODE_OUT);
					//一人称カメラを閉じる
					cameraMan.GetPlayCamera().ClosePlVision();
				}
			}

			//UFOにつかまれているときは一人称カメラにしない
			if (ufo.GetPLFound())
			{
				//ブロック配置だったら切る
				if (player.GetSetBlockFlag())
				{
					//サウンドを再生
					Sound::Play(SE_BLOCK_MODE_OUT);
					//一人称カメラを閉じる
					cameraMan.GetPlayCamera().ClosePlVision();
				}
				//アイテム使用画面だったら閉じる
				if (cameraMan.GetPlVisionFlag())
					cameraMan.GetPlayCamera().ClosePlVision();
			}

			//エネミーの通常処理
			enemy.Step(player, item, backGraund, m_PauseFlag,goal.GetIsGoalFlag());
			//チェックポイントの通常処理
			checkPoint.Step();

			//UFO
			//発見ゲージが指定より大きくなった
			if (Data::GetFoundGauge() >= REQUEST_UFO_COUNT)
			{
				//UFOを呼び出す
				ufo.RequestUFO(player.GetPos());
			}
			//UFOの通常処理
			ufo.Step(player);

			//アイテムの通常処理
			item.Step(
				player		.GetPos(),			cameraMan.GetCameraForcus(),
				player		.GetRot().y,		cameraMan.GetRot(),
				player		.GetSpeed(),		cameraMan.GetPlVisionFlag(),
				cameraMan	.GetForcusF(),
				player		.GetSetBlockFlag()
			);

			//安全地帯の通常処理
			safetyArea.Step(player);

			//ブロックの通常処理(配置含む)
			block.Step(player.GetSetBlockFlag());
		}

		//==================================================

		//アイテムをつかむ処理：ブロック配置モードじゃない
		if (!player.GetSetBlockFlag())
			ItemCatch();

		//ステージの通常処理
		backGraund.Step();
	}

	//当たり判定
	Collision();

	//UFOに捕まっていなかったら
	if (!ufo.GetCatchPlayer())
	{
		//通常カメラ
		cameraMan.Step(player.GetPos(), player.GetRot().y, player.GetState(), ufo);
	}
	else
	{
		//UFO捕獲後のゲームオーバーカメラ
		VECTOR plPos = ufo.GetPos();
		plPos.y -= ufo.UFO_HEIGHT;
		cameraMan.GameoverCamera(plPos, player.GetPos(), player.GetRot().y);
	}

	//当たり判定_カメラと物体
	PlayCamera playCamera = cameraMan.GetPlayCamera();
	float cameraDiscoveryRange = CAMERA_LEMGTH + CAMERA_LEMGTH_UFO;
	backGraund.CheckStageBlockToCamera(cameraMan);													//ステージ
	ufo.CheckUfoToCamera(playCamera.GetPos(), playCamera.GetPlVisionFlag(), cameraDiscoveryRange);	//UFO
	checkPoint.CheckCheckPointToCamera(player.GetPos(), playCamera.GetPos(), CAMERA_LEMGTH);		//チェックポイント
}

bool PlayScene::PauseStep()
{
	//ゲームオーバー条件に達したら
	if (ufo.GetCatchPlayer()||
		player.GetPos().y <= DEATH_Y / 2)
	{
		m_PauseFlag = true;

		Sound::SetFadeVolume(SE_LOOP_PLAY,			0, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_ENEMY_FOUND,	0, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_UFO,			0, SOUND_FADEOUT_SPEED);
		Sound::SetFadeVolume(SE_LOOP_UFO_CATCH,		0, SOUND_FADEOUT_SPEED);

		return false;
	}

	//ポーズ画面の通常処理
	pause.Step();

	//ポーズ画面が開いていたら
	if (pause.GetIsSelect())
		switch (pause.GetOption())
		{
		case OPTION_FROM_FIRST:			//ステージをやり直す

			if (!Fade::GetInstance()->GetFadeNow())
			{
				//終了処理へ
				m_SceneID = PLAY_SCENE_END;
				//未選択に戻す
				pause.SetOption(OPTION_NEUTRAL);
				//ポーズ画面をとじる
				pause.SetIsPause(false);

				//次のシーンを設定
				Data::SetNextScene(SCENE_PLAY);

				//次回初期位置にする
				Data::SetReSpawnFlag(false);

				return true;
			}

			break;

		case OPTION_STAGE_SELECTION:	//ステージ選択に戻る

			if (!Fade::GetInstance()->GetFadeNow())
			{
				//終了処理へ
				m_SceneID = PLAY_SCENE_END;
				//未選択に戻す
				pause.SetOption(OPTION_NEUTRAL);
				//ポーズ画面をとじる
				pause.SetIsPause(false);

				//次のシーンを設定
				Data::SetNextScene(SCENE_PLAY);
				Data::SetNextScene(SCENE_SELECT);

				//次回初期位置にする
				Data::SetReSpawnFlag(false);

				return true;
			}

			break;

		case OPTION_BACK_GAME:		//ゲームに戻る
			//未選択に戻す
			pause.SetOption(OPTION_NEUTRAL);
			//ポーズ画面をとじる
			pause.SetIsPause(false);
			break;

		default:
			break;
		}

	return false;
}

void PlayScene::ItemCatch()
{
	int itemIndex = -1;
	//掴んだら
	if (player.CatchItem(item, cameraMan.GetRot(), itemIndex, cameraMan.GetCameraForcus()))
	{
		//掴まれてることにする
		item.RequestCatch(itemIndex);

		//アイテムの位置をプレイヤーに合わせる
		VECTOR itemPos = player.GetPos();
		item.SetPos(itemIndex, itemPos);
	}

	//一人称カメラのじゃなかったら以下処理しない
	if (!cameraMan.GetPlVisionFlag())
		return;

	//アイテムを投げる
	bool throwFlag = player.ThrowItem(item, cameraMan.GetRot(), cameraMan.GetForcusF());
	//標的を設定
	enemy.ItemTarget(item, player, cameraMan.GetRot(), cameraMan.GetCameraForcus(), throwFlag);
}

void PlayScene::Collision()
{
	//ブロックの設置場所を計算する
	block.CheckSetBlockPos	(cameraMan, backGraund, player.GetSetBlockFlag());

	//CollisionManager::GetInstance()->CheckGoalToPayer			(player, goal);				//プレイヤーとゴール
	//CollisionManager::GetInstance()->CheckEnemyToPlayer			(player, enemy);			//プレイヤーとエネミー
	//CollisionManager::GetInstance()->CheckEnemyToEnemy			(enemy);					//エネミーとエネミー
	//if (CollisionManager::GetInstance()->CheckEnemyType1ToItem	(enemy, item))				//アイテムとエネミー
	//	m_CloseFlag = true;	//一人称カメラを自動で閉じる
	//CollisionManager::GetInstance()->CheckSetBlockToPlayer		(player, block);			//設置ブロックとプレイヤー
	//CollisionManager::GetInstance()->CheckSetBlockToEnemyType1	(enemy, block);				//設置ブロックとエネミー
	//CollisionManager::GetInstance()->CheckSetBlockToItem		(item, block);				//設置ブロックとアイテム
	//CollisionManager::GetInstance()->CheckCheckPointToPayer		(checkPoint, player);		//チェックポイントとプレイヤー

	////座標が確定される
	//CollisionManager::GetInstance()->CheckStageBlockToPlayer	(player, backGraund);		//プレイヤーとステージ
	//CollisionManager::GetInstance()->CheckStageBlockToEnemyType1(enemy, backGraund);		//エネミーとステージ
	//if (CollisionManager::GetInstance()->CheckStageBlockToItem	(item, backGraund))			//アイテムとステージ
	//	m_CloseFlag = true;	//一人称カメラを自動で閉じる
	//CollisionManager::GetInstance()->CheckStageBlockToPlRing	(player, backGraund,block);	//プレイヤーリングとステージ
	//CollisionManager::GetInstance()->CheckStageBlockToLaser		(laser, backGraund);		//レーザーとステージ

	////レーザー関連
	//CollisionManager::GetInstance()->CheckSetBlockToLaser		(laser, block);				//配置ブロックとレーザー

	//当たり判定
	CollisionManager::GetInstance()->Update();

	//レーザーとプレイヤー
	laser.CheckLaserToPlayer(player.GetPos(), enemy);

	//チューリアルを距離で透かす
	UI.CheckTutorialToPlayer(player.GetPos(), cameraMan.GetPlayCamera().GetPos());
}