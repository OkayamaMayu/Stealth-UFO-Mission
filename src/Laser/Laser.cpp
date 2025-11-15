#include"Laser.h"
#include"../MyLib/MyLib.h"

void Laser::Init(VECTOR vPos, float fRot)
{
	Init();

	float setRot = fRot;
	VECTOR rot = { 0.0f,setRot ,0.0f };
	m_vRot = rot;

	VECTOR move = Math::GetMoveVec(m_vRot,0.1f);
	m_vPos = vPos;
	m_vPos = m_vNextPos = VAdd(m_vPos, move);
	
	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase* hitCollision) {Hit(hitCollision); });
}

void Laser::Init()
{
	m_vScale			= Vector::MakeVec(MODEL_SCALE);
	m_fLaserLength[0]	= m_fLaserLength[1] = LASER_MAX_LENGTH;
	m_OldPlayerHitFlag	= false;
	m_PlayerHitFlag		= false;
	m_BlockHitFlag		= false;
}

void Laser::Load(int originHandle)
{
	//ブロックをコピー
	m_iHandle = MV1DuplicateModel(originHandle);
}

void Laser::Start()
{
	//大きさを変える
	MV1SetScale(m_iHandle, m_vScale);

	//情報の更新
	Update();
	//構造体の設定
	UpdateCollision();

	//エフェクトの再生
	m_iEffectHandle = CEffekseerCtrl::Request(EFFECT_LASER, m_vNextPos, true);
}

void Laser::Step(){
	//長さが変更されていたらエフェクトの位置を変更する
	if (m_fLaserLength[0] != m_fLaserLength[1]){
		CEffekseerCtrl::SetPosition(m_iEffectHandle, m_vNextPos);
	}

	//ブロックに当たっていなかったら長さを更新する
	if (!m_BlockHitFlag)m_fLaserLength[0] = LASER_MAX_LENGTH;

	//現在の長さを保存する
	m_fLaserLength[1] = m_fLaserLength[0];
	//構造体の更新
	UpdateCollision();

	if (!m_OldPlayerHitFlag && m_PlayerHitFlag){
		//プレイヤーが発見されたときに回数を加算
		Data::GetEvaluationData().m_iFoundNum++;
		Data::UpFoundGauge(20);
	}

	//フラグを更新
	m_OldPlayerHitFlag = m_PlayerHitFlag;
	m_PlayerHitFlag = false;
	m_BlockHitFlag = false;
}

void Laser::Draw()
{
	DrawLine3D(m_vPos, m_vNextPos, COLOR[YELLOW]);

	if (m_iHandle == -1)return;
	MV1DrawModel(m_iHandle);
}

void Laser::Fin()
{
	CModel::Fin();
	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

//コリジョン情報の更新
void Laser::UpdateCollision() {
	LineSegment setCollision = m_Collision.GetCollision();
	//座標を設定
	setCollision.startPos = m_vPos;
	VECTOR move = Math::GetMoveVec(m_vRot, m_fLaserLength[0]);
	m_vNextPos = VAdd(m_vPos, move);
	setCollision.endPos = m_vNextPos;
	//情報を更新
	m_Collision.SetCollision(setCollision);
}

//当たった処理
void Laser::Hit(CollisionBase* hitCollision) {
	//プレイヤーとの衝突処理
	HitPlayer(hitCollision);
	//ブロックとの衝突処理
	HitBlock(hitCollision);
}

//プレイヤーに当たった処理
void Laser::HitPlayer(CollisionBase* hitCollision) {
	//プレイヤー以外なら実行しない
	if (hitCollision->GetKind() != KIND_PLAYER)return;

	//発見状態にする
	Data::SetFoundFlag(true);

	//プレイヤーのコリジョン情報を受け取る
	CollisionAABB* playerCollision = static_cast<CollisionAABB*>(hitCollision);

	//プレイヤーの中心座標
	VECTOR playerCenterPos = playerCollision->GetCollision().centerPos;
	//線分のベクトル
	VECTOR lineVec = VSub(m_Collision.GetCollision().endPos, m_Collision.GetCollision().startPos);
	//線分の開始地点と球のベクトル
	VECTOR lineToPlayerVec = VSub(playerCenterPos, m_Collision.GetCollision().startPos);
	//プレイヤーともっとも近い位置の線分の割合
	float t = VDot(lineToPlayerVec, lineVec) / VDot(lineVec, lineVec);
	//端にあわせる
	t = Math::Clamp(t, 0.0f, 1.0f);
	//線上のプレイヤーともっとも近い座標を求める
	VECTOR lineMove = VScale(lineVec, t);
	VECTOR lineNear = VAdd(m_Collision.GetCollision().startPos, lineMove);

	//プレイヤーの当たった点を設定
	VECTOR hitPos = lineNear;
	//エフェクトの表示
	CEffekseerCtrl::Request(EFFECT_LASER, hitPos, false);

	//衝突フラグを設定
	m_PlayerHitFlag = true;
}

//ブロックに当たった処理
void Laser::HitBlock(CollisionBase* hitCollision) {
	//ブロック類でなければ実行しない
	if (hitCollision->GetKind() != KIND_BLOCK)return;

	//ブロックのコリジョン情報を受け取る
	CollisionAABB* blockCollision = static_cast<CollisionAABB*>(hitCollision);
	VECTOR blockPos = blockCollision->GetCollision().centerPos;
	VECTOR blockSize= blockCollision->GetCollision().size;

	//衝突位置を計算
	VECTOR hitPos = blockPos;
	VECTOR vec = Math::GetMoveVec(m_vRot, blockSize.x);
	vec = VScale(vec, -1.0f);
	hitPos = VAdd(hitPos, vec);

	//ブロックに当たった位置を設定
	m_vNextPos = hitPos;
	//長さを設定
	float langth = Math::GetDistance(m_vPos, m_vNextPos);
	SetLaserLength(langth);

	//構造体の更新
	UpdateCollision();

	//衝突フラグ
	m_BlockHitFlag = true;
}