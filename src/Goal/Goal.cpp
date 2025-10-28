#include"Goal.h"

void Goal::Init(VECTOR vPos)
{
	Init();

	//変数を設定
	m_vPos		= vPos;
	m_vScale	= MODEL_SCALE_V;
	m_vStartPos = m_vPos;

	Updata();
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

	Updata();
}

void Goal::Step()
{
	if (GetAnimeID() == ANIME_DEFAULT)
	{
		//ゴールが浮遊する
		if (m_FloatingFlag)
		{
			m_fFloating -= MOVE_SPEED;
			if (m_fFloating <= MOVE_MIN_LIMIT)
			{
				m_FloatingFlag = false;
			}
		}
		else
		{
			m_fFloating += MOVE_SPEED;
			if (m_fFloating >= MOVE_MAX_LIMIT)
			{
				m_FloatingFlag = true;
			}
		}

		//移動
		m_vPos.y = m_vStartPos.y + m_fFloating;
	}
	else
	{
		//ゴールが停滞
		if (m_FloatingFlag)
		{
			//一度高さを基準に合わせる
			if (Math::MatchSpecifiedNum(m_fFloating, DOWN_LIMIT, DOWN_SPEED))
			{
				//一定時間待機
				if(Math::MatchSpecifiedNum(m_fUpCount, WAIT_TIME,FRAME_TIME))
				{
					//上昇させる
					m_FloatingFlag = false;
					//サウンドを再生
					Sound::Play(SE_GOLA_FLY);
				}
			}
			//移動
			m_vPos.y = m_vStartPos.y + m_fFloating;
		}
		//上昇
		else
		{
			if (Math::MatchSpecifiedNum(m_fFloating, 0.0f, FLOATING_SPEED))
			{
				//一気に上昇させる
				//空に飛んでいく
				m_fUpSpeed	-= FLOATING_SPEED_ADD;
				m_vPos.y	-= m_fUpSpeed;

				//上昇している感をサイズを変更して上げる
				Math::MatchSpecifiedNum(m_vScale.x, 0.0f, FLOATING_SIZE_SPEED);
				Math::MatchSpecifiedNum(m_vScale.y, 0.0f, FLOATING_SIZE_SPEED);
				Math::MatchSpecifiedNum(m_vScale.z, 0.0f, FLOATING_SIZE_SPEED);

				if (m_vScale.z == 0.0f)
				{
					if (!m_NextSceneTrg)
					{
						//シーンを変更するためのフェードアウト
						Fade::GetInstance()->RequestFade(Fade::FADE_OUT, SCENE_CHANGE_FADEOUT_SPEED);
					}
					m_NextSceneTrg = true;
				}
			}
			else
			{
				//移動
				m_vPos.y = m_vStartPos.y + m_fFloating;
			}
		}
	}

	//回転
	m_vRot.y += ROT_SPEED;
	//回転が一周(180度)をこえると変数をリセット
	Math::RotReset(m_vRot);

	//ゴール演出--------------------------------------------------
	if (!m_IsGoalFlag)
		return;

	if (GetAnimeID() != ANIME_TRANSFORM)
	{
		//変形させる
		RequestAnimation(ANIME_TRANSFORM, ANIME_SPEED[ANIME_TRANSFORM], ANIMSTATE_END);
	}

	UpdateBlendRate();
	UpdateAnim();
	Updata();
}

void Goal::Fin()
{

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