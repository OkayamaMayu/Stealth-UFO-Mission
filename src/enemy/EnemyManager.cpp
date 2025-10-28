#include"DxLib.h"
#include"EnemyManager.h"
#include"../MyLib/MyLib.h"

void EnemyManager::Init(LoadStageData&data)
{
	//エネミー１の総数
	m_iEnemyType1Num	= data.GetEnemyType1Num();

	//エネミー総数に数を追加
	m_iEnemyMaxNum		= m_iEnemyType1Num;
	m_EnemyType1		= nullptr;
	if (m_EnemyType1 == nullptr)
	{
		m_EnemyType1	= new EnemyType1[m_iEnemyType1Num];
	}
	
	//エネミーを初期化
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;
	
		m_EnemyType1[i].Init(
			data.GetEnemyType1SpawnPos(i),
			data.GetEnemyType1MovePos(i),
			data.GetEnemyType1Rot(i,0),
			data.GetEnemyType1Rot(i,1));
	}

	//エネミー通過ポイントの数を取得
	m_iEnemyPointNum		= data.GetEnemyPointNum();
	m_vEnemyPointPos		= nullptr;
	if (m_vEnemyPointPos == nullptr)
	{
		m_vEnemyPointPos	= new VECTOR[m_iEnemyPointNum];
	}
	for (int i = 0; i < m_iEnemyPointNum; i++)
	{
		m_vEnemyPointPos[i] = data.GetEnemyPointPos(i);
	}

	m_fTargetImageRot		= 0.0f;
}

void EnemyManager::Load()
{
	m_iLookOnImageHandle = LoadGraph(ENEMY_TARGET_IMAGE_PATH.c_str());

	//オリジナルモデルを読み込み
	int m_iEnemyType1Handel = MV1LoadModel(ENEMY_MODEL_PATH[ENEMY_TYPE_1].c_str());
	int m_iEnemyType2Handel = MV1LoadModel(ENEMY_MODEL_PATH[ENEMY_TYPE_2].c_str());
	//コピー
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;

		m_EnemyType1[i].Load(m_iEnemyType1Handel);
	}
}

void EnemyManager::Start()
{
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;

		m_EnemyType1[i].Start();
	}
}

void EnemyManager::Step(Player& pl, ItemManager& itemMana, BackGround& block, bool gameOverFlag, bool clearFlag)
{
	bool soundFlag = false;
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;

		m_EnemyType1[i].Step(pl, itemMana, block, gameOverFlag, clearFlag);

		if (m_EnemyType1[i].GetFoundType() == EnemyBase::FOUND_PLAYER)
		{
			soundFlag = true;
		}
	}

	//ターゲット画像の回転
	m_fTargetImageRot += TARGET_ROT_SPEED;
	Math::RotReset(m_fTargetImageRot);

	//発見状態で流すループサウンド
	if (soundFlag)
	{
		Sound::Play(SE_LOOP_ENEMY_FOUND);
	}
	else
	{
		Sound::StopSound(SE_LOOP_ENEMY_FOUND);
	}
}


void EnemyManager::Updata()
{
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;
		m_EnemyType1[i].Updata();
	}
}

void EnemyManager::Draw()
{
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;

		if(m_EnemyType1[i].GetIsUse())
		{
			m_EnemyType1[i].Draw();

			//ロックオンの画像を表示
			if (m_EnemyType1[i].GetLookOnFlag())
			{
				VECTOR pos = m_EnemyType1[i].GetPos();
				pos.y += m_EnemyType1[i].GetSize().y / 2;
				DrawBillboard3D(pos, BILLBOARD_POS, BILLBOARD_POS, BILLBOARD_SIZE, m_fTargetImageRot, m_iLookOnImageHandle, true);
			}
		}
	}
}

void EnemyManager::Fin()
{
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
		{
			continue;
		}

		m_EnemyType1[i].Fin();
	}

	//メモリ開放
	if(m_EnemyType1!=nullptr)
	{
		delete[] m_EnemyType1;
		m_EnemyType1 = nullptr;
	}
}

//--------------------------------------------------------

VECTOR EnemyManager::HitWall(int ID)
{
	VECTOR nextPoint = {};
	for (int i = 0; i < m_iEnemyPointNum; i++)
	{
		//新しいポイントと現在のポイントを比べて距離が近いほうにする
		if (Math::GetDistance(m_EnemyType1[ID].GetPos(), m_vEnemyPointPos[i]) < Math::GetDistance(m_EnemyType1[ID].GetPos(), nextPoint))
		{
			//次の座標に通過ポイントを入れる
			nextPoint = m_vEnemyPointPos[i];
		}
	}

	return nextPoint;
}

//ジャンプ：該当エネミー
void EnemyManager::HitJump(int ID)
{
	m_EnemyType1[ID].SetGravity(ENEMY_JUMP_POWER);
}

void EnemyManager::ItemTarget(ItemManager& itemMana, Player& pl, float rot, VECTOR vFocus, bool throwFlag)
{
	//2D画面の中央座標を3D座標に変更させる
	//zはカメラとの距離
	VECTOR screenPos = { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,RANGE_CAMERA };
	VECTOR centerPos = ConvScreenPosToWorldPos(screenPos);

	//プレイヤーの座標。中心座標
	VECTOR plPos = pl.GetPos();
	plPos.y += PLAYER_SIZE.y / 2;

	//ターゲットを消すフラグ
	bool tageRFlag = true;	

	int itemMaxNum = itemMana.GetItemMaxNum();
	for (int i = 0; i < itemMaxNum; i++)
	{
		//当たり判定をとらないなら
		if (!itemMana.GetItem(i).GetIsUse())
			continue;
		Item& item = itemMana.GetItem(i);

		//キャッチされているか投げられていたらターゲットを消させない
		if (item.GetCatchFlag()||item.GetThrow())
			tageRFlag = false;
		else
			continue;

		VECTOR	eneTrgetPos		= {};		//ターゲットにするエネミーの座標
		bool	setTarget		= false;	//ターゲットがいるうえでアイテムが投げられたフラグ
		bool	eneTrgetFlag	= false;	//ターゲットが選択されているか
		int		eneTrgetNum		= -1;		//ターゲットになるエネミーのID
		for (int eneNum = 0; eneNum < m_iEnemyType1Num; eneNum++)
		{
			//エネミーの座標を頭付近に調節
			VECTOR enePos = m_EnemyType1[eneNum].GetPos();
			enePos.y += (m_EnemyType1[eneNum].GetSize().y / ENEMY_SIZE_RATIO) * ENEMY_SIZE_SCALE;

			//向きを逆にする
			float checkRot = rot + Math::ChangeDegToRad(180);
			//ロックオンフラグの初期化
			m_EnemyType1[eneNum].SetLookOnFlag(false);

			//プレイヤー視界に入っていたら
			if (Math::CheckVision(centerPos, vFocus, checkRot, enePos, VIEW_RANGE_RANGE, FIELD_OF_VIEW))
			{
				if(!eneTrgetFlag)
				{
					//ターゲットON
					m_EnemyType1[eneNum].SetLookOnFlag(true);
					eneTrgetFlag	= true;
					eneTrgetPos		= enePos;
					eneTrgetNum		= eneNum;
				}
				else
				{
					//比較して中央に近いほうに変更する
					float eneToPlDis[2]		= {};
					VECTOR checkEnePos[2]	= { enePos ,eneTrgetPos };

					for (int i_ = 0; i_ < 2; i_++)
					{		
						float	fSp			= Math::GetDistance(centerPos, enePos);				//画面の中央とエネミーの座標の距離
						VECTOR	plRot		= pl.GetRot();										//プレイヤーの角度（進行方向）
						VECTOR	vSp			= Math::GetMoveVec(plRot, fSp);						//進行方向に画面とエネミーの距離分伸ばす
						VECTOR	checkCenter	= VAdd(centerPos, vSp);								//調べる用の画面中央座標にする
						eneToPlDis[i_]		= Math::GetDistance(checkCenter, checkEnePos[i_]);	//二つの座標の距離を調べる
					}

					//新しい座標と保存している座標の距離を比べて更新する
					if (eneToPlDis[0] < eneToPlDis[1])
					{
						//ターゲットON
						m_EnemyType1[eneNum].SetLookOnFlag(true);
						eneTrgetFlag		= true;
						eneTrgetPos			= enePos;
						//保存していたターゲットをOFF
						m_EnemyType1[eneTrgetNum].SetLookOnFlag(false);
						eneTrgetNum			= eneNum;
					}
				}
			}

			//ターゲットになっているかつ投げられている
			if (m_EnemyType1[eneNum].GetLookOnFlag() && throwFlag)
			{
				//投げられる先を設定
				item.SetTarget(enePos);

				//ステータス継続時間を設定
				m_EnemyType1[eneNum].SetCurrentStateTime(STUN_TIME);

				//ターゲットかつ投げられたフラグをON
				setTarget = true;

				break;
			}
		}

		//投げつけられたら終了
		if (setTarget)
		{
			break;
		}
	}

	//ターゲットを消すフラグがONなら
	if (tageRFlag)
	{
		for (int eneNum = 0; eneNum < m_iEnemyType1Num; eneNum++)
		{
			m_EnemyType1[eneNum].SetLookOnFlag(false);
		}
	}
}

//エフェクトを停止する
void EnemyManager::StopEffect()
{
	for (int i = 0; i < m_iEnemyType1Num; i++)
	{
		if (m_EnemyType1 == nullptr)
			continue;

		//エフェクトを停止する
		m_EnemyType1[i].StopEffect();
	}
}