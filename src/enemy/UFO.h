#pragma once
#include"DxLib.h"
#include"../Model/Model.h"
#include"../Player/Player.h"


class UFO:public CModel
{
public:
	enum STATE
	{
		STATE_NEUTRAL,		//何もない状態
		STATE_IN,			//場に出る
		STATE_MOVE,			//移動
		STATE_DISCOVERY,	//プレイヤーを発見
		STATE_OUT,			//場を去る
	};

	enum MOVE
	{
		MOVE_STAY,			//待機
		MOVE_CHASE,			//追跡
	};

	const string	UFO_MODEL_PATH			=			//モデルパス
	{ "data/enemy/model/UFO.x" };
	const float		INOUT_SIZE_SPEED		= 0.001f;	//場に出る/去る大きさの変化
	const float		UFO_INOUT_SPEED			= 0.7f;		//場に出る/去るスピード
	const float		INOUT_SPC_SPEED			= 0.05f;	//場に出る/去るスペキュラカラーの変更速度
	const float		SEARCH_RADIUS_SPEED		= 0.7f;		//サーチ半径の拡大縮小速度
	const float		UFO_STATE_TIME			= 2.0f;		//行動継続時間
	const float		UFO_MAX_SPEED			= 5.0f;		//最高速度
	const float		UFO_SPEED_ADD			= 0.02f;	//スピードの加減速	
	const float		UFO_SCALE				= 0.1f;		//モデルの大きさ
	const float		UFO_HEIGHT				= 40.0f;	//サーチの高さ
	const float		UFO_RADIUS				= 20.0f;	//サーチの半径
	const float		PULLOUT_HEIGHT			= 20.0f;	//UFOの入退場の高さ
	const float		ANIME_SPEED				= 0.25f;	//アニメの再生速度
	const float		ALPHA_MIN				= 0.4f;		//透明度の最小値
	const float		ALPHA_SPEED				= 0.05f;	//透明度の変更
	const float		BARRAGE_RADIUS			= 10.0f;	//連打中のUI半径
	const float		BARRAGE_RADIUS_SCALE	= 5.0f;		//連打中のUIの半径のスケール
	const float		BARRAGE_HEIGHT			= 150.0f;	//連打中のUIの高さ
	const float		BARRAGE_LINE_SIZE		= 3.0f;		//連打中のUIのラインサイズ
	const float		MOVE_LIMIT				= 20.0f;	//移動制限
	const float		SPEED_ADD				= 0.7f;		//加速度
	const float		CHASE_TIME				= 0.3f;		//追跡時間
	const float		SEARCH_RADIUS_RATIO		= 1.5f;		//サーチの割合
	const float		CATCH_DISTANCE			= 15.0f;	//確保範囲
	const int		UFO_SPAWN_RAND			= 50;		//出現範囲
	const int		QTE_BUTTON_NUM			= 40;		//連打回数の基準
	const int		PULLOUT_RANGE			= 50;		//撤退範囲
	const int		SEARCH_ALPHA			= 70;		//サーチの円錐の透明度
	const int		SEARCH_POLYGON_NUM		= 16;		//サーチの底辺ポリゴン数
	const int		BARRAGE_BACK_ALPHA		= 70;		//連打中の背景の透明度
	const int		BARRAGE_MAIN_ALPHA		= 60;		//連打中の本体の透明度
	const int		BARRAGE_VERTEX_NUM		= 4;		//連打中の頂点数
	const int		MOVE_PROBABILITY		= 50;		//移動方向の確率

	//----------------------------------

	VECTOR	m_vDrawPos;					//表示用座標
	VECTOR	m_vKeepLStick;				//Lスティックの値を保存
	STATE	m_State;					//状態
	MOVE	m_MoveState;				//移動タイプ
	int		m_iButtonCount;				//ボタンを押した
	int		m_iQTECount;				//連打回数
	float	m_fScale;					//大きさ
	float	m_fUpPos;					//上昇値（演出用）
	float	m_fCurrentMoveTime;			//移動時間
	float	m_fSpeedAdd;				//スピード加減速
	float	m_fConeRadius;				//サーチの半径
	float	m_fConeBottom;				//サーチの高さ
	float	m_fAlpha;					//透明度
	float	m_fSpc;						//スペキュラカラー_グリーン
	float	m_fMoveCount;				//移動の時間
	bool	m_IsUse;					//存在フラグ
	bool	m_PLFound;					//プレイヤー発見(連打時で使う)
	bool	m_HitCameraFlag;			//カメラと当たっているフラグ
	bool	m_CatchPlayer;				//プレイヤーを確保フラグ

public:
	void Init();
	void Load();
	void Start();
	void Step(Player& pl);
	void Draw();
	void Fin();
	//UI用
	void DrawUI();

	//UFOを呼び出す
	void RequestUFO(VECTOR plPos);
	//使用フラグの呼び出し
	bool GetIsUse() { return m_IsUse; }
	//カメラとの衝突フラグの取得
	bool GetHitCameraFlag() { return m_HitCameraFlag; }
	//カメラとの衝突フラグの設定
	void SetHitCameraFlag(bool set) { m_HitCameraFlag = set; }
	//プレイヤーを連れ去ろうとしているフラグ
	bool GetPLFound() {	return m_PLFound; }
	//プレイヤーを捕獲したフラグ
	bool GetCatchPlayer() { return m_CatchPlayer; }

private:
	void Updata();
	//待機
	void Stay(VECTOR plPos, bool safetyAreaFlag);	
	//移動
	void Chase();
	//プレイヤー探索
	void Search(Player& pl);
	//捕まった時の連打用
	void QuickTimerEvent(Player& pl);
	//向かう場所の取得
	VECTOR GetSearchPos(VECTOR vPos);
};