#include"CheckPointManager.h"

void CheckPointManager::Init(VECTOR startPos, float startRot, LoadStageData& setData)
{
	//初期位置
	m_vReSpawnPos	= startPos;
	m_fReSpawnRot	= startRot;

	//チェックポイントの数
	m_iCheckPointNum = setData.GetCheckPointNum();

	if (m_CheckPoint == nullptr)
	{
		m_CheckPoint = new CheckPoint[m_iCheckPointNum];
	}

	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)
			continue;

		m_CheckPoint[i].Init(setData.GetCheckPointPos(i), setData.GetCheckPointRot(i));
	}
}

void CheckPointManager::Load()
{
	int oriHandle = MV1LoadModel(MODEL_PATH.c_str());

	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)
			continue;

		m_CheckPoint[i].Load(oriHandle);
	}
}

void CheckPointManager::Start()
{
	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)
			continue;

		m_CheckPoint[i].Start();
	}
}

void CheckPointManager::Step()
{
	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)
			continue;

		m_CheckPoint[i].Step();
	}
}

void CheckPointManager::Draw()
{
	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)
			continue;

		m_CheckPoint[i].Draw();
	}
}

void CheckPointManager::Fin()
{
	if (Data::GetReSpawnFlag())
		return;

	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)
			continue;

		m_CheckPoint[i].Fin();
	}

	if (m_CheckPoint != nullptr)
	{
		delete[] m_CheckPoint;
		m_CheckPoint = nullptr;
	}
}

bool CheckPointManager::IsHit(int ID, VECTOR setReSpawnPos, float setReSpawnRot)
{	
	if (m_CheckPoint == nullptr)
		return false;

	//起動するか確認
	if (m_CheckPoint[ID].IsHit())
	{
		//リスポーン地点を設定
		m_vReSpawnPos = setReSpawnPos;
		//現在の向きを設定
		m_fReSpawnRot = setReSpawnRot;

		return true;
	}

	return false;
}