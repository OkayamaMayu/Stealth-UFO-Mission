#pragma once
#include "../MyLib/MyLib/Input.h"
#include <iostream>
#include <vector>
using namespace std;

//操作入力ID
enum OPERATION_INPUT_ID
{
	INPUT_MOVE_LEFT,						//移動_左
	INPUT_MOVE_RIGHT,						//移動_右
	INPUT_MOVE_PROGRESS,					//移動_前進
	INPUT_MOVE_REVERSE,						//移動_後進

	INPUT_JUMP,								//ジャンプ

	INPUT_ITEM_MODE,						//アイテムモード
	INPUT_ITEM_THROW,						//アイテムを投げる

	INPUT_BLOCK_MODE,						//ブロック配置モード
	INPUT_BLOCK_PLACE,						//ブロックを配置する

	INPUT_CAMERA_LEFT,						//カメラ_左回転
	INPUT_CAMERA_RIGHT,						//カメラ_右回転

	INPUT_CAMERA_ZOOM,						//カメラ_ズーム
	INPUT_CAMERA_ZOOM_OUT,					//カメラ_ズームアウト

	INPUT_OPTION_LEFT,						//メニューを開く_左キー
	INPUT_OPTION_RIGHT,						//メニューを開く_右キー

	INPUT_DETERMINATION,					//決定
	INPUT_BACK,								//戻る

	OPERATION_INPUT_NUM						//操作の数
};

const float CONTROLLER_INPUT_MIN = 0.3f;	//スティック入力時の最小倒し具合

class InputManager
{
private:

	//入力媒体
	enum DEVICE_TYPE
	{
		DEVICE_TYPE_KEY,					//キーボード
		DEVICE_TYPE_MOUSE,					//マウス
		DEVICE_TYPE_CONTROLLER,				//コントローラー
							
		DEVICE_TYPE_NUM						//入力媒体の数
	};
	
private:
	static int m_InputMedia[DEVICE_TYPE_NUM][OPERATION_INPUT_NUM];

public:
	//初期化
	static void Init();
	//通常処理
	static void Step();

	//入力情報_今押された
	static bool CheckPush(int operationInputID);
	//入力情報_押し続けられている
	static bool CheckKeep(int operationInputID);
	//入力情報_離された
	static bool CheckRelease(int operationInputID);

	/*========================
			各種行動判定
	==========================*/
	//入力情報_プレイヤー移動
	//引数	：スティックの倒し具合を格納する変数
	static bool CheckPlayerMove(VECTOR& stickState);

	//入力情報_カメラ回転
	//引数	：スティックの倒し具合を格納する変数
	static bool CheckCameraMove(VECTOR& stickState);

	//入力情報_カメラの上下
	//引数	：スティックの倒し具合を格納する変数
	static bool CheckCameraUpDown(VECTOR& stickState);

	//入力情報_カメラのズーム/ズームアウト
	//引数	：スティックの倒し具合を格納する変数
	static bool CheckCameraZoom(VECTOR& stickState);

	//入力情報_UFOへの抵抗
	//引数	：スティックの倒し具合を格納する変数
	static int CheckResistanceToUFO(VECTOR& stickState);

	//入力情報_←
	static bool CheckSelectLeft();

	//入力情報_→
	static bool CheckSelectRight();

	//入力情報_↑
	static bool CheckSelectUp();

	//入力情報_↓
	static bool CheckSelectDown();
};