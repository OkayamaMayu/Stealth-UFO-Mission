#pragma once
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"

class Item:public CModel
{
public:
	//サイズ（半径）
	const VECTOR	ITEM_SIZE					= {25.0f * MODEL_SCALE, 25.0f * MODEL_SCALE, 25.0f * MODEL_SCALE };	
	const VECTOR	ITEM_SCALE[3]				=			//モデルの大きさ：0,持っている状態　1,地面に落ちている状態　2,一人称カメラ
	{ 
		{ 0.05f, 0.05f, 0.05f },
		{ 0.04f, 0.04f, 0.04f },
		{ 0.08f, 0.08f, 0.08f } 
	};
	const VECTOR	CATCH_ITEM_POS[2]			=			//アイテム所持中のカメラによる位置
	{ 
		{ 0.0f, -8.0f, -15.0f }, 
		{ -4.5f, 15.0f, -3.0f } 
	};
	const float		ITEM_BOUND_MAX_SPEED		= 0.5f;		//バウンドの最大値
	const float		ITEM_MAX_SPEED				= 1.5f;		//投げた時の最大スピード
	const float		ITEM_FOUND_TIME				= 2.0f;		//発見される可能性のある時間
	const float		ITEM_RESPAWN_TIME			= 10.0f;	//アイテムが再配置されるまでの時間	
	const float		SPAWN_GRAVITY				= 0.5f;		//スポーン時の重力
	const float		SPAWN_HEIGHT				= 10.0f;	//スポーン時の高さ
	const float		ALPHA_SPEED					= 0.05f;	//透明度の変更速度
	const float		BOUND_POWER_MIN				= 0.01f;	//バウンドの最小値
	const float		SOUND_RANGE					= 80.0f;	//サウンドの再生範囲
	const float		CATCH_FPS_ALPHA				= 0.5f;		//所持中の透明度
	const float		CATCH_FPS_ALPHA_SPEED		= 0.01f;	//所持中の透明度の変更速度
	const float		CATCH_DEFAULT_ALPHA_SPEED	= 0.05f;	//通常カメラの透明度変更速度
	const float		CATCH_SETBLOCK_ALPHA_SPEED	= 0.1f;		//一人称時のブロック配置モードの透明度の変更速度
	const float		CATCH_FRY_SPEED				= 0.02f;	//所持中の浮遊速度
	const float		CATCH_FRY_LIMIT				= 1.0f;		//浮遊の制限
	const float		DECELERATION_SPEED			= 0.015f;	//投げた時の減速速度
	const float		ACCELERATION_SPEED			= 0.05f;	//投げた時の加速速度
	const float		THROW_INIT_SPEED			= -2.0f;	//投げた瞬間の移動速度
	const float		REFLECTION_ROT				= 90.0f;	//反射角度
	const float		REFLECTION_SPEED_SCALE		= 0.8f;		//反射時の速度の減速倍率
	const float		TARGET_HEIGHT				= 7.0f;		//ターゲットの高さ
	const int		BOUND_POWER_RATIO			= 10;		//バウンド力の割合
	const int		THROW_SPEED_SCALE			= 5;		//投げた時の速度の倍率

private:
	CollisionSphere	m_Collision;				//コリジョン情報
	VECTOR			m_vSpawnPos;				//出現位置
	VECTOR			m_vTargetPos;				//投げた先のターゲット
	VECTOR			m_vPlPos;					//プレイヤーの座標を保存する
	float			m_fBoundValue;				//バウンド用
	float			m_fBoundMax;				//バウンド最大値
	float			m_fGravityAdd;				//加算する重力
	float			m_fFryPos;					//つかみ時の浮遊
	float			m_fSpeed;					//移動量総合値
	float			m_fThrowRot;				//投げられた向き
	float			m_fFoundCount;				//発見される可能性のある時間
	float			m_fAlpha;					//モデルの透明度
	float			m_fRespawnTimeCount;		//リスポーンまでの時間
	bool			m_FoundFlagPl;				//掴むことのできるフラグ
	bool			m_IsUse;					//使用フラグ
	bool			m_Fall;						//落ちフラグ
	bool			m_Throw;					//投げられフラグ
	bool			m_CatchFlag;				//持ってるフラグ
	bool			m_FryUp;					//持ってる時の動きフラグ
	bool			m_FoundFlag;				//発見される可能性のあるフラグ
	bool			m_EnemyHitFlag;				//エネミーに当たるフラグ
	bool			m_EnemyTargetFlag;			//投げた時にエネミーをターゲットにする
	bool			m_DrawFlag;					//描画フラグ
	bool			m_ModeCloseFlag;			//アイテムモードを閉じるフラグ
	int				m_iDrawHandle[2];			//保存しておくハンドル

public:

	void Init(VECTOR setPos, VECTOR setRot);
	void Load(int originHandle1, int originHandle2);
	void Start();
	void Step(VECTOR plPos,float plRot,VECTOR plSpeed, bool plVisionFlag, bool blockModeFlag, float focusRot=0.0f);
	void Draw();
	void Fin();

	//地面
	bool HitGround(float posY);
	//天井
	void HitCeiling();

	//コリジョン情報の呼び出し
	CollisionSphere	GetCollision() { return m_Collision; }
	//コリジョン情報の設定
	void			SetCollision(CollisionSphere set) { m_Collision = set; }
	//コリジョンを登録
	void			RegisterCollision() { CollisionManager::GetInstance()->RegisterCollision(&m_Collision); }

	//当たった処理
	void Hit(CollisionBase* hitCollision);

private:
	void Init();
	//つかんでいる時の動き
	void CatchMove(VECTOR plPos, float plRot, VECTOR plSpeed, bool plVisionFlag,float focusRot, bool blockModeFlag);

	//X軸の当たった処理
	void			HitX(VECTOR hitPos, VECTOR hitSize);
	//Y軸の当たった処理
	void			HitY(VECTOR hitPos, VECTOR hitSize);
	//Z軸の当たった処理
	void			HitZ(VECTOR hitPos, VECTOR hitSize);

	//コリジョン情報の更新
	void			UpdateCollision();
public:
	//使用フラグの取得
	bool GetIsUse() { return m_IsUse; }
	//使用フラグの設定
	void SetIsUse(bool set) { m_IsUse = set; }
	//つかんでいるフラグの取得
	bool GetCatchFlag() { return m_CatchFlag; }
	//アイテムをつかませる
	void RequestCatch();
	//アイテムを投げさせる
	void RequestThrow(float plRotY, float plRotX);
	//速度を設定
	void SetSpeed(VECTOR set) { m_vSpeed = set; }
	//投げる
	void ThrowMove();
	//掴めるかのフラグの取得
	bool GetFoundFlag() { return m_FoundFlag; }
	//座標の取得
	VECTOR GetPos() { return m_vPos; }
	//プレイヤーのつかめる範囲にあるかの設定
	void SetFoundPLFlag(bool set) { m_FoundFlagPl = set; }
	//エネミーに当たったフラグの取得
	bool GetEnemyHitFlag() { return m_EnemyHitFlag; }
	//エネミーに当たったフラグの設定
	void SetEnemyHitFlag(bool set) { m_EnemyHitFlag = set; }
	//ターゲットの設定
	void SetTarget(VECTOR set);
	//ターゲットをターゲットにするフラグの設定
	void SetTargetFlag(bool set) { m_EnemyTargetFlag = set; }
	//ターゲットをターゲットにするフラグの取得
	bool GetTargetFlag() { return m_EnemyTargetFlag; }
	//ターゲット座標の取得
	VECTOR GetTargetPos() { return m_vTargetPos; }
	//エネミーに当てる
	void HitEnemy();
	//投げるフラグの取得
	bool GetThrow() { return m_Throw; }
	//反射処理
	void Reflection(int mul = 1);
	//アイテムモードを閉じるフラグを取得
	bool GetModeCloseFlag() { return m_ModeCloseFlag; }
};