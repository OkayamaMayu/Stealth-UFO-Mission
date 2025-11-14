#include"Block.h"

void Block::Init()
{
	CModel::Init();

	//変数の初期化
	m_IsUse			= false;
	m_fBlockCount	= 0.0f;

	//スケールの設定
	m_vScale = MODEL_SCALE_V;
	//サイズを設定
	m_vSize = VGet(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	//構造体の設定
	UpdateCollision();
}

void Block::Load(int originHandle)
{
	//ブロックをコピー
	m_iHandle = MV1DuplicateModel(originHandle);
}

void Block::Step()
{
	m_Collision.SetIsCollision(false);

	//設置されていなかったら実行しない
	if (!m_IsUse)return;

	m_Collision.SetIsCollision(true);

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
	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

//----------------------------------------------------------------

//ブロックの設置：設置位置
bool Block::RequestBlock(VECTOR vPos)
{
	//すでに設置されていたら実行しない
	if (m_IsUse)return false;

	//設置状態にする
	m_IsUse			= true;
	m_vPos			= vPos;
	m_vNextPos		= m_vPos;
	m_fBlockCount	= 0.0f;

	//当たり判定情報の更新
	UpdateCollision();

	//SEの再生
	Sound::Play(SE_BLOCK_SET);

	return true;
}

//コリジョン情報の更新
void Block::UpdateCollision() {
	AABB setCollision = m_Collision.GetCollision();
	//サイズを設定
	setCollision.size = m_vSize;
	//中心座標を設定
	setCollision.centerPos = m_vNextPos;
	//情報を更新
	m_Collision.SetCollision(setCollision);
}