#include"LaserManager.h"

void LaserManager::Init(LoadStageData& data)
{
	//レーザーの総数を取得
	m_iLaserNum		= data.GetLaserNum();
	laser			= new Laser[m_iLaserNum];

	for (int i = 0; i < m_iLaserNum; i++)
	{
		VECTOR pos	= data.GetLaserPos(i);
		float rot	= data.GetLaserRot(i);
		laser[i].Init(pos, rot);
	}
}

void LaserManager::Load()
{
	int oriHandle = MV1LoadModel(MODEL_PATH.c_str());

	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
		laser[i].Load(oriHandle);
	}
}

void LaserManager::Start()
{
	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
			laser[i].Start();
	}
}

void LaserManager::Step()
{
	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
		laser[i].Step();
	}
}

void LaserManager::Draw()
{
	for (int i = 0; i < m_iLaserNum; i++)
	{
		if (laser != nullptr)
		laser[i].Draw();
	}
}

void LaserManager::Fin()
{
	if (laser != nullptr)
	{
		for (int i = 0; i < m_iLaserNum; i++)
		{
			laser[i].Fin();
		}

		//メモリの開放
		delete[] laser;
		laser = nullptr;
	}
}

//レーザーとプレイヤー
void LaserManager::CheckLaserToPlayer(VECTOR playerPos, EnemyManager& enemyManager)
{
	//レーザーとプレイヤー
	bool laserHitFlag = false;

	//プレイヤー
	VECTOR checkPlayerPos = playerPos;
	checkPlayerPos.y += PLAYER_SIZE.y / 2;
	VECTOR playerSize = PLAYER_SIZE;

	//直径にする
	VECTOR checkPlayerSize = VScale(playerSize, 2.0f);
	checkPlayerSize.y /= 2;

	bool posSetFlag = false;
	for (int i = 0; i < m_iLaserNum; i++)
	{
		//現在のレーザーの長さ以上に距離があったら実行しない
		if (Math::GetDistance(checkPlayerPos, laser[i].GetPos()) >= laser[i].GetLaserLength()) continue;
	
		//レーザーの情報を設定
		VECTOR laserSize = MODEL_SCALE_V;
		//基準となる座標と伸ばした後の座標
		VECTOR laserPos[2] = { laser[i].GetPos(),laser[i].GetPos() };
		VECTOR laserRot = laser[i].GetRot();
		float laserLength = laser[i].GetLaserLength();

		VECTOR laserVec = {};
		
		//伸ばした長さ
		float laserMovePoint = 0.0f;

		while (true)
		{
			//伸ばす
			Math::MatchSpecifiedNum(laserMovePoint, laserLength + 1.0f, 1.0f);
			laserVec = Math::GetMoveVec(laserRot, laserMovePoint);
			laserPos[1] = VAdd(laserPos[0], laserVec);

			//レーザーが一定まで伸びた
			if (laserMovePoint > laserLength) break;
			
			//レーザーがプレイヤーに当たっていなかったら
			if (!Collision::Rect3D(
				laserPos[1], laserSize,
				checkPlayerPos, checkPlayerSize)) continue;

			Data::SetFoundFlag(true);

			posSetFlag = true;

			VECTOR hitPos = checkPlayerPos;
			hitPos.y = laserPos[1].y;
			hitPos = laserPos[1];
			CEffekseerCtrl::Request(EFFECT_LASER, hitPos, false);

			break;
		}
		
		if (posSetFlag) break;
	}


	//プレイヤーとエネミー：プレイヤーが反応していたら
	if (!posSetFlag)
	{
		laserHitFlag = false;
		return;
	}

	int enemyMaxNum = enemyManager.GetEnemyType1MaxNum();
	for (int i = 0; i < enemyMaxNum; i++)
	{
		//一定距離までは判定しない
		if (Math::GetDistance(checkPlayerPos, enemyManager.GetEnemyType1(i).GetPos()) >= ENEMY_FOUNDSIZE * 2) continue;

		//エネミーを発見状態にする
		EnemyType1& enemy = enemyManager.GetEnemyType1(i);
		enemy.PlayerDiscovery(false);
	}

	//プレイヤーが発見されたときに回数を加算
	if (laserHitFlag) return;
	laserHitFlag = true;
	Data::GetEvaluationData().m_iFoundNum++;
	Data::UpFoundGauge(20);
	
}
