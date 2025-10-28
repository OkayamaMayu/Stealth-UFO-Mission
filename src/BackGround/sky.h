#pragma once
#include"DxLib.h"
#include"../MyLib/MyLib.h"

class Sky
{
private:
	//モデルの種類
	enum MODEL_ID
	{
		SKY_DOME,	//スカイドーム
		GRID,		//格子

		MODEL_NUM,	//MODELの数
	};
	const float		SKY_SPEED = 0.0001f;	//天球の回転速度
	const float		MODEL_SCALE = 0.2f;
	//モデルパス
	const string	SKY_MODEL_PATH[MODEL_NUM] =
	{ 
		"data/map/sky/sky.x",			//スカイドーム
		"data/map/sky/wire.x"			//格子
	};

private:
	VECTOR	m_vPos;						//モデルの座標（共通）
	VECTOR	m_vRot;						//モデルの回転（共通）
	int		m_iHandle[MODEL_NUM];		//モデルハンドル

public:
	void Init(VECTOR cameraPos);
	void Load();
	void Start();
	void Step(VECTOR cameraPos);
	void Draw();
	void Fin();
};
