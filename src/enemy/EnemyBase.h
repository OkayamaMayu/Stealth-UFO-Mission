#pragma once
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"
#include"../Item/ItemManager.h"
#include"../BackGround/StageBlockManager.h"
#include"../Player/Player.h"

const float ENEMY_FOUNDSIZE		= 80.0f;					//視界の大きさ

class EnemyBase :public CModel
{
public:
	//エネミーの状態
	enum ENEMY_STATUS
	{
		ENEMY_STATE_NORMAL,			//通常
		ENEMY_STATE_MOVE,			//移動
		ENEMY_STATE_DISCOVERY,		//発見
		ENEMY_STATE_STUN,			//気絶

		ENEMY_STATE_NUM,
	};

	//発見対象の種類
	enum FOUND_ID
	{
		FOUND_NEUTRAL = -1,			//未発見
		FOUND_PLAYER,				//プレイヤー
		FOUND_ITEM,					//アイテム
		ENEMY_LOSE,					//見失い

		FOUNT_TYPE_NUM
	};

	//エネミーの反応
	enum ENEMY_REACTION
	{
		ENEMY_REACTION_FOUND,		//発見状態
		ENEMY_REACTION_LOST,		//見失い状態

		ENEMY_REACTION_NUM,
	};

protected:
	const float		ENEMY_STATE_MAXTIME		= 5.0f;			//行動切り替え時間
	const float		ENEMY_MOVE_MAX			= 25.0f;		//移動範囲
	const float		ENEMY_RESPAWN_TIME		= 3.0f;			//エネミー落下時のリスポーン時間
	const float		MOVE_ROT_SPEED			= 0.03f;		//移動時の回転速度
	const float		RESET_ROT_SPEED			= 0.06f;		//初期位置に戻る時の回転速度
	const float		RESET_MOVE_SCALE		= -1.5f;		//初期位置に戻る時の移動速度
	const float		RESET_ACCEPTABLE		= 10.0f;		//初期位置の許容範囲
	const float		TRACKING_MOVE_SCALE		= 0.034f;		//追跡速度の倍率
	const float		TRACKING_ROT_SPEED		= 5.0f;			//追跡時の回転速度
	const float		TRACKING_CERTAIN		= 20.0f;		//追跡時の近付ける距離
	const float		ITEM_DISCOVERY_TIME		= 3.0f;			//アイテムの発見可能時間
	const float		ITEM_CERTAIN			= 40.0f;		//アイテムの感知可能範囲
	const float		ITEM_FOUND_SOUND_AREA	= 80.0f;		//発見時のサウンド再生範囲
	const float		FOUND_GAUGE_UP			= 0.3f;			//発見時のゲージ上昇値
	const float		FOUND_MIN_TIME			= 5.0f;			//発見時間の最低時間
	const float		RESPAWN_GRAVITY			= 0.5f;			//リスポーン時の重力
	const float		RESPAWN_POS_Y			= 10.0f;		//リスポーン時の高さ
	const float		ENEMY_JUMP_POWER		= 1.0f;			//ジャンプ力
	const int		ACT_PROBABILITY			= 50;			//行動の確率

protected:
	ENEMY_STATUS	m_State;								//状態
	FOUND_ID		m_FoundType;							//発見タイプ
	VECTOR			m_vSpawnPos;							//初期位置
	float			m_fGravityAdd;							//重力加算
	float			m_fCurrentStateTime;					//状態経過時間
	float			m_fStateTime;							//状態維持時間
	float			m_fNextMoveRot;							//進行方向
	float			m_fSpeed;								//移動速度
	float			m_fStunMoveSpeed;						//スタン状態の動きの速さ
	float			m_fPlFoundCound;						//プレイヤーを発見している時間
	float			m_fRespawnTimeCount;					//リスポーンまでの時間
	float			m_fHitBlockCount;						//ステージブロックに連続で当たっている時間
	bool			m_IsUse;								//生存フラグ
	bool			m_InitPosFlag;							//移動範囲の限界に到達
	bool			m_StunMoveFlag[3];						//スタン状態の動きフラグ
	bool			m_LookOnFlag;							//ロックオンされているフラグ
	bool			m_HitBlockFlag;							//ステージブロックに当たっている
	bool			m_ProgressImpossibleFlag;				//進行不能フラグ
	int				m_iItemIndex;							//発見してるアイテムの番号
	int				m_iEffectHnadle[ENEMY_REACTION_NUM];	//エフェクトハンドル

public:
	EnemyBase() {};
	~EnemyBase() {};

	void			Init();
	virtual	void	Init(VECTOR vPos, VECTOR vRot);
	void			Load(int originHandle);
	void			Start();
	virtual void	Step(Player& player, ItemManager& itemMana, StageBlockManager& block) {};
	void			Draw();
	void			Fin();

	//ブロックに衝突したフラグの設定
	void			SetHitBlockFlag(bool set) { m_HitBlockFlag = set; }
	//エフェクトの停止
	void			StopEffect();

protected:
	//移動処理
	virtual	void	Move(VECTOR plPos, float speed);
	//追跡
	void			TrackingMove(Player& player, VECTOR itemPos, float speed);
	//発見
	void			Discovery(Player& player, ItemManager& itemMana, StageBlockManager& block);
	//リスポーン
	void			Respawn();

	//プレイヤーに当たった時の処理
	void			HitPlayer(CollisionBase* hitBase);

public:
	//生存フラグを取得
	bool			GetIsUse() { return m_IsUse; }
	//地面/天井に当たった場合の重力のリセット
	void			HitGravityReset();
	//移動方向を取得するフラグ
	bool			GetInitPosFlag() { return m_InitPosFlag; }
	//プレイヤー発見
	//引数：発見回数を追加するのかのフラグ
	void			PlayerDiscovery(bool type = true);
	//座標を取得
	VECTOR			GetPos() { return m_vPos; }
	//状態を取得
	ENEMY_STATUS	GetState() { return m_State; }
	void			SetState(ENEMY_STATUS set) { m_State = set; }
	//現在の行動時間を取得
	void			SetCurrentStateTime(float set = 0.0f) { m_fCurrentStateTime = set; }
	//ロックオンフラグの取得
	bool			GetLookOnFlag() { return m_LookOnFlag; }
	//ロックオンフラグの設定
	void			SetLookOnFlag(bool set) { m_LookOnFlag = set; }
	//配置ブロックと当たった
	void			HitSetBlock();
	//発見対象の取得
	FOUND_ID		GetFoundType() { return m_FoundType; }
	//重力を設定
	void			SetGravity(float set) { m_fGravityAdd = set; }
	//進行不能フラグを取得
	bool			GetProgressImpossibleFlag() { return m_ProgressImpossibleFlag; }
	//進行不能フラグを設定
	void			SetProgressImpossibleFlag(bool set) { m_ProgressImpossibleFlag = set; }
	//アイテムを当てられた処理
	void			HitItem(){ m_State = ENEMY_STATE_STUN; }

protected:
	/*--------------------------------
			  アニメーション
	----------------------------------*/
	enum ANIME_ID
	{
		ANIMID_DEFFAULT,		//デフォ
		ANIMID_DEF_TO_SEARCH,	//デフォ→サーチ
		ANIMID_SEARCH,			//サーチ
		ANIMID_LOOK,			//見渡し
		ANIMID_SEARCH_TO_DEF,	//サーチ→デフォ

		ANIME_NUM,				//アニメ総数
	};
	//アニメーションの速度
	const float ANIMATION_SPEED[ANIME_NUM] =
	{
		1.0f,					//デフォ
		1.0f,					//デフォ→サーチ
		1.0f,					//サーチ
		0.7f,					//見渡し
		1.0f,					//サーチ→デフォ
	};

	void			Step();

private:
	//何もしてない
	void			ExecDafault();
	//デフォ→サーチ
	void			ChangeDafToSearch();
	//サーチ
	void			ExecSearch();
	//見渡し
	void			ExecLook();
	//サーチ→デフォ
	void			ChangeSearchToDef();
};
