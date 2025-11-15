#pragma once
#include "DxLib.h"
#include "../MyLib/MyLib.h"

class PlayerRing :public CModel
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

	const float RING_ROT_SPEED	= 0.01f;	//リングの回転速度
	const float	MAX_LENGTH		= 100.0f;	//最大長さ

private:
	CollisionLineSegment m_Collision;		//コリジョン情報
	bool	m_DrawRingFlag;					//リングの表示フラグ
	int		m_iDrawHandle[MODEL_TYPE_NUM];	//表示切替ハンドル

public:
	void	Init(VECTOR setPos, VECTOR setRot);
	void	Init();
	void	Load();
	void	Start();
	void	Step();
	void	Update();
	void	Draw(bool drawFrg);
	void	Fin();

	//描画フラグの設定
	void	SetDrawRingFlag(bool set) { m_DrawRingFlag = set; }
	//当たった処理
	void Hit(CollisionBase* hitCollision);

private:
	//コリジョン情報の更新
	void UpdateCollision();
};