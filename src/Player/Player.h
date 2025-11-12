#pragma once
#include"../Model/Model.h"
#include"../Item/ItemManager.h"
#include"PlayerRing.h"
#include"../MyLib/MyLib.h"

//プレイヤーの状態
enum PLAYR_STATE
{
	PLAYER_STATE_DEFFAULT,		//待機中
	PLAYER_STATL_MOVE,			//歩行中
	PLAYER_STATL_RUN,			//走り中
	PLAYER_STATE_JUNMP,			//ジャンプ中
	PLAYER_STATE_FALL,			//落下

	PLAYER_STATE_QTE,			//連打中
	PLAYER_STATE_LIBERATION,	//解放

	PLAYER_STATE_NUM,
};

//モデルのスケール
const float		MODEL_SIZE_SCALE						= 0.055f;						
//プレイヤーのサイズ
const VECTOR	PLAYER_SIZE								= { 80.0f * MODEL_SIZE_SCALE,290.0f * MODEL_SIZE_SCALE,60.0f * MODEL_SIZE_SCALE };

class Player :public CModel
{
private:
	const string	PLAYER_MODER_PATH					= "data/pl/model/pl_ver2.x";	//モデルパス
	const float		VISION_CHANGE_FADE_SPEED			= 0.05f;						//視点変更時のモデルの不透明度の変化速度
	const float		SAFETYAREA_MODEL_FADE				= 0.2f;							//影(安置)内のモデルの不透明度
	const float		SAFETYAREA_CHANGE_FADE_SPEED		= 0.05f;						//影(安置)内のモデルの不透明度の変化速度
	const float		SAFETYAREA_OUT_CHANGE_FADE_SPEED	= 0.1f;							//不透明度を通常状態に戻す速度
	const float		MOVE_SPEED_ADD						= 0.03f;						//移動速度の加算
	const float		RADIUS								= 5.0f;							//半径
	const float		FEVER_TIME							= 3.0f;							//無敵時間
	const float		MOVE_SOUND_COUNT[2]					= { 0.8f,0.32f };				//足音の調節
	const float		DASH_FOUND_GAUGE_UP_SPEED			= 0.05f;						//走り状態の注目度の上昇値
	const float		JUMP_FOUND_GAUGE_UP_SPEED			= 2.0f;							//ジャンプ時の注目度の上昇値
	const float		FOUND_GAUGE_DOWN_SPEED				= 0.5f;							//注目度の下降値
	const float		JUMP_PLAYER_SPEED					= 1.3f;							//ジャンプの上昇値
	const float		MOVE_PLAYER_SPEED					= 0.35f;						//移動スピード
	const float		ROTATION_PLAYER_SPEED				= 0.2f;							//回転速度
	const float		PL_DASH_SPEED						= MOVE_PLAYER_SPEED * 2.0f;		//走った時の速度
	const float		PL_SLOW_SPEED						= MOVE_PLAYER_SPEED * 0.8f;		//減速時の速度
	const float		PL_RUN_NUM							= 0.75f;						//倒し度の走り判定
	const float		ITEM_FOCUS_DISTANCE					= 50.0f;						//アイテムを光らせる
	const float		ITEM_GET_DISTANCE					= 5.0f;							//アイテムの取得距離
	const float		UFO_CATCH_SPEED						= 0.05f;						//UFOに吸われる速度
	const float		GAMEOVER_MODEL_SIZE_SPEED			= 0.0005f;						//ゲームオーバー時のモデルサイズの変更スピード
	const float		GAMEOVER_UP_SPEED					= 1.0f;							//ゲームオーバー時の上昇速度
	const float		THROW_ROT_X							= 45.0f;						//投げた瞬間のX軸回転
	const int		DEATH_HEIGHT_SCALE					= 5;							//落下時の高さの倍率

private:
	CollisionAABB	m_Collision;			//コリジョン情報
	PLAYR_STATE		m_State;				//プレイヤーの状態
	PlayerRing		PlayerRing;				//足元リング
	VECTOR			m_vNextRot;				//プレイヤーの回転先
	float			m_fSpeed;				//移動スピード
	float			m_fFeverCount;			//無敵時間のカウント
	float			m_fGravityAdd;			//プレイヤーにかかる重力
	float			m_fJumpCoolTime;		//ジャンプのクールタイム
	float			m_fModelFade;			//モデルの不透明度
	float			m_fMoveSoundCount;		//足音の調節
	bool			m_FeverTime;			//無敵時間のフラグ
	bool			m_JumpCTFlag;			//ジャンプのクールタイムフラグ
	bool			m_IsDraw;				//描画フラグ
	bool			m_ItemCatch;			//物をつかんでいる
	bool			m_SafetyAreaInFlag;		//影の中にいる
	bool			m_SetBlockFlag;			//ブロック配置モードフラグ
	bool			m_GameoverUfoFlag;		//UFOでゲームオーバーになるフラグ
	int				m_iJumpCount;			//ジャンプした回数

public:
	Player();
	~Player();

	void Init();
	void Init(VECTOR setPos, VECTOR setRot);
	void Load();
	void Start();
	void Step(VECTOR ufoPos, VECTOR vCameraRot, float fRot, bool cameraFlag, bool gameoverFlag);
	void Draw();
	void Fin();
	void Updata();

	//無敵時間フラグ取得
	bool		GetFeverTimeFlag() { return m_FeverTime; }
	//地面
	void		HitGround();
	//天井
	void		HitCeiling();
	//状態を取得
	PLAYR_STATE GetState() { return m_State; }
	//状態を設定
	void		SetState(PLAYR_STATE set) { m_State = set; }
	//速度を取得
	VECTOR		GetSpeed() { return m_vSpeed; }
	//アイテムを掴む：アイテムマネージャー, 角度, アイテムのIDを取得
	bool		CatchItem(ItemManager& itemMana,float rot,int& ID, VECTOR vForcus);
	//投げる
	bool		ThrowItem(ItemManager& itemMana, float camaraRot, float focusRot);
	//座標を取得
	VECTOR		GetPos() { return m_vPos; }
	//描画フラグを設定
	void		SetIsDraw(bool set) { m_IsDraw = set; }
	//安全地帯にいるかの取得
	bool		GetSafetyAreaInFlag() { return m_SafetyAreaInFlag;}
	//安全地帯にいるかの設定
	void		SetSafetyAreaInFlag(bool set) { m_SafetyAreaInFlag = set; }
	//無敵時間付与
	void		RequestFeverTime();
	//アイテムを持っているフラグ
	bool		GetItemCatch() { return m_ItemCatch; }
	//ブロック配置モードフラグの取得
	bool		GetSetBlockFlag() { return m_SetBlockFlag; }
	//ブロック配置モードフラグの設定
	void		SetSetBlockFlag(bool set) { m_SetBlockFlag = set; }
	//リング関係
	void		SetDrawRingFlag(bool set) { PlayerRing.SetDrawRingFlag(set); }
	//足元リングの座標を更新
	void		UpdateRingPos(VECTOR setPos) { PlayerRing.UpdateRingPos(setPos); }
	//リングの座標を設定
	void		SetRingPos(VECTOR set) { PlayerRing.SetRingPos(set); }
	//リングの座標を取得
	VECTOR		GetRingPos() { return PlayerRing.GetRingPos(); }

private:
	//移動処理
	void		Move(VECTOR vCameraRot, float fRot, bool cameraFlag);
	//ジャンプ処理
	void		Jump(bool cameraFlag);
	//無敵時間
	void		FaverTime();

	//当たった処理
	void		Hit(CollisionBase* hitCollision);

	//X軸の当たった処理
	void		HitX(CollisionBase hitCollision);
	//Y軸の当たった処理
	void		HitY(CollisionBase hitCollision);
	//Z軸の当たった処理
	void		HitZ(CollisionBase hitCollision);

//アニメ=====================================================
public:
	enum tagAnim
	{
		ANIMEID_T,				//Tスタンス
		ANIMID_DEFFAULT,		//デフォルト
		ANIMID_WALK,			//歩き
		ANIMID_RUN,				//走り
		ANIMID_JUMP_ASCENT,		//ジャンプ上昇
		ANIMID_JUMP_ABIDE,		//ジャンプ耐空
		ANIMID_JUMP_FALL,		//落下
		ANIMID_JUMP_LANDING,	//着地
		ANIMID_NOGRA,			//浮遊
		ANIMID_LANDING,			//解放後の着地

		ANIMID_NUM,				//全アニメーション数
	};

	const float ANIME_SPEED[ANIMID_NUM] =
	{
		0.0f,					//Tスタンス
		0.5f,					//デフォルト
		1.3f,					//歩き
		1.0f,					//走り
		1.0f,					//ジャンプ上昇
		1.0f,					//ジャンプ耐空
		1.0f,					//落下
		0.75f,					//着地
		1.0f,					//浮遊
		1.0f,					//浮遊の着地
	};
	const float WALK_BLEND_SPEED		= 1.5f;		//歩行へのブレンド速度
	const float RUN_BLEND_SPEED			= 1.5f;		//走りへのブレンド速度
	const float JUMP_ASCENT_BLEND_SPEED = 1.5f;		//ジャンプ上昇へのブレンド速度
	const float JUMP_ABIDE_BLEND_SPEED	= 2.5f;		//ジャンプ耐空へのブレンド速度
	const float JUMP_FALL_BLEND_SPEED	= 5.0f;		//ジャンプ落下へのブレンド速度
	const float NOGRA_BLEND				= 0.7f;		//浮遊へのブレンド

	//アニメのステップ
	void Step();

private:
	//ジャンプ上昇アニメーションが終わったら
	void EndJumpAscentAnimation();
	//ジャンプ耐空アニメーションが終わったら
	void EndJumpAbideAnimation();
	//落下アニメーションが終わったら
	void EndFallAnimation();
	//浮解放後着地アニメーションが終わったら
	void EndLandingAnimation();
};