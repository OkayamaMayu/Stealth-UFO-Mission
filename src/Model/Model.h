#pragma once
#include"DxLib.h"

class CModel
{
public:
	//アニメの再生方法
	enum ANIMSTATE
	{
		ANIMSTATE_NORMAL,							//アニメーションを終了
		ANIMSTATE_LOOP,								//アニメーションをループ
		ANIMSTATE_END,								//アニメーションを最後で固定
	};
	const float		INIT_BLEND			= 1.0f;		//ブレンドの初期化
	const float		INIT_BLEND_SPEED	= 0.05f;	//ブレンドの速度

protected:

	struct ANIM_DATA
	{
		float m_fEndFrm;		//アニメ全再生時間
		float m_fFrm;			//アニメ再生時間
		float m_fSpd;			//アニメ再生速度
		int m_iHandle;			//アニメハンドル
		int m_iID;				//再生中のアニメID
		ANIMSTATE m_iState;		//アニメの状態
	};

	ANIM_DATA	m_AnimData;			//アニメ再生関連データ
	ANIM_DATA	m_OldAnimData;		//ブレンド用
	VECTOR		m_vPos;				//座標
	VECTOR		m_vNextPos;			//次の座標
	VECTOR		m_vRot;				//回転角度
	VECTOR		m_vSize;			//モデルサイズ
	VECTOR		m_vScale;			//拡大縮小率
	VECTOR		m_vSpeed;			//移動増加量
	float		m_fBlend;			//アニメーションのブレンド率
	float		m_fBlendSpd;		//ブレンドの速度
	bool		m_AnimeChangeFlag;	//アニメーション変更中のフラグ
	int			m_iHandle;			//モデルハンドル

public:
	//初期化
	virtual void	Init() = 0;
	//終了処理
	virtual void	Fin() = 0;
	//数値の更新処理
	virtual void	Update();
	//モデルの描画
	virtual void	Draw();

	//座標を取得
	VECTOR			GetPos() { return m_vPos; }
	//座標のセット
	virtual void	SetPos(VECTOR set) { m_vPos = set; }
	//次回の座標を取得
	VECTOR			GetNextPos() { return m_vNextPos; }
	//次回の座標をセット
	void			SetNextPos(VECTOR set) { m_vNextPos = set; }
	//角度を取得
	VECTOR			GetRot() { return m_vRot; }
	//角度を設定
	void			SetRot(VECTOR set) { m_vRot = set; }
	//移動量を設定
	void			SetSpeed(VECTOR set) { m_vSpeed = set; }
	//モデルサイズを取得
	VECTOR			GetSize() { return m_vSize; }
	//モデルサイズを設定
	void			SetSize(VECTOR set) { m_vSize = set; }

	//-----------------------------------------
	//			　画像読み込み関数
	//-----------------------------------------
	//モデル通常ロード
	//FileName	:	読み込みたいモデルデータ名
	bool			LoadModel(const char* pFileName);
	//モデルコピーロード：すでに読み込んであるモデルを複数表示したい場合に使用
	//iSrcHudl	:	読み込みたいモデルのハンドル
	bool			DuplicateModel(int iSrcHndl);
	//モデルデータ削除
	void			DeleteModel();

	//-----------------------------------------
	//			アニメーション関連
	//-----------------------------------------
	//アニメアップデート：アニメ更新処理、毎回呼ぶ
	void			UpdateAnim();
	//アニメリクエスト
	//iAnimID		:	アタッチするアニメ番号
	//iAnimSpd		:	アニメ再生速度
	//iAnimSrcHndl	:	アニメーションを持っているモデルのハンドル
	//					（-1ならばこのモデルの使用）
	//NameChack		:	別モデルのアニメを使用する場合、モデルのフレーム数が一致していない場合
	//					アニメをアタッチするか（iAnimSrd = -1の場合は無視）
	void			Request(int iAnimID, float iAnimSpd, int iAnimSrcHndl = -1, bool NameChack = false);
	//ループアニメリクエスト：アニメ最終フレームになったら最初に戻る
	void			RequestLoop(int iAnimID, float iAnimSpd, int iAnimSrcHndl = -1, bool NameChack = false);
	//エンドループリクエスト：アニメが最終フレームになったらそのままで止まる
	void			RequestEndLoop(int iAnimID, float iAnimSpd, int iAnimSrcHndl = -1, bool NameChack = false);
	//アニメ最後かチェックする
	bool			GetChecknd();
	//指定のアニメが最後かチェックする
	bool			GetChecknd(int iAnimeID);
	//再生中のアニメを取得
	int				GetAnimeID() { return m_AnimData.m_iID; }
	//アニメーションの再生(ブレンド)
	//AnimeID		：アタッチするアニメ番号
	//iAnimSpd		：アニメ再生速度
	//BlendSpd		：ブレンドの速度
	void			RequestAnimation(int AnimeID, float iAnimSpd, ANIMSTATE animState = ANIMSTATE_LOOP, float BlendSpd = 1.0f);
	//アニメーションブレンドの更新
	void			UpdateBlendRate();

private:
	//完全にアニメをデタッチ
	void			DetachAnim();
};
