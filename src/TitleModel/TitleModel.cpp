#include"TitleModel.h"

void TitleModel::Init()
{
	CModel::Init();
}

void TitleModel::Init(VECTOR setPos)
{
	Init();
	m_vPos = m_vNextPos = setPos;

	CModel::Update();
}

void TitleModel::Load(const char* pFileName, float modelSize, ANIME_ID animeID, float snimeSp, ANIMSTATE animeState )
{ 
	m_iHandle = MV1LoadModel(pFileName);

	RequestAnimation(animeID, snimeSp, animeState);
	
	m_vScale = Vector::MakeVec(modelSize);

	CModel::Update();
}

void TitleModel::Step()
{
	//スピードが設定されている(回転予定がある)
	if (m_vSpeed.x != 0.0f && m_vSpeed.y != 0.0f)
	{
		//skyモデルを少し回転
		m_vRot = VAdd(m_vRot, m_vSpeed);
	}

	//アップデート
	Update();
	UpdateBlendRate(); 
	UpdateAnim();
}

void TitleModel::Draw()
{
	CModel::Draw();
}

void TitleModel::Fin()
{
	CModel::Fin();
}

void TitleModel::ChangeAnimation(ANIME_ID animeID, float animeSp, ANIMSTATE animState)
{
	RequestAnimation(animeID, animeSp, animState);
}

void TitleModel::RequestAnime(ANIME_ID animeID, float animeSp, ANIMSTATE animState)
{
	RequestEndLoop(animeID, animeSp);
}