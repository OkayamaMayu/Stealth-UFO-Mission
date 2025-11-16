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

	//座標の適用
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
bool BlockManager::RequestBlock(){
	//配置ゲージが満たされていなかったら終了
	if (m_BlockSetFlag && Data::GetBlockGauge() < SET_BLOCK_POINT)return false;
	//ブロック配置可能になっていなかったら終了
	if(!m_BlockSetFlag)return false;

	//配置キーが押されなかったら終了
	if (!InputManager::CheckPush(INPUT_BLOCK_PLACE))return false;

	for (int i = 0; i < BLOCK_MAX_NUM; i++){
		//配置されなかったら終了
		if (!block[i].RequestBlock(m_vSetBlockPos))continue;

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
	return false;
}

//配置ブロック位置の設定
void BlockManager::CheckSetBlockPos(CameraManager& cameraManager, StageBlockManager& stage, bool setMode) {
	//ブロックを配置できないようにする
	m_BlockSetFlag = false;

	//配置カメラになっていなければ終了
	if (!cameraManager.GetPlVisionFlag())return;

	//配置可能ブロックの数を計算
	int blockNum = (int)(Data::GetBlockGauge() / SET_BLOCK_POINT);
	if (!setMode)return;
	if (blockNum <= 0)return;

	//画面の中心座標
	VECTOR screenPos = { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f };
	//スクリーン座標をワールド座標にする
	screenPos = ConvScreenPosToWorldPos(screenPos);

	//カメラの視点座標
	VECTOR cameraPos = cameraManager.GetCameraForcus();

	//ブロックのサイズを取得
	VECTOR blockSize = Vector::MakeVec(BLOCK_SIZE);

	//配置予定座標
	VECTOR createPos = screenPos;
	createPos.y -= blockSize.y;

	//カメラの座標から視点の座標までのベクトルを生成
	VECTOR v = Vector::VecCreate(cameraPos, screenPos);
	//現在の長さ分まで伸ばす
	v = Vector::VecScale(v, 100.0f);
	//座標を伸ばす
	VECTOR lineEndPos = VAdd(createPos, v);

	LineSegment lineSegment = {};
	lineSegment.startPos = screenPos;
	lineSegment.endPos = lineEndPos;

	for (int i = 0; i < MAP_AREA_NUM; i++) {
		AABB areaCollison = stage.GetStageBlockArea(i).GetCollision().GetCollision();
		//エリアに当たっていなければ実行しない
		if (!Collision::IsCollidingAABBToLineSegment(areaCollison, lineSegment))continue;
		for (auto stageBlock : stage.GetStageBlock(i)) {

			//実行しない
			if (stageBlock.GetBlockType() == StageBlock::BLOCK_AIR)continue;
			if (stageBlock.GetBlockType() == StageBlock::BLOCK_WALL)continue;
			//ステージブロックを取得
			VECTOR stageBlockPos = stageBlock.GetPos();

			//既に配置されたブロックとの判定
			for (int b = 0; b < BLOCK_MAX_NUM; b++) {
				//配置されていなかったら実行しない
				if (!block[b].GetIsUse())continue;

				//ステージブロックの情報を入手
				AABB aabb = block[b].GetCollision().GetCollision();
				//配置ブロックに当たっていなかったら実行しない
				if (!Collision::IsCollidingAABBToLineSegment(aabb, lineSegment))continue;

				//ブロックの上辺に設定
				createPos = aabb.centerPos;
				createPos.y = createPos.y + BLOCK_SIZE * 2;

				//配置位置を設定
				m_vSetBlockPos = createPos;
				m_BlockSetFlag = true;
				//終了
				return;
			}

			//ステージブロックの情報を入手
			AABB aabb = stageBlock.GetCollision()->GetCollision();
			//当たっていなかったら
			if (!Collision::IsCollidingAABBToLineSegment(aabb, lineSegment))continue;

			//床にあげる
			createPos = aabb.centerPos;
			createPos.y += blockSize.y * 2;

			//配置位置を設定
			m_vSetBlockPos = createPos;
			m_BlockSetFlag = true;
			//終了
			return;
		}
	}
}