#pragma once
#include"../Model/Model.h"
#include"../MyLib/MyLib.h"

class TitleModel :public CModel
{
public:
	enum ANIME_ID
	{
		ANIME_DEF,			//通常アニメーション
		ANIME_GO,			//発射アニメーション

		ANIMID_NUM,			//全アニメーション数
	};

	void Init();
	void Init(VECTOR setPos);
	void Load(const char* pFileName, float modelSize = 0.05f, ANIME_ID animeID = ANIME_DEF,float snimeSp=0.3f, ANIMSTATE animeState = ANIMSTATE_LOOP);
	void Step();
	void Draw();
	void Fin();

	//アニメーションを変更する
	void ChangeAnimation(ANIME_ID animeID, float animeSp = 1.0f, ANIMSTATE animState = ANIMSTATE_END);
	//アニメーションを再生する
	void RequestAnime(ANIME_ID animeID, float animeSp = 1.0f, ANIMSTATE animState = ANIMSTATE_END);

private:

};