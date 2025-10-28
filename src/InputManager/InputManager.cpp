#include "InputManager.h"

//変数の宣言
int InputManager::m_InputMedia[DEVICE_TYPE_NUM][OPERATION_INPUT_NUM];

//初期化
void InputManager::Init()
{
	Input::Init();

	for (int deviceType = 0; deviceType < DEVICE_TYPE_NUM; deviceType++)
	{
		for (int inputID = 0; inputID < OPERATION_INPUT_NUM; inputID++)
		{
			m_InputMedia[deviceType][inputID] = NULL;
		}
	}

	//入力情報

	/*==========================================================
							キーボード
	==========================================================*/
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_MOVE_LEFT] = KEY_INPUT_A;							//移動_左
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_MOVE_RIGHT] = KEY_INPUT_D;							//移動_右
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_MOVE_PROGRESS] = KEY_INPUT_W;						//移動_前進
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_MOVE_REVERSE] = KEY_INPUT_S;						//移動_後進
																	
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_JUMP] = KEY_INPUT_SPACE;							//ジャンプ
																	
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_ITEM_MODE] = KEY_INPUT_E;							//アイテムモード
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_ITEM_THROW] = KEY_INPUT_RETURN;						//アイテムを投げる
																	
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_BLOCK_MODE] = KEY_INPUT_Q;							//ブロック配置モード
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_BLOCK_PLACE] = KEY_INPUT_RETURN;					//ブロックを配置する
																	
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_CAMERA_LEFT] = KEY_INPUT_LEFT;						//カメラ_左回転
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_CAMERA_RIGHT] = KEY_INPUT_RIGHT;					//カメラ_右回転
																	
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_CAMERA_ZOOM] = KEY_INPUT_UP;						//カメラ_ズーム
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_CAMERA_ZOOM_OUT] = KEY_INPUT_DOWN;					//カメラ_ズームアウト
																
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_OPTION_LEFT] = KEY_INPUT_LCONTROL;					//メニューを開く_左キー
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_OPTION_RIGHT] = KEY_INPUT_RCONTROL;					//メニューを開く_右キー
																	
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_DETERMINATION] = KEY_INPUT_RETURN;					//決定
	m_InputMedia[DEVICE_TYPE_KEY][INPUT_BACK] = KEY_INPUT_BACK;								//戻る

	/*==========================================================
								マウス
	==========================================================*/
	m_InputMedia[DEVICE_TYPE_MOUSE][INPUT_ITEM_MODE] = MOUSE_INPUT_RIGHT;					//アイテムモード
	m_InputMedia[DEVICE_TYPE_MOUSE][INPUT_ITEM_MODE] = MOUSE_INPUT_RIGHT;					//アイテムモード
	m_InputMedia[DEVICE_TYPE_MOUSE][INPUT_ITEM_THROW] = MOUSE_INPUT_LEFT;					//アイテムを投げる
																	
	m_InputMedia[DEVICE_TYPE_MOUSE][INPUT_BLOCK_PLACE] = MOUSE_INPUT_LEFT;					//ブロックを配置する
																																			
	m_InputMedia[DEVICE_TYPE_MOUSE][INPUT_DETERMINATION] = MOUSE_INPUT_LEFT;				//決定

	/*==========================================================
							コントローラー
	==========================================================*/
	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_JUMP] = XINPUT_BUTTON_A;						//ジャンプ

	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_ITEM_MODE] = XINPUT_BUTTON_RIGHT_SHOULDER;	//アイテムモード
	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_ITEM_THROW] = XINPUT_BUTTON_A;				//アイテムを投げる

	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_BLOCK_MODE] = XINPUT_BUTTON_LEFT_SHOULDER;	//ブロック配置モード
	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_BLOCK_PLACE] = XINPUT_BUTTON_A;				//ブロックを配置する

	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_OPTION_LEFT] = XINPUT_BUTTON_BACK;			//メニューを開く_左キー
	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_OPTION_RIGHT] = XINPUT_BUTTON_START;			//メニューを開く_右キー

	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_DETERMINATION] = XINPUT_BUTTON_A;			//決定
	m_InputMedia[DEVICE_TYPE_CONTROLLER][INPUT_BACK] = XINPUT_BUTTON_B;						//戻る
}

//通常処理
void InputManager::Step()
{
	Input::Step();
}

//入力情報_今押された
bool InputManager::CheckPush(int operationInputID)
{
	//キーボード
	if (m_InputMedia[DEVICE_TYPE_KEY][operationInputID] != NULL &&
		Input::Key::Push(m_InputMedia[DEVICE_TYPE_KEY][operationInputID]))
	{
		return true;
	}

	//マウス
	if (m_InputMedia[DEVICE_TYPE_MOUSE][operationInputID] != NULL &&
		Input::Mouse::Push(m_InputMedia[DEVICE_TYPE_MOUSE][operationInputID]))
	{
		return true;
	}

	//コントローラー
	if (m_InputMedia[DEVICE_TYPE_CONTROLLER][operationInputID] != NULL &&
		Input::Controller::Push(m_InputMedia[DEVICE_TYPE_CONTROLLER][operationInputID]))
	{
		return true;
	}

	return false;
}

//入力情報_押し続けられている
bool InputManager::CheckKeep(int operationInputID)
{
	//キーボード
	if (m_InputMedia[DEVICE_TYPE_KEY][operationInputID] != NULL &&
		Input::Key::Keep(m_InputMedia[DEVICE_TYPE_KEY][operationInputID]))
	{
		return true;
	}

	//マウス
	if (m_InputMedia[DEVICE_TYPE_MOUSE][operationInputID] != NULL &&
		Input::Mouse::Keep(m_InputMedia[DEVICE_TYPE_MOUSE][operationInputID]))
	{
		return true;
	}

	//コントローラー
	if (m_InputMedia[DEVICE_TYPE_CONTROLLER][operationInputID] != NULL &&
		Input::Controller::Keep(m_InputMedia[DEVICE_TYPE_CONTROLLER][operationInputID]))
	{
		return true;
	}

	return false;
}

//入力情報_離された
bool InputManager::CheckRelease(int operationInputID)
{
	//キーボード
	if (m_InputMedia[DEVICE_TYPE_KEY][operationInputID] != NULL &&
		Input::Key::Release(m_InputMedia[DEVICE_TYPE_KEY][operationInputID]))
	{
		return true;
	}

	//マウス
	if (m_InputMedia[DEVICE_TYPE_MOUSE][operationInputID] != NULL &&
		Input::Mouse::Release(m_InputMedia[DEVICE_TYPE_MOUSE][operationInputID]))
	{
		return true;
	}

	//コントローラー
	if (m_InputMedia[DEVICE_TYPE_CONTROLLER][operationInputID] != NULL &&
		Input::Controller::Release(m_InputMedia[DEVICE_TYPE_CONTROLLER][operationInputID]))
	{
		return true;
	}

	return false;
}

/*========================
		各種行動判定
==========================*/
//入力情報_プレイヤー移動
//引数	：スティックの倒し具合を格納する変数
bool InputManager::CheckPlayerMove(VECTOR& stickState)
{
	//コントローラーの倒し具合を取得
	Input::Controller::LStickIncline(stickState.x, stickState.z);
	stickState = VScale(stickState, -1.0f);
	
	//キーボードでの操作確認
	if (CheckKeep(INPUT_MOVE_LEFT))
	{
		stickState.x = 1.0f;
	}
	if (CheckKeep(INPUT_MOVE_RIGHT))
	{
		stickState.x = -1.0f;
	}
	if (CheckKeep(INPUT_MOVE_PROGRESS))
	{
		stickState.z = -1.0f;
	}
	if (CheckKeep(INPUT_MOVE_REVERSE))
	{
		stickState.z = 1.0f;
	}

	//入力されているかの確認
	bool isInput = false;
	if (fabsf(stickState.x) >= CONTROLLER_INPUT_MIN|| fabsf(stickState.z) >= CONTROLLER_INPUT_MIN)
	{
		isInput = true;
	}

	return isInput;
}

//入力情報_カメラ回転
//引数	：スティックの倒し具合を格納する変数
bool InputManager::CheckCameraMove(VECTOR& stickState)
{
	//コントローラーの倒し具合を取得
	Input::Controller::RStickIncline(stickState.x, stickState.z);
	stickState = VScale(stickState, -1.0f);

	//キーボードでの操作確認
	if (CheckKeep(INPUT_CAMERA_LEFT))
	{
		stickState.x = 1.0f;
	}
	if (CheckKeep(INPUT_CAMERA_RIGHT))
	{
		stickState.x = -1.0f;
	}

	//入力されているかの確認
	bool isInput = false;
	if (fabsf(stickState.x) >= CONTROLLER_INPUT_MIN)
	{
		isInput = true;
	}

	return isInput;
}

//入力情報_カメラのズーム/ズームアウト
//引数	：スティックの倒し具合を格納する変数
bool InputManager::CheckCameraUpDown(VECTOR& stickState)
{
	//コントローラーの倒し具合を取得
	Input::Controller::RStickIncline(stickState.x, stickState.y);

	//キーボードでの操作確認
	if (CheckKeep(INPUT_CAMERA_ZOOM))
	{
		stickState.y = 1.0f;
	}
	if (CheckKeep(INPUT_CAMERA_ZOOM_OUT))
	{
		stickState.y = -1.0f;
	}

	//入力されているかの確認
	bool isInput = false;
	if (fabsf(stickState.y) > CONTROLLER_INPUT_MIN)
	{
		isInput = true;
	}

	return isInput;
}

//入力情報_カメラのズーム/ズームアウト
//引数	：スティックの倒し具合を格納する変数
bool InputManager::CheckCameraZoom(VECTOR& stickState)
{
	//コントローラーの倒し具合を取得
	Input::Controller::RStickIncline(stickState.x, stickState.y);
	stickState = VScale(stickState, -1.0f);

	//キーボードでの操作確認
	if (CheckKeep(INPUT_CAMERA_ZOOM))
	{
		stickState.y = 1.0f;
	}
	if (CheckKeep(INPUT_CAMERA_ZOOM_OUT))
	{
		stickState.y = -1.0f;
	}

	//入力されているかの確認
	bool isInput = false;
	if (fabsf(stickState.y) >= CONTROLLER_INPUT_MIN)
	{
		isInput = true;
	}

	return isInput;
}

//入力情報_UFOへの抵抗
//引数	：スティックの倒し具合を格納する変数
int InputManager::CheckResistanceToUFO(VECTOR& stickState)
{
	int inputNum = 0;

	//移動キーの入力回数を取得
	if (InputManager::CheckPush(INPUT_MOVE_LEFT))
	{
		inputNum++;
	}
	if (InputManager::CheckPush(INPUT_MOVE_RIGHT))
	{
		inputNum++;
	}
	if (InputManager::CheckPush(INPUT_MOVE_PROGRESS))
	{
		inputNum++;
	}
	if (InputManager::CheckPush(INPUT_MOVE_REVERSE))
	{
		inputNum++;
	}

	//ジャンプキーの入力回数を取得
	if (InputManager::CheckPush(INPUT_JUMP))
	{
		inputNum++;
	}

	//アイテムを投げる(行動)の入力回数を取得
	if (InputManager::CheckPush(INPUT_ITEM_THROW))
	{
		inputNum++;
	}

	//コントローラーのボタンの入力回数を取得
	if (Input::Controller::Push(XINPUT_BUTTON_A))
	{
		inputNum++;
	}
	if (Input::Controller::Push(XINPUT_BUTTON_B))
	{
		inputNum++;
	}
	if (Input::Controller::Push(XINPUT_BUTTON_X))
	{
		inputNum++;
	}
	if (Input::Controller::Push(XINPUT_BUTTON_Y))
	{
		inputNum++;
	}

	//コントローラーの倒し具合を取得
	Input::Controller::LStickIncline(stickState.x, stickState.z);

	//入力回数を返す
	return inputNum;
}

//入力情報_←
bool InputManager::CheckSelectLeft()
{
	//コントローラーの倒し具合を取得
	VECTOR stickStateLeft = {};
	VECTOR stickStateRight = {};
	Input::Controller::LStickIncline(stickStateLeft.x, stickStateLeft.y);
	Input::Controller::RStickIncline(stickStateRight.x, stickStateRight.y);

	//←入力_キーボード, コントローラー
	if (Input::Key::Keep(KEY_INPUT_A) || Input::Key::Keep(KEY_INPUT_LEFT) ||
		Input::Controller::Keep(XINPUT_BUTTON_DPAD_LEFT) ||
		stickStateLeft.x <= -CONTROLLER_INPUT_MIN || stickStateRight.x <= -CONTROLLER_INPUT_MIN)
	{
		return true;
	}

	return false;
}

//入力情報_→
bool InputManager::CheckSelectRight()
{
	//コントローラーの倒し具合を取得
	VECTOR stickStateLeft = {};
	VECTOR stickStateRight = {};
	Input::Controller::LStickIncline(stickStateLeft.x, stickStateLeft.y);
	Input::Controller::RStickIncline(stickStateRight.x, stickStateRight.y);

	//→入力_キーボード, コントローラー
	if (Input::Key::Keep(KEY_INPUT_D) || Input::Key::Keep(KEY_INPUT_RIGHT) ||
		Input::Controller::Keep(XINPUT_BUTTON_DPAD_RIGHT) || 
		stickStateLeft.x >= CONTROLLER_INPUT_MIN || stickStateRight.x >= CONTROLLER_INPUT_MIN)
	{
		return true;
	}

	return false;
}

//入力情報_↑
bool InputManager::CheckSelectUp()
{
	//コントローラーの倒し具合を取得
	VECTOR stickStateLeft = {};
	VECTOR stickStateRight = {};
	Input::Controller::LStickIncline(stickStateLeft.x, stickStateLeft.y);
	Input::Controller::RStickIncline(stickStateRight.x, stickStateRight.y);

	//↑入力_キーボード, コントローラー
	if (Input::Key::Keep(KEY_INPUT_W) || Input::Key::Keep(KEY_INPUT_UP) ||
		Input::Controller::Keep(XINPUT_BUTTON_DPAD_UP) || 
		stickStateLeft.y >= CONTROLLER_INPUT_MIN || stickStateRight.y >= CONTROLLER_INPUT_MIN)
	{
		return true;
	}

	return false;
}

//入力情報_↓
bool InputManager::CheckSelectDown()
{
	//コントローラーの倒し具合を取得
	VECTOR stickStateLeft = {};
	VECTOR stickStateRight = {};
	Input::Controller::LStickIncline(stickStateLeft.x, stickStateLeft.y);
	Input::Controller::RStickIncline(stickStateRight.x, stickStateRight.y);

	//↓入力_キーボード, コントローラー
	if (Input::Key::Keep(KEY_INPUT_S) || Input::Key::Keep(KEY_INPUT_DOWN) ||
		Input::Controller::Keep(XINPUT_BUTTON_DPAD_DOWN) || 
		stickStateLeft.y <= -CONTROLLER_INPUT_MIN || stickStateRight.y <= -CONTROLLER_INPUT_MIN)
	{
		return true;
	}

	return false;
}
