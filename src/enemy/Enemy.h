#pragma once
#include "EnemyBase.h"

//エネミー１---------------------------------------------------------------------------------------
const int			MOVE_POINT_NUM					= 2;				//エネミーの移動ポイント数

class EnemyType1 :public EnemyBase
{
private:
	//モデルのサイズ
	const VECTOR	ENEMY_TYPE1_SIZE				= { 60.0f * MODEL_SCALE, 314.0f * MODEL_SCALE, 60.0f * MODEL_SCALE };
	const float		MOVE_MIN_SPEED					= 2.0f;				//移動速度の最小値
	const float		ENEMY_TYPE1_SPEED				= 0.1f;				//移動スピード
	const float		ENEMY_TYPE1_DISCOVERY_SPEED		= 1.0f;				//発見時のスピード
	const float		ENEMY_STUN_MOVE_SPEED_MAX[2]	= { 0.03f,0.01f };	//スタン中の回転スピード
	const float		ENEMY_STUN_MOVE_SPEED			= 0.0003f;			//スタン中の回転スピードの加速
	const float		SPEED_SCALE						= 0.1f;				//移動の倍率
	const float		STUN_MOVE_SPEED					= 0.01f;			//気絶状態の速度
	const float		HIT_RESET_TIME					= 1.5f;				//ブロックに当たっている状態のリセット時間
	const float		FOUND_GAUGE_UP					= 0.05f;			//発見時のゲージ上昇値
	const float		ROT_RETURN_SPEED				= 0.01f;			//気絶状態から基本体制に戻す
	const float		NORMAL_ROT_SPEED				= 0.05f;			//通常状態の回転速度
	const float		STUN_ROT_Z						= 0.5f;				//気絶中のZ軸回転
	const float		STUN_ROT_X						= 0.3f;				//気絶中のX軸回転
	const int		MOVE_MAX_SPEED					= 3;				//ランダムに速さを出す場合の最大値

private:
	CollisionAABB	m_Collision;										//コリジョン情報
	VECTOR			m_vMoveFinPos;										//移動先
	VECTOR			m_vNextMovePos;										//次に向かうところ
	float			m_fStayRot[MOVE_POINT_NUM];							//移動先の向き
	bool			m_MovePoint;										//移動フラグ

public:
	void			Init(VECTOR vStartPos, VECTOR vGoalPos, float vStartRot, float vGoalRot);
	void			Step(Player& pl,ItemManager& itemMana, BackGround& block, bool gameOverFlag, bool clearFlag);
	
	//サイズを取得
	VECTOR			GetSize() { return ENEMY_TYPE1_SIZE; }
	//移動先のポイントを設定
	void			SetEnemyPointPos(VECTOR set) { m_vNextMovePos = set; }

private:
	void			Move(VECTOR plPos, float speed);
};