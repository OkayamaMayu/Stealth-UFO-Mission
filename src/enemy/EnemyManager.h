#pragma once
#include"Enemy.h"
#include"../LoadStageData/LoadStageData.h"

class EnemyManager
{
private:
	//エネミーのタイプ
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_1,		//タイプ1
		ENEMY_TYPE_2,		//タイプ2

		ENEMY_TYPE_NUM
	};

	//ターゲット画像パス
	const string	ENEMY_TARGET_IMAGE_PATH				= "data/system/lockOn.png";
	//モデルパス
	const string	ENEMY_MODEL_PATH[ENEMY_TYPE_NUM]	=
	{
		"data/enemy/model/enemyType1/enemy.x",				//タイプ1
		"data/enemy/model/enemyType2/enemyType2.x",			//タイプ2
	};
	const float		TARGET_ROT_SPEED				= 0.01f;	//ターゲット画像の回転スピード
	const float		BILLBOARD_POS					= 0.5f;		//ビルボード表示の位置
	const float		BILLBOARD_SIZE					= 20.0f;	//ビルボード表示のサイズ
	const float		ENEMY_JUMP_POWER				= 1.0f;		//ジャンプ力
	const float		RANGE_CAMERA					= 0.2f;		//カメラとの距離
	const float		VIEW_RANGE_RANGE				= 60.0f;	//視界範囲
	const float		FIELD_OF_VIEW					= 200.0f;	//視野角
	const float		STUN_TIME						= -2.0f;	//気絶状態の時間
	const int		ENEMY_SIZE_RATIO				= 3;		//サイズの割合
	const int		ENEMY_SIZE_SCALE				= 2;		//サイズの倍率

private:
	EnemyType1* m_EnemyType1;								//エネミー1の情報
	VECTOR*		m_vEnemyPointPos;							//エネミー通過ポイント
	float		m_fTargetImageRot;							//投擲標的の画像回転
	int			m_iLookOnImageHandle;						//ロックオン画像のハンドル
	int			m_iEnemyMaxNum;								//敵の総数
	int			m_iEnemyType1Num;							//エネミータイプ１の数
	int			m_iEnemyPointNum;							//エネミー通過ポイントの数

public:
	EnemyManager() {};
	~EnemyManager() {};

	void		Init(LoadStageData &data);
	void		Load();
	void		Start();
	void		Step(Player& pl,ItemManager& itemMana, BackGround& block, bool gameOverFlag, bool clearFlag);
	void		Updata();
	void		Draw();
	void		Fin();

	//アイテムのターゲットに入るか判定
	void		ItemTarget(ItemManager& itemMana,  Player &pl, float rot, VECTOR vFocus, bool throwFlag);
	//エフェクトを停止する
	void		StopEffect();

	//-----------------------
	//		エネミー１
	//-----------------------
	//個別で取得
	EnemyType1& GetEnemyType1(int ID) { return m_EnemyType1[ID]; }
	//総数を取得
	int			GetEnemyType1MaxNum() { return m_iEnemyType1Num; }
	//エネミー通過ポイント
	int			GetEnemyPointNum() { return m_iEnemyPointNum;}
	//通過ポイントの座標を取得
	VECTOR		GetEnemyPoint(int ID) { return m_vEnemyPointPos[ID]; }
	//壁の衝突：該当エネミー1
	VECTOR		HitWall(int ID);
	//ジャンプ：該当エネミー
	void		HitJump(int ID);
	//サイズを取得
	VECTOR		GetEnemyType1Size() { return m_EnemyType1[0].GetSize(); }
};