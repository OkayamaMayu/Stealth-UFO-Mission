#include"DxLib.h"
#include"../MyLib/MyLib.h"
#include"ItemManager.h"

void ItemManager::Init(LoadStageData& data)
{
	m_iItemMaxNum = data.GetItemNum();
	item = nullptr;

	if (item == nullptr)
	{
		item = new Item[m_iItemMaxNum];
	}

	for (int i = 0; i < m_iItemMaxNum; i++)
	{
		if (item == nullptr)
			continue;

		VECTOR itemPos = data.GetItemSpawnPos(i);
		itemPos.y += GetItem(i).ITEM_SIZE.y;
		item[i].Init(itemPos, VGet(0.0f, 0.0f, 0.0f));
	}
}

void ItemManager::Load()
{
	//オリジナルモデルの読み込み
	int m_iOriginHandle[2];
	for(int i=0;i<2;i++)
		m_iOriginHandle[i] = MV1LoadModel(MODEL_PATH[i].c_str());

	for(int i=0;i<m_iItemMaxNum;i++)
	{
		item[i].Load(m_iOriginHandle[0],m_iOriginHandle[1]);
	}
}

void ItemManager::Start()
{
	for (int i = 0;i < m_iItemMaxNum;i++)
	{
		item[i].Start();
	}
}

void ItemManager::Step(VECTOR plPos, VECTOR focusPos, float plRot, float plCamaraRot, VECTOR plSpeed, bool plVisionFlag, float focusRot, bool blockModeFlag)
{
	for (int i = 0; i < m_iItemMaxNum; i++)
	{
		//カメラのタイプで分岐
		if (plVisionFlag)
		{
			//カメラの向きを含めた実行
			item[i].Step(focusPos, plCamaraRot, plSpeed, plVisionFlag, blockModeFlag, focusRot);
		}
		else
		{
			item[i].Step(plPos, plRot, plSpeed, plVisionFlag, blockModeFlag);
		}
	}
}

void ItemManager::Draw()
{
	for (int i = 0; i < m_iItemMaxNum; i++)
	{
		if (item == nullptr)
			continue;

		if (item[i].GetIsUse())
			item[i].Draw();
	}
}

void ItemManager::Fin()
{
	for (int i = 0; i < m_iItemMaxNum; i++)
	{
		if (item == nullptr)
			continue;

		item[i].Fin();
	}

	//メモリの開放
	if (item != nullptr)
	{
		delete[] item;
		item = nullptr;
	}
}