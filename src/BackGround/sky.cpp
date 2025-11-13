#include"sky.h"
#include"../MyLib/MyLib.h"

void Sky::Init(VECTOR cameraPos)
{
	//変数の初期化
	memset(&m_vRot, 0, sizeof(VECTOR));
	
	//適用
	m_vPos = cameraPos;
	for (int i = 0; i < MODEL_NUM; i++)
	{
		MV1SetPosition(m_iHandle[i], cameraPos);	//座標
		MV1SetRotationXYZ(m_iHandle[i], m_vRot);	//回転
	}
}

void Sky::Load()
{
	//モデル読み込み
	for (int i = 0;i < MODEL_NUM;i++)
	{
		m_iHandle[i] = MV1LoadModel(SKY_MODEL_PATH[i].c_str());
	}
}

void Sky::Start()
{
	VECTOR scale = Vector::MakeVec(Sky::MODEL_SCALE);
	for(int i=0;i<MODEL_NUM;i++)
	{
		//モデルスケールを適用
		MV1SetScale(m_iHandle[i], scale);
	}
}

void Sky::Step(VECTOR cameraPos)
{
	//回転
	m_vRot.y += SKY_SPEED;
	m_vRot.x += SKY_SPEED / 2;

	//変数の適用
	m_vPos = cameraPos;
	for (int i = 0; i < MODEL_NUM; i++)
	{
		//座標を適用
		MV1SetPosition(m_iHandle[i], m_vPos);	
		//スカイドームのみ回転を適用する
		if(i== SKY_DOME)
		{
			//回転を適用
			MV1SetRotationXYZ(m_iHandle[i], m_vRot);
		}
	}
}

void Sky::Draw()
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		if(m_iHandle[i]!=-1)
			MV1DrawModel(m_iHandle[i]);
	}
}

void Sky::Fin()
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		MV1DeleteModel(m_iHandle[i]);
	}
}
