#pragma once

//プレイ評価
enum STAGE_EVALUATION
{
	STAGE_EVALUATION_NODATA,					//記録なし

	STAGE_EVALUATION_S,							//評価_S
	STAGE_EVALUATION_A,							//評価_A
	STAGE_EVALUATION_B,							//評価_B
	STAGE_EVALUATION_C,							//評価_C

	STAGE_EVALUATION_NUM						//評価の数
};

//セーブデータ
struct SaveData
{
	int m_ClearStage	= -1;					//クリアしたステージを入れる
	int m_LastPlayStage = 0;					//最後に遊んだステージ番号

	STAGE_EVALUATION m_StageEvaluation[6] = {};	//プレイ評価
};

struct SaveLoad
{
	SaveData data;								//セーブデータ

	//書き込み
	void Save();
	
	//読み込み
	void Load();
};
