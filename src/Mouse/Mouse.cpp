#include"Mouse.h"
#include"../MyLib/MyLib.h"

Mouse* Mouse::m_Instance = nullptr;

//コンストラクタ
Mouse::Mouse()
{
}

//デストラクタ
Mouse::~Mouse()
{
}

//インスタンスの生成
void Mouse::Create()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Mouse();
	}
}

//インスタンスの削除
void Mouse::Destroy()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

//インスタンスの所得
Mouse* Mouse::GetInstance()
{
	if (m_Instance == nullptr)
	{
		//インスタンスがない場合は生成
		Create();
	}

	return m_Instance;
}

void Mouse::Init()
{
	m_vMousePos[0]			= GetMousePos();
	m_vMousePos[1]			= m_vMousePos[0];
	m_fMouseDrawTime		= MOUSECURSOR_DRAW_TIME;
	m_MouseCursorDrawFlag	= false;
	m_MouseMoveFlag			= false;
	m_iHandle				= -1;
	m_iMouseCursorAlpha		= 0;
}

void Mouse::Load()
{
	if (m_iHandle == -1)
	{
		m_iHandle = LoadGraph(MOUSE_IMAGE_PATH.c_str());
	}
}

void Mouse::Step()
{
	//新鮮な座標を取得
	m_vMousePos[0] = GetMousePos();

	m_MouseMoveFlag = false;
	//マウスが動いていたら
	if (Math::GetDistance(m_vMousePos[0], m_vMousePos[1]) > 1.0f ||
		Input::Mouse::Keep(MOUSE_INPUT_LEFT) || Input::Mouse::Keep(MOUSE_INPUT_RIGHT))
	{
		//カーソルを表示
		m_MouseCursorDrawFlag	= true;
		m_fMouseDrawTime		= 0.0f;
		m_MouseMoveFlag			= true;
	}

	//カーソルをフェードインする
	if (m_MouseCursorDrawFlag)
	{
		Math::MatchSpecifiedNum(m_iMouseCursorAlpha, 100, CURSOR_FADEIN_SPEED);
	}
	else
	{
		Math::MatchSpecifiedNum(m_iMouseCursorAlpha, 0, CURSOR_FADEOUT_SPEED);
	}

	//一定時間マウスが動いていなかったら
	if (Math::MatchSpecifiedNum(m_fMouseDrawTime, MOUSECURSOR_DRAW_TIME, FRAME_TIME))
	{
		//表示しない
		m_MouseCursorDrawFlag = false;
	}

	//座標を更新
	m_vMousePos[1] = m_vMousePos[0];
}

//マウスカーソルを表示
void Mouse::Draw()
{
	Draw::DrawBlendModeAlpha(m_iMouseCursorAlpha);
	VECTOR mousePos = GetMousePos();
	Draw::DrawGraphV(mousePos, m_iHandle, true);
	Draw::DrawBlendModeReset();
}

void Mouse::Fin()
{
	
}

//====================================================================

//当たり判定：対象の座標(中心), 対象のサイズ(直径)
bool Mouse::Collision(VECTOR vPos, VECTOR vSize)
{
	VECTOR mSize = { MOUSE_COLLISION_SIZE,MOUSE_COLLISION_SIZE,0.0f };
	
	if (Collision::RectCenter(m_vMousePos[0], mSize, vPos, vSize))
	{
		return true;
	}

	return false;
}
//当たり判定：対象の座標(中心), 対象のサイズ(半径)
bool Mouse::Collision(VECTOR vPos, float fSize)
{
	VECTOR mSize = { MOUSE_COLLISION_SIZE,MOUSE_COLLISION_SIZE,0.0f };

	if (Math::GetDistance(m_vMousePos[0], vPos) <= fSize)
	{
		return true;
	}

	return false;
}

//マウスの座標を取得
VECTOR Mouse::GetMousePos()
{
	int x, y;
	GetMousePoint(&x, &y);

	VECTOR mousePos = { (float)x,(float)y,0.0f };

	return mousePos;
}

void Mouse::Reset()
{
	//新鮮な座標を取得
	m_vMousePos[0] = GetMousePos();
	//座標を更新
	m_vMousePos[1] = m_vMousePos[0];

	//カーソルを表示
	m_MouseCursorDrawFlag = false;
	m_fMouseDrawTime = MOUSECURSOR_DRAW_TIME;
}