#pragma once
#include"CheckPoint.h"
#include"../LoadStageData/LoadStageData.h"

class CheckPointManager
{
private:
	//モデルサイズ
	const float		MODEL_SIZE		= 5.0f;	
	//モデルパス
	const string	MODEL_PATH		= "data/checkPoint/checkPoint.mv1" ;

private:
	CheckPoint* m_CheckPoint;		//チェックポイント
	VECTOR		m_vReSpawnPos;		//リスポーン座法
	float		m_fReSpawnRot;		//リスポーン回転
	int			m_iCheckPointNum;	//チェックポイントの数

public:
	void		Init(VECTOR startPos,float startRot, LoadStageData& setData);
	void		Load();
	void		Start();
	void		Step();
	void		Draw();
	void		Fin();

	bool		IsHit(int ID,VECTOR setReSpawnPos,float setReSpawnRot);

	//チェックポイントの個別データの取得
	CheckPoint& GetCheckPoint(int ID) { return m_CheckPoint[ID]; }
	//チェックポイントの数を取得
	int			GetCheckPointNum() { return m_iCheckPointNum; }
	//リスポーン座標取得
	VECTOR		GetReSpawnPos() { return m_vReSpawnPos; }
	//リスポーン回転取得
	float		GetReSpawnRot() { return m_fReSpawnRot; }
	//モデルのサイズ取得
	float		GetModelSize() { return MODEL_SIZE; }

	//チェックポイントを距離で透かす
	void CheckCheckPointToCamera(VECTOR playerPos, VECTOR cameraPos, float cameraLemgth);

};