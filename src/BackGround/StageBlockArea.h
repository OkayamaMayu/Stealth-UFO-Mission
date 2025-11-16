#pragma once
#include"../LoadStageData/LoadStageData.h"
#include"StageBlock.h"

class StageBlockArea :public CModel{
private:
	CollisionAABB		m_Collision;	//区画の当たり判定
	vector<StageBlock>	m_StageBlock;	//ステージブロック

public:
	void Init(AABB set);
	void Init();
	void Load(int originHandle[]);
	void Start();
	void Step();
	void Draw();
	void Fin();

	//ブロックを格納
	void				SetStageBlock(StageBlock set) { m_StageBlock.push_back(set); }
	//ブロックを取り出す
	StageBlock			GetStageBlock(int ID) { return m_StageBlock[ID]; }
	//ブロック群を呼び出す
	vector<StageBlock> &GetAraeBlock() { return m_StageBlock; }
	//コリジョン情報を取り出す
	CollisionAABB		GetCollision() { return m_Collision; }

	//当たった処理
	void Hit(CollisionBase* hitCollision);
private:
	//当たり判定
	bool HitBlock(AABB stageBlockCollison,CollisionBase* hitCollision);
};