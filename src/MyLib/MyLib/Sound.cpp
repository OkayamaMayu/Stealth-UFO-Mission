#include"sound.h"
#include"DxLib.h"
#include"../MyLib.h"

int Sound::handle[SOUND_MAX_NUM];
int Sound::vol[SOUND_MAX_NUM];
int Sound::setVol[SOUND_MAX_NUM];
int	Sound::fadeSpeed[SOUND_MAX_NUM];
bool Sound::fadeFlag[SOUND_MAX_NUM];//再生位置
int Sound::soundPos[SOUND_MAX_NUM];
bool Sound::soundPlayFlag;

//初期化
void Sound::Init()
{
	for (int i = 0; i < SOUND_MAX_NUM; i++)
	{
		//読み込み
		handle[i] = LoadSoundMem(SOUND_DATA[i].soundPath.c_str());

		//ボリュームの設定
		int vo = SOUND_DATA[i].soundVolume;
		SetVolume((SOUND_TYPE)i, 100);

		setVol[i] = vol[i];
		fadeSpeed[i] = 0;
		fadeFlag[i] = false;
	}

	soundPlayFlag = true;
}

//通常処理
void Sound::Step()
{
	for (int i = 0; i < SOUND_MAX_NUM; i++)
	{
		if (!fadeFlag[i])
		{
			continue;
		}

		if (Math::MatchSpecifiedNum(vol[i], setVol[i], fadeSpeed[i]))
		{
			fadeFlag[i] = false;
		}
		SetVolume((SOUND_TYPE)i, vol[i]);

		if (vol[i] == 0 && setVol[i] == 0)
		{
			StopSound((SOUND_TYPE)i);
		}

		//サウンドを再生するフラグ
		if (soundPlayFlag)
		{
			SetVolume((SOUND_TYPE)i, vol[i]);
		}
		else
		{
			SetVolume((SOUND_TYPE)i, 0);
		}
	}
}

//終了処理
void Sound::Fin()
{
	//サウンドを破棄
	for (int i = 0; i < SOUND_MAX_NUM; i++)
	{
		DeleteSoundMem(handle[i]);
	}
}

//=================================================================================

//サウンドの再生処理：種類
void Sound::Play(SOUND_TYPE type, bool playBackFlag)
{
	if (!soundPlayFlag)
		return;

	if (type < BGM_MAX_NUM)
	{
		//BGMの再生
		if (CheckSoundMem(handle[type]) == 0)
		{
			PlaySoundMem(handle[type], DX_PLAYTYPE_LOOP, true);

			//前回の位置から再生
			if (!playBackFlag)
				return;

			SetCurrentPositionSoundMem(soundPos[type], handle[type]);
		}
	}
	else
	{
		//SEの再生
		PlaySoundMem(handle[type], DX_PLAYTYPE_BACK, true);
	}
}

//サウンドの停止：種類
void Sound::StopSound(SOUND_TYPE type)
{
	soundPos[type] = (int)GetCurrentPositionSoundMem(handle[type]);

	//サウンドの停止
	StopSoundMem(handle[type]);
}

//サウンド全停止
void Sound::StopAll(int type)
{
	//BGM停止
	if (type == 2 || type == 0)
	{
		//BGM停止
		for (int i = 0; i < BGM_MAX_NUM; i++)
		{
			StopSound((SOUND_TYPE)i);
		}
	}

	//SE停止
	if (type == 2 || type == 1)
	{
		//0だと消さない
		if (type != 0)
		{
			//SE停止
			for (int i = BGM_MAX_NUM; i < SOUND_MAX_NUM; i++)
			{
				StopSound((SOUND_TYPE)i);
			}
		}
	}
}


//サウンドが流れているか
bool Sound::Check(SOUND_TYPE type)
{
	if (CheckSoundMem(handle[type]) == 1)
	{
		return true;   //鳴っていたら
	}
	else
	{
		return false;   //鳴っていなかったら
	}
}

//サウンドの音量調節：種類,音量(ﾊﾟｰｾﾝﾃｰｼﾞ)
void Sound::SetVolume(SOUND_TYPE type, int volume)
{
	vol[type] = volume;

	if (volume <= 0)
	{
		//音量の設定（0)
		ChangeVolumeSoundMem(0, handle[type]);
	}
	else
	{
		//音量の設定（0～100%）
		float vo = volume / 100.0f;
		float v = 255 * ((SOUND_DATA[type].soundVolume * vo) / 100);
		ChangeVolumeSoundMem((int)v, handle[type]);
	}
}

//フェードイン・フェードアウト：セットしたい音量, 速度
void Sound::SetFadeVolume(SOUND_TYPE type, int setVolume, int setSpeed)
{
	if (vol[type] == setVolume || fadeFlag[type])
	{
		return;
	}

	setVol[type] = setVolume;
	fadeSpeed[type] = setSpeed;
	fadeFlag[type] = true;
}