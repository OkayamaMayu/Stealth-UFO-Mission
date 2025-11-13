#include"CheckPoint.h"

void CheckPoint::Init(VECTOR setPos, float setRot)
{
	Init();

	m_vPos		= m_vNextPos = setPos;
	m_vRot.y	= setRot;
}

void CheckPoint::Init()
{
	CModel::Init();

	m_fModelFade			= 1.0f;
	m_IsActive				= false;
	m_SemitransparentFlag	= false;
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
}

//=====================================================

bool CheckPoint::IsHit()
{
	//既に有効なら以下実行しない
	if (m_IsActive)
		return false;

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