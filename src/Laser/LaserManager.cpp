#include"LaserManager.h"

void LaserManager::Init(LoadStageData& data)
{
	//レーザーの総数を取得
	m_iLaserNum		= data.GetLaserNum();
	laser			= new Laser[m_iLaserNum];

	for (int i = 0; i < m_iLaserNum; i++)
	{
		VECTOR pos	= data.GetLaserPos(i);
		float rot	= data.GetLaserRot(i);
		laser[i].Init(pos, rot);
	}
}

void LaserManager::Load()
{
	int oriHandle = MV1LoadModel(MODEL_PATH.c_str());

	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
		laser[i].Load(oriHandle);
	}
}

void LaserManager::Start()
{
	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
			laser[i].Start();
	}
}

void LaserManager::Step()
{
	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
		laser[i].Step();
	}
}

void LaserManager::Draw()
{
	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
		laser[i].Draw();
	}
}

void LaserManager::Fin()
{
	if (laser != nullptr)
	{
		for (int i = 0; i < m_iLaserNum; i++)
		{
			laser[i].Fin();
		}

		//メモリの開放
		delete[] laser;
		laser = nullptr;
	}
}
