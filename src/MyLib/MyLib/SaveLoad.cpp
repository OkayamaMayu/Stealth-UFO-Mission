#include"DxLib.h"
#include"SaveLoad.h"
	
//セーブデータのファイルパス
char saveDataFilePath[] =
{
	"data/saveData/PlayData.bin"
};

//書き込み
void SaveLoad::Save()
{
	FILE* fp;

	fopen_s(&fp, saveDataFilePath, "wb");

	if (fp != NULL)
	{
		//書き出し
		fwrite(&data, sizeof(SaveData), 1, fp);

		//ファイルを閉じる
		fclose(fp);	
	}
}

//読み込み
void SaveLoad::Load()
{
	FILE* fp;

	fopen_s(&fp, saveDataFilePath, "rb");

	if (fp != NULL)
	{
		//読み込み
		fread(&data, sizeof(SaveData), 1, fp);

		//ファイルを閉じる
		fclose(fp);
	}
}
