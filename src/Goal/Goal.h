#pragma once
#include"../MyLib/MyLib.h"
#include"../Model/Model.h"



class Goal:public CModel
{
public:
	enum ANIME
	{
		ANIME_DEFAULT,		//通常
		ANIME_TRANSFORM,	//変形

		ANIME_NUM,
	};

	//モデルサイズ
	const VECTOR GOAL_SIZE = 
	{ 
		81.0f	* MODEL_SCALE,
		350.0f	* MODEL_SCALE,
		82.0f	* MODEL_SCALE
	};
	//モデルパス
	const string	GOAL_MODEL_PATH				= "data/map/goal/goal.mv1";
	const float		ANIME_SPEED[ANIME_NUM]		={1.0f,0.25f};	//アニメのスピード
	const float		GOAL_EFFECT_HEIGHT			= 0.2f;			//モデルを基準にしたエフェクトの高さ
	const float		MOVE_SPEED					= 0.02f;		//モデルのスピード
	const float		MOVE_MIN_LIMIT				= 0.0f;			//最小移動制限
	const float		MOVE_MAX_LIMIT				= 3.0f;			//最大移動制限
	const float		DOWN_LIMIT					= 5.0f;			//ゴール後の待機位置
	const float		DOWN_SPEED					= 0.05f;		//ゴール後の移動速度
	const float		WAIT_TIME					= 0.5f;			//待機時間
	const float		FLOATING_SPEED				= 0.07f;		//上昇速度
	const float		FLOATING_SPEED_ADD			= 0.1f;			//上昇速度の加速度
	const float		FLOATING_SIZE_SPEED			= 0.001f;		//上昇時のサイズ変更速度
	const float		ROT_SPEED					= 0.01f;		//回転速度
	const int		SCENE_CHANGE_FADEOUT_SPEED	= 5;			//シーン変更時のフェードアウト速度

private:
	VECTOR	m_vStartPos;		//初期位置
	float	m_fFloating;		//浮遊
	float	m_fUpSpeed;			//上昇速度
	float	m_fUpCount;			//上昇までの時間
	bool	m_IsGoalFlag;		//ゴールしたか
	bool	m_NextSceneTrg;		//シーンがすすむフラグ
	bool	m_FloatingFlag;		//trueだと浮く
	int		m_iEffectHandle;	//常時設定用エフェクトハンドル

public:
	void Init(VECTOR vPos);
	void Init();
	void Load();
	void Start();
	void Step();
	void Fin();

	//ゴールに当たった場合
	void HitGoal();
	//ゴールしたフラグの取得
	bool GetIsGoalFlag() { return m_IsGoalFlag; }
	//次のシーンに移行するフラグ
	bool GetNextSceneFlag() { return m_NextSceneTrg; }
};