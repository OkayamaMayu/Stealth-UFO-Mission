#pragma once
#include"Laser.h"
#include"../LoadStageData/LoadStageData.h"

class LaserManager
{
private:
	//モデルパス
	const string	MODEL_PATH = "data/enemy/model/laser/laser.x";

private:
	Laser*			laser;			//レーザー情報
	int				m_iLaserNum;	//レーザーの数

public:
	void			Init(LoadStageData& data);
	void			Load();
	void			Start();
	void			Step();
	void			Draw();
	void			Fin();

public:
	//レーザーの情報を取得
	Laser&			GetLaser(int ID) { return laser[ID]; }
	//レーザーの数を取得
	int				GetLaserNum() { return m_iLaserNum; }
};