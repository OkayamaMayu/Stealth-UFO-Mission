#include"Model.h"
#include"../MyLib/MyLib.h"

#define VECTOR_ZERO VGet(0.0f,0.0f,0.0f)

//---------------------------
//			初期化
//---------------------------
void CModel::Init()
{
	//変数の初期化
	memset(&m_vPos, 0, sizeof(VECTOR));
	memset(&m_vNextPos, 0, sizeof(VECTOR));
	memset(&m_vRot, 0, sizeof(VECTOR));
	memset(&m_vSize, 0, sizeof(VECTOR));
	ZeroMemory(&m_AnimData, sizeof(m_AnimData));
	ZeroMemory(&m_OldAnimData, sizeof(m_OldAnimData));
	ZeroMemory(&m_EditAxisFlag, sizeof(m_EditAxisFlag));
	m_vScale				= MODEL_SCALE_V;
	m_fBlend				= INIT_BLEND;
	m_fBlendSpd				= INIT_BLEND_SPEED;
	m_iHandle				= -1;
	m_AnimData.m_iHandle	= -1;
	m_AnimData.m_iID		= -1;
	m_OldAnimData.m_iHandle = -1;
	m_OldAnimData.m_iID		= -1;
	m_AnimeChangeFlag		= false;
}

//---------------------------
//		　終了処理
//---------------------------
void CModel::Fin()
{
	DeleteModel();
}

//---------------------------
//		　更新処理
//---------------------------
void CModel::Update()
{
	m_vPos = m_vNextPos;

	if (m_iHandle == -1)
		return;

	//情報の更新
	MV1SetPosition(m_iHandle, m_vPos);
	MV1SetRotationXYZ(m_iHandle, m_vRot);
	MV1SetScale(m_iHandle, m_vScale);
}

//---------------------------
//		　描画処理
//---------------------------
void CModel::Draw()
{
	if (m_iHandle == -1)
		return;

	Update();
	MV1DrawModel(m_iHandle);
}

//-----------------------------------------
//			　画像読み込み関数
//-----------------------------------------

//データ読み込み
bool CModel::LoadModel(const char* pFileName)
{
	//モデル読み込み済み
	if (m_iHandle != -1)
		return false;

	m_iHandle = MV1LoadModel(pFileName);

	//モデル読み込み失敗
	if (m_iHandle == -1)
		return false;

	return true;
}

//コピーデータ読み込み
bool CModel::DuplicateModel(int iSrcHndl)
{
	//モデル読み込み済み
	if (m_iHandle != -1)
		return false;

	m_iHandle = MV1DuplicateModel(iSrcHndl);

	//モデル読み込み失敗
	if (m_iHandle == -1)
		return false;

	return true;
}

//モデルデータ削除
void CModel::DeleteModel()
{
	if (m_iHandle == -1)
		return;

	MV1DeleteModel(m_iHandle);
	m_iHandle = -1;
}

//-----------------------------------------
//			アニメーション関連
//-----------------------------------------

//アニメリクエスト
void CModel::Request(int iAnimID, float iAnimSpd, int iAnimSrcHndl, bool NameChack)
{
	//アニメ削除
	DetachAnim();

	//アニメ再生＆各種データをセット
	m_AnimData.m_iHandle = MV1AttachAnim(m_iHandle, iAnimID, iAnimSrcHndl, NameChack);
	m_AnimData.m_iID = iAnimID;
	m_AnimData.m_fEndFrm = MV1GetAnimTotalTime(m_iHandle, iAnimID);
	m_AnimData.m_fSpd = iAnimSpd;
	m_AnimData.m_iState = ANIMSTATE_NORMAL;
	m_AnimData.m_fFrm = 0.0f;
}

//ループアニメリクエスト
void CModel::RequestLoop(int iAnimID, float iAnimSpd, int iAnimSrcHndl, bool NameChack)
{
	Request(iAnimID, iAnimSpd, iAnimSrcHndl, NameChack);

	//ここだけループに変える
	m_AnimData.m_iState = ANIMSTATE_LOOP;
}

//エンドループアニメリクエスト
void CModel::RequestEndLoop(int iAnimID, float iAnimSpd, int iAnimSrcHndl, bool NameChack)
{
	Request(iAnimID, iAnimSpd, iAnimSrcHndl, NameChack);

	//ここだけエンドループに変える
	m_AnimData.m_iState = ANIMSTATE_END;
}

//アニメーションデタッチ
void CModel::DetachAnim()
{
	if (m_AnimData.m_iHandle != -1)
	{
		//アニメーションをデタッチ
		MV1DetachAnim(m_iHandle, m_AnimData.m_iHandle);
		m_AnimData.m_iHandle = -1;
	}
}

//アニメアップデート
void CModel::UpdateAnim()
{
	//アニメーションが再生されていない
	if (m_AnimData.m_iHandle == -1)
		return;

	//アニメーション速度更新
	m_AnimData.m_fFrm += m_AnimData.m_fSpd;

	if (m_AnimData.m_fFrm >= m_AnimData.m_fEndFrm)
	{
		//最終フレームまで来たら、状態に合わせて動作を変える
		switch (m_AnimData.m_iState)
		{
			//通常は終了させる
		case ANIMSTATE_NORMAL:
			DetachAnim();
			m_AnimData.m_fEndFrm = 0.0f;
			m_AnimData.m_fFrm = 0.0f;
			m_AnimData.m_fSpd = 0.0f;
			return;

			//ループは最初に戻るだけ
		case ANIMSTATE_LOOP:
			m_AnimData.m_fFrm = 0.0f;
			break;

			//エンドループは最後で止める
		case ANIMSTATE_END:
			m_AnimData.m_fFrm = m_AnimData.m_fEndFrm;
			break;

		default:
			break;
		}
	}

	//再生時間設定
	MV1SetAttachAnimTime(m_iHandle, m_AnimData.m_iHandle, m_AnimData.m_fFrm);

	//以下ブレンド処理==========================================
	//ブレンドアニメーションを更新
	if (m_OldAnimData.m_iHandle != -1)
	{
		if (m_OldAnimData.m_fFrm > m_OldAnimData.m_fEndFrm)
		{
			m_OldAnimData.m_fFrm = 0.0f;
		}
		MV1SetAttachAnimTime(m_iHandle, m_OldAnimData.m_iHandle, m_OldAnimData.m_fFrm);
	}
}

//-------------------------------
//	　アニメ最後かチェックする
//-------------------------------
bool CModel::GetChecknd()
{
	//アニメーションが再生されていない
	if (m_AnimData.m_iHandle == -1)
		return false;

	//最後になっていたらtrueを返す
	if (m_AnimData.m_fFrm >= m_AnimData.m_fEndFrm)
		return true;

	return false;
}

//-------------------------------
//指定のアニメが最後かチェックする
//-------------------------------
bool CModel::GetChecknd(int iAnimeID)
{
	//アニメーションが再生されていないか指定のアニメーションでないなら実行しない
	if (m_AnimData.m_iHandle == -1 || m_AnimData.m_iID != iAnimeID)
		return false;

	if (m_AnimData.m_fFrm >= m_AnimData.m_fEndFrm)
	{
		return true;
	}

	return false;
}

//---------------------------
//		ブレンドの再生
//---------------------------
void CModel::RequestAnimation(int AnimeID, float iAnimSpd, ANIMSTATE animState, float BlendSpd)
{
	if (m_AnimData.m_iID == AnimeID || m_AnimeChangeFlag)
		return;

	// 過去のアニメーションを保存
	m_OldAnimData.m_iHandle = m_AnimData.m_iHandle;

	// ブレンド率を初期化
	m_fBlend = 0.0f;
	if (m_OldAnimData.m_iHandle == -1)
	{
		m_fBlend = 1.0f;
	}

	// 再生させたいアニメーションをアタッチ
	m_AnimData.m_iHandle = MV1AttachAnim(m_iHandle, AnimeID);
	m_AnimData.m_iID = AnimeID;

	// 情報を保存
	m_OldAnimData.m_fFrm = m_AnimData.m_fFrm;
	m_OldAnimData.m_fSpd = m_AnimData.m_fSpd;
	m_OldAnimData.m_fEndFrm = m_AnimData.m_fEndFrm;

	// 再生時間を初期化
	m_AnimData.m_fFrm = 0.0f;
	m_AnimData.m_fSpd = iAnimSpd;
	m_AnimData.m_fEndFrm = MV1GetAnimTotalTime(m_iHandle, AnimeID);

	m_AnimeChangeFlag = true;
	m_fBlendSpd = INIT_BLEND_SPEED * BlendSpd;

	//再生タイプを設定
	m_AnimData.m_iState = animState;
}

//---------------------------
//		ブレンド更新
//---------------------------
void CModel::UpdateBlendRate()
{
	// 過去アニメーション情報があるかチェック
	if (m_OldAnimData.m_iHandle != -1)
	{
		// ブレンド率を加算
		m_fBlend += m_fBlendSpd;
		if (m_fBlend > 1.0f)
		{
			// ブレンド率が1.0を超えたら過去のアニメIDをデタッチする。
			MV1DetachAnim(m_iHandle, m_OldAnimData.m_iHandle);
			m_OldAnimData.m_iHandle = -1;
			m_fBlend = 1.0f;
		}
		else
		{
			// ブレンド率をアニメIDに設定
			MV1SetAttachAnimBlendRate(m_iHandle, m_OldAnimData.m_iHandle, 1.0f - m_fBlend);
		}
	}

	//変更中フラグを折る
	if (m_AnimeChangeFlag && m_fBlend >= 1.0f)
	{
		m_AnimeChangeFlag = false;
	}
	// 最新のアニメIDにブレンド率を設定
	MV1SetAttachAnimBlendRate(m_iHandle, m_AnimData.m_iHandle, m_fBlend);
}

//修正可能軸を設定する
void CModel::SetEditAxisFlag() {
	m_EditAxisFlag.x = m_EditAxisFlag.y = m_EditAxisFlag.z = true;
	if (m_vPos.x == m_vNextPos.x)m_EditAxisFlag.x = false;
	if (m_vPos.y == m_vNextPos.y)m_EditAxisFlag.y = false;
	if (m_vPos.z == m_vNextPos.z)m_EditAxisFlag.z = false;
}