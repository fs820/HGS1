//---------------------------------------
//
//パーティクル表示処理の定義・宣言[exef.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXEF_H_
#define _EXEF_H_

#include"main.h"

#define MAX_EXEF (1280)//最大数
#define EF_NUM (5)//フレーム発生数
#define EXEF_LIFE (80)//残留具合

void InitExef(void);//パーティクルの初期化処理
void UninitExef(void);//パーティクルの終了処理
void UpdateExef(void);//パーティクルの更新処理
void DrawExef(void);//パーティクルの描画処理
void SetExef(D3DXVECTOR3 pos,float fLength,bool bSenter);//パーティクル設定

#endif _EXEF_H_