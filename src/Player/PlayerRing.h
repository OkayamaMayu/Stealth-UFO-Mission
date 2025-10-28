#pragma once
#include "DxLib.h"
#include "../MyLib/MyLib.h"

class PlayerRing
{
private:
	//モデルの種類
	enum MODEL_TYPE
	{
		NORMAL,			//通常
		ENEMY_FOUND,	//エネミー発見時

		MODEL_TYPE_NUM,	//モデルの種類数
	};

	//モデルパス
	const string MODEL_PATH[MODEL_TYPE_NUM] =
	{
		"data/pl/model/plRing/plRing_normal.x",
		"data/pl/model/plRing/plRing_enemy.x"
	};

	const float RING_ROT_SPEED = 0.01f;		//リングの回転速度

private:
	VECTOR	m_vPlRingRot;					//リングの回転値
	VECTOR	m_vPlRingPos;					//リングの座標
	bool	m_DrawRingFlag;					//リングの表示フラグ
	int		m_iRingHandle;					//足元リングのハンドル
	int		m_iDrawHandle[MODEL_TYPE_NUM];	//表示切替ハンドル

public:
	void	Init(VECTOR setPos, VECTOR setRot);
	void	Load();
	void	Start();
	void	Step();
	void	Updata();
	void	Draw(bool drawFrg);
	void	Fin();

	//描画フラグの設定
	void	SetDrawRingFlag(bool set) { m_DrawRingFlag = set; }
	//座標を更新
	void	UpdateRingPos(VECTOR setPos) { MV1SetPosition(m_iRingHandle, setPos); }
	//座標を設定
	void	SetRingPos(VECTOR set) { m_vPlRingPos = set; }
	//座標を取得
	VECTOR	GetRingPos() { return m_vPlRingPos; }
};