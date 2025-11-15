#pragma once
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"

const float BLOCK_SURVIVAL_TIME = 10.0f;	//ブロックの生存時間

class Block :public CModel
{
private:
	const float		BLOCK_SIZE = 5.0f;	//ブロックのサイズ

	CollisionAABB	m_Collision;		//コリジョン情報
	float			m_fBlockCount;		//消えるまでのカウント
	bool			m_IsUse;			//設置フラグ

public:
	void Init();
	void Load(int originHandle);
	void Step();
	void Draw();
	void Fin();

public:
	//ブロックの設置：設置位置
	bool RequestBlock(VECTOR vPos);

public:
	bool GetIsUse() { return m_IsUse; }
	//コリジョン情報を取得
	CollisionAABB	GetCollision() { return m_Collision; }
	//コリジョン情報の更新
	void			UpdateCollision();
	//コリジョン情報の設定
	void			SetCollision(CollisionAABB set) { m_Collision = set; }
};