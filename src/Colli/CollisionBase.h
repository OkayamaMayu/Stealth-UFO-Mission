#pragma once
#include<functional>
#include"../Model/Model.h"
using namespace std;

//当たり判定のタイプ
enum COLLISION_TYPE {
	TYPE_NEUTRAL = -1,

	TYPE_AABB,		//箱
	TYPE_SPHERE,	//球
	TYPE_LINE		//線分
};

//識別ID
enum COLLISION_KIND {
	KIND_STAGE		= 0,	//ステージブロック
	KIND_PLAYER,			//プレイヤー
	KIND_GOAL,				//ゴール
	KIND_ENEMY		= 100,	//エネミー
	KIND_ITEM		= 200,	//アイテム
	KIND_BLOCK		= 300,	//配置ブロック
	KIND_CHECKPOINT = 400,	//チェックポイント
};

class CollisionBase {
protected:
	COLLISION_TYPE	m_CollisionType;				//コリジョンタイプ
	CModel*			m_Owner;						//オーナーのアドレス
	bool			m_IsCollision;					//コリジョン有効フラグ
	int				m_iKind;						//kindメンバー
	function<void(CollisionBase*)> m_OnHitCollback;	//ヒット時のコールバック関数

public:
	CollisionBase();																	//コンストラクタ
	COLLISION_TYPE	GetCollisionType() { return m_CollisionType; }						//コリジョンタイプを取得
	CModel*			GetOwner() { return m_Owner; }										//オーナーのアドレスを取得
	bool			IsCollision() { return m_IsCollision; }								//コリジョン有効フラグを取得
	int				GetKind() { return m_iKind; }										//kindメンバーを取得
	void			HitCollision(CollisionBase* base) {	if(m_OnHitCollback) m_OnHitCollback(base); }

	void SetOwner(CModel* set) { m_Owner = set; }										//オーナーのアドレスを設定
	void SetIsCollision(bool set) { m_IsCollision = set; }								//コリジョン有効フラグを設定
	void SetKind(int set) { m_iKind = set; }											//kindメンバーを設定
	void SetOnHitCollback(function<void(CollisionBase*)> set) { m_OnHitCollback = set; }//ヒット時のコールバック関数を設定
};