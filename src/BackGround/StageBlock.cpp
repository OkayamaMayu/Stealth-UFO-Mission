#include"StageBlock.h"

void StageBlock::Init() {
	CModel::Init();
}

void StageBlock::Init(SET_BLOCK blockType, VECTOR setPos) {
	Init();

	//ブロックタイプを設定
	m_Type = blockType;
	//座標を取得
	m_vPos = m_vNextPos = setPos;
	//ハンドルを初期化
	m_iHandle = -1;
	//描画フラグを初期化
	m_IsDraw = true;
	//透明度を初期化
	m_fAlpha = 1.0f;
	//サイズを設定
	m_vSize = VGet(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	m_Collision.SetKind(KIND_STAGE);
	//構造体の設定
	AABB setCollision = {};
	//サイズを設定
	setCollision.size = m_vSize;
	//中心座標を設定
	setCollision.centerPos = m_vPos;
	//情報を登録
	m_Collision.SetCollision(setCollision);
	CollisionManager::GetInstance()->RegisterCollision(&m_Collision);
}

void StageBlock::Load(int modelHandle) {
	//ブロックのタイプ情報からモデルをコピー
	m_iHandle = MV1DuplicateModel(modelHandle);
}

void StageBlock::Start() {
	//モデルのスケールを設定
	MV1SetScale(m_iHandle, MODEL_SCALE_V);
	//モデルの座標を設定
	MV1SetPosition(m_iHandle, m_vPos);
}

void StageBlock::Step() {
	//透明度を設定
	float nextAlpha = m_fAlpha;

	//透明度が指定のどちらかと一致していたら実行しない
	if (m_IsDraw && nextAlpha == 1.0f)return;
	if (!m_IsDraw && nextAlpha == SEMITRANSPARENT_ALPHA)return;
		
	if (m_IsDraw){
		//透過させない
		Math::MatchSpecifiedNum(nextAlpha, 1.0f, SEMITRANSPARENT_ALPHA_SPEED);
	}
	else{
		//透過する
		Math::MatchSpecifiedNum(nextAlpha, SEMITRANSPARENT_ALPHA, SEMITRANSPARENT_ALPHA_SPEED);
	}

	//透明度を適用する
	m_fAlpha = nextAlpha;
	MV1SetOpacityRate(m_iHandle, m_fAlpha);
}

void StageBlock::Draw() {
	CModel::Draw();
}

void StageBlock::Fin() {
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}