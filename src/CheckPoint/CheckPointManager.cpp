#include"CheckPointManager.h"

void CheckPointManager::Init(VECTOR startPos, float startRot, LoadStageData& setData)
{
	//初期位置
	m_ReSpawnInfo.pos	= startPos;
	m_ReSpawnInfo.rot = startRot;

	//チェックポイントの数
	m_iCheckPointNum = setData.GetCheckPointNum();

	if (m_CheckPoint == nullptr)
	{
		m_CheckPoint = new CheckPoint[m_iCheckPointNum];
	}

	//kindを個別で分ける変数
	int kindNum = 0;
	for (int i = 0; i < m_iCheckPointNum; i++){
		if (m_CheckPoint == nullptr)continue;

		m_CheckPoint[i].Init(setData.GetCheckPointPos(i), setData.GetCheckPointRot(i));

		//kindを設定する
		CollisionSphere setCollision = m_CheckPoint[i].GetCollision();
		setCollision.SetKind(KIND_CHECKPOINT + kindNum);
		m_CheckPoint[i].SetCollision(setCollision);
		//コリジョンを登録
		m_CheckPoint[i].RegisterCollision();

		//次の番号へ変更
		kindNum++;
	}
}

void CheckPointManager::Load()
{
	int oriHandle = MV1LoadModel(MODEL_PATH.c_str());

	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)continue;

		m_CheckPoint[i].Load(oriHandle);
	}
}

void CheckPointManager::Start()
{
	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)continue;

		m_CheckPoint[i].Start();
	}
}

void CheckPointManager::Step(){
	for (int i = 0; i < m_iCheckPointNum; i++){
		if (m_CheckPoint == nullptr)continue;

		m_CheckPoint[i].Step();

		//リスポーンが起動していなければ実行しない
		if (!m_CheckPoint[i].GetCheckPointFlag())continue;

		//リスポーン情報を設定
		m_ReSpawnInfo = m_CheckPoint[i].GetReSpawnInfo();
		m_CheckPoint[i].SetCheckPointFlag(false);
	}
}

void CheckPointManager::Draw()
{
	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)continue;

		m_CheckPoint[i].Draw();
	}
}

void CheckPointManager::Fin()
{
	if (Data::GetReSpawnFlag())
		return;

	for (int i = 0; i < m_iCheckPointNum; i++)
	{
		if (m_CheckPoint == nullptr)continue;

		m_CheckPoint[i].Fin();
	}

	if (m_CheckPoint != nullptr)
	{
		delete[] m_CheckPoint;
		m_CheckPoint = nullptr;
	}
}

//チェックポイントを距離で透かす
void CheckPointManager::CheckCheckPointToCamera(VECTOR playerPos, VECTOR cameraPos, float cameraLemgth){
	for (int i = 0; i < m_iCheckPointNum; i++){
		//チェックポイントの座標
		VECTOR checkPointPos = m_CheckPoint[i].GetPos();
		//半透明フラグをきる
		m_CheckPoint[i].SetSemitransparentFlag(false);

		//カメラとチェックポイントの距離
		float checkPointToCameraDistance = Math::GetDistance(checkPointPos, cameraPos);

		//カメラと一定以上離れていたら終了
		if (checkPointToCameraDistance > cameraLemgth) continue;
		//チェックポイントとプレイヤーのカメラまでの距離を見て
		//チェックポイント方が遠い場合は終了
		if (checkPointToCameraDistance > Math::GetDistance(playerPos, cameraPos)) continue;
			
		//半透明にする
		m_CheckPoint[i].SetSemitransparentFlag(true);
	}
}