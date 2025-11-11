#pragma once
#include"DxLib.h"
#include"StageBlock.h"
#include"../LoadStageData/LoadStageData.h"
#include"../Camera/CameraManager.h"

class StageBlockManager
{
private:
	//ブロックのモデルパス
	const string	BLOCK_MODLE_PATH[StageBlock::BLOCK_TYPE_NUM] = {
		"data/map/stage/base/stage_block4.x" ,				//通常ブロック_１
		"data/map/stage/base/stage_block2.x" ,				//通常ブロック_２
		"data/map/stage/base/stage_block3.x" ,				//壁
		"data/map/stage/base/stage_block1.x" ,				//空気ブロック
	};

private:
	StageBlock*		block;									//ブロック情報
	int				m_iBlockNum;							//ブロックの数
	int				m_iHandleOrigin[StageBlock::BLOCK_TYPE_NUM];		//オリジナルハンドル

public:
	StageBlockManager();
	void		Init(LoadStageData &data);
	void		Load();
	void		Start();
	void		Step();
	void		Draw();
	void		Fin();

	//ブロックの数を取得
	int			GetBlockNum() { return m_iBlockNum; }
	//個別で取得
	StageBlock& GetBlock(int ID) { return block[ID]; }
	//ブロックを距離で透かす
	void		CheckStageBlockToCamera(CameraManager& cameraManager);
};