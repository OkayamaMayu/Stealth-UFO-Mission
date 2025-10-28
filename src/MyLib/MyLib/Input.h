#pragma once
#include "DxLib.h"

const int KEY_BUF_LEN			= 256;
const int MOUSE_BUF_LEN			= 3;
const int CONTROLLER_BUF_LEN	= 16;

//マウスホイールの状態
enum MOUSE_WHEEL_STATUS
{
	MOUSE_WHEEL_BACK,	//奥
	MOUSE_WHEEL_FRONT,	//手前
};

class Input
{
private:
	
	static char				m_CurrentKeyBuf[KEY_BUF_LEN];		//現在フレームのキー情報
	static char				m_CurrentMouseBuf[MOUSE_BUF_LEN];	//現在フレームのマウス情報
	static XINPUT_STATE		m_CurrentControllerBuf;				//現在のフレームのコントローラー情報
	
	static char				m_PreKeyBuf[KEY_BUF_LEN];			//前のフレームのキー情報
	static char				m_PreMouseBuf[MOUSE_BUF_LEN];		//前のフレームのマウス情報
	static XINPUT_STATE		m_PreControllerBuf;					//前のフレームのコントローラー情報
	
	static int				m_iMouseWheel;						//マウスホイール情報

public:
	//入力制御初期化
	static void Init();

	//入力制御ステップ
	//他のStepより早く呼ぶ
	static void Step();

	//何かしらが今押された
	static bool CheckInputPush();

	/*==========================================================
							キーボード
	==========================================================*/
	struct Key
	{
		//今押された：KEY_INPUT_～
		static bool Push(int key_code);

		//押し続けられている：KEY_INPUT_～
		static bool Keep(int key_code);

		//たった今離された：KEY_INPUT_～
		static bool Release(int key_code);

		//=======================================

		//単純に押されているか：KEY_INPUT_～
		static bool Down(int key_code);
	};

	/*==========================================================
								マウス
	==========================================================*/
	struct Mouse
	{
		//今押された：MOUSE_INPUT_LEFT or MOUSE_INPUT_RIGHT
		static bool Push(int key_code);

		//押し続けられている：MOUSE_INPUT_LEFT or MOUSE_INPUT_RIGHT
		static bool Keep(int key_code);

		//たった今離された：MOUSE_INPUT_LEFT or MOUSE_INPUT_RIGHT
		static bool Release(int key_code);

		//マウスホイール：MOUSE_WHEEL_BACK = 奥　MOUSE_WHEEL_FRONT = 手前
		static bool Wheel(MOUSE_WHEEL_STATUS state);
	};

	/*==========================================================
							コントローラー
	==========================================================*/
	/*
	 メモ
		XINPUT_BUTTON_A					// Aボタン
		XINPUT_BUTTON_B					// Bボタン
		XINPUT_BUTTON_X					// Xボタン
		XINPUT_BUTTON_Y					// Yボタン

		XINPUT_BUTTON_DPAD_UP			// デジタル方向ボタン上
		XINPUT_BUTTON_DPAD_DOWN			// デジタル方向ボタン下
		XINPUT_BUTTON_DPAD_LEFT			// デジタル方向ボタン左
		XINPUT_BUTTON_DPAD_RIGHT		// デジタル方向ボタン右

		XINPUT_BUTTON_LEFT_SHOULDER		// LBボタン
		XINPUT_BUTTON_RIGHT_SHOULDER	// RBボタン

		XINPUT_BUTTON_LEFT_THUMB		// 左スティック押し込み
		XINPUT_BUTTON_RIGHT_THUMB		// 右スティック押し込み

		XINPUT_BUTTON_START				// STARTボタン
		XINPUT_BUTTON_BACK				// BACKボタン

	*/
	struct Controller
	{
		//接続状態の確認
		static bool CheckControllerConnected();

		//今押された：XINPUT_BUTTON_～
		static bool Push(int button_code);
		//押し続けられている：XINPUT_BUTTON_～
		static bool Keep(int button_code);
		//たった今離された：XINPUT_BUTTON_～
		static bool Release(int button_code);

		//左スティック：倒し度の格納先
		static void LStickIncline(float &stickX,float &stickY);
		//右スティック：倒し度の格納先
		static void RStickIncline(float& stickX, float& stickY);

		//LT：倒し度の格納先
		static void LeftTriggerIncline(float& trigger);
		//LT：倒し度の格納先
		static void RightTriggerIncline(float& trigger);
	};
};