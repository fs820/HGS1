//---------------------------------------
//
//セーブ・ロード処理[data.cpp]
//Author fuma sato
//
//---------------------------------------

#include"data.h"

//------------------
//ソート関数
//------------------
int *Soat(int* pData)
{
	//変数を宣言
	int nRam = 0;

	for (int i = 0; i < MAX_DATA; i++)
	{//比較対象1
		for (int i2 = i + 1; i2 < MAX_DATA+1; i2++)
		{//比較対象2
			if (pData[i] < pData[i2])
			{
				//大きいほうに入れ替える
				nRam = pData[i];
				pData[i] = pData[i2];
				pData[i2] = nRam;
			}
		}
	}
	return &pData[0];
}
//---------------------------------------
//セーブ処理
//---------------------------------------
void SaveData(int* pData)
{
	FILE* pFile;//ファイルポインタを宣言

	pFile = fopen(DATA_FILE, "wb");//ファイルを開く
	if (pFile != NULL)
	{//開けたら
		fwrite(pData, sizeof(int), MAX_DATA,pFile);//数値を書き入れ

		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
		MessageBox(hWnd, "セーブエラー", "セーブできなかったよ", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//ロード処理
//---------------------------------------
int *LoadData(void)
{
	FILE* pFile;//ファイルポインタを宣言
	static int aData[MAX_DATA] = { 0 };

	for (int i = 0; i < MAX_DATA; i++)
	{
		aData[i] = 0;
	}

	pFile = fopen(DATA_FILE, "rb");//ファイルを開く
	if (pFile != NULL)
	{//開けたら
		fread(&aData[0], sizeof(int), MAX_DATA, pFile);//数値を書き入れ

		fclose(pFile);//ファイルを閉じる
		return &aData[0];
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
		MessageBox(hWnd, "ロードエラー", "ロードできなかったよ", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		return &aData[0];
	}
}