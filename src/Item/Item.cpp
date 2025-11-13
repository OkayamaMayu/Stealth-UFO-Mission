#include"DxLib.h"
#include"../MyLib/MyLib.h"
#include"Item.h"

void Item::Init(VECTOR setPos, VECTOR setRot)
{
	Init();

	//座標の設定
	m_vSpawnPos		= setPos;
	m_vPos			= setPos;
	m_vNextPos		= m_vPos;
	m_vRot			= setRot;

	Update();
}

void Item::Init()
{
	m_IsUse				= true;
	m_Fall				= false;
	m_Throw				= false;
	m_CatchFlag			= false;
	m_FryUp				= true;
	m_FoundFlag			= false;
	m_FoundFlagPl		= false;
	m_EnemyHitFlag		= false;
	m_EnemyTargetFlag	= false;
	m_DrawFlag			= true;
	m_fBoundValue		= 0.0f;
	m_fBoundMax			= 0.0f;
	m_fGravityAdd		= 0.0f;
	m_fFryPos			= 0.0f;
	m_fSpeed			= 0.0f;
	m_fFoundCount		= 0.0f;
	m_fRespawnTimeCount = 0.0f;
	m_fAlpha				= 1.0f;

	CModel::Init();
}

void Item::Load(int originHandle1,int originHandle2)
{
	if (m_iHandle == -1)
	{
		m_iDrawHandle[0] = MV1DuplicateModel(originHandle1);
		m_iDrawHandle[1] = MV1DuplicateModel(originHandle2);
	}
}

void Item::Start()
{
	//サイズを設定
	m_vScale	= ITEM_SCALE[1];
	m_iHandle	= m_iDrawHandle[0];
	MV1SetScale(m_iHandle, m_vScale);

	Update();
}

void Item::Step(VECTOR plPos, float plRot, VECTOR plSpeed, bool plVisionFlag, bool blockModeFlag, float focusRot)
{
	//透明度から描画フラグを変更
	if (m_fAlpha > 0)
	{
		m_DrawFlag = true;
	}
	else
	{
		m_DrawFlag = false;
	}

	//アイテムが使われていなかったら一定時間経過で復活（エネミーに当てた後）
	bool respawnFlag = false;
	if (!m_IsUse)
		if (Math::MatchSpecifiedNum(m_fRespawnTimeCount, ITEM_RESPAWN_TIME, FRAME_TIME))
		{
			respawnFlag = true;
		}

	//再配置
	if (m_vNextPos.y <= DEATH_Y ||
		respawnFlag)
	{
		Init();

		//変数の設定
		m_fRespawnTimeCount = 0.0f;
		m_IsUse				= true;
		m_Throw				= false;
		m_fGravityAdd		= SPAWN_GRAVITY;
		m_vNextPos			= m_vSpawnPos;
		m_vNextPos.y		+= SPAWN_HEIGHT;
		m_vPos				= m_vNextPos;
		m_EnemyTargetFlag	= false;
	}

	//アイテムが未使用の場合は以下行わない
	if (!m_IsUse)
		return;

	//持っていたらプレイヤーの座標に合わせる
	m_vPlPos = plPos;

	//アイテムの大きさを変更
	m_vScale = ITEM_SCALE[1];
	//一人称カメラになっていたら
	if(m_CatchFlag&& plVisionFlag)
	{
		//アイテムの大きさを変更
		m_vScale = ITEM_SCALE[2];
	}
	
	if (m_CatchFlag)
	{
		m_FoundFlagPl	= true;
	}
	m_Fall				= false;

	//次の位置を更新
	m_vNextPos			= m_vPos;
	//マイナスだと落ちている
	if (m_fGravityAdd < 0 && !m_CatchFlag)
	{
		//0以下(数値が設定されていない)
		if (fabs(m_fBoundMax) <= 0.0f)
		{
			//バウンドの最大値を格納
			m_fBoundMax = m_vPos.y;
		}

		//落ちてる判定
		m_Fall = true;
	}

	//投げられた挙動
	ThrowMove();
	
	//掴まれてたら
	if(m_CatchFlag)
	{
		//つかんでる時の動き
		CatchMove(plPos, plRot, plSpeed, plVisionFlag, focusRot, blockModeFlag);
	}
	//掴まれてなかったら
	else
	{
		//透明度を設定
		Math::MatchSpecifiedNum(m_fAlpha, 1.0f, ALPHA_SPEED);
		MV1SetOpacityRate(m_iHandle, m_fAlpha);

		//重力をかける
		m_fGravityAdd	+= GRAVITY;
		m_vNextPos.y	+= m_fGravityAdd;
	}

	//発見される可能性のある時間を加算
	if (Math::MatchSpecifiedNum(m_fFoundCount, ITEM_FOUND_TIME, FRAME_TIME))
	{
		m_FoundFlag = false;
	}

	//座標更新
	Update();	
}

void Item::Draw()
{
	//ステージに存在していたら
	if(m_IsUse)
	{
		//保存中モデルの情報を更新
		MV1SetPosition(m_iDrawHandle[0], MV1GetPosition(m_iHandle));
		MV1SetPosition(m_iDrawHandle[1], MV1GetPosition(m_iHandle));
		MV1SetScale(m_iDrawHandle[0], MV1GetScale(m_iHandle));
		MV1SetScale(m_iDrawHandle[1], MV1GetScale(m_iHandle));

		//一定距離に近付いている状態ならモデルを変更
		if (!m_FoundFlagPl)
		{
			m_iHandle = m_iDrawHandle[0];
		}
		else
		{
			m_iHandle = m_iDrawHandle[1];
		}

		if(m_DrawFlag)
			MV1DrawModel(m_iHandle);
	}
}

void Item::Fin()
{

}

//========================================================

void Item::HitCeiling()
{
	m_fGravityAdd = 0.0f;
}

bool Item::HitGround(float posY)
{
	//落ちている状態でバウンド値が0以下なら(バウンド開始）
	if (m_Fall &&m_fBoundValue <= 0.0f)
	{
		//バウンド値を最大に
		m_fBoundValue = (m_fBoundMax - posY) / BOUND_POWER_RATIO / 2;
	}

	//バウンド
	m_fGravityAdd = m_fBoundValue;
	//バウンド値が0過上なら
	if (m_fBoundValue > 0.0f)
	{
		//バウンドするたびに半減
		m_fBoundValue /= 2;	

		if (m_fBoundValue >= BOUND_POWER_MIN)
		{
			//バウンドエフェクトの再生
			CEffekseerCtrl::Request(EFFECT_BOUND_RING1, m_vPos, false);
		}

		//一定距離にプレイヤーがいたらサウンドを再生
		if (Math::GetDistance(m_vPlPos, m_vPos) <= SOUND_RANGE)
			Sound::Play(SE_ITEM_BOUND);
	}
	else
	{
		m_fBoundMax		= 0.0f;
		m_fBoundValue	= 0.0f;
	}
	
	//投げた時の発見フラグ
	if (m_Throw|| m_Fall)
	{
		m_FoundFlag			= true;
		m_fFoundCount		= 0.0f;
	}

	if(m_EnemyHitFlag)
	{
		m_EnemyHitFlag		= false;
		m_EnemyTargetFlag	= false;

		return true;
	}

	return false;
}

//つかんでる時の動き
void Item::CatchMove(VECTOR plPos, float plRot, VECTOR plSpeed, bool plVisionFlag, float focusRot ,bool blockModeFlag)
{
	//アイテムの位置（プレイヤー座標基準）
	m_vPos = plPos;
	//一人称カメラ
	if (plVisionFlag)
	{
		//アイテムの位置を調節
		m_vPos = VAdd(m_vPos, CATCH_ITEM_POS[0]);

		//透明度の変更
		if(!blockModeFlag)
			Math::MatchSpecifiedNum(m_fAlpha, CATCH_FPS_ALPHA, CATCH_FPS_ALPHA_SPEED);
		else //ブロック配置モードの場合所持アイテムを消す
			Math::MatchSpecifiedNum(m_fAlpha, 0.0f, CATCH_SETBLOCK_ALPHA_SPEED);

		MV1SetOpacityRate(m_iHandle, m_fAlpha);
	}
	//二人称カメラ
	else
	{
		//アイテムの位置を調節
		m_vPos = VAdd(m_vPos, CATCH_ITEM_POS[1]);

		//アイテムの大きさを変更
		m_vScale = ITEM_SCALE[1];

		Math::MatchSpecifiedNum(m_fAlpha, 1.0f, CATCH_DEFAULT_ALPHA_SPEED);
		MV1SetOpacityRate(m_iHandle, m_fAlpha);
	}

	//プレイヤーを基準にした移動処理
	MATRIX pos1 = MGetTranslate(VScale(plPos, -1.0f));	//プレイヤーを原点に戻す
	MATRIX pos2 = MGetTranslate(plPos);					//プレイヤーを現在地に戻す
	MATRIX rotY = MGetRotY(plRot);						//プレイヤーをカメラの向きを参照して回転
	MATRIX rotX = MGetRotX(focusRot);					//プレイヤーをカメラの向きを参照して回転
	MATRIX plMove = MGetTranslate(plSpeed);				//プレイヤーの移動

	//行列の合成
	MATRIX matrix;
	matrix = MMult(pos1, rotY);
	//matrix = MMult(matrix, rotX);
	matrix = MMult(matrix, pos2);

	//座標の適応
	m_vNextPos = VTransform(m_vPos, matrix);

	//-----------------------------------------------------

	//浮遊
	if (m_FryUp)
	{
		m_fFryPos -= CATCH_FRY_SPEED;
		if (m_fFryPos <= -CATCH_FRY_LIMIT)
		{
			m_FryUp = false;
		}
	}
	else
	{
		m_fFryPos += CATCH_FRY_SPEED;
		if (m_fFryPos >= CATCH_FRY_LIMIT)
		{
			m_FryUp = true;
		}
	}

	//座標を加算
	m_vNextPos.y += m_fFryPos;

	m_vPos = m_vNextPos;

	Update();
}

//投げられた挙動
void Item::ThrowMove()
{
	//投げられている状態だったら以下実行
	if (!m_Throw)
	{
		m_EnemyTargetFlag = false;
		return;
	}

	//透明度を変更
	MV1SetOpacityRate(m_iHandle, 1.0f);
	if(!m_EnemyTargetFlag)
	{
		//一人称カメラを基準に移動量を計算
		VECTOR throwRot = { m_fThrowRot , m_vRot.y,0.0f };
		m_vSpeed		= Math::GetMoveVec(throwRot, m_fSpeed);

		//移動
		m_vNextPos		= VAdd(m_vNextPos, m_vSpeed);

		//減速していく
		Math::MatchSpecifiedNum(m_fSpeed, 0.0f, DECELERATION_SPEED);

		//0になったら投げられている状態をやめる
		if (m_fSpeed <= 0.0f)
		{
			m_Throw = false;
		}
	}
	else
	{
		//加速していく
		Math::MatchSpecifiedNum(m_fSpeed, ITEM_MAX_SPEED * THROW_SPEED_SCALE, ACCELERATION_SPEED);

		//移動
		m_vNextPos = VAdd(m_vNextPos, VScale(m_vSpeed, m_fSpeed));

		//一定の速度になったら終了
		if (m_fSpeed >= ITEM_MAX_SPEED * THROW_SPEED_SCALE)
		{
			m_Throw				= false;
			m_EnemyTargetFlag	= false;
		}
	}
}

//プレイヤー側で使うキャッチした瞬間の処理
void Item::RequestCatch()
{ 
	m_CatchFlag		= true;
	m_fFryPos		= 0.0f;
	m_fBoundValue	= 0.0f; 

	//サウンドの再生
	Sound::Play(SE_ITEM_GET);
}

//プレイヤー側で使う投げ処理
void Item::RequestThrow(float plRotY,float plRotX)
{ 
	//フラグの設定
	m_CatchFlag			= false;
	m_EnemyTargetFlag	= false;
	m_Throw				= true; 
	m_EnemyHitFlag		= true;

	//変数の設定
	m_fSpeed			= ITEM_MAX_SPEED; 
	m_fGravityAdd		= ITEM_BOUND_MAX_SPEED;
	m_vRot.y			= plRotY + Math::ChangeDegToRad(180);
	m_fThrowRot			= plRotX;

	//画面の中心座標の
	VECTOR screenPos	= { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2,0.0f };
	//スクリーン座標をワールド座標にする
	screenPos			= ConvScreenPosToWorldPos(screenPos);
	VECTOR throwRot		= { m_fThrowRot, m_vRot.y, 0.0f };
	VECTOR  sp			= Math::GetMoveVec(throwRot, THROW_INIT_SPEED);
	screenPos			= VAdd(screenPos, sp);

	//座標の適応
	m_vPos = m_vNextPos = screenPos;

	//アイテムの大きさを変更
	m_vScale = ITEM_SCALE[0];

	m_fAlpha = 0.0f;
}

//反射処理
void Item::Reflection(int mul)
{
	//角度の設定
	VECTOR setRot	= m_vRot;
	setRot.y		+= Math::ChangeDegToRad(REFLECTION_ROT * mul);
	m_vRot			= setRot;

	//反射時の減速
	m_fSpeed		*= REFLECTION_SPEED_SCALE;

	//サウンドを再生
	Sound::Play(SE_ITEM_BOUND);
}

void Item::SetTarget(VECTOR set)
{ 
	//ターゲット情報を設定
	m_vTargetPos		= set; 
	m_EnemyTargetFlag	= true; 

	//速度の計算
	VECTOR sp			= Math::GetMoving(m_vPos, set, 1.0f);
	m_vSpeed			= sp;

	//アイテムの移動方向を設定
	VECTOR itemRot		= m_vRot;
	itemRot.y			= Math::GetAngle(m_vPos, m_vTargetPos);
	m_vRot				= itemRot;

	//座標の変更
	VECTOR tagePos		= set;
	tagePos.y			+= TARGET_HEIGHT;
	m_vPos				= m_vNextPos = set;
}

void Item::HitEnemy()
{
	//フラグの設定
	m_EnemyHitFlag		= false;
	m_EnemyTargetFlag	= false;
	m_Throw				= false;
	m_IsUse				= false;

	//エフェクトの再生
	CEffekseerCtrl::Request(EFFECT_ENEMY_REACTION3, m_vPos,false);
}