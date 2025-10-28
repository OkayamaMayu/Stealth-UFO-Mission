#pragma once

class Fade
{
public:
	//フラッシュの色ID
	enum FLASH_COLOR_ID
	{
		WHITE,		//白
		BLACK,		//黒

		FLASH_COLOR_NUM,
	};

	//フェード演出の種類
	enum FADE_ID
	{
		FADE_NONE,	//演出していない
		FADE_IN,	//フェードイン
		FADE_OUT,	//フェードアウト
	};

	//フェード演出用の色
	const unsigned int	FADE_COLLOR						= GetColor(0,0,0);
	//フラッシュの色
	const unsigned int	FLASH_COLOR[FLASH_COLOR_NUM]	= { GetColor(0,0,0),GetColor(255,255,255) };
	//フラッシュの速度
	const int			FLASH_SPEED						= 10;
private:
	Fade();
	~Fade();

	static Fade*	m_Instance;		//インスタンス

	FLASH_COLOR_ID	m_FlashColor;	//フラッシュの色
	FADE_ID			m_FadeID;		//フェード演出の種類
	int				m_iFadeAlpha;	//フェード演出のアルファ値
	int				m_iFlashAlpha;	//フラッシュ演出のアルファ値
	int				m_iFadeSpeed;	//フェード演出の速度
	bool			m_FlashFlag;	//フラッシュ演出のフラグ

public:
	static void		Create();		//インスタンスの生成
	static void		Destroy();		//インスタンスの削除
	static Fade*	GetInstance();	//インスタンスの取得

	void			Init();
	void			Step();
	void			Draw();

	//フェード演出のリクエスト
	void			RequestFade(FADE_ID setIn, int setSpeed);
	//フラッシュ演出のリクエスト
	void			RequestFlash(FLASH_COLOR_ID set = WHITE);

	//現在フェード演出をしているか
	bool			GetFadeNow(){ if (m_FadeID != FADE_NONE)return true; return false; }
	//現在フラッシュ演出をしているか
	bool			GetFlashNow(){ return m_FlashFlag; }

private:
	//フェードインの処理
	void FadeIn();
	//フェードアウトの処理
	void FadeOut();
	//フラッシュの処理
	void Flash();
};