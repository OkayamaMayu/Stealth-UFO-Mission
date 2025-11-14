#pragma once
#include"../MyLib/MyLib.h"
#include"../Model/Model.h"

const float BLOCK_SIZE = 5.0f;	//ブロックのサイズ

class StageBlock :public CModel {
public:
	//ブロックの種類
	enum SET_BLOCK{
		BLOCK_NORMAL_1,			//通常ブロック_１
		BLOCK_NORMAL_2,			//通常ブロック_２
		BLOCK_WALL,				//壁
		BLOCK_AIR,				//空気ブロック

		BLOCK_TYPE_NUM
	};

private:
	const float		SEMITRANSPARENT_ALPHA = 0.15f;			//半透明のアルファ値
	const float		SEMITRANSPARENT_ALPHA_SPEED = 0.05f;	//透明度の増加量


private:
	CollisionAABB	m_Collision;						//コリジョン情報
	SET_BLOCK		m_Type;							//ブロックのタイプ
	float			m_fAlpha;							//ブロックの透明度
	bool			m_IsDraw;							//ブロックの描画フラグ

public:
	void Init();
	void Init(SET_BLOCK blockType,VECTOR setPos);
	void Load(int modelHandle);
	void Start();
	void Step();
	void Draw();
	void Fin();

	//ブロックのタイプを取得
	SET_BLOCK		GetBlockType() { return m_Type; }
	//ブロックを透かすかの設定
	void			SetIsDraw(bool set) { m_IsDraw = set; }
	//コリジョン情報を取得
	CollisionAABB	GetCollision() { return m_Collision; }
};