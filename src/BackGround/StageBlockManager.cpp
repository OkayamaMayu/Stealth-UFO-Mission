#include"StageBlockManager.h"
#include"../MyLib/MyLib.h"

StageBlockManager::StageBlockManager(){
	//オリジナルのモデルを読み込み
	for (int i = 0; i < StageBlock::BLOCK_TYPE_NUM; i++) {
		m_iHandleOrigin[i] = -1;
	}
}

void StageBlockManager::Init(LoadStageData &data){
	//数の取得
	m_iBlockNum = data.GetBlockNum();
	//ブロック情報の動的確保
	block = new StageBlock[m_iBlockNum];

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
		block[i].Init(type, data.GetBlockPos(i));
	}
}

void StageBlockManager::Load(){
	if (block == nullptr)return;
	//オリジナルのモデルを読み込み

	for (int i = 0;i < StageBlock::BLOCK_TYPE_NUM;i++){
		if (m_iHandleOrigin[i] != -1) continue;
		
		m_iHandleOrigin[i] = MV1LoadModel(BLOCK_MODLE_PATH[i].c_str());
	}

	//ブロックのモデル読み込み
	for (int i = 0;i < m_iBlockNum;i++){
		//空気ブロックなら実行しない
		if (block[i].GetBlockType() == StageBlock::BLOCK_AIR) continue;
		
		block[i].Load(m_iHandleOrigin[block[i].GetBlockType()]);
	}
}

void StageBlockManager::Start(){
	for (int i = 0;i < m_iBlockNum;i++){
		block[i].Start();
	}
}

void StageBlockManager::Step(){
	//スケブロックの処理
	for (int i = 0; i < m_iBlockNum; i++){
		if (block == nullptr)continue;
		block[i].Step();
	}
}

void StageBlockManager::Draw(){
	//ブロックの数を回す
	for (int i = 0; i < m_iBlockNum; i++){
		//ブロックを表示
		if (block == nullptr)continue;
		//空気ブロックは実行しない
		if (block[i].GetBlockType() == StageBlock::BLOCK_AIR)continue;
		block[i].Draw();
	}
}

void StageBlockManager::Fin(){
	//メモリ開放
	if (block == nullptr)return;
	
	delete[] block;
	block = nullptr;
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
	
	//直径にする
	VECTOR checkCameraSize = VScale(cameraSize, 2.0f);
	
	//ブロックのサイズ
	VECTOR blockSize = VGet(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	//直径にする
	VECTOR checkblockSize = VScale(blockSize, 2.0f);
	
	for (int i = 0; i < m_iBlockNum; i++){
		VECTOR blockPos = block[i].GetPos();
		block[i].SetIsDraw(true);
	
		//ブロック/プレイヤー座標とカメラの座標の距離をそれぞれ計算する
		float distanceBlock = Math::GetDistance(blockPos, cameraPos);
		float distancePlayerPos = Math::GetDistance(cameraFocusPos, cameraPos);
	
		//ブロックがプレイヤーの下(地面)にあるか、ブロックがプレイヤーの奥にあった場合は透かせない
		if (cameraFocusPos.y >= blockPos.y || distanceBlock >= distancePlayerPos) continue;
	
		if(!camera.GetUfoFlag()){
			//一定より遠くだと実行しない
			if (camera.GetPlVisionFlag()) continue;
			if (Math::GetDistance(blockPos, cameraPos) > CAMERA_LEMGTH) continue;
			
			block[i].SetIsDraw(false);
		}
		else{
			//一定より遠くだと実行しない
			if (camera.GetPlVisionFlag()) continue;
			if (Math::GetDistance(blockPos, cameraPos) > CAMERA_LEMGTH + CAMERA_LEMGTH_UFO) continue;
			
			block[i].SetIsDraw(false);
		}
	}
}