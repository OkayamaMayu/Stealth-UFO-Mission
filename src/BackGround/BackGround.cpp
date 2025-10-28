#include"BackGround.h"
#include"../MyLib/MyLib.h"

BackGround::BackGround()
{
	//オリジナルのモデルを読み込み
	for (int i = 0; i < BLOCK_TYPE_NUM; i++)
	{
		m_iHandleOrigin[i] = -1;
	}
}

void BackGround::Init(LoadStageData &data)
{
	//数の取得
	m_iBlockNum = data.GetBlockNum();
	//ブロック情報の動的確保
	block = new Block[m_iBlockNum];

	for (int i = 0;i < m_iBlockNum;i++)
	{
		//ブロックタイプを取得
		//データからブロックタイプを変換
		switch (data.GetBlockType(i))
		{
			case data.BLOCK_NORMAL:	//通常ブロック
				block[i].m_iType = BLOCK_NORMAL_1;
				break;

			case data.BLOCK_4:		//通常ブロック追加
				block[i].m_iType = BLOCK_NORMAL_2;
				break;

			case data.BLOCK_AIR:	//空気ブロック
				block[i].m_iType = BLOCK_AIR;
				break;

			case data.BLOCK_WALL:	//壁
				block[i].m_iType = BLOCK_WALL;
				break;

		default:
			break;
		}

		//座標を取得
		block[i].m_vPos		= data.GetBlockPos(i);
		//ハンドルを初期化
		block[i].m_iHandle	= -1;
		//描画フラグを初期化
		block[i].m_IsDraw	= true;
		//透明度を初期化
		block[i].m_fAlpha	= 1.0f;
	}
}

void BackGround::Load()
{
	//オリジナルのモデルを読み込み
	for (int i = 0;i < BLOCK_TYPE_NUM;i++)
	{
		if (m_iHandleOrigin[i] != -1)
		{
			continue;
		}
		m_iHandleOrigin[i] = MV1LoadModel(BLOCK_MODLE_PATH[i].c_str());
	}

	//ブロックのモデル読み込み
	for (int i = 0;i < m_iBlockNum;i++)
	{
		//空気ブロックなら実行しない
		if (block[i].m_iType == BLOCK_AIR)
		{
			continue;
		}
		//ブロックのタイプ情報からモデルをコピー
		block[i].m_iHandle = MV1DuplicateModel(m_iHandleOrigin[block[i].m_iType]);
	}
}

void BackGround::Start()
{
	for (int i = 0;i < m_iBlockNum;i++)
	{
		//モデルのスケールを設定
		MV1SetScale(block[i].m_iHandle, MODEL_SCALE_V);
		//モデルの座標を設定
		MV1SetPosition(block[i].m_iHandle, block[i].m_vPos);
	}
}

void BackGround::Step()
{
	//スケブロックの処理
	for (int i = 0; i < m_iBlockNum; i++)
	{
		//透明度を設定
		float nextAlpha = block[i].m_fAlpha;

		//透明度が指定のどちらかと一致していたら実行しない
		if (block[i].m_IsDraw && nextAlpha == 1.0f ||
			!block[i].m_IsDraw && nextAlpha == SEMITRANSPARENT_ALPHA)
			continue;

		if (block[i].m_IsDraw)
		{
			//透過させない
			Math::MatchSpecifiedNum(nextAlpha, 1.0f, SEMITRANSPARENT_ALPHA_SPEED);
		}
		else
		{
			//透過する
			Math::MatchSpecifiedNum(nextAlpha, SEMITRANSPARENT_ALPHA, SEMITRANSPARENT_ALPHA_SPEED);
		}

		//透明度を適応する
		block[i].m_fAlpha = nextAlpha;
		MV1SetOpacityRate(block[i].m_iHandle, block[i].m_fAlpha);
	}
}

void BackGround::Draw()
{
	//ブロックの数を回す
	for (int i = 0; i < m_iBlockNum; i++)
	{
		//ブロックを表示
		if (block != nullptr)
		{
			//空気ブロックでなければ描画
			if (block[i].m_iType != BLOCK_AIR)
				MV1DrawModel(block[i].m_iHandle);
		}
	}
}

void BackGround::Fin()
{
	//メモリ開放
	if(block != nullptr)
	{
		delete[] block;
		block = nullptr;
	}
}
