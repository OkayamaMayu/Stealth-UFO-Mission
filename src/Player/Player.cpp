#include"DxLib.h"
#include"Player.h"
#include"../MyLib/MyLib.h"


//---------------------------------------------------------------

Player::Player()
{
	//プレイヤーの初期化
	memset(&m_vPos,0,sizeof(VECTOR));
	memset(&m_vRot,0,sizeof(VECTOR));
	memset(&m_vSpeed, 0, sizeof(VECTOR));
	memset(&m_vNextRot, 0, sizeof(VECTOR));

	//フラグの設定
	m_IsDraw			= true;
	m_FeverTime			= false;
	m_JumpCTFlag		= false;
	m_ItemCatch			= false;
	m_SafetyAreaInFlag	= false;
	m_SetBlockFlag		= false;

	//変数の初期化
	m_iHandle			= -1;
	m_iJumpCount		= 0;
	m_fJumpCoolTime		= 0.0f;
	m_fGravityAdd		= 0.0f;
	m_fFeverCount		= 0.0f;
	m_fSpeed			= 0.0f;
	m_fModelFade		= 1.0f;

	//状態の設定
	m_State				= PLAYER_STATE_DEFFAULT;
}

Player::~Player()
{
	Fin();
}

//=======================================================
void Player::Init()
{
	//プレイヤーの向き設定
	Update();

	//変数の初期化
	m_iJumpCount		= 0;
	m_fSpeed			= 0.0f;
	m_fFeverCount		= 0.0f;
	m_fGravityAdd		= 0.0f;
	m_fJumpCoolTime		= 0.0f;
	m_fMoveSoundCount	= 0.0f;
	m_fModelFade		= 1.0f;

	//フラグの設定
	m_IsDraw			= true;
	m_FeverTime			= false;
	m_JumpCTFlag		= false;
	m_ItemCatch			= false;
	m_SafetyAreaInFlag	= false;
	m_GameoverUfoFlag	= false;
	m_SetBlockFlag		= false;

	//状態の設定
	m_State				= PLAYER_STATE_DEFFAULT;

	//モデルの透明度の設定
	MV1SetOpacityRate(m_iHandle, m_fModelFade);
}

void Player::Init(VECTOR setPos, VECTOR setRot)
{
	//変数の初期化
	CModel::Init();
	Init();

	//座標の設定
	m_vPos		= setPos;
	m_vNextPos	= setPos;
	//回転の設定
	m_vRot		= setRot;
	m_vNextRot	= m_vRot;
	memset(&m_vSpeed, 0, sizeof(VECTOR));

	Update();

	//リングの初期化
	PlayerRing.Init(m_vPos, m_vRot);

	m_vSize = PLAYER_SIZE;
	m_vSize.y /= 2.0f;
}

void Player::Load()
{
	if (m_iHandle == -1)
	{
		m_iHandle = MV1LoadModel(PLAYER_MODER_PATH.c_str());
	}

	//リングの読み込み
	PlayerRing.Load();
}

void Player::Start()
{
	//大きさを変える
	m_vScale = Vector::MakeVec(MODEL_SIZE_SCALE);
	MV1SetScale(m_iHandle, m_vScale);

	//アニメーションの再生
	RequestAnimation(ANIMID_DEFFAULT, ANIME_SPEED[ANIMID_DEFFAULT]);

	//情報の更新
	Update();

	//リングの準備処理
	PlayerRing.Start();

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	m_Collision.SetKind(KIND_PLAYER);
	//構造体の設定
	UpdateCollision();
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase *hitCollision, COLLISION_AXIS axis) {Hit(hitCollision,axis);});
	//情報を登録
	CollisionManager::GetInstance()->RegisterCollision(&m_Collision);
}

void Player::Step(VECTOR ufoPos,VECTOR vCameraRot,float fRot, bool cameraFlag, bool gameoverFlag)
{
	m_GameoverUfoFlag = gameoverFlag;
	//ゲームオーバー時の処理
	if (gameoverFlag)
	{
		//完全に表示する
		m_fModelFade	= 1.0f;

		//UFOに合わせて移動
		m_vNextPos.x	= ufoPos.x;
		m_vNextPos.y	+= GAMEOVER_UP_SPEED;
		m_vNextPos.z	= ufoPos.z;

		//サイズを小さくしていく
		float scale		= m_vScale.x;
		Math::MatchSpecifiedNum(scale, 0.0f, GAMEOVER_MODEL_SIZE_SPEED);
		m_vScale		= Vector::MakeVec(scale);

		//見えなくなったら
		if (scale <= 0.0f)
		{
			//次のシーンに移行する
			Data::SetNextScene(SCENE_GAMEOVER);
		}
	}

	//IDが落下状態以下なら
	if (m_State <= PLAYER_STATE_FALL)
	{
		//重量がマイナス（落下する）状態なら
		if (m_fGravityAdd < 0)
		{
			//落下状態に
			m_State = PLAYER_STATE_FALL;
		}
	}

	//重力
	if (m_State <= PLAYER_STATE_FALL||m_State== PLAYER_STATE_LIBERATION)
	{
		//重力を加算
		m_fGravityAdd += GRAVITY;
	}

	//移動
	if(m_State<= PLAYER_STATE_FALL)
	{
		Move(vCameraRot, fRot, cameraFlag);
	}
	else if(m_State== PLAYER_STATE_QTE)
	{
		//移動量を計算
		m_vSpeed	= Math::GetMoving(m_vPos, ufoPos, UFO_CATCH_SPEED);
		m_vNextPos	= VAdd(m_vPos, m_vSpeed);
	}

	//落下状態かUFOに解放された後なら
	if (m_State <= PLAYER_STATE_FALL || m_State == PLAYER_STATE_LIBERATION)
	{
		//重力を座標に加算
		m_vNextPos.y += m_fGravityAdd;
	}

	//コリジョン情報の更新
	UpdateCollision();

	//無敵時間処理
	FaverTime();
	//座標更新
	//Update();
	//アニメ
	Step();

	//モデルの不透明度変更
	//一人称カメラ
	if (cameraFlag&&!gameoverFlag)
	{
		//一人称視点
		if (Math::MatchSpecifiedNum(m_fModelFade, 0.0f, VISION_CHANGE_FADE_SPEED))
		{
			m_IsDraw	= false;
		}
	}
	else
	{
		m_SetBlockFlag	= false;
		m_IsDraw		= true;
	}

	//影内だと透明度を変更
	if (m_SafetyAreaInFlag && !cameraFlag)
	{
		Math::MatchSpecifiedNum(m_fModelFade, SAFETYAREA_MODEL_FADE, SAFETYAREA_CHANGE_FADE_SPEED);
	}
	
	//プレイヤーが完全に表示される
	if (!cameraFlag && !m_SafetyAreaInFlag)
	{
		m_IsDraw = true;

		Math::MatchSpecifiedNum(m_fModelFade, 1.0f, SAFETYAREA_OUT_CHANGE_FADE_SPEED);
	}
	
	MV1SetOpacityRate(m_iHandle, m_fModelFade);

	//落下でのゲームオーバー処理
	if (m_vNextPos.y <= DEATH_Y / 2)
	{
		m_IsDraw = true;
	}
	//指定の高さまで落ちたらシーン移行
	if (m_vNextPos.y <= DEATH_Y * DEATH_HEIGHT_SCALE)
	{
		Data::SetNextScene(SCENE_GAMEOVER);
	}

	//リングの通常処理
	PlayerRing.Step();
}

void Player::Draw()
{
	Update();

	//リングの描画
	PlayerRing.Draw(m_GameoverUfoFlag);

	//二人称視点の時だけ移す
	if (m_IsDraw && m_iHandle != -1)
	{
		//本体モデルの描画
		MV1DrawModel(m_iHandle);
	}
}

void Player::Fin()
{
	PlayerRing.Fin();

	CModel::Fin();
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

void Player::Update()
{
	CModel::Update();

	PlayerRing.Update();
}

//=======================================================

//移動処理
void Player::Move(VECTOR vCameraRot,float fRot, bool cameraFlag)
{
	m_vNextPos = m_vPos;

	//ジャンプ処理
	Jump(cameraFlag);

	VECTOR stickLeft = {};
	//入力されていたら
	if (InputManager::CheckPlayerMove(stickLeft))
	{
		//ダッシュ中かつ通常カメラの時は速い
		if (m_State == PLAYER_STATL_RUN && !cameraFlag)
		{
			//最大スピードが二倍
			Math::MatchSpecifiedNum(m_fSpeed, PL_DASH_SPEED, MOVE_SPEED_ADD);

			//発見度が上がる
			Data::UpFoundGauge(DASH_FOUND_GAUGE_UP_SPEED);
		}
		else
		{
			//一人称カメラ
			if (cameraFlag)
			{
				//通常
				Math::MatchSpecifiedNum(m_fSpeed, PL_SLOW_SPEED, MOVE_SPEED_ADD);
			}
			else //通常カメラ
			{
				//通常
				if (m_State != PLAYER_STATE_JUNMP)
				{
					Math::MatchSpecifiedNum(m_fSpeed, MOVE_PLAYER_SPEED, MOVE_SPEED_ADD);
				}
				else
				{
					Math::MatchSpecifiedNum(m_fSpeed, PL_DASH_SPEED, MOVE_SPEED_ADD);
				}
			}

		}

		//加算用スピードをリセット
		m_vSpeed = VECTOR_ZERO;

		//ジャンプしていなかったら歩き状態に
		if (m_State != PLAYER_STATE_JUNMP && m_State != PLAYER_STATE_FALL)
		{
			m_State = PLAYER_STATL_MOVE;
		}
	}
	else
	{
		//加算用スピードを0に戻す
		Math::MatchSpecifiedNum(m_fSpeed, 0.0f, MOVE_SPEED_ADD);

		//移動スピード本体を0に戻す
		Math::MatchSpecifiedNum(m_vSpeed.x, 0.0f, MOVE_SPEED_ADD);
		Math::MatchSpecifiedNum(m_vSpeed.z, 0.0f, MOVE_SPEED_ADD);

		//空中にいなかったら
		if (m_State != PLAYER_STATE_JUNMP && m_State != PLAYER_STATE_FALL)
		{
			//プレイヤーの状態をデフォルトに戻す
			m_State = PLAYER_STATE_DEFFAULT;
		}

		//見つかっていなかったら
		if (!Data::GetFoundFlag())
		{
			//発見度が下がる
			Data::DownFoundGauge(FOUND_GAUGE_DOWN_SPEED);
		}
	}

	//プレイヤーが空中にいなかったら
	if (m_State != PLAYER_STATE_JUNMP && m_State != PLAYER_STATE_FALL)
	{
		//コントローラー情報
		if (fabs(stickLeft.x) >= PL_RUN_NUM || fabs(stickLeft.z) >= PL_RUN_NUM)
		{
			//ダッシュ状態に変更
			if (m_State == PLAYER_STATL_MOVE)
			{
				m_State = PLAYER_STATL_RUN;
			}
		}
		else
		{
			//歩き状態に変更
			if (m_State == PLAYER_STATL_RUN)
			{
				m_State = PLAYER_STATL_MOVE;
			}
		}
	}

	//移動
	m_vSpeed.x += m_fSpeed * stickLeft.x;
	m_vSpeed.z += m_fSpeed * stickLeft.z;

	//共通の計算----------------------------------------------------------------

	if (m_State >= PLAYER_STATL_MOVE && m_State <= PLAYER_STATE_FALL)
	{
		//行列の計算
		MATRIX pos1 = MGetTranslate(VScale(m_vPos, -1.0f));	//プレイヤーを原点に戻す
		MATRIX pos2 = MGetTranslate(m_vPos);				//プレイヤーを現在地に戻す
		MATRIX rot = MGetRotY(fRot);						//プレイヤーをカメラの向きを参照して回転
		MATRIX plMove = MGetTranslate(m_vSpeed);			//プレイヤーの移動

		//行列の合成
		MATRIX matrix;
		matrix = MMult(pos1, plMove);
		matrix = MMult(matrix, rot);
		matrix = MMult(matrix, pos2);

		//座標の変換
		VECTOR rotMove;
		rotMove = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
		m_vNextPos = VTransform(m_vPos, matrix);
	}

	//位置が変わっていたら
	if ((int)m_vNextPos.x != (int)m_vPos.x ||
		(int)m_vNextPos.z != (int)m_vPos.z)
	{
		//回転先を計算
		m_vNextRot.y = Math::GetAngle(m_vPos, m_vNextPos);
	}

	//判断用回転変数
	float fRotY = m_vRot.y;
	if(!Math::MatchSpecifiedNum(fRotY,m_vNextRot.y, ROTATION_PLAYER_SPEED))
	{
		VECTOR nextPos = VAdd(m_vPos, Math::GetMoveVec(m_vNextRot.y, -1.0f));
		//進行方向に少しずつ回転
		m_vRot = Math::GetMovingRot(m_vPos, m_vRot, nextPos, ROTATION_PLAYER_SPEED);
	}
	else
	{
		//固定する
		m_vRot = m_vNextRot;
	}

	//回転が一周(180度)をこえると変数をリセット
	Math::RotReset(m_vRot);
	Math::RotReset(m_vNextRot);
}

//ジャンプ
void Player::Jump(bool cameraFlag)
{
	//まだジャンプしていなくて通常カメラの場合
	if ( m_iJumpCount <= 0 && !cameraFlag )
	{
		//ジャンプ入力が入る
		if (InputManager::CheckPush(INPUT_JUMP))
		{
			//ジャンプ上昇量を入れる
			m_fGravityAdd = JUMP_PLAYER_SPEED;
			//ジャンプ回数を加算
			m_iJumpCount++;
			//ジャンプ状態にする
			m_State = PLAYER_STATE_JUNMP;

			//サウンドを再生
			Sound::Play(SE_PLAYER_JUMP);
		}
	}
}
//無敵時間付与
void Player::RequestFeverTime()
{
	m_FeverTime		= true;
	m_fFeverCount	= 0.0f;
}

//無敵時間
void Player::FaverTime()
{
	//無敵時間だったら
	if (!m_FeverTime)
		return;

	//時間を加算
	m_fFeverCount += FRAME_TIME;
	//無敵時間がキレる時間になったら
	if (m_fFeverCount >= FEVER_TIME)
	{
		//時間をリセット
		m_fFeverCount = 0.0f;

		//無敵時間をOFF
		m_FeverTime = false;

		//３Ｄモデルの不透明度を100%にする
		MV1SetOpacityRate(m_iHandle, 1.0f);
	}
}

//地面判定
void Player::HitGround()
{
	//ジャンプできるようにする
	m_iJumpCount	= 0;
	m_JumpCTFlag	= true;

	//重力を初期化
	m_fGravityAdd	= 0.0f;

	//落下中なら
	if (m_State == PLAYER_STATE_FALL)
	{
		//デフォルト状態に戻す
		m_State = PLAYER_STATE_DEFFAULT;
		//発見ゲージを上昇
		Data::UpFoundGauge(JUMP_FOUND_GAUGE_UP_SPEED);

		//エフェクトの表示
		CEffekseerCtrl::Request(EFFECT_BOUND_RING2, m_vPos, false);

		//サウンドを再生
		Sound::Play(SE_PLAYER_LANDING);
	}
}

//天井判定
void Player::HitCeiling()
{
	//重力の初期化
	m_fGravityAdd = 0.0f;
}

//アイテムを掴む
bool Player::CatchItem(ItemManager& itemMana, float rot, int& ID, VECTOR vForcus)
{
	//物をつかんでいる
	if (m_ItemCatch)
		return false;

	for (int itemInex = 0; itemInex < itemMana.GetItemMaxNum(); itemInex++)
	{
		//アイテムの情報を取得
		Item& item = itemMana.GetItem(itemInex);

		//使用されていない場合と捕まれている場合は実行しない
		if (!item.GetIsUse() || item.GetCatchFlag())
			continue;

		//掴める範囲にあるアイテムをリセット
		item.SetFoundPLFlag(false);

		//アイテムに近付いたら自動取得
		if (Math::GetDistance(m_vPos, item.GetPos()) <= ITEM_FOCUS_DISTANCE)
		{
			//一定距離内にあったら白アウトラインに変更
			item.SetFoundPLFlag(true);

			//取得範囲に入ったら
			if (Math::GetDistance(m_vPos, item.GetPos()) <= ITEM_GET_DISTANCE)
			{
				//アイテムを取得
				ID			= itemInex;
				m_ItemCatch = true;

				//終了
				return true;
			}
		}
	}

	return false;
}

//アイテムを投げる
bool Player::ThrowItem(ItemManager& itemMana, float camaraRot,float focusRot)
{
	//物をつかんでいない
	if (!m_ItemCatch)
		return false;

	for (int itemInex = 0; itemInex < itemMana.GetItemMaxNum(); itemInex++)
	{
		//アイテムの情報を取得
		Item& item = itemMana.GetItem(itemInex);

		//使用されていない場合と捕まれていない場合は実行しない
		if (!item.GetIsUse() || !item.GetCatchFlag())
			continue;

		//投げる入力
		if (InputManager::CheckPush(INPUT_ITEM_THROW))
		{
			//持ってない状態にする
			m_ItemCatch			= false;

			//画面の中心座標
			VECTOR screenPos	= { SCREEN_SIZE_X / 2,SCREEN_SIZE_Y / 2, 0.0f };
			//スクリーン座標をワールド座標にする
			screenPos			= ConvScreenPosToWorldPos(screenPos);

			//進行方向
			float cRot			= camaraRot ;
			//進行方向から移動ベクトルを計算する
			VECTOR goalSp		= Math::GetMoveVec(cRot, 1.0f);

			//向かう先の座標を計算
			VECTOR goalPos		= VAdd(m_vPos, goalSp);
			//アイテム座標を取得
			VECTOR itemPos		= item.GetPos();
			//アイテムの位置と進んだ位置から角度を求める
			float itemRot		= Math::GetAngle(goalPos, itemPos);

			//現在の角度を取得
			float itemRotX		= focusRot;
			//X軸に回転
			itemRotX			+= Math::ChangeDegToRad(THROW_ROT_X);

			//アイテムのを投げる
			item.RequestThrow(itemRot, focusRot);

			//サウンドを再生
			Sound::Play(SE_ITEM_THROW);

			//処理を終了する
			return true;
		}
	}

	return false;
}

//当たった処理
void Player::Hit(CollisionBase* hitCollision, COLLISION_AXIS axis) {
	//アイテムだと実行しない
	if (hitCollision->GetCollisionType() >= KIND_ITEM && hitCollision->GetCollisionType() > KIND_BLOCK)
		return;

	COLLISION_AXIS collisionAxis = axis;
	if (collisionAxis == AXIS_X || collisionAxis == AXIS_Z)
		collisionAxis = CollisionManager::GetInstance()->SelectModifyingAxis(&m_Collision, hitCollision);

	//当たった先の情報
	VECTOR hitPos = {};
	VECTOR hitSize = {};
	switch (hitCollision->GetCollisionType())
	{
	case TYPE_AABB: {
		CollisionAABB* hitAABB = static_cast<CollisionAABB*>(hitCollision);
		hitPos = hitAABB->GetCollision().centerPos;
		hitSize = hitAABB->GetCollision().size;
		break;
	}
	case TYPE_SPHERE: {
		CollisionSphere* hitSphere = static_cast<CollisionSphere*>(hitCollision);
		hitPos = hitSphere->GetCollision().centerPos;
		hitSize.y = hitSphere->GetCollision().radius;
		break;
	}
	default:
		break;
	}

	//軸を修正
	switch (collisionAxis)
	{
	case AXIS_Y:
		HitY(hitPos, hitSize);
		break;

	case AXIS_X:
		HitX(hitPos, hitSize);
		break;

	case AXIS_Z:
		HitZ(hitPos, hitSize);
		break;

	default:
		break;
	}
}

//X軸の当たった処理
void Player::HitX(VECTOR hitPos, VECTOR hitSize){
	if (m_vPos.x == m_vNextPos.x)return;

	//情報を取得
	AABB MyCollision = m_Collision.GetCollision();

	VECTOR playerPos = MyCollision.centerPos;
	VECTOR playerSize = MyCollision.size;
	if (playerPos.x < hitPos.x){
		//←側にあった
		playerPos.x -= (playerPos.x + playerSize.x) - (hitPos.x - hitSize.x);
	}
	if (playerPos.x > hitPos.x){
		//→側に当たった
		playerPos.x += (hitPos.x + hitSize.x) - (playerPos.x - playerSize.x);
	}

	//適応
	m_vNextPos.x = playerPos.x;
	//コリジョン情報の更新
	UpdateCollision();
}
//Y軸の当たった処理
void Player::HitY(VECTOR hitPos, VECTOR hitSize) {
	if (m_vPos.y == m_vNextPos.y)return;

	//情報を取得
	AABB MyCollision = m_Collision.GetCollision();

	VECTOR playerPos = MyCollision.centerPos;
	VECTOR playerSize = MyCollision.size;
	if (playerPos.y > hitPos.y) {
		//床に当たった
		playerPos.y += (hitPos.y + hitSize.y) - (playerPos.y - playerSize.y);
		HitGround();
	}
	if (playerPos.y < hitPos.y) {
		//天井に当たった
		playerPos.y -= (playerPos.y + playerSize.y) - (hitPos.y - hitSize.y);
		HitCeiling();
	}

	//適応
	m_vNextPos.y = playerPos.y;
	//座標を足元に移動
	m_vNextPos.y -= playerSize.y;
	//コリジョン情報の更新
	UpdateCollision();
}
//Z軸の当たった処理
void Player::HitZ(VECTOR hitPos, VECTOR hitSize) {
	if (m_vPos.z == m_vNextPos.z)return;

	//情報を取得
	AABB MyCollision = m_Collision.GetCollision();

	VECTOR playerPos = MyCollision.centerPos;
	VECTOR playerSize = MyCollision.size;
	if (playerPos.z < hitPos.z) {
		playerPos.z -= (playerPos.z + playerSize.z) - (hitPos.z - hitSize.z);
	}
	if (playerPos.z  > hitPos.z ) {
		playerPos.z += (hitPos.z + hitSize.z) - (playerPos.z - playerSize.z);
	}

	//適応
	m_vNextPos.z = playerPos.z;
	//コリジョン情報の更新
	UpdateCollision();
}

//コリジョン情報の更新
void Player::UpdateCollision() {
	AABB setCollision = m_Collision.GetCollision();
	//サイズを設定
	setCollision.size = m_vSize;
	//中心座標を設定
	setCollision.centerPos = m_vNextPos;
	setCollision.centerPos.y += setCollision.size.y;	
	//情報を更新
	m_Collision.SetCollision(setCollision);
}

/*==============================
		アニメーション
================================*/
//アニメのステップ
void Player::Step()
{
	//アニメーションが自動で移行する処理
	switch (m_AnimData.m_iID)
	{
		//------------------------------------------------------------

	case ANIMID_JUMP_ASCENT:				//ジャンプ上昇
		EndJumpAscentAnimation();
		break;

	case ANIMID_JUMP_FALL:			//落下
		EndFallAnimation();
		break;

	default:
		break;
	}

	//プレイヤーの行動で変化
	switch (m_State)
	{
	case PLAYER_STATE_DEFFAULT:		//デフォルト
		m_fMoveSoundCount = 0.0f;

		//着地アニメーションが最後まで再生されているか着地以外ならデフォルトアニメーションの再生
		if (GetChecknd(ANIMID_JUMP_LANDING) ||GetAnimeID() != ANIMID_JUMP_LANDING)
			RequestAnimation(ANIMID_DEFFAULT, ANIME_SPEED[ANIMID_DEFFAULT], ANIMSTATE_LOOP);
		break;

	case PLAYER_STATL_MOVE:			//歩行
		//サウンドの再生
		if (Math::MatchSpecifiedNum(m_fMoveSoundCount, MOVE_SOUND_COUNT[0], FRAME_TIME))
		{
			m_fMoveSoundCount = 0.0f;
			Sound::Play(SE_PLAYER_MOVE);
		}

		//歩行アニメーションの再生
		RequestAnimation(ANIMID_WALK, ANIME_SPEED[ANIMID_WALK], ANIMSTATE_LOOP, WALK_BLEND_SPEED);
		break;

	case PLAYER_STATL_RUN:			//走る
		//サウンドの再生
		if (Math::MatchSpecifiedNum(m_fMoveSoundCount, MOVE_SOUND_COUNT[1], FRAME_TIME))
		{
			m_fMoveSoundCount = 0.0f;
			Sound::Play(SE_PLAYER_MOVE);
		}

		//走りアニメーションの再生
		RequestAnimation(ANIMID_RUN, ANIME_SPEED[ANIMID_RUN], ANIMSTATE_LOOP, RUN_BLEND_SPEED);
		break;

	case PLAYER_STATE_JUNMP:		//ジャンプ
		//ジャンプ上昇アニメーションの再生
		RequestAnimation(ANIMID_JUMP_ASCENT, ANIME_SPEED[ANIMID_JUMP_ASCENT], ANIMSTATE_END, JUMP_ASCENT_BLEND_SPEED);
		break;

	case PLAYER_STATE_FALL:			//落下
		//現在再生中のアニメーション
		switch (m_AnimData.m_iID)
		{
		case ANIMID_JUMP_ABIDE:		//ジャンプ下降
		case ANIMID_JUMP_FALL:		//耐空
			//落下アニメーションの再生
			EndJumpAbideAnimation();
			break;
		
		default:
			//耐空アニメーションの再生
			RequestAnimation(ANIMID_JUMP_ABIDE, ANIME_SPEED[ANIMID_JUMP_ABIDE], ANIMSTATE_END, JUMP_ABIDE_BLEND_SPEED);
			break;
		}

		break;

	case PLAYER_STATE_QTE:			//連打中
		//浮遊アニメーションの再生
		RequestAnimation(ANIMID_NOGRA, ANIME_SPEED[ANIMID_NOGRA], ANIMSTATE_LOOP, NOGRA_BLEND);
		break;

	case PLAYER_STATE_LIBERATION:	//解放
		//現在再生中のアニメーション
		switch (m_AnimData.m_iID)
		{
		case ANIMID_NOGRA:			//浮遊
			RequestAnimation(ANIMID_LANDING, ANIME_SPEED[ANIMID_LANDING], ANIMSTATE_END);
			break;

		case ANIMID_LANDING:		//解放後の着地
			EndLandingAnimation();
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	//アニメ情報の更新
	UpdateBlendRate();
	UpdateAnim();
}

//============================================================

//ジャンプ
void Player::EndJumpAscentAnimation()
{
	//アニメ終了後
	if (GetChecknd(ANIMID_JUMP_ASCENT))
	{
		//プレイヤーの状態を落下状態にする
		m_State = PLAYER_STATE_FALL;
	}
}
//落下
void Player::EndJumpAbideAnimation()
{
	//アニメ終了後
	if (GetChecknd(ANIMID_JUMP_ABIDE))
	{
		//落下アニメーションを再生する
		RequestAnimation(ANIMID_JUMP_FALL, ANIME_SPEED[ANIMID_JUMP_FALL], ANIMSTATE_LOOP, JUMP_FALL_BLEND_SPEED);
	}
	
}
//落下
void Player::EndFallAnimation()
{
	if (m_State != PLAYER_STATE_FALL && m_State != PLAYER_STATE_QTE)
	{
		//着地アニメーションを再生する
		RequestEndLoop(ANIMID_JUMP_LANDING, ANIME_SPEED[ANIMID_JUMP_LANDING]);
	}
}

//============================================================

//浮遊の着地
void Player::EndLandingAnimation()
{
	//アニメ終了後
	if (GetChecknd(ANIMID_LANDING))
	{
		//プレイヤーの状態をデフォルト状態にする
		m_State = PLAYER_STATE_DEFFAULT;
	}
}