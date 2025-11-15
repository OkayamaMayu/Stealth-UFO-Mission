#include"CheckPoint.h"

void CheckPoint::Init(VECTOR setPos, float setRot)
{
	Init();

	m_vPos		= m_vNextPos = setPos;
	m_vRot.y	= setRot;

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	//構造体の設定
	UpdateCollision();
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase* hitCollision) {Hit(hitCollision); });
}

void CheckPoint::Init()
{
	CModel::Init();

	m_fModelFade			= 1.0f;
	m_IsActive				= false;
	m_SemitransparentFlag	= false;
	m_CheckPointFlag		= false;
	m_iEffectHandle			= -1;
}

void CheckPoint::Load(int originHandle)
{
	DuplicateModel(originHandle);
}

void CheckPoint::Start()
{
	//スケールの設定
	m_vScale = Vector::MakeVec(MODEL_SCALE);
	//情報の適用
	Update();

	//リスポーンしていなかったらアニメーションを初期化
	if (!Data::GetReSpawnFlag())
		RequestAnimation(ANIMEID_STAY, ANIMATION_SPEED[ANIMEID_STAY]);

	//透明度の更新
	MV1SetOpacityRate(m_iHandle, m_fModelFade);
}

void CheckPoint::Step()
{
	//アニメの通常処理
	AnimeStep();

	//透明度の変位
	FadeStep();
}

void CheckPoint::Draw()
{
	CModel::Draw();
}

void CheckPoint::Fin()
{
	CModel::Fin();
	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

//=====================================================

bool CheckPoint::IsHit(){
	//チェックポイントを有効化
	m_IsActive = true;
	//チェンジアニメを再生
	RequestAnimation(ANIMEID_CHANGE, ANIMATION_SPEED[ANIMEID_CHANGE], ANIMSTATE_END);

	//サウンドを再生
	Sound::Play(SE_CHECK_POINT);

	//エフェクトを再生
	CEffekseerCtrl::Request(EFFECT_CREATE_BLOCK,m_vPos, false);

	return true;
}

//アニメの通常処理
void CheckPoint::AnimeStep()
{
	//チェックポイントが有効なら
	if (m_IsActive)
	{
		//チェンジアニメ終了時にアクティブ常時アニメに変更
		if (GetChecknd(ANIMEID_CHANGE))
			RequestAnimation(ANIMEID_ACTIVE, ANIMATION_SPEED[ANIMEID_ACTIVE]);
	}

	//アニメの更新
	UpdateBlendRate();
	UpdateAnim();
}

//透明度の変位
void CheckPoint::FadeStep()
{
	//半透明にする
	if (m_SemitransparentFlag)
	{
		Math::MatchSpecifiedNum(m_fModelFade, MODEL_SEMITRANSPARENT, MODEL_FADE_SPEED);
	}
	//透過を消す
	else
	{
		Math::MatchSpecifiedNum(m_fModelFade, 1.0f, MODEL_FADE_SPEED);
	}

	//透明度の更新
	MV1SetOpacityRate(m_iHandle, m_fModelFade);
}

//=====================================================

//当たった処理
void CheckPoint::Hit(CollisionBase* hitCollision) {
	//プレイヤー以外なら実行しない
	if (hitCollision->GetKind() != KIND_PLAYER)return;
	//既に有効なら実行しない
	if (m_IsActive)return;

	//プレイヤー座標を取得
	VECTOR playerPos = hitCollision->GetOwner()->GetPos();
	//プレイヤーの向きを取得
	VECTOR playerRot = hitCollision->GetOwner()->GetRot();

	IsHit();
	//リスポーン情報を取得
	m_ReSpawnInfo.pos	= playerPos;
	m_ReSpawnInfo.rot	= playerRot.y;
	m_CheckPointFlag	= true;
}

//コリジョン情報の更新
void CheckPoint::UpdateCollision() {
	Sphere setCollision = m_Collision.GetCollision();
	//サイズを設定
	setCollision.radius = MODEL_SIZE * CHECKPOINT_HIT_SCALE;
	//中心座標を設定
	setCollision.centerPos = m_vNextPos;
	//情報を更新
	m_Collision.SetCollision(setCollision);
}