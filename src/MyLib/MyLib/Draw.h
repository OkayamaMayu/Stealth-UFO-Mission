#pragma once
#include<iostream>
#include<string>
using namespace std;

struct Draw
{
	//ベクターを使用した描画
	static void DrawGraphV(VECTOR vPos, int GrHandle, bool transFlag);
	//ベクターを使用した描画
	static void DrawRotaGraphV(VECTOR vPos, float ExRate, float Angle, int GrHundle, bool TransFlag, bool ReverseXFlag = 0, bool ReverseYFlag = 0);

	//アルファ値付き描画：濃さはパーセンテージ
	static void DrawGraphAlpha(float fx, float fy, int alpha, int GrHandle, bool transFlag);
	static void DrawGraphAlpha(VECTOR vPos, int alpha, int GrHandle, bool transFlag);

	//アルファ値付き描画：濃さはパーセンテージ
	static void DrawRotaGraphAlpha(float fx, float fy, float ExRate, float Angle, int alpha, int GrHundle, bool TransFlag, bool ReverseXFlag = 0, bool ReverseYFlag = 0);
	static void DrawRotaGraphAlpha(VECTOR vPos, float ExRate, float Angle, int alpha, int GrHundle, bool TransFlag, bool ReverseXFlag = 0, bool ReverseYFlag = 0);

	//文字列中央揃え描画
	static void DrawFormatStringCenter(VECTOR vPos, unsigned int color, const char* formatString, ...);
	//アルファ値付き文字列描画：濃さはパーセンテージ
	static void DrawFormatStringAlpha(VECTOR vPos,int alpha, unsigned int color, const char* formatString, ...);
	//アルファ値付き文字列中央揃え描画：濃さはパーセンテージ
	static void DrawFormatStringCenterAlpha	(VECTOR vPos, int alpha, unsigned int color, const char* formatString, ...);

	//ブレンドモードをリセット
	static void DrawBlendModeReset(){ SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }
	//ブレンドモード変更：アルファチャンネル
	static void DrawBlendModeAlpha(int alpha);
};
