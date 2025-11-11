#pragma once
#include"../BackGround/StageBlockManager.h"
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"

class Laser :public CModel
{
public:
	//本体のモデルサイズ
	const VECTOR	LASER_SIZE			= { 24.0f * MODEL_SCALE,21.0f * MODEL_SCALE,100.0f * MODEL_SCALE };
	//最大長さ
	const float		LASER_MAX_LENGTH	= 100.0f;

private:

	float	m_fLaserLength[2];	//レーザーの長さ（1は保存用）
	int		m_iEffectHandle;	//常時設定用EFFECTハンドル

public:
	void	Init(VECTOR vPos, float fRot);
	void	Init();
	void	Load(int originHandle);
	void	Start();
	void	Step();
	void	Draw();
	void	Fin();

public:
	//レーザーの長さを取得
	float	GetLaserLength() { return m_fLaserLength[0]; }
	//レーザーの長さを設定
	void	SetLaserLength(float set) { m_fLaserLength[0] = set; }
};