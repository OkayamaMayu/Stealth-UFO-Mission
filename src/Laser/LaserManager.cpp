#include"LaserManager.h"

void LaserManager::Init(LoadStageData& data)
{
	//レーザーの総数を取得
	m_iLaserNum		= data.GetLaserNum();
	laser			= new Laser[m_iLaserNum];

	//kindを個別で分ける変数
	int kindNum = 0;
	for (int i = 0; i < m_iLaserNum; i++){
		VECTOR pos	= data.GetLaserPos(i);
		float rot	= data.GetLaserRot(i);
		laser[i].Init(pos, rot);

		//kindを設定する
		CollisionLineSegment setCollision = laser[i].GetCollision();
		setCollision.SetKind(KIND_LASER + kindNum);
		laser[i].SetCollision(setCollision);
		//コリジョンを登録
		laser[i].RegisterCollision();

		//次の番号へ変更
		kindNum++;
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