#include"Select.h"
#include"../MyLib/MyLib.h"

bool Select::m_SelectMouseFlag;			//マウスで選択できるか
bool Select::m_DecisionFlag;			//選択中フラグ
bool Select::m_DecisionToMouseFlag;		//マウスが選択中フラグ

float Select::m_fDecisionTime;			//現在の選択時間

//----------------------------------------------------------------------------

void Select::Init()
{
	m_SelectMouseFlag		= true;
	m_DecisionFlag			= false;
	m_DecisionToMouseFlag	= false;
	m_fDecisionTime			= 0.0f;
}

//マウスが選択できるか判定(四角)：中心座標とサイズ(直径)
bool Select::SelectMouse(VECTOR vPos, VECTOR vSize)
{
	m_SelectMouseFlag = false;

	if (Mouse::GetInstance()->Collision(vPos, vSize))
	{
		m_SelectMouseFlag = true;

		return true;
	}

	return false;
}
//マウスが選択できるか判定(円形)：中心座標とサイズ(半径)
bool Select::SelectMouse(VECTOR vPos, float fSize)
{
	m_SelectMouseFlag = false;

	if (Mouse::GetInstance()->Collision(vPos, fSize))
	{
		m_SelectMouseFlag = true;

		return true;
	}

	return false;
}

//選択中の処理
bool Select::SelectStep()
{
	//決定
	if(InputManager::CheckPush(INPUT_DETERMINATION) && m_SelectMouseFlag|| InputManager::CheckPush(INPUT_DETERMINATION))
	{
		m_DecisionFlag = true;

		//マウスだったらマウスでの入力フラグを入れる
		if (Input::Mouse::Push(MOUSE_INPUT_LEFT))
		{
			m_DecisionToMouseFlag = true;
		}
	}

	//長押し中
	if (m_DecisionFlag)
		if (InputManager::CheckKeep(INPUT_DETERMINATION))
		{
			//長押しを加算
			if (Math::MatchSpecifiedNum(m_fDecisionTime, DECISION_TUME, FRAME_TIME))
			{
				SelectReset();

				return true;
			}
		}

	//判定が切れる
	if (InputManager::CheckRelease(INPUT_DETERMINATION))
	{
		SelectReset();
	}

	if (!m_DecisionFlag)
	{
		SelectReset();
	}

	return false;
}

//描画：表示座標
void Select::Draw(VECTOR vPos)
{
	if (!m_DecisionFlag)
		return;

	float percent = 0;
	percent = (float)m_fDecisionTime / DECISION_TUME * HOLD_GAUGE_SIZE;

	//背景
	Draw::DrawBlendModeAlpha(HOLD_GAUGE_BACK_ALPHA);
	DrawCircleAA(vPos.x, vPos.y, HOLD_GAUGE_SIZE * HOLD_GAUGE_SCALE + 1, HOLD_GAUGE_CIRCLE_POSNUM, COLOR[BLACK], true);
	Draw::DrawBlendModeReset();

	//本体
	Draw::DrawBlendModeAlpha(HOLD_GAUGE_MAIN_ALPHA);
	DrawCircleAA(vPos.x, vPos.y, percent * HOLD_GAUGE_SCALE, HOLD_GAUGE_CIRCLE_POSNUM, COLOR[CYAN], true);
	Draw::DrawBlendModeReset();

	//枠
	DrawCircleAA(
		vPos.x, vPos.y, 
		HOLD_GAUGE_SIZE * HOLD_GAUGE_SCALE + 1,
		HOLD_GAUGE_CIRCLE_POSNUM, COLOR[YELLOW], 
		false,
		HOLD_GAUGE_LINE_YELLOW_SIZE
	);
	//枠
	DrawCircleAA(
		vPos.x, vPos.y, 
		HOLD_GAUGE_SIZE * HOLD_GAUGE_SCALE + 2,
		HOLD_GAUGE_CIRCLE_POSNUM, COLOR[BLACK], 
		false, 
		HOLD_GAUGE_LINE_BLACK_SIZE
	);
}

//選択をリセット
void Select::SelectReset()
{
	m_DecisionFlag			= false;
	m_DecisionToMouseFlag	= false;
	m_fDecisionTime			= 0.0f;
}