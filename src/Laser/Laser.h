#pragma once
#include"../BackGround/StageBlockManager.h"
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"

class Laser :public CModel
{
public:
	//本体のモデルサイズ
	const VECTOR	LASER_SIZE			= { 24.0f * MODEL_SCALE,21.0f * MODEL_SCALE,100.0f * MODEL_SCALE };
	//最大長さ
	const float		LASER_MAX_LENGTH	= 100.0f;

private:
	CollisionLineSegment	m_Collision;		//コリジョン情報
	float					m_fLaserLength[2];	//レーザーの長さ（1は保存用）
	bool					m_OldPlayerHitFlag;	//プレイヤーが衝突した古いフラグ
	bool					m_PlayerHitFlag;	//プレイヤーが衝突したフラグ
	bool					m_BlockHitFlag;		//ブロックに当たったフラグ
	int						m_iEffectHandle;	//常時設定用EFFECTハンドル

public:
	void	Init(VECTOR vPos, float fRot);
	void	Init();
	void	Load(int originHandle);
	void	Start();
	void	Step();
	void	Draw();
	void	Fin();

public:
	//レーザーの長さを取得
	float	GetLaserLength() { return m_fLaserLength[0]; }
	//レーザーの長さを設定
	void	SetLaserLength(float set) { m_fLaserLength[0] = set; }

	//コリジョン情報の呼び出し
	CollisionLineSegment	GetCollision() { return m_Collision; }
	//コリジョン情報の設定
	void					SetCollision(CollisionLineSegment set) { m_Collision = set; }
	//コリジョンを登録
	void					RegisterCollision() { CollisionManager::GetInstance()->RegisterCollision(&m_Collision); }

	//当たった処理
	void Hit(CollisionBase* hitCollision);

private:
	//コリジョン情報の更新
	void UpdateCollision();
	//プレイヤーに当たった処理
	void HitPlayer(CollisionBase* hitCollision);
	//ブロックに当たった処理
	void HitBlock(CollisionBase* hitCollision);
};