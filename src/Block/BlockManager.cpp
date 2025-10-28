#include"BlockManager.h"

void BlockManager::Init()
{
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		block[i].Init();
	}
}

void BlockManager::Load()
{
	int oriHandle = MV1LoadModel(MODEL_PATH.c_str());
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		//ブロックのモデル読み込み
		block[i].Load(oriHandle);
	}

	//設置位置確認用モデルをコピー
	m_iHandle = MV1DuplicateModel(oriHandle);
}

void BlockManager::Start()
{
	//透明度の設定
	MV1SetOpacityRate(m_iHandle, MODEL_ALPHA);
	//モデルのスケールの設定
	MV1SetScale(m_iHandle, MODEL_SCALE_V);
}

bool BlockManager::Step(bool setModeFlag)
{
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		block[i].Step();
	}

	//ブロック配置モードなら実行
	if (setModeFlag)
	{
		//設置できたらtrueを返す
		if (RequestBlock())
		{
			return true;
		}
	}

	//座標の適応
	MV1SetPosition(m_iHandle, m_vSetBlockPos);

	return false;
}

void BlockManager::Draw()
{
	//すでに設置されたブロックの描画
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		if (block != nullptr)
			block[i].Draw();
	}

	//設置可能になったら描画
	if (m_BlockSetFlag)
	{
		if (m_iHandle != -1)
			MV1DrawModel(m_iHandle);
	}
}

void BlockManager::Fin()
{
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		block[i].Fin();
	}
}

//----------------------------------------------------------------

//ブロックの設置
bool BlockManager::RequestBlock()
{
	//ブロック配置可能になっていなかったら
	if (m_BlockSetFlag && Data::GetBlockGauge() < SET_BLOCK_POINT)
		return false;

	//配置キーが押されたら実行
	if (InputManager::CheckPush(INPUT_BLOCK_PLACE))
	{
		for (int i = 0; i < BLOCK_MAX_NUM; i++)
		{
			if (block[i].RequestBlock(m_vSetBlockPos))
			{
				//配置ポイントを減少
				Data::SetBlockGauge(Data::GetBlockGauge() - SET_BLOCK_POINT);

				//設置位置を格納
				VECTOR vPos = m_vSetBlockPos;
				//ブロック分ずらす
				vPos.y -= BLOCK_SIZE;

				//エフェクトを再生
				CEffekseerCtrl::Request(EFFECT_CREATE_BLOCK, vPos, false);

				return true;
			}
		}
	}

	return false;
}