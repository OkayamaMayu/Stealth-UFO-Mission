#include"DebugString.h"
#include<cstdarg>	//可変引数の処理用
#include<cstdio>	//vsnprintfの使用用
#include<Windows.h>

//インスタンスの初期化
DebugString* DebugString::m_Instance = nullptr;

//コンストラクタ
DebugString::DebugString()
{
	//リストの初期化
	m_TextInfoList.resize(TEXTINFO_LIST_SIZE);
	for (TextInfo& value : m_TextInfoList)
	{
		value.isUse = false;
	}
}

//デストラクタ
DebugString::~DebugString()
{

}

//インスタンスの生成
void DebugString::Create()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new DebugString();
	}
}

//インスタンスの削除
void DebugString::Destroy()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

//インスタンスの所得
DebugString* DebugString::GetInstance()
{
	if (m_Instance == nullptr)
	{
		//インスタンスがない場合は生成
		Create();
	}

	return m_Instance;
}

//デバック文言データを追加
void DebugString::AddString(int setX, int setY, string setStr)
{
	TextInfo setData = { setX, setY, setStr, DEFAULT_COLOR, true };
	AddTextInfo(setData);
}

//デバック文言データを追加
void DebugString::AddFormatString(int setX, int setY, const char* format, ...)
{
	//フォーマット済みの文字列を格納するバッファ
	char buffer[CHAR_SIZE];

	//可変引数を処理
	va_list args;
	va_start(args, format);
	//フォーマット済みの文字列を生成
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	AddString(setX, setY, buffer);
}

//描画
void DebugString::Draw()
{
	if (!isDebug)
		return;

	for (TextInfo& value : m_TextInfoList)
	{
		if (value.isUse)
		{
			DrawString(value.x, value.y, value.debugString.c_str(), value.color);
			//表示したら未使用にする
			value.isUse = false;
		}
	}
}

//出力ログに表示
void DebugString::DrawLogString(string setStr)
{
	setStr += "\n";

	OutputDebugString(setStr.c_str());
}
void DebugString::DrawLogFormatString(const char* format, ...)
{
	if (!isDebug)
	{
		return;
	}

	//フォーマット済みの文字列を格納するバッファ
	char buffer[CHAR_SIZE];

	//可変引数を処理
	va_list args;
	va_start(args, format);
	//フォーマット済みの文字列を生成
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	DrawLogString(buffer);
}

//listにデータを追加
void DebugString::AddTextInfo(TextInfo setInfo)
{
	for (TextInfo& value : m_TextInfoList)
	{
		if (!value.isUse)
		{
			//データを追加
			value = setInfo;
			return;
		}
	}
}