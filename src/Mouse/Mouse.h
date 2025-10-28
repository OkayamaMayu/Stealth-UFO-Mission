#pragma once
#include"DxLib.h"
#include"../MyLib/MyLib.h"

class Mouse
{
private:
	const string	MOUSE_IMAGE_PATH		=		//画像パス
	{ "data/system/mouseCursor_35,44.png" };	
	const float		MOUSECURSOR_DRAW_TIME	= 3.0f;	//マウスカーソルの表示時間
	const float		MOUSE_COLLISION_SIZE	= 1.0f;	//マウスの当たり判定の大きさ
	const int		CURSOR_FADEIN_SPEED		= 20;	//カーソルが現れる速度
	const int		CURSOR_FADEOUT_SPEED	= 5;	//カーソルが消える速度

private:
	Mouse();
	~Mouse();

	static Mouse*	m_Instance;

	VECTOR			m_vMousePos[2];				//マウスの座標：0,メイン / 1,保存 
	float			m_fMouseDrawTime;			//カーソルを表示する時間
	bool			m_MouseCursorDrawFlag;		//カーソルの表示管理フラグ
	bool			m_MouseMoveFlag;			//マウスが動いているか
	int				m_iHandle;					//画像ハンドル
	int				m_iMouseCursorAlpha;		//カーソルの透明度（0~100％）

public:
	static Mouse*	GetInstance();	//インスタンスの取得

	void	Init();
	void	Load();
	void	Step();			//他より早く呼び出す
	void	Draw();			//マウスカーソルを表示
	void	Fin();

	//当たり判定：対象の座標(中心), 対象のサイズ(直径)
	bool	Collision(VECTOR vPos, VECTOR vSize);
	//当たり判定：対象の座標(中心), 対象のサイズ(半径)
	bool	Collision(VECTOR vPos, float fSize);
	//マウスの座標を取得
	VECTOR	GetMousePos();
	//マウスリセット
	void	Reset();
	//マウス描画フラグの取得
	bool	GetMouseCursorDrawFlag() { return m_MouseCursorDrawFlag; }
	//マウスが動いているか調べる
	bool	GetMouseMove() { return m_MouseMoveFlag; }

private:
	static void	Create();		//インスタンスの生成
	static void	Destroy();		//インスタンスの削除
};