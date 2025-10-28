#pragma once
#include"Item.h"
#include"../LoadStageData/LoadStageData.h"

class ItemManager
{

private:

	enum ITEM_NUM
	{
		ITEM_TYPE1,

		ITEM_TYPE_MAX_NUM,
	};

	//アイテムモデルのパス
	const string MODEL_PATH[2] =
	{
		"data/item/item1_1.x",
		"data/item/item1_2.x",
	};

	Item*	item;			//アイテム情報
	int		m_iItemMaxNum;	//アイテムの数

public:

	void Init(LoadStageData &data);
	void Load();
	void Start();
	void Step(VECTOR plPos, VECTOR focusPos, float plRot,float plCamaraRot, VECTOR plSpeed,bool plVisionFlag,float focusRot, bool blockModeFlag);
	void Draw();
	void Fin();

public:
	//アイテム情報を取得
	Item&	GetItem(int ID) { return item[ID]; }
	//アイテムの座標を取得
	VECTOR	GetItemPos(int ID) { return item[ID].GetPos(); }
	//アイテムをキャッチさせる
	void	RequestCatch(int ID) { item[ID].RequestCatch(); }
	//座標の設定
	void	SetPos(int ID, VECTOR set) { item[ID].SetPos(set);}
	//アイテムの最大数を取得
	int		GetItemMaxNum() { return m_iItemMaxNum; }
};