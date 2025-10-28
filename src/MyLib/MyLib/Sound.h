#pragma once
#include<string>
using namespace std;

//サウンドの種類
enum SOUND_TYPE
{
	//BGM=====================
	//音楽
	BGM_TITLE,											//タイトル画面
	BGM_SELECT,											//ステージセレクト画面
	BGM_PLAY,											//プレイ画面
	BGM_RESULT,											//リザルト画面
	BGM_GAMEOVER,										//ゲームオーバー画面
	BGM_END_1,											//エンディング_1
	BGM_END_2,											//エンディング_2
	//ループSE
	SE_LOOP_PLAY,										//プレイ中環境音
	SE_LOOP_SET_BLOCK_CHARGE,							//ブロック配置ゲージチャージ音
	SE_LOOP_ENEMY_FOUND,								//エネミー_追跡中
	SE_LOOP_UFO,										//UFO_環境音
	SE_LOOP_UFO_CATCH,									//UFO_捕獲

	BGM_MAX_NUM,										//BGMの数
	//SE=====================
	SE_ENTER = BGM_MAX_NUM,								//決定
	SE_CURSOR,											//矢印
	SE_PLAYER_MOVE,										//プレイヤー_移動
	SE_PLAYER_JUMP,										//プレイヤー_ジャンプ
	SE_PLAYER_LANDING,									//プレイヤー_着地
	SE_PLAYER_VIEW_CHANGE,								//プレイヤー_視点切り替え
	SE_ENEMY_FOUND,										//エネミー_発見
	SE_UFO_SPAWN,										//UFO_出現
	SE_UFO_MOVE,										//UFO_移動
	SE_UFO_QTE,											//UFO_捕獲時
	SE_ITEM_GET,										//アイテム_取得
	SE_ITEM_THROW,										//アイテム_投げる
	SE_ITEM_BOUND,										//アイテム_バウンド
	SE_ITEM_HIT,										//アイテム_当たる
	SE_BLOCK_GET,										//配置ブロック_取得
	SE_BLOCK_SET,										//配置ブロック_配置
	SE_BLOCK_MODE_IN,									//配置ブロック_モードに入る
	SE_BLOCK_MODE_OUT,									//配置ブロック_モードを切る
	SE_CHECK_POINT,										//チェックポイント起動時
	SE_GOLA_HIT,										//ゴール_当たる
	SE_GOLA_FLY,										//ゴール_飛んでいく
	SE_PLAY_PAUSE,										//ポーズ_切替
	SE_GAMEOVER,										//ゲームオーバー_入り
	SE_RESULT_1,										//リザルト_配置
	SE_RESULT_2,										//リザルト_評価

	SOUND_MAX_NUM										//サウンドの数
};

struct SoundData
{
	int		soundVolume;								//ボリューム
	string	soundPath;								//ファイルパス
};

//サウンドの情報：音量, パス
const SoundData SOUND_DATA[SOUND_MAX_NUM]
{
	//BGM==========================
	//音楽
	{30,	"data/sound/bgm/title.mp3"},				//タイトル画面
	{30,	"data/sound/bgm/select.mp3"},				//ステージセレクト画面
	{35,	"data/sound/bgm/play.mp3"},					//プレイ画面
	{30,	"data/sound/bgm/result.mp3"},				//リザルト画面
	{40,	"data/sound/bgm/gameover.mp3"},				//ゲームオーバー画面
	{70,	"data/sound/bgm/end_1.mp3"},				//エンディング_1
	{70,	"data/sound/bgm/end_2.mp3"},				//エンディング_2

	//ループSE
	{30,	"data/sound/se/loop/play.mp3"},				//プレイ中環境音
	{100,	"data/sound/se/loop/setBlockCharge.mp3"},	//ブロック配置ゲージチャージ音
	{40,	"data/sound/se/loop/enemyFound.mp3"},		//エネミー_追跡中
	{40,	"data/sound/se/loop/ufo.mp3"},				//UFO_環境音
	{80,	"data/sound/se/loop/ufoCatch.mp3"},			//UFO_捕獲

	//SE=====================
	{49,	"data/sound/se/systemSelect.mp3"},			//決定
	{49,	"data/sound/se/systemArrow.mp3"},			//矢印
	{50,	"data/sound/se/plMove.mp3"},				//プレイヤー_移動
	{72,	"data/sound/se/plJump.mp3"},				//プレイヤー_ジャンプ
	{110,	"data/sound/se/plLanding.mp3"},				//プレイヤー_着地
	{100,	"data/sound/se/plViewChange.mp3"},			//プレイヤー_視点切り替え
	{70,	"data/sound/se/enemyFound.mp3"},			//エネミー_発見
	{50,	"data/sound/se/ufoSpawn.mp3"},				//UFO_出現
	{60,	"data/sound/se/ufoMove.mp3"},				//UFO_移動
	{100,	"data/sound/se/ufoQTE.mp3"},				//UFO_捕獲時
	{90,	"data/sound/se/itemGet.mp3"},				//アイテム_取得
	{50,	"data/sound/se/itemThrow.mp3"},				//アイテム_投げる
	{100,	"data/sound/se/itemBound.mp3"},				//アイテム_バウンド
	{60,	"data/sound/se/itemHit.mp3"},				//アイテム_当たる
	{90,	"data/sound/se/setBlockChargeOn.mp3"},		//配置ブロック_取得
	{100,	"data/sound/se/setBlock.mp3"},				//配置ブロック_配置
	{100,	"data/sound/se/setBlockMode1.mp3"},			//配置ブロック_モードに入る
	{100,	"data/sound/se/setBlockMode2.mp3"},			//配置ブロック_モードを切る
	{100,	"data/sound/se/checkPoint.mp3"},			//チェックポイント起動時
	{70,	"data/sound/se/gola.mp3"},					//ゴール_当たる
	{70,	"data/sound/se/golaFly.mp3"},				//ゴール_飛んでいく
	{50,	"data/sound/se/pause.mp3"},					//ポーズ_切替
	{36,	"data/sound/se/gameover.mp3"},				//ゲームオーバー_入り
	{70,	"data/sound/se/result1.mp3"},				//リザルト_配置
	{70,	"data/sound/se/result2.mp3"},				//リザルト_評価
};

class Sound
{
private:
	//音ハンドル
	static int	handle[SOUND_MAX_NUM];
	//再生位置
	static int	soundPos[SOUND_MAX_NUM];
	//現在の音量
	static int	vol[SOUND_MAX_NUM];
	//セットする音量
	static int	setVol[SOUND_MAX_NUM];
	//フェード速度
	static int	fadeSpeed[SOUND_MAX_NUM];
	//フェードを使っているフラグ
	static bool fadeFlag[SOUND_MAX_NUM];
	//サウンドを流すフラグ
	static bool soundPlayFlag;

public:
	//サウンドまとめ初期化
	static void Init();
	static void Step();
	static void Fin();

	//サウンドの通常処理：種類, playBackFlag = 途中再生フラグ
	static void Play(SOUND_TYPE type, bool playBackFlag = false);

	//サウンドの停止：種類
	static void StopSound(SOUND_TYPE type);

	//サウンド全停止：0 == BGMのみ停止, 1 == SEのみ停止, 2 == BGM,SEを停止
	static void StopAll(int type = 0);

	//サウンドが流れているか
	static bool Check(SOUND_TYPE type);

	//サウンドの音量調節：種類,音量(ﾊﾟｰｾﾝﾃｰｼﾞ)
	static void SetVolume(SOUND_TYPE type, int volume);

	//フェードイン・フェードアウト：セットしたい音量, 速度
	static void SetFadeVolume(SOUND_TYPE type, int setVolume, int setSpeed = 5);

	//サウンドを流すフラグを設定
	static void SetSoundPlayFlag(bool set){ soundPlayFlag = set; }
	//サウンドを流すフラグを取得
	static bool GetSoundPlayFlag(){ return soundPlayFlag; }
};