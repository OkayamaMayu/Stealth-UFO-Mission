#pragma once
#include"Block.h"
#include"../Camera/CameraManager.h"
#include"../BackGround/StageBlockManager.h"

const int	BLOCK_MAX_NUM = 20;		//最大設置可能数
class BlockManager
{
private:
	const string	MODEL_PATH	=  "data/block/block.x" ;
	const float		MODEL_ALPHA		= 0.5f;	//確認用モデルのアルファ値
	const float		BLOCK_SIZE		= 5.0f;	//配置ブロックのサイズ

private:
	VECTOR	m_vSetBlockPos;				//ブロックが配置位置
	Block	block[BLOCK_MAX_NUM];		//ブロック情報
	bool	m_BlockSetFlag;				//ブロック配置可能
	int		m_iHandle;					//設置位置確認用モデルハンドル

public:
	void	Init();
	void	Load();
	void	Start();
	bool	Step(bool setModeFlag);
	void	Draw();
	void	Fin();

private:
	//ブロックの設置
	bool	RequestBlock();

public:
	//ブロックの個別情報の取得
	Block&	GetBlock(int ID) { return block[ID]; }
	VECTOR	GetSetBlockPos() { return m_vSetBlockPos; }
	void	SetBlockPos(VECTOR set) { m_vSetBlockPos = set; }
	bool	GetBlockSetFlag() { return m_BlockSetFlag; }
	void	SetBlockSetFlag(bool set) { m_BlockSetFlag = set; }
	//配置ブロック位置の設定
	void	CheckSetBlockPos(CameraManager& cameraManager, StageBlockManager& stage, bool setMode);
};