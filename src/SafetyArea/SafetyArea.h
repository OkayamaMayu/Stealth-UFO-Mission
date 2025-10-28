#pragma once
#include"../MyLib/MyLib.h"
#include"../Player/Player.h"

const float SAFETYAREA_SIZE = 7.0f;	//安全地帯のサイズ

class SafetyArea
{
private:
	const float MODEL_ALPHA = 0.8f;	//モデルの透明度

private:
	VECTOR	m_vPos;					//座標
	bool	m_PlIn;					//プレイヤーが入っている
	int		m_iHandle;				//モデルハンドル
	int		m_iEffectHandle;		//常時設定用エフェクトハンドル

public:
	void Init(VECTOR vPos);
	void Load(int originHandle);
	void Start();
	void Step(Player& pl);
	void Draw();
	void Fin();

	//座標を取得
	VECTOR GetPos() { return m_vPos; }
	//プレイヤーが中に入っているフラグの設定
	void SetPlInFlag(bool set) { m_PlIn = set; }
};