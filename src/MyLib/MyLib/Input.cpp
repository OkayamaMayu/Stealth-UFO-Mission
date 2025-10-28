#include"input.h"
#include"math.h"

//現在フレームのキー情報
char Input::m_CurrentKeyBuf[KEY_BUF_LEN];
//現在フレームのマウス情報
char Input::m_CurrentMouseBuf[MOUSE_BUF_LEN];
//現在のフレームのコントローラー情報
XINPUT_STATE Input::m_CurrentControllerBuf;

//前のフレームのキー情報
char Input::m_PreKeyBuf[KEY_BUF_LEN];
//前のフレームのキー情報
char Input::m_PreMouseBuf[MOUSE_BUF_LEN];
//前のフレームのコントローラー情報
XINPUT_STATE Input::m_PreControllerBuf;

//マウスホイール情報
int Input::m_iMouseWheel;

//入力制御初期化
void Input::Init()
{
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		m_CurrentKeyBuf[index] = '\0';
		m_PreKeyBuf[index] = '\0';
	}

	for (int index = 0; index < MOUSE_BUF_LEN; index++)
	{
		m_CurrentMouseBuf[index] = '\0';
		m_PreMouseBuf[index] = '\0';
	}

	for (int index = 0; index < CONTROLLER_BUF_LEN; index++)
	{
		m_CurrentControllerBuf.Buttons[index] = '\0';
		m_PreControllerBuf.Buttons[index] = '\0';
	}

	m_iMouseWheel = 0;
}

//入力制御ステップ
//他のStepより早く呼ぶ
void Input::Step()
{
	//前フレームのキー情報変数に記録しておく
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		m_PreKeyBuf[index] = m_CurrentKeyBuf[index];
	}

	for (int index = 0; index < MOUSE_BUF_LEN; index++)
	{
		m_PreMouseBuf[index] = m_CurrentMouseBuf[index];
	}

	for (int index = 0; index < CONTROLLER_BUF_LEN; index++)
	{
		m_PreControllerBuf.Buttons[index] = m_CurrentControllerBuf.Buttons[index];
	}

	//現在のキー情報を取得
	GetHitKeyStateAll(m_CurrentKeyBuf);

	//現在のマウス情報を取得
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		m_CurrentMouseBuf[MOUSE_INPUT_LEFT] = 1;
	}
	else
	{
		m_CurrentMouseBuf[MOUSE_INPUT_LEFT] = 0;
	}
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		m_CurrentMouseBuf[MOUSE_INPUT_RIGHT] = 1;
	}
	else
	{
		m_CurrentMouseBuf[MOUSE_INPUT_RIGHT] = 0;
	}

	//マウスホイールの数値取得
	m_iMouseWheel = GetMouseWheelRotVol();

	//現在のコントローラー情報を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &m_CurrentControllerBuf);
}

//何かしらが押される
bool Input::CheckInputPush()
{
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		if (Key::Push(index))
		{
			return true;
		}
	}

	for (int index = 0; index < MOUSE_BUF_LEN; index++)
	{
		if (Mouse::Push(index))
		{
			return true;
		}
	}

	for (int index = 0; index < CONTROLLER_BUF_LEN; index++)
	{
		if (Controller::Push(index))
		{
			return true;
		}
	}

	return false;
}

/*==========================================================
						キーボード
==========================================================*/

//今押された：KEY_INPUT_～
bool Input::Key::Push(int key_code)
{
	//前フレで押されてない　かつ　現フレで押されている
	if (m_PreKeyBuf[key_code] == 0 && m_CurrentKeyBuf[key_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//押し続けられている：KEY_INPUT_～
bool Input::Key::Keep(int key_code)
{
	//前フレで押されている　かつ　現フレで押されている
	if (m_PreKeyBuf[key_code] == 1 && m_CurrentKeyBuf[key_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//たった今離された：KEY_INPUT_～
bool Input::Key::Release(int key_code)
{
	//前フレで押されている　かつ　現フレで押されていない
	if (m_PreKeyBuf[key_code] == 1 && m_CurrentKeyBuf[key_code] == 0)
	{
		return true;
	}

	//押されてるので false
	return false;
}

//=========================================================================================================

//単純に押されているか：KEY_INPUT_～
bool Input::Key::Down(int key_code)
{
	//現フレで押されている（前フレの状態は関係なし）
	if (m_CurrentKeyBuf[key_code] == 1)
	{
		return true;
	}

	return false;
}

/*==========================================================
							マウス
==========================================================*/
//今押された：MOUSE_INPUT_LEFT or MOUSE_INPUT_RIGHT
bool Input::Mouse::Push(int key_code)
{
	//前フレで押されてない　かつ　現フレで押されている
	if (m_PreMouseBuf[key_code] == 0 && m_CurrentMouseBuf[key_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//押し続けられている：MOUSE_INPUT_LEFT or MOUSE_INPUT_RIGHT
bool Input::Mouse::Keep(int key_code)
{
	//前フレで押されている　かつ　現フレで押されている
	if (m_PreMouseBuf[key_code] == 1 && m_CurrentMouseBuf[key_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//たった今離された：MOUSE_INPUT_LEFT or MOUSE_INPUT_RIGHT
bool Input::Mouse::Release(int key_code)
{
	//前フレで押されている　かつ　現フレで押されていない
	if (m_PreMouseBuf[key_code] == 1 && m_CurrentMouseBuf[key_code] == 0)
	{
		return true;
	}

	//押されてるので false
	return false;
}

//マウスホイール：MOUSE_WHEEL_BACK = 奥　MOUSE_WHEEL_FRONT = 手前
bool Input::Mouse::Wheel(MOUSE_WHEEL_STATUS state)
{
	//奥
	if (state == MOUSE_WHEEL_BACK)
	{
		if (m_iMouseWheel > 0)		//+
		{
			return true;
		}
	}

	//手前
	else if (state == MOUSE_WHEEL_FRONT)
	{
		if (m_iMouseWheel < 0)		//-
		{
			return true;
		}
	}

	return false;
}

/*==========================================================
							コントローラー
==========================================================*/

//接続状態の確認
bool Input::Controller::CheckControllerConnected()
{
	if (GetJoypadNum() >= 1)
	{
		return true;
	}

	return false;
}

//今押された：XINPUT_BUTTON_～
bool Input::Controller::Push(int button_code)
{
	//前フレで押されてない　かつ　現フレで押されている
	if (m_PreControllerBuf.Buttons[button_code] == 0 && m_CurrentControllerBuf.Buttons[button_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//押し続けられている：XINPUT_BUTTON_～
bool Input::Controller::Keep(int button_code)
{
	//前フレで押されている　かつ　現フレで押されている
	if (m_PreControllerBuf.Buttons[button_code] == 1 && m_CurrentControllerBuf.Buttons[button_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//たった今離された：XINPUT_BUTTON_～
bool Input::Controller::Release(int button_code)
{
	//前フレで押されている　かつ　現フレで押されていない
	if (m_PreControllerBuf.Buttons[button_code] == 1 && m_CurrentControllerBuf.Buttons[button_code] == 0)
	{
		return true;
	}

	//押されてるので false
	return false;
}

//左スティック：倒し度の格納先
void Input::Controller::LStickIncline(float& stickX, float& stickY)
{
	stickX = 0.0f;
	stickY = 0.0f;

	//横方向
	//128＝倒されていない
	if (m_CurrentControllerBuf.ThumbLX == 128)
	{
		stickX = 0.0f;
	}
	//倒されている
	else
	{
		//-（←）
		if (m_CurrentControllerBuf.ThumbLX <= 128)
		{
			stickX = (float)m_CurrentControllerBuf.ThumbLX / 32768.0f;
		}
		//+（→）
		else
		{
			stickX = (float)m_CurrentControllerBuf.ThumbLX / 32767.0f;
		}
	}
	
	//縦方向
	//128＝倒されていない
	if (m_CurrentControllerBuf.ThumbLY == 128)
	{
		stickY = 0.0f;
	}
	//倒されている
	else
	{
		//-（↓）
		if (m_CurrentControllerBuf.ThumbLY <= 128)
		{
			stickY = (float)m_CurrentControllerBuf.ThumbLY / 32768.0f;
		}
		//+（↑）
		else
		{
			stickY = (float)m_CurrentControllerBuf.ThumbLY / 32767.0f;
		}
	}

	//ほぼ誤差なので消す
	if (fabs(stickX) <= 0.06f)
	{
		stickX = 0.0f;
	}
	if (fabs(stickY) <= 0.06f)
	{
		stickY = 0.0f;
	}
}

//右スティック：倒し度の格納先
void Input::Controller::RStickIncline(float& stickX, float& stickY)
{
	stickX = 0.0f;
	stickY = 0.0f;

	//横方向
	//128＝倒されていない
	if (m_CurrentControllerBuf.ThumbRX == 128)
	{
		stickX = 0.0f;
	}
	//倒されている
	else
	{
		//-（←）
		if (m_CurrentControllerBuf.ThumbRX <= 128)
		{
			stickX = (float)m_CurrentControllerBuf.ThumbRX / 32768.0f;
		}
		//+（→）
		else
		{
			stickX = (float)m_CurrentControllerBuf.ThumbRX / 32767.0f;
		}
	}

	//縦方向
	//128＝倒されていない
	if (m_CurrentControllerBuf.ThumbRY == 128)
	{
		stickY = 0.0f;
	}
	//倒されている
	else
	{
		//-（↓）
		if (m_CurrentControllerBuf.ThumbRY <= 128)
		{
			stickY = (float)m_CurrentControllerBuf.ThumbRY / 32768.0f;
		}
		//+（↑）
		else
		{
			stickY = (float)m_CurrentControllerBuf.ThumbRY / 32767.0f;
		}
	}

	//ほぼ誤差なので消す
	if (fabs(stickX) <= 0.06f)
	{
		stickX = 0.0f;
	}
	if (fabs(stickY) <= 0.06f)
	{
		stickY = 0.0f;
	}
}

//LT：倒し度の格納先
void Input::Controller::LeftTriggerIncline(float& trigger)
{
	trigger = 0.0f;
	trigger= (float)m_CurrentControllerBuf.LeftTrigger / 255.0f;
}

//LT：倒し度の格納先
void Input::Controller::RightTriggerIncline(float& trigger)
{
	trigger = 0.0f;
	trigger = (float)m_CurrentControllerBuf.RightTrigger / 255.0f;
}