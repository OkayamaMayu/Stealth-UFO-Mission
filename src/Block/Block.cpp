#include"Block.h"

void Block::Init()
{
	CModel::Init();

	//変数の初期化
	m_IsUse			= false;
	m_fBlockCount	= 0.0f;

	//スケールの設定
	m_vScale = MODEL_SCALE_V;
}

void Block::Load(int originHandle)
{
	//ブロックをコピー
	m_iHandle = MV1DuplicateModel(originHandle);
}

void Block::Step()
{
	//設置されていなかったら実行しない
	if (!m_IsUse)
		return;

	//消えるまでカウント
	if (Math::MatchSpecifiedNum(m_fBlockCount, BLOCK_SURVIVAL_TIME, FRAME_TIME))
	{
		//時間になったらブロックを消す
		m_IsUse			= false;
		m_fBlockCount	= 0.0f;
	}
}

void Block::Draw()
{
	//設置されていたら描画
	if (m_IsUse)
	{
		CModel::Draw();
	}
}

void Block::Fin()
{
	CModel::Fin();
}

//----------------------------------------------------------------

//ブロックの設置：設置位置
bool Block::RequestBlock(VECTOR vPos)
{
	//すでに設置されていたら実行しない
	if (m_IsUse)
		return false;

	//設置状態にする
	m_IsUse			= true;
	m_vPos			= vPos;
	m_fBlockCount	= 0.0f;

	//SEの再生
	Sound::Play(SE_BLOCK_SET);

	return true;
}
