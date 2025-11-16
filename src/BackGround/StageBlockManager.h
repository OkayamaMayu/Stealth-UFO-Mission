#pragma once
#include"DxLib.h"
#include"StageBlockArea.h"
#include"../Camera/CameraManager.h"

//区画の数
const int MAP_AREA_NUM_X = 4;
const int MAP_AREA_NUM_Z = 4;
//マップの区画のサイズ
const VECTOR MAP_AREA_SIZE = {
	MAP_NUM_X * BLOCK_SIZE * 2 / MAP_AREA_NUM_X,
	MAP_NUM_Z * BLOCK_SIZE * 2,
	MAP_NUM_Y * BLOCK_SIZE * 2 / MAP_AREA_NUM_Z,
};
//マップの最小Y座標
const float MAP_MIN_Y = -40;
//マップの区画数
const int	MAP_AREA_NUM = MAP_AREA_NUM_X * MAP_AREA_NUM_Z;

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
	StageBlockArea	m_StageBlockArea[MAP_AREA_NUM];			//ブロックの区画
	int				m_iBlockNum;									//ブロックの数
	int				m_iHandleOrigin[StageBlock::BLOCK_TYPE_NUM];	//オリジナルハンドル

public:
	StageBlockManager();
	void		Init(LoadStageData &data);
	void		Load();
	void		Start();
	void		Step();
	void		Draw();
	void		Fin();

	//ブロックの数を取得
	int					GetBlockNum() { return m_iBlockNum; }
	//マップの区画を取得
	StageBlockArea		GetStageBlockArea(int ID) { return m_StageBlockArea[ID]; }
	//区画のブロック群を呼び出す
	vector<StageBlock>	GetStageBlock(int ID) { return m_StageBlockArea[ID].GetAraeBlock(); }
	//個別で取得
	StageBlock			GetBlock(int area, int ID) { return m_StageBlockArea[area].GetStageBlock(ID); }
	//ブロックを距離で透かす
	void				CheckStageBlockToCamera(CameraManager& cameraManager);
};