#pragma once
#include"SafetyArea.h"
#include"../LoadStageData/LoadStageData.h"

class SafetyAreaManager
{
private:
	//モデルパス
	const string	SAFETYAREA_MODEL_PATH	= "data/map/stage/safetyArea/safetyArea.x" ;
	const float		POS_ASCENT				= 0.01f;	//モデルの位置を少し上げる
	const float		BLOCK_GAUGE_UP			= 15.0f;	//ブロック配置ゲージの上昇値

private:
	SafetyArea*		safetyArea;				//安全地帯の情報
	int				m_iSafetyAreaMaxNum;	//安全地帯の最大数

public:
	void Init(LoadStageData& data);
	void Load();
	void Start();
	void Step(Player& pl);
	void Draw();
	void Fin();

	int GetSafetyAreaMaxNum() { return m_iSafetyAreaMaxNum; }
	SafetyArea& GetSafetyArea(int ID) { return safetyArea[ID]; }
};