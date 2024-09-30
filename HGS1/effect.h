//---------------------------------------
//
//エフェクト表示処理の定義・宣言[effect.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (655360)//最大数

void InitEffect(void);//エフェクトの初期化処理
void UninitEffect(void);//エフェクトの終了処理
void UpdateEffect(void);//エフェクトの更新処理
void DrawEffect(void);//エフェクトの描画処理
void SetEffect(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col, float fRadius, int nLife, bool bSenter);//エフェクト設定

#endif _EFFECT_H_
