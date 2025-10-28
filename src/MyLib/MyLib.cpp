#include"MyLib.h"

//作成中限定---------------------------------

EvaluationData Data::	m_EvaluationData;				//ステージ評価
SaveLoad Data::			m_saveData;						//セーブデータ
SaveLoad Data::			m_saveDataBeginning;			//セーブデータ
SCENE Data::			m_Scene;						//シーン
SCENE Data::			m_NextScene;					//次のシーン
STAGE Data::			m_SelectStage;					//選択中のステージ
float Data::			m_fBlockGauge;					//ブロック生成ゲージ
float Data::			m_fDrawBlockGauge;				//ブロック生成ゲージ
float Data::			m_fFoundGauge;					//発見ゲージ
int Data::				m_EffectID[EFFECT_ID_MAX_NUM];	//エフェクシア用ID
bool Data::				m_ReSpawnFlag;					//リスポーンフラグ
bool Data::				m_FoundFlag;					//発見されてるフラグ

//-------------------------------------------

void Data::Step()
{
	//ブロック配置ゲージの数値を調節
	Math::MatchSpecifiedNum(m_fDrawBlockGauge, m_fBlockGauge, 1.5f);
	if (m_fDrawBlockGauge - m_fBlockGauge > SET_BLOCK_POINT)
	{
		m_fDrawBlockGauge -= SET_BLOCK_POINT;
	}
}

//-------------------------------------------

//確率出します：int ％
bool RandPercent(int iPercent)
{
	if (GetRand(100) < iPercent)
	{
		return true;
	}

	return false;
}