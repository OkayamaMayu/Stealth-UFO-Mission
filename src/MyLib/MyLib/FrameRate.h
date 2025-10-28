#pragma once

#define FRAME_RATE		(60)					//設定フレームレート（60FPS）
#define FRAME_RATE_TIME	(1000 / FRAME_RATE)		//1フレームの時間（ミリ秒）
#define FRAME_TIME		(1.0f / FRAME_RATE)		//カウント用

//フレームレート情報
struct FrameRateInfo
{
	int		currentTime;	//現在の時間
	int		lastFrameTime;	//前回のフレーム実行時の時間
	int		count;			//フレームカウント用
	int		calcFpsTime;	//FPSを計算した時間
	float	fps;			//計測したFPS（表示用）
};

//FPSチェック
bool FPS();

//FPS計算
void CalcFPS();

//FPS表示（デバック用）
void DrawFPS();