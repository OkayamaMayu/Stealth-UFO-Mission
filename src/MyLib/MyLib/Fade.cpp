#include"DxLib.h"
#include"Fade.h"
#include"../MyLib.h"

//インスタンスの初期化
Fade* Fade::m_Instance = nullptr;

//コンストラクタ
Fade::Fade()
{
}

//デストラクタ
Fade::~Fade()
{
}

//インスタンスの生成
void Fade::Create()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Fade();
	}
}

//インスタンスの削除
void Fade::Destroy()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

//インスタンスの所得
Fade* Fade::GetInstance()
{
	if (m_Instance == nullptr)
	{
		//インスタンスがない場合は生成
		Create();
	}

	return m_Instance;
}

//===============================================

void Fade::Init()
{
	//変数の初期化
	m_FlashColor	= WHITE;
	m_FadeID		= FADE_NONE;
	m_iFadeAlpha	= 0;
	m_iFlashAlpha	= 0;
	m_iFadeSpeed	= 0;
	m_FlashFlag		= false;
}

void Fade::Step()
{
	//フェード演出の処理
	switch (m_FadeID)
	{
	case Fade::FADE_IN:
		FadeIn();
		break;

	case Fade::FADE_OUT:
		FadeOut();
		break;

	default:
		break;
	}

	//フラッシュ演出の処理
	if (m_FlashFlag)
	{
		Flash();
	}
}

void Fade::Draw()
{
	//フェード演出
	//透明度変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_iFadeAlpha);
	//描画
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, FADE_COLLOR, true);
	//表示を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//フラッシュ演出
	//fadeで透明度変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_iFlashAlpha);
	//描画
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, FLASH_COLOR[m_FlashColor], true);
	//表示を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//===========================================================================================

//フェード演出のリクエスト
void Fade::RequestFade(FADE_ID setIn, int setSpeed)
{
	m_FadeID		= setIn;
	m_iFadeSpeed	= setSpeed;
}
//フェードインの処理
void Fade::FadeIn()
{
	//透明度を変更
	if (Math::MatchSpecifiedNum(m_iFadeAlpha, 0, m_iFadeSpeed))
	{
		//完了したら終了
		m_FadeID = FADE_NONE;
	}
}
//フェードアウトの処理
void Fade::FadeOut()
{
	//透明度を変更
	if (Math::MatchSpecifiedNum(m_iFadeAlpha, 255, m_iFadeSpeed))
	{
		//完了したら終了
		m_FadeID = FADE_NONE;
	}
}

//===========================================================================================

//フラッシュ演出のリクエスト
void Fade::RequestFlash(FLASH_COLOR_ID set)
{
	m_FlashColor = set;

	m_FlashFlag = true;
	m_iFlashAlpha = 255;
}
//フラッシュの処理
void Fade::Flash()
{
	//透明度を変更
	if (Math::MatchSpecifiedNum(m_iFlashAlpha, 0, FLASH_SPEED))
	{
		//完成したら終了
		m_iFlashAlpha = 0;
		m_FlashFlag = false;
	}
}