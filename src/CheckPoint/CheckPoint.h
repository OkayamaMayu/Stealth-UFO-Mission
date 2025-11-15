#pragma once
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"

//リスポーン地点
struct ReSpawnInfo
{
	VECTOR	pos;		//座標
	float	rot;		//回転
};

class CheckPoint :public CModel
{
private:
	//アニメーションID
	enum ANIME_ID
	{
		ANIMEID_STAY,		//起動していない状態
		ANIMEID_ACTIVE,		//起動中
		ANIMEID_CHANGE,		//アニメーションの移行

		ANIME_NUM
	};
	//アニメーションの速度
	const float ANIMATION_SPEED[ANIME_NUM] =
	{
		1.0f,				//起動していない状態
		1.0f,				//起動中
		1.0f,				//アニメーションの移行
	};

	const float CHECKPOINT_HIT_SCALE	= 3.0f;		//チェックポイントの大きめ当たり判定のスケール
	const float MODEL_SEMITRANSPARENT	= 0.4f;		//モデルの半透明時の値
	const float MODEL_FADE_SPEED		= 0.05f;	//透明度の変化速度
	const float	MODEL_SIZE				= 5.0f;		//モデルサイズ
private:
	CollisionSphere	m_Collision;					//コリジョン情報
	ReSpawnInfo		m_ReSpawnInfo;					//リスポーン情報
	float			m_fModelFade;					//透明度
	bool			m_IsActive;						//使用中か
	bool			m_SemitransparentFlag;			//半透明にするフラグ
	bool			m_CheckPointFlag;				//チェックポイント起動フラグ
	int				m_iEffectHandle;				//エフェクトハンドル
	
public:
	//初期化
	void Init(VECTOR setPos, float setRot);
	//読み込み
	void Load(int originHandle);
	//読み込み後の設定
	void Start();
	//通常処理
	void Step();
	//描画
	void Draw();
	//終了
	void Fin();

	//プレイヤーが接触した時の処理
	bool IsHit();
	//使用済みか確認
	bool GetIsActive() { return m_IsActive; }
	//半透明のフラグセット
	void SetSemitransparentFlag(bool set) { m_SemitransparentFlag = set; }
	//リスポーン情報
	ReSpawnInfo		GetReSpawnInfo() { return m_ReSpawnInfo; }
	//チェックポイントフラグを取得
	bool			GetCheckPointFlag() { return m_CheckPointFlag; }
	//チェックポイントフラグを設定
	void			SetCheckPointFlag(bool set) { m_CheckPointFlag = set; }

	//当たった処理
	void			Hit(CollisionBase* hitCollision);

	//コリジョン情報を取得
	CollisionSphere	GetCollision() { return m_Collision; }
	//コリジョン情報の更新
	void			UpdateCollision();
	//コリジョン情報の設定
	void			SetCollision(CollisionSphere set) { m_Collision = set; }
	//コリジョンを登録
	void			RegisterCollision() { CollisionManager::GetInstance()->RegisterCollision(&m_Collision); }

private:
	//初期化
	void Init();
	//アニメの通常処理
	void AnimeStep();
	//透明度の変位
	void FadeStep();
};