#pragma once
#include"DxLib.h"

const float DECISION_TUME					= 0.5f;		//長押し決定の時間
const float	HOLD_GAUGE_SIZE					= 10.0f;	//長押し状態のUIサイズ
const float	HOLD_GAUGE_SCALE				= 5.0f;		//長押し状態のUIスケール
const int	HOLD_GAUGE_BACK_ALPHA			= 30;		//長押し状態のUI背景の透明度
const int	HOLD_GAUGE_MAIN_ALPHA			= 60;		//長押し状態のUI本体の透明度
const int	HOLD_GAUGE_CIRCLE_POSNUM		= 4;		//UIの頂点数
const int	HOLD_GAUGE_LINE_YELLOW_SIZE		= 4;		//長押し状態の黄色枠の太さ
const int	HOLD_GAUGE_LINE_BLACK_SIZE		= 3;		//長押し状態の黒色枠の太さ

class Select
{
private:
	static bool		m_SelectMouseFlag;			//マウスで選択できるか
	static bool		m_DecisionFlag;				//選択中フラグ
	static bool		m_DecisionToMouseFlag;		//マウスが選択中フラグ
	static float	m_fDecisionTime;			//現在の選択時間

public:
	static void Init();

	//マウスが選択できるか判定(四角)：中心座標とサイズ(直径)
	static bool SelectMouse(VECTOR vPos, VECTOR vSize);
	//マウスが選択できるか判定(円形)：中心座標とサイズ(半径)
	static bool SelectMouse(VECTOR vPos, float fSize);
	//選択中の処理
	static bool SelectStep();
	//描画：表示座標
	static void Draw(VECTOR vPos);
	//選択をリセット
	static void SelectReset();

public:
	//選択している
	static bool GetDecisionFlag() { return m_DecisionFlag; }
	//マウスの選択権限
	static bool GetSelectMouseFlag() { return m_SelectMouseFlag; }
	//マウスで選択している
	static bool GetDecisionToMouseFlag() { return m_DecisionToMouseFlag; }
};