#include"SafetyArea.h"

void SafetyArea::Init(VECTOR vPos)
{
	//変数の初期化
	m_vPos			= vPos;
	m_iHandle		= -1;
	m_iEffectHandle = -1;
	m_PlIn			= false;
}

void SafetyArea::Load(int originHandle)
{
	if (m_iHandle == -1)
	{
		m_iHandle = MV1DuplicateModel(originHandle);
		MV1SetScale(m_iHandle, MODEL_SCALE_V);
	}
}

void SafetyArea::Start()
{
	//座標の設定
	MV1SetPosition(m_iHandle, m_vPos);
	//透明度の設定
	MV1SetOpacityRate(m_iHandle, MODEL_ALPHA);

	//エフェクトの再生
	m_iEffectHandle = CEffekseerCtrl::Request(EFFECT_ARROW, m_vPos, true);
}

void SafetyArea::Step(Player& pl)
{
	//プレイヤーが中に入っていたら
	if (!m_PlIn)
	{
		//エフェクトが再生されていなければ再生する
		if(!CEffekseerCtrl::IsActive(m_iEffectHandle))
			m_iEffectHandle = CEffekseerCtrl::Request(EFFECT_ARROW, m_vPos, true);
	}
	else
	{
		//エフェクトを停止する
		CEffekseerCtrl::Stop(m_iEffectHandle);
	}
}

void SafetyArea::Draw()
{
	MV1DrawModel(m_iHandle);
}

void SafetyArea::Fin()
{
	//エフェクトを停止する
	CEffekseerCtrl::Stop(m_iEffectHandle);
}
