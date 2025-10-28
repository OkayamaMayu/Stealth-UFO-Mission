#pragma once
#include"DxLib.h"
#include<list>
#include<string>
#include"../MyLib.h"

using namespace std;

#define DEFAULT_COLOR	COLOR[WHITE]					//デバック文字の基本色
const bool				isDebug = false;				//デバックフラグ
const int				TEXTINFO_LIST_SIZE		= 50;	//デバック文言表示リストのサイズ
const int				CHAR_SIZE				= 512;	//表示文字列のサイズ

//デバック文言表示用構造体
struct TextInfo
{
	int					x;				//表示座標：X
	int					y;				//表示座標：Y
	string				debugString;	//文言
	unsigned int		color;			//文字の色
	bool				isUse;			//使用フラグ
};

class DebugString
{
private:
	DebugString();
	~DebugString();

	static DebugString* m_Instance;		//インスタンス
	list<TextInfo>		m_TextInfoList;	//デバック文言表示リスト

public:
	static void			Create();		//インスタンスの生成
	static void			Destroy();		//インスタンスの削除
	static DebugString* GetInstance();	//インスタンスの取得

	//デバック文言データを追加
	void AddFormatString(int setX, int setY, const char* format, ...);

	//出力ログに表示
	void DrawLogFormatString(const char* format, ...);

	//描画
	void Draw();

private:
	//デバック文言データを追加
	void AddString(int setX, int setY, string setStr);
	//出力ログに表示
	void DrawLogString(string setStr);

	//listにデータを追加
	void AddTextInfo(TextInfo setInfo);
};