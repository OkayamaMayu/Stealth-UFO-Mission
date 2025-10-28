#include "LoadStageData.h"
#include "../MyLib/MyLib.h"

void LoadStageData::Load()
{
	for (int i = 0;i < BLOCK_NUM;i++)
	{
		m_iSetNumCount[i] = 0;
	}

	//ステージ
	StageLoad();

	//カメラの通過ポイントとプレイヤーの距離
	float dis[2];
	VECTOR set		= {};
	int cameraCount = 0;
	while (cameraCount < m_iCameraPointNum)
	{
		set = m_vCameraPoint[cameraCount];

		for (int i = cameraCount; i < m_iCameraPointNum; i++)
		{
			dis[0] = Math::GetDistance(m_vPlPos, set);
			dis[1] = Math::GetDistance(m_vPlPos, m_vCameraPoint[i]);

			//プレイヤーに遠い順に並べ替える
			if (dis[0] > dis[1])
			{
				m_vCameraPoint[cameraCount] = m_vCameraPoint[i];
				m_vCameraPoint[i]			= set;
				set							= m_vCameraPoint[cameraCount];
			}
		}

		//次のポイントを調べる
		cameraCount++;
	}
}

//初期化
void LoadStageData::Init()
{
	//動的確保
	DynamicAllocation();
	//変数の初期化
	NumericInit();
}
//動的確保
void LoadStageData::DynamicAllocation()
{
	//ブロックの数を取得
	m_iBlockNum			= setData.blockNum;
	//ブロックの数で変数を動的確保
	if (m_iBlockNum != 0)
	{
		m_iBlockType	= new int[m_iBlockNum];
		m_vBlockPos		= new VECTOR[m_iBlockNum];
	}
	else
	{
		m_iBlockType	= nullptr;
		m_vBlockPos		= nullptr;
	}

	//オブジェクトの動的確保=================================
	//エネミーの数を取得
	m_iEnemyType1Num			= setData.enemy1Num;
	//エネミーの数、変数を動的確保
	if (m_iEnemyType1Num != 0)
	{
		m_vEnemyType1Pos		= new VECTOR[m_iEnemyType1Num];
		m_vEnemyType1MovePos	= new VECTOR[m_iEnemyType1Num];
		for (int i = 0; i < 2; i++)
		{
			m_fEnemyType1Rot[i] = new float[m_iEnemyType1Num];
		}
	}
	else
	{
		m_vEnemyType1Pos		= nullptr;
		m_vEnemyType1MovePos	= nullptr;
		for (int i = 0; i < 2; i++)
		{
			m_fEnemyType1Rot[i] = nullptr;
		}
	}

	//アイテムの数を取得
	m_iItemNum		= setData.itemNum;
	//アイテムの数、変数を動的確保
	if (m_iItemNum != 0)
	{
		m_vItemPos = new VECTOR[m_iItemNum];
	}
	else
	{
		m_vItemPos = nullptr;
	}

	//安置の数を取得
	m_iSafetyAreaNum		= setData.safetyAreaNum;
	//安置の数、変数を動的確保
	if (m_iSafetyAreaNum != 0)
	{
		m_vSafetyAreaPos	= new VECTOR[m_iSafetyAreaNum];
	}
	else
	{
		m_vSafetyAreaPos	= nullptr;
	}

	//チェックポイントの数を取得
	m_iCheckPointNum		= setData.checkPointNum;
	//チェックポイントの動的確保
	if (m_iCheckPointNum != 0)
	{
		m_vCheckPointPos	= new VECTOR[m_iCheckPointNum];
		m_fCheckPointRot	= new float[m_iCheckPointNum];
	}
	else
	{
		m_vCheckPointPos	= nullptr;
		m_fCheckPointRot	= nullptr;
	}

	//エネミー通過ポイントの数を取得
	m_iEnemyPointNum		= setData.enemyPointNum;
	if (m_iEnemyPointNum != 0)
	{
		//エネミーの通過ポイント
		m_vEenemyPointPos	= new VECTOR[m_iEnemyPointNum];
	}
	else
	{
		m_vEenemyPointPos	= nullptr;
	}

	//レーザーの数を取得
	m_iLaserNum				= setData.laserNum;
	if (m_iLaserNum != 0)
	{
		//レーザーの要素の動的確保
		m_vLaserPos			= new VECTOR[m_iLaserNum];
		m_fLaserRot			= new float[m_iLaserNum];
	}
	else
	{
		m_vLaserPos			= nullptr;
		m_fLaserRot			= nullptr;
	}
}
//数値の初期化
void LoadStageData::NumericInit()
{
	//カメラの通過ポイント初期化
	m_iCameraPointNum = 0;

	//ブロックの初期化
	if (m_iBlockType != nullptr)
	{
		memset(m_iBlockType, 0, sizeof(int));
		memset(m_vBlockPos, 0, sizeof(VECTOR));
	}

	//エネミーの初期化
	if (m_vEnemyType1Pos != nullptr)
	{
		memset(m_vEnemyType1Pos, 0, sizeof(VECTOR));
		memset(m_vEnemyType1MovePos, 0, sizeof(VECTOR));
		for (int i = 0; i < 2; i++)
		{
			memset(m_fEnemyType1Rot[i], 0, sizeof(float));
		}
	}

	//エネミーの通過ポイントの初期化
	if (m_vEenemyPointPos != nullptr)
	{
		memset(m_vEenemyPointPos, 0, sizeof(VECTOR));
	}

	//アイテムの初期化
	if (m_vItemPos != nullptr)
	{
		memset(m_vItemPos, 0, sizeof(VECTOR));
	}

	//安置の初期化
	if (m_vSafetyAreaPos != nullptr)
	{
		memset(m_vSafetyAreaPos, 0, sizeof(VECTOR));
	}

	//チェックポイントの初期化
	if (m_fCheckPointRot != nullptr)
	{
		memset(m_fCheckPointRot, 0, sizeof(float));
	}

	//レーザーの初期化
	if (m_vLaserPos != nullptr)
	{
		memset(m_vLaserPos, 0, sizeof(VECTOR));
		memset(m_fLaserRot, 0, sizeof(float));
	}
}

//終了処理
void LoadStageData::Fin()
{
	//メモリ開放

	//ブロック関連
	if (m_iBlockType != nullptr)
	{
		delete[] m_iBlockType;
		m_iBlockType = nullptr;
	}
	if (m_vBlockPos != nullptr)
	{
		delete[] m_vBlockPos;
		m_vBlockPos = nullptr;
	}

	//エネミー関連
	if (m_vEnemyType1Pos != nullptr)
	{
		delete[] m_vEnemyType1Pos;
		m_vEnemyType1Pos = nullptr;
	}
	if (m_vEnemyType1MovePos != nullptr)
	{
		delete[] m_vEnemyType1MovePos;
		m_vEnemyType1MovePos = nullptr;
	}
	for (int i = 0; i < 2; i++)
	{
		delete[] m_fEnemyType1Rot[i];
		m_fEnemyType1Rot[i] = nullptr;
	}
	//エネミーの通過ポイント
	if (m_vEenemyPointPos != nullptr)
	{
		delete[] m_vEenemyPointPos;
		m_vEenemyPointPos = nullptr;
	}

	//アイテム
	if (m_vItemPos != nullptr)
	{
		delete[] m_vItemPos;
		m_vItemPos = nullptr;
	}

	//安置
	if (m_vSafetyAreaPos != nullptr)
	{
		delete[] m_vSafetyAreaPos;
		m_vSafetyAreaPos = nullptr;
	}

	//レーザー
	if (m_vLaserPos != nullptr)
	{
		delete[] m_vLaserPos;
		m_vLaserPos = nullptr;
	}
	if (m_fLaserRot != nullptr)
	{
		delete[] m_fLaserRot;
		m_fLaserRot = nullptr;
	}

	//チェックポイント
	if (m_vCheckPointPos != nullptr)
	{
		delete[] m_vCheckPointPos;
		m_vCheckPointPos = nullptr;
	}
	if (m_fCheckPointRot != nullptr)
	{
		delete[] m_fCheckPointRot;
		m_fCheckPointRot = nullptr;
	}
}

void LoadStageData::StageLoad()
{
	//データを読む
	FILE* fp;
	int stageNum = Data::GetSelectStage();
	fopen_s(&fp, STAGE_FILE_PATH[stageNum].c_str(), "rb");
	if (fp != NULL)
	{
		fread(&setData, sizeof(setData), 1, fp);
		fclose(fp);
	}

	//値の初期化(0)
	Init();
	
	//ブロックの読み込み
	BlockLoad();

	//ステージ以外の読み込み
	for (int iZ = 0; iZ < MAP_NUM_Z; iZ++)
	{
		for (int iY = 0; iY < MAP_NUM_Y; iY++)
		{
			for (int iX = 0; iX < MAP_NUM_X; iX++)
			{
				if (setData.typeOdject[iZ][iY][iX] == -1)
				{
					continue;
				}

				ObjectLoad(iX, iY, iZ);
			}
		}
	}
}

//ブロックの読み込み
void LoadStageData::BlockLoad()
{
	for (int iZ = 0; iZ < MAP_NUM_Z; iZ++)
	{
		for (int iY = 0; iY < MAP_NUM_Y; iY++)
		{
			for (int iX = 0; iX < MAP_NUM_X; iX++)
			{
				//ブロックが存在しなかったら実行しない
				if (setData.type[iZ][iY][iX] == -1)
					continue;

				//ブロックの種類を設定
				m_iBlockType[m_iSetNumCount[BLOCK_NORMAL]]			= setData.type[iZ][iY][iX];

				if(m_iBlockType[m_iSetNumCount[BLOCK_NORMAL]]== BLOCK_NORMAL)
				{
					int blockType = 1;

					//交互に色を変えるようにする
					if (iX % 2 == 0)
					{
						blockType *= -1;
					}
					if (iY % 2 == 0)
					{
						blockType *= -1;
					}

					//ブロックの色を交互に変える
					if (blockType == -1)
					{
						m_iBlockType[m_iSetNumCount[BLOCK_NORMAL]]	= BLOCK_4;
					}
				}

				//座標を設定
				m_vBlockPos[m_iSetNumCount[BLOCK_NORMAL]]			= setData.pos[iZ][iY][iX];

				//ブロックを進める
				m_iSetNumCount[BLOCK_NORMAL]++;
			}
		}
	}
}

//プレイヤーとか
void LoadStageData::ObjectLoad(int iX, int iY, int iZ)
{
	//座標の読み込み
	switch (setData.typeOdject[iZ][iY][iX])
	{
	case BLOCK_PL:
	{
		//座標を設定
		m_vPlPos = setData.posOdject[iZ][iY][iX];
		//プレイヤーの向きを設定
		m_vPlRot = setData.playerRot;

		break;
	}

	case BLOCK_ENEMY1:	//エネミー1
	{
		//座標を設定
		VECTOR setPos											= setData.posOdject[iZ][iY][iX];
		m_vEnemyType1Pos[m_iSetNumCount[BLOCK_ENEMY1]]			= setPos;

		//エネミーの移動先を設定
		m_vEnemyType1MovePos[m_iSetNumCount[BLOCK_ENEMY1]]		= setData.enemyPosMovrPoint[iZ][iY][iX];

		//移送先の開店を設定
		for (int i = 0;i < 2;i++)
		{
			m_fEnemyType1Rot[i][m_iSetNumCount[BLOCK_ENEMY1]]	= setData.enemyRot[iZ][iY][iX][i];
		}

		//次に進める
		m_iSetNumCount[BLOCK_ENEMY1]++;

		break;
	}

	case BLOCK_ITEM:	//アイテム
	{
		//座標を設定
		VECTOR setPos							= setData.posOdject[iZ][iY][iX];
		m_vItemPos[m_iSetNumCount[BLOCK_ITEM]]	= setPos;

		//次に進める
		m_iSetNumCount[BLOCK_ITEM]++;

		break;
	}

	case BLOCK_SAFETYAREA:	//安置
	{
		//座標を設定
		VECTOR setPos											= setData.posOdject[iZ][iY][iX];
		m_vSafetyAreaPos[m_iSetNumCount[BLOCK_SAFETYAREA]]		= setPos;

		//少し浮かべて違和感がないようにする
		m_vSafetyAreaPos[m_iSetNumCount[BLOCK_SAFETYAREA]].y	+= SAFETYAREA_UP_HEIGHT;

		//次に進める
		m_iSetNumCount[BLOCK_SAFETYAREA]++;

		break;
	}

	case BLOCK_GOAL:	//ゴール
	{
		//座標を設定
		m_vGoalPos												= setData.posOdject[iZ][iY][iX];

		break;
	}

	case BLOCK_ENEMY_POINT:	//エネミー通過ポイント
	{
		//座標を設定
		VECTOR setPos											= setData.posOdject[iZ][iY][iX];
		m_vEenemyPointPos[m_iSetNumCount[BLOCK_ENEMY_POINT]]	= setPos;

		//次に進める
		m_iSetNumCount[BLOCK_ENEMY_POINT]++;

		break;
	}

	case BLOCK_LASER:	//レーザー
	{
		//座標を設定
		VECTOR setPos								= setData.posOdject[iZ][iY][iX];
		m_vLaserPos[m_iSetNumCount[BLOCK_LASER]]	= setPos;

		//向きを設定
		m_fLaserRot[m_iSetNumCount[BLOCK_LASER]]	= setData.odjectRot[iZ][iY][iX];

		//次に進める
		m_iSetNumCount[BLOCK_LASER]++;

		break;
	}

	case BLOCK_CAMERA_POINT:	//カメラ通過ポイント
	{
		m_vCameraPoint[m_iCameraPointNum] = setData.posOdject[iZ][iY][iX];
		m_iCameraPointNum++;

		break;
	}

	case BLOCK_CHECK_POINT:		//チェックポイント
	{
		//座標を設定
		VECTOR setPos										= setData.posOdject[iZ][iY][iX];
		//ブロック分浮かせる
		setPos.y											+= CHECKPOINT_UP_HEIGHT;
		m_vCheckPointPos[m_iSetNumCount[BLOCK_CHECK_POINT]] = setPos;

		//向きを設定
		m_fCheckPointRot[m_iSetNumCount[BLOCK_CHECK_POINT]] = setData.odjectRot[iZ][iY][iX];

		//次に進める
		m_iSetNumCount[BLOCK_CHECK_POINT]++;

		break;
	}

	default:
		break;
	}
}