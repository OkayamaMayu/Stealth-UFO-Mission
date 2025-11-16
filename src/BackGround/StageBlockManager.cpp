#include"StageBlockManager.h"
#include"../MyLib/MyLib.h"
#include <numeric>

StageBlockManager::StageBlockManager(){
	//オリジナルのモデルを読み込み
	for (int i = 0; i < StageBlock::BLOCK_TYPE_NUM; i++) {
		m_iHandleOrigin[i] = -1;
	}
}

void StageBlockManager::Init(LoadStageData &data){
	//区画サイズを計算
	VECTOR halfDelimiterSize = VScale(MAP_AREA_SIZE, 0.5f);
	int areaNum = 0;
	for (int areaZ = 0; areaZ < MAP_AREA_NUM_Z; areaZ++) {
		for (int areaX = 0; areaX < MAP_AREA_NUM_X; areaX++) {
			//最小座標を設定
			VECTOR delimiterPos = {
				-MAP_AREA_SIZE.x * areaX,
				MAP_MIN_Y,
				-MAP_AREA_SIZE.z * areaZ
			};

			//コリジョン情報の設定
			AABB collision = {};
			collision.centerPos = VAdd(delimiterPos, halfDelimiterSize);
			collision.size = halfDelimiterSize;
			m_StageBlockArea[areaNum].Init(collision);
			areaNum++;
		}
	}

	//数の取得
	m_iBlockNum = data.GetBlockNum();

	for (int i = 0;i < m_iBlockNum;i++){
		//ブロックタイプを取得
		//データからブロックタイプを変換
		StageBlock::SET_BLOCK type = StageBlock::BLOCK_NORMAL_1;
		switch (data.GetBlockType(i)){
			case data.BLOCK_NORMAL:	//通常ブロック
				type = StageBlock::BLOCK_NORMAL_1;
				break;

			case data.BLOCK_4:		//通常ブロック追加
				type = StageBlock::BLOCK_NORMAL_2;
				break;

			case data.BLOCK_AIR:	//空気ブロック
				type = StageBlock::BLOCK_AIR;
				break;

			case data.BLOCK_WALL:	//壁
				type = StageBlock::BLOCK_WALL;
				break;

		default:
			break;
		}

		//ブロックの初期化
		StageBlock stageBlock;
		stageBlock.Init(type, data.GetBlockPos(i));
		//ブロック情報を受け取る
		AABB blockCollison = stageBlock.GetCollision().GetCollision();

		for (int areaID = 0; areaID < MAP_AREA_NUM; areaID++) {
			//エリア情報を受け取る
			AABB area = m_StageBlockArea[areaID].GetCollision().GetCollision();
			//配属エリアを決める
			//当たっていなかったら実行しない
			if (!Collision::IsCollidingAABBToAABB(area, blockCollison))continue;

			//ブロックを格納
			m_StageBlockArea[areaID].SetStageBlock(stageBlock);
		}
	}
}

void StageBlockManager::Load(){
	//オリジナルのモデルを読み込み
	for (int i = 0;i < StageBlock::BLOCK_TYPE_NUM;i++){
		if (m_iHandleOrigin[i] != -1) continue;
		
		m_iHandleOrigin[i] = MV1LoadModel(BLOCK_MODLE_PATH[i].c_str());
	}

	for (int i = 0; i < MAP_AREA_NUM; i++) {
		m_StageBlockArea[i].Load(m_iHandleOrigin);
	}
}

void StageBlockManager::Start(){
	for (int i = 0; i < MAP_AREA_NUM; i++) {
		m_StageBlockArea[i].Start();
	}
}

void StageBlockManager::Step(){
	for (int i = 0; i < MAP_AREA_NUM; i++) {
		m_StageBlockArea[i].Step();
	}
}

void StageBlockManager::Draw(){
	for (int i = 0; i < MAP_AREA_NUM; i++) {
		m_StageBlockArea[i].Draw();
	}
}

void StageBlockManager::Fin(){
	for (int i = 0; i < MAP_AREA_NUM; i++) {
		m_StageBlockArea[i].Fin();
	}
}

//ブロックを距離で透かす
void StageBlockManager::CheckStageBlockToCamera(CameraManager& cameraManager) {
	//プレイカメラを取得
	PlayCamera& camera = cameraManager.GetPlayCamera();

	//カメラ情報
	VECTOR cameraPos = camera.GetPos();
	VECTOR cameraSize = { 1.0f,1.0f,1.0f };	
	//フォーカス（プレイヤーの位置）
	VECTOR cameraFocusPos = camera.GetForcus();
	cameraFocusPos.y += -FORCUS_OFFSET_Y - camera.GetForcusF();
	//カメラの座標から視点までの情報
	LineSegment cameraLine = {};
	cameraLine.startPos = cameraPos;
	cameraLine.startPos = cameraFocusPos;
	
	for (int i = 0; i < MAP_AREA_NUM; i++) {
		//現在のエリアにカメラがいるか調べる
		//エリアに当たっていなければ実行しない 
		AABB areaCollison = m_StageBlockArea[i].GetCollision().GetCollision();
		if (!Collision::IsCollidingAABBToLineSegment(areaCollison, cameraLine))continue;

		//所属しているブロックとカメラを調べる
		for (auto& stageBlock : m_StageBlockArea[i].GetAraeBlock()) {
			VECTOR blockPos = stageBlock.GetPos();
			stageBlock.SetIsDraw(true);

			//ブロック/プレイヤー座標とカメラの座標の距離をそれぞれ計算する
			float distanceBlock = Math::GetDistance(blockPos, cameraPos);
			float distancePlayerPos = Math::GetDistance(cameraFocusPos, cameraPos);

			//ブロックがプレイヤーの下(地面)にあるか、ブロックがプレイヤーの奥にあった場合は透かせない
			if (cameraFocusPos.y >= blockPos.y || distanceBlock >= distancePlayerPos) continue;

			if (!camera.GetUfoFlag()) {
				//一定より遠くだと実行しない
				if (camera.GetPlVisionFlag()) continue;
				if (Math::GetDistance(blockPos, cameraPos) > CAMERA_LEMGTH) continue;

				stageBlock.SetIsDraw(false);
			}
			else {
				//一定より遠くだと実行しない
				if (camera.GetPlVisionFlag()) continue;
				if (Math::GetDistance(blockPos, cameraPos) > CAMERA_LEMGTH + CAMERA_LEMGTH_UFO) continue;

				stageBlock.SetIsDraw(false);
			}
		}
	}
}
