#pragma once
#include"DxLib.h"
#include"../LoadStageData/LoadStageData.h"
#include"../MyLib/MyLib.h"
#include"../Camera/CameraManager.h"

const float BLOCK_SIZE = 5.0f;	//ブロックのサイズ

class BackGround
{
public:
	//ブロックの種類
	enum SET_BLOCK
	{
		BLOCK_NORMAL_1,			//通常ブロック_１
		BLOCK_NORMAL_2,			//通常ブロック_２
		BLOCK_WALL,				//壁
		BLOCK_AIR,				//空気ブロック

		BLOCK_TYPE_NUM
	};

private:
	//ブロックの情報
	struct Block
	{
		SET_BLOCK	m_iType;								//ブロックのタイプ
		VECTOR		m_vPos;									//ブロックの座標
		float		m_fAlpha;								//ブロックの透明度
		bool		m_IsDraw;								//ブロックの描画フラグ
		int			m_iHandle;								//ブロックのモデルハンドル
	};
	//ブロックのモデルパス
	const string	BLOCK_MODLE_PATH[BLOCK_TYPE_NUM] = {
		"data/map/stage/base/stage_block4.x" ,				//通常ブロック_１
		"data/map/stage/base/stage_block2.x" ,				//通常ブロック_２
		"data/map/stage/base/stage_block3.x" ,				//壁
		"data/map/stage/base/stage_block1.x" ,				//空気ブロック
	};
	const float		SEMITRANSPARENT_ALPHA = 0.15f;			//半透明のアルファ値
	const float		SEMITRANSPARENT_ALPHA_SPEED = 0.05f;	//透明度の増加量

private:
	Block*			block;									//ブロック情報
	int				m_iBlockNum;							//ブロックの数
	int				m_iHandleOrigin[BLOCK_TYPE_NUM];		//オリジナルハンドル

public:
	BackGround();
	void		Init(LoadStageData &data);
	void		Load();
	void		Start();
	void		Step();
	void		Draw();
	void		Fin();

	//ブロックの座標：ブロックのID
	VECTOR		GetPos(int ID) { return block[ID].m_vPos; }
	//ブロックの数を取得
	int			GetBlockNum() { return m_iBlockNum; }
	//ブロックのタイプを取得
	SET_BLOCK	GetBlockType(int ID) { return block[ID].m_iType; }
	//ブロックを透かすかの設定
	void		SetIsDraw(int ID, bool set) { block[ID].m_IsDraw = set; }

	//ブロックを距離で透かす
	void CheckStageBlockToCamera(CameraManager& cameraManager);
};