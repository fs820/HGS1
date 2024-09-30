//---------------------------------------
//
//セーブ・ロード処理の定義・宣言[data.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _DATA_H_
#define _DATA_H_

#include"main.h"
#include<stdio.h>

#define DATA_FILE "data\\DATA\\data.bin"
#define MAX_DATA (10)//保存するスコアの数

//プロトタイプ宣言
int *Soat(int* pData);//ソート処理
void SaveData(int* pData);//セーブ処理
 int* LoadData(void);//ロード処理

#endif _DATA_H_
