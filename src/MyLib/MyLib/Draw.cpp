#include"DxLib.h"
#include"Draw.h"
#include<cstdarg>	//可変引数の処理用
#include<cstdio>	//vsnprintfの使用用
#include<Windows.h>

//ベクターを使用した描画
void Draw::DrawGraphV(VECTOR vPos, int GrHandle, bool transFlag)
{
	DrawGraphF(vPos.x, vPos.y, GrHandle, transFlag);
}
//ベクターを使用した描画
void Draw::DrawRotaGraphV(VECTOR vPos, float ExRate, float Angle, int GrHundle, bool TransFlag, bool ReverseXFlag, bool ReverseYFlag)
{
	DrawRotaGraphF(vPos.x, vPos.y, ExRate, Angle, GrHundle, TransFlag, ReverseXFlag, ReverseYFlag);
}

//アルファ値付き描画：濃さはパーセンテージ
void Draw::DrawGraphAlpha(float fx, float fy, int alpha, int GrHandle, bool transFlag)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);

	DrawGraphF(fx, fy, GrHandle, transFlag);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void Draw::DrawGraphAlpha(VECTOR vPos, int alpha, int GrHandle, bool transFlag)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);

	DrawGraphF(vPos.x, vPos.y, GrHandle, transFlag);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//アルファ値付き描画：濃さはパーセンテージ
void Draw::DrawRotaGraphAlpha(float fx, float fy, float ExRate, float Angle, int alpha, int GrHundle, bool TransFlag, bool ReverseXFlag, bool ReverseYFlag)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);
	DrawRotaGraphF(fx, fy, ExRate, Angle, GrHundle, TransFlag, ReverseXFlag, ReverseYFlag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void Draw::DrawRotaGraphAlpha(VECTOR vPos, float ExRate, float Angle, int alpha, int GrHundle, bool TransFlag, bool ReverseXFlag, bool ReverseYFlag)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);
	DrawRotaGraphF(vPos.x, vPos.y, ExRate, Angle, GrHundle, TransFlag, ReverseXFlag, ReverseYFlag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//文字列中央揃え
void Draw::DrawFormatStringCenter(VECTOR vPos, unsigned int color, const char* formatString, ...)
{
	VECTOR vStrPos = vPos;
	int fontSize = GetFontSize();

	string str = formatString;
	float posX = str.length() / 2.0f * (float)fontSize;	//文字数をカウント

	char buffer[255] = {};

	//可変引数を処理
	va_list args;
	va_start(args, formatString);
	//フォーマット済みの文字列を生成
	vsnprintf(buffer, sizeof(buffer), formatString, args);
	va_end(args);

	string s = buffer;

	DrawFormatStringF(vStrPos.x - posX / 2.0f, vStrPos.y - fontSize / 2.0f, color, s.c_str());
}
//アルファ値付き文字列描画
void Draw::DrawFormatStringAlpha(VECTOR vPos, int alpha, unsigned int color, const char* formatString, ...)
{
	char buffer[521] = {};

	//可変引数を処理
	va_list args;
	va_start(args, formatString);
	//フォーマット済みの文字列を生成
	vsnprintf(buffer, sizeof(buffer), formatString, args);
	va_end(args);

	string s = buffer;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);
	DrawFormatStringF(vPos.x, vPos.y, color, s.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}
//アルファ値付き文字列中央揃え描画
void Draw::DrawFormatStringCenterAlpha(VECTOR vPos, int alpha, unsigned int color, const char* formatString, ...)
{
	char buffer[255] = {};

	//可変引数を処理
	va_list args;
	va_start(args, formatString);
	//フォーマット済みの文字列を生成
	vsnprintf(buffer, sizeof(buffer), formatString, args);
	va_end(args);

	string s = buffer;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);
	DrawFormatStringCenter(vPos, color, s.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
}

//ブレンドモード変更：アルファチャンネル
void Draw::DrawBlendModeAlpha(int alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha / 100);
}