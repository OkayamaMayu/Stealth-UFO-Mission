#include"DxLib.h"
#include"../MyLib/MyLib.h"
#include"SafetyAreaManager.h"

void SafetyAreaManager::Init(LoadStageData& data)
{
	//動的確保
	m_iSafetyAreaMaxNum = data.GetSafetyAreaNum();
	safetyArea = nullptr;

	if (safetyArea == nullptr)
	{
		safetyArea = new SafetyArea[m_iSafetyAreaMaxNum];
	}

	for (int i = 0; i < m_iSafetyAreaMaxNum; i++)
	{
		if (safetyArea == nullptr)
			continue;

		//モデルの位置を調整
		VECTOR safetyAreaPos = data.GetSafetyAreaSpawnPos(i);
		safetyAreaPos.y		+= POS_ASCENT;
		safetyArea[i].Init(safetyAreaPos);
	}
}

void SafetyAreaManager::Load()
{
	int m_iOriginHandle = MV1LoadModel(SAFETYAREA_MODEL_PATH.c_str());

	for (int i = 0; i < m_iSafetyAreaMaxNum; i++)
	{
		if (safetyArea == nullptr)
			continue;

		safetyArea[i].Load(m_iOriginHandle);
	}
}

void SafetyAreaManager::Start()
{
	for (int i = 0; i < m_iSafetyAreaMaxNum; i++)
	{
		if (safetyArea == nullptr)
			continue;

		safetyArea[i].Start();
	}
}

void SafetyAreaManager::Step(Player& pl)
{
	bool plInFlag = false;
	for (int i = 0; i < m_iSafetyAreaMaxNum; i++)
	{
		if (safetyArea == nullptr)
			continue;

		safetyArea[i].Step(pl);
		safetyArea[i].SetPlInFlag(false);

		//プレイヤーの座標を取得
		VECTOR plPos = pl.GetPos();
		
		//プレイヤーが影に入っている
		if (Math::GetDistance(safetyArea[i].GetPos(), plPos) <= SAFETYAREA_SIZE)
		{
			//ブロック配置ゲージの上昇
			plInFlag = true;
			Data::AddBlockGauge(BLOCK_GAUGE_UP);

			safetyArea[i].SetPlInFlag(true);
		}			
	}
	pl.SetSafetyAreaInFlag(plInFlag);
}

void SafetyAreaManager::Draw()
{
	for (int i = 0; i < m_iSafetyAreaMaxNum; i++)
	{
		if (safetyArea == nullptr)
			continue;

		safetyArea[i].Draw();
	}
}

void SafetyAreaManager::Fin()
{
	for (int i = 0; i < m_iSafetyAreaMaxNum; i++)
	{
		if (safetyArea == nullptr)
			continue;

		safetyArea[i].Fin();
	}

	//メモリの開放
	if (safetyArea != nullptr)
	{
		delete[] safetyArea;
		safetyArea = nullptr;
	}
}