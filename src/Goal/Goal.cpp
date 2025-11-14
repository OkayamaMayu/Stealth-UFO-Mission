#include"Goal.h"

void Goal::Init(VECTOR vPos)
{
	Init();

	//変数を設定
	m_vPos		= vPos;
	m_vNextPos	= vPos;
	m_vScale	= MODEL_SCALE_V;
	m_vSize		= GOAL_SIZE;
	m_vSize.y	/= 2.0f;
	m_vStartPos = m_vPos;

	//コリジョン情報の設定
	m_Collision.SetOwner(this);
	m_Collision.SetKind(KIND_GOAL);
	//構造体の設定
	UpdateCollision();
	//当たった時の処理
	m_Collision.SetOnHitCollback([this](CollisionBase* hitCollision) {Hit(hitCollision); });
	//情報を登録
	CollisionManager::GetInstance()->RegisterCollision(&m_Collision);
	Update();
}
void Goal::Init()
{
	//変数の初期化
	m_fFloating		= 0.0f;
	m_fUpSpeed		= 0.0f;
	m_fUpCount		= 0.0f;
	m_IsGoalFlag	= false;
	m_NextSceneTrg	= false;
	m_FloatingFlag	= true;
	m_iHandle		= -1;

	memset(&m_vRot, (int)0.0f, sizeof(VECTOR));
}

void Goal::Load()
{
	m_iHandle = MV1LoadModel(GOAL_MODEL_PATH.c_str());
}

void Goal::Start()
{
	//ゴールにエフェクトを発生
	VECTOR vPos = m_vPos;
	vPos.y		+= GOAL_EFFECT_HEIGHT;
	m_iEffectHandle = CEffekseerCtrl::Request(EFFECT_GOAL_POINT, vPos, true);
	RequestEndLoop(ANIME_DEFAULT, ANIME_SPEED[ANIME_DEFAULT]);

	Update();
}

void Goal::Step(){
	m_vNextPos = m_vPos;

	DefaultMove();
	GoalMove();

	//回転
	m_vRot.y += ROT_SPEED;
	//回転が一周(180度)をこえると変数をリセット
	Math::RotReset(m_vRot);

	//ゴール演出--------------------------------------------------
	if (!m_IsGoalFlag)return;

	UpdateBlendRate();
	UpdateAnim();
	Update();

	if (GetAnimeID() == ANIME_TRANSFORM)return;
	//変形させる
	RequestAnimation(ANIME_TRANSFORM, ANIME_SPEED[ANIME_TRANSFORM], ANIMSTATE_END);
}

void Goal::Fin(){
	//当たり判定を削除
	CollisionManager::GetInstance()->UnRegisterCollision(&m_Collision);
}

//通常の動き
void Goal::DefaultMove() {
	if (GetAnimeID() != ANIME_DEFAULT)return;

	//ゴールが浮遊する
	if (m_FloatingFlag)m_fFloating -= MOVE_SPEED;
	if (!m_FloatingFlag)m_fFloating += MOVE_SPEED;
	
	//移動
	m_vNextPos.y = m_vStartPos.y + m_fFloating;

	if (m_FloatingFlag && m_fFloating <= MOVE_MIN_LIMIT)m_FloatingFlag = false;
	if (!m_FloatingFlag && m_fFloating >= MOVE_MAX_LIMIT)m_FloatingFlag = true;
}
//ゴール時の動き
void Goal::GoalMove() {
	if (GetAnimeID() == ANIME_DEFAULT)return;

	//ゴールが停滞
	if (m_FloatingFlag) {
		//一度高さを基準に合わせる
		Math::MatchSpecifiedNum(m_fFloating, DOWN_LIMIT, DOWN_SPEED);

		//移動
		m_vNextPos.y = m_vStartPos.y + m_fFloating;

		if (m_fFloating != DOWN_LIMIT)return;

		//一定時間待機
		Math::MatchSpecifiedNum(m_fUpCount, WAIT_TIME, FRAME_TIME);
		if (m_fUpCount != WAIT_TIME)return;

		//上昇させる
		m_FloatingFlag = false;
		//サウンドを再生
		Sound::Play(SE_GOLA_FLY);
	}
	else{
		//上昇
		Math::MatchSpecifiedNum(m_fFloating, 0.0f, FLOATING_SPEED);
		if (m_fFloating== 0.0f){
			//一気に上昇させる
			//空に飛んでいく
			m_fUpSpeed -= FLOATING_SPEED_ADD;
			m_vNextPos.y -= m_fUpSpeed;

			//上昇している感をサイズを変更して上げる
			Math::MatchSpecifiedNum(m_vScale.x, 0.0f, FLOATING_SIZE_SPEED);
			Math::MatchSpecifiedNum(m_vScale.y, 0.0f, FLOATING_SIZE_SPEED);
			Math::MatchSpecifiedNum(m_vScale.z, 0.0f, FLOATING_SIZE_SPEED);

			if (m_vScale.z != 0.0f)return;
			if (m_NextSceneTrg)return;
			
			//シーンを変更するためのフェードアウト
			Fade::GetInstance()->RequestFade(Fade::FADE_OUT, SCENE_CHANGE_FADEOUT_SPEED);
			
			m_NextSceneTrg = true;
		}
		else{
			//移動
			m_vNextPos.y = m_vStartPos.y + m_fFloating;
		}
	}
}

void Goal::HitGoal()
{
	m_IsGoalFlag	= true;
	m_FloatingFlag	= true;

	//サウンドを停止する
	Sound::StopSound(SE_LOOP_SET_BLOCK_CHARGE);
	Sound::StopSound(SE_LOOP_ENEMY_FOUND);
	Sound::StopSound(SE_LOOP_UFO);

	//エフェクトを停止する
	CEffekseerCtrl::Stop(m_iEffectHandle);

	//フラッシュ演出を入れる
	Fade::GetInstance()->RequestFlash();
}

//当たった処理
void Goal::Hit(CollisionBase* hitCollision) {
	//プレイヤー以外なら実行しない
	if (hitCollision->GetKind() != KIND_PLAYER)return;

	HitGoal();
	CEffekseerCtrl::Request(EFFECT_GOAL, m_vPos, false);
	Sound::Play(SE_GOLA_HIT);
}

//コリジョン情報の更新
void Goal::UpdateCollision() {
	AABB setCollision = m_Collision.GetCollision();
	//サイズを設定
	setCollision.size = m_vSize;
	//中心座標を設定
	setCollision.centerPos = m_vNextPos;
	setCollision.centerPos.y += setCollision.size.y;
	//情報を更新
	m_Collision.SetCollision(setCollision);
}