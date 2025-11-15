#pragma once
#include"CheckPoint.h"
#include"../LoadStageData/LoadStageData.h"

class CheckPointManager
{
private:
	
	//モデルパス
	const string	MODEL_PATH		= "data/checkPoint/checkPoint.mv1" ;

private:
	CheckPoint* m_CheckPoint;		//チェックポイント
	ReSpawnInfo	m_ReSpawnInfo;		//リスポーン情報
	int			m_iCheckPointNum;	//チェックポイントの数

public:
	void		Init(VECTOR startPos,float startRot, LoadStageData& setData);
	void		Load();
	void		Start();
	void		Step();
	void		Draw();
	void		Fin();

	//チェックポイントの個別データの取得
	CheckPoint& GetCheckPoint(int ID) { return m_CheckPoint[ID]; }
	//チェックポイントの数を取得
	int			GetCheckPointNum() { return m_iCheckPointNum; }
	//リスポーン座標取得
	VECTOR		GetReSpawnPos() { return m_ReSpawnInfo.pos; }
	//リスポーン回転取得
	float		GetReSpawnRot() { return m_ReSpawnInfo.rot; }

	//チェックポイントを距離で透かす
	void CheckCheckPointToCamera(VECTOR playerPos, VECTOR cameraPos, float cameraLemgth);
};