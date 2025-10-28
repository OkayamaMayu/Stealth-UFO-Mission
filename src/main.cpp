#include"DxLib.h"	//DXライブラリのインクルード
#include"MyLib/MyLib.h"
#include"Scene/SceneManager.h"

SceneManager sceneManager;

// Win32アプリケーションは WinMain関数 から始まる
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウの名前を変える
	SetMainWindowText("ステルスUFOミッション");

	//ウィンドウの状態を設定する
	ChangeWindowMode(false);

	SetMouseDispFlag(false);

	//アンチエイリアスを設定する
	SetFullSceneAntiAliasingMode(4, 2);

	// 画面サイズを変更
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);

	// １メートルに相当する値を設定する
	Set3DSoundOneMetre(5.0f);

	SetWindowIconID(101);

	//DXライブラリの初期化
	if (DxLib_Init() == -1) {
		return -1;
	}

	//描画するスクリーンを設定する
	SetDrawScreen(DX_SCREEN_BACK);

	//-----------------------------------------
	//一番最初に１回だけやる処理をここに書く

	//インプット初期化
	InputManager::Init();
	//エフェクシア初期化
	//エフェクトの同時表示最大数, パーティクルの最大数
	CEffekseerCtrl::Init(1000, 2000);

	//環境=============================
	//標準ライトのディフューズカラー
	SetLightDifColor(GetColorF(0.7f, 1.0f, 1.0f, 0.0f));

	// フォグを有効にする
	SetFogEnable(TRUE);
	// フォグの色
	SetFogColor(0, 155, 255);
	// フォグの開始距離を０、終了距離を１０００にする
	SetFogStartEnd(50.0f, 1000.0f);

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	//-----------------------------------------

	//ゲームメインループ
	while (ProcessMessage() != -1)
	{
		if (FPS())
		{
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
				//エスケープキーが押されたら終了
				break;
			}

			//画面に表示されたものを初期化
			ClearDrawScreen();

			//インプット通常処理
			InputManager::Step();
			

			//-----------------------------------------
			//ここからゲームの本体を書くことになる
			//-----------------------------------------

			sceneManager.Loop();

			//サウンド通常処理
			Sound::Step();

			sceneManager.Draw();

			//FPS計算
			CalcFPS();
			//FPS表示（デバック用）
			//DrawFPS();

			//フリップ関数
			ScreenFlip();
		}
	}

	//-----------------------------------------
	//最後に１回だけやる処理をここに書く

	CEffekseerCtrl::Exit();

	//-----------------------------------------
	//DXライブラリの後処理
	DxLib_End();

	return 0;
}

