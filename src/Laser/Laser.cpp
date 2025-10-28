#include"Laser.h"
#include"../MyLib/MyLib.h"

void Laser::Init(VECTOR vPos, float fRot)
{
	m_vPos = vPos;
	m_vNextPos = vPos;
	
	float setRot = fRot;

	VECTOR rot = { 0.0f,setRot ,0.0f };

	m_vRot = rot;

	Init();
}

void Laser::Init()
{
	m_vScale			= Vector::MakeVec(MODEL_SCALE);
	m_fLaserLength[0]	= m_fLaserLength[1] = 0.0f;
}

void Laser::Load(int originHandle)
{
	//ブロックをコピー
	m_iHandle = MV1DuplicateModel(originHandle);
}

void Laser::Start()
{
	//大きさを変える
	MV1SetScale(m_iHandle, m_vScale);
	//エフェクトの再生
	m_iEffectHandle = CEffekseerCtrl::Request(EFFECT_LASER, m_vNextPos, true);
	//情報の更新
	Updata();
}

void Laser::Step()
{
	Updata();

	//長さが変更されていたらエフェクトの位置を変更する
	if (m_fLaserLength[0] != m_fLaserLength[1])
	{
		CEffekseerCtrl::SetPosition(m_iEffectHandle, m_vNextPos);
	}

	//現在の長さを保存する
	m_fLaserLength[1] = m_fLaserLength[0];
}

void Laser::Draw()
{
	DrawLine3D(m_vPos, m_vNextPos, COLOR[YELLOW]);
	CModel::Draw();
}

void Laser::Fin()
{
	CModel::Fin();
}

