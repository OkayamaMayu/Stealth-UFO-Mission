#include "PlayerRing.h"
#include "../MyLib/MyLib.h"

void PlayerRing::Init(VECTOR setPos, VECTOR setRot)
{
	m_iRingHandle	= -1;

	MV1SetScale(m_iDrawHandle[0], MODEL_SCALE_V);
	MV1SetScale(m_iDrawHandle[1], MODEL_SCALE_V);
	
	//座標回転の設定
	m_vPlRingPos	= setPos;
	m_vPlRingRot	= setRot;
	m_vPlRingRot.y += Math::ChangeDegToRad(180.0f);
}

void PlayerRing::Load()
{
	for (int i = 0; i < MODEL_TYPE_NUM; i++)
	{
		m_iDrawHandle[i] = MV1LoadModel(MODEL_PATH[i].c_str());
	}
}

void PlayerRing::Start()
{
	//初期表示モデルはノーマルにする
	m_iRingHandle = m_iDrawHandle[NORMAL];

	MV1SetPosition(m_iRingHandle, m_vPlRingPos);
	MV1SetRotationXYZ(m_iRingHandle, m_vPlRingRot);
	MV1SetScale(m_iRingHandle, MODEL_SCALE_V);
}

void PlayerRing::Step()
{
	//リングを回転
	m_vPlRingRot.y += RING_ROT_SPEED;
}

void PlayerRing::Update()
{
	MV1SetRotationXYZ(m_iRingHandle, m_vPlRingRot);

	//情報の更新
	for (int i = 0; i < MODEL_TYPE_NUM; i++)
	{
		MV1SetPosition(m_iDrawHandle[i], MV1GetPosition(m_iRingHandle));
		MV1SetRotationXYZ(m_iDrawHandle[i], MV1GetRotationXYZ(m_iRingHandle));
		MV1SetScale(m_iDrawHandle[i], MODEL_SCALE_V);
	}
}

void PlayerRing::Draw(bool drawFrg)
{
	//発見されているかでモデルを分ける
	if (Data::GetFoundFlag())
	{
		m_iRingHandle = m_iDrawHandle[ENEMY_FOUND];
	}
	else
	{
		m_iRingHandle = m_iDrawHandle[NORMAL];
	}

	//リング
	UpdateRingPos(m_vPlRingPos);
	if (m_DrawRingFlag && !drawFrg)
	{
		//リングを描画
		MV1DrawModel(m_iRingHandle);
	}
}

void PlayerRing::Fin()
{

}
