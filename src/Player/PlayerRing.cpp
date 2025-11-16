#include "PlayerRing.h"
#include "../MyLib/MyLib.h"
#include "../Block/BlockManager.h"

void PlayerRing::Init(VECTOR setPos, VECTOR setRot)
{
	Init();
	
	//座標回転の設定
	m_vPos		= m_vNextPos = setPos;
	m_vRot		= setRot;
	m_vRot.y	+= Math::ChangeDegToRad(180.0f);

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	m_Collision.SetKind(KIND_PLAYER_RING);
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase* hitCollision) {Hit(hitCollision); });
	CollisionManager::GetInstance()->RegisterCollision(&m_Collision);
}

void PlayerRing::Init() {
	CModel::Init();

	MV1SetScale(m_iDrawHandle[0], MODEL_SCALE_V);
	MV1SetScale(m_iDrawHandle[1], MODEL_SCALE_V);

	m_DrawRingFlag = false;
}

void PlayerRing::Load()
{
	for (int i = 0; i < MODEL_TYPE_NUM; i++)
	{
		m_iDrawHandle[i] = MV1LoadModel(MODEL_PATH[i].c_str());
	}
}

void PlayerRing::Start()
{
	//初期表示モデルはノーマルにする
	m_iHandle = m_iDrawHandle[NORMAL];

	MV1SetPosition(m_iHandle, m_vPos);
	MV1SetRotationXYZ(m_iHandle, m_vRot);
	MV1SetScale(m_iHandle, MODEL_SCALE_V);
}

void PlayerRing::Step()
{
	//リングを回転
	m_vRot.y += RING_ROT_SPEED;

	m_DrawRingFlag = false;

	m_vNextPos = m_vPos;
	UpdateCollision();
}

void PlayerRing::Update()
{
	m_vPos = m_vNextPos;

	MV1SetRotationXYZ(m_iHandle, m_vRot);

	//情報の更新
	for (int i = 0; i < MODEL_TYPE_NUM; i++)
	{
		MV1SetPosition(m_iDrawHandle[i], MV1GetPosition(m_iHandle));
		MV1SetRotationXYZ(m_iDrawHandle[i], MV1GetRotationXYZ(m_iHandle));
		MV1SetScale(m_iDrawHandle[i], MODEL_SCALE_V);
	}
}

void PlayerRing::Draw(bool drawFrg){
	Update();

	//発見されているかでモデルを分ける
	if (Data::GetFoundFlag()){
		m_iHandle = m_iDrawHandle[ENEMY_FOUND];
	}
	else{
		m_iHandle = m_iDrawHandle[NORMAL];
	}

	//リング
	MV1SetPosition(m_iHandle, m_vPos);	
	if (!m_DrawRingFlag || drawFrg)return;
	//リングを描画
	MV1DrawModel(m_iHandle);
}

void PlayerRing::Fin()
{
	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

//コリジョン情報の更新
void PlayerRing::UpdateCollision() {
	LineSegment setCollision = m_Collision.GetCollision();
	//座標を設定
	setCollision.startPos = m_vPos;
	//最大値を設定
	VECTOR endPos = m_vPos;
	endPos.y -= MAX_LENGTH;
	setCollision.endPos = endPos;
	//情報を更新
	m_Collision.SetCollision(setCollision);
}

//当たった処理
void PlayerRing::Hit(CollisionBase* hitCollision) {
	//ブロック類でなければ実行しない
	if (hitCollision->GetKind() != KIND_BLOCK)return;
	//配置ブロックが配置されていなければ実行しない
	Block* block = static_cast<Block*>(hitCollision->GetOwner());
	if (!block->GetIsUse())return;

	//ブロックの情報を受け取る
	CollisionAABB* blockCollision = static_cast<CollisionAABB*>(hitCollision);
	VECTOR blockPos = blockCollision->GetCollision().centerPos;
	VECTOR blockSize = blockCollision->GetCollision().size;

	//ブロックの上辺
	m_vNextPos.y = blockPos.y + blockSize.y+0.5f;

	m_DrawRingFlag = true;
}