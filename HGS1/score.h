//---------------------------------------
//
//スコア処理の定義・宣言[score.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

#define SCORE_MAX (10)//桁数
#define U_MAX_S (10)//テクスチャ数(横)
#define V_MAX_S (1)//テクスチャ数(縦)
#define SCORE_WIDTH (350.0f)//サイズ
#define SCORE_HEIGHT (70.0f)//サイズ
#define DIGIT (10)//進数

#define SCORE_HIT (100)//敵に当たった時
#define SCORE_DES (1000)//敵を倒した時
#define SCORE_HIT_BOSS (1000)//ボスに当たった時
#define SCORE_DES_BOSS (100000)//ボスを倒したとき
#define SCORE_MISS (1000)//ダメージを受けた時
#define SCORE_DIE (10000)//死んだとき

//プロトタイプ宣言
void InitScore(void);//スコアの初期化処理
void UninitScore(void);//スコアの終了処理
void UpdateScore(void);//スコアの更新処理
void DrawScore(void);//スコアの描画処理
void SetScore(int nScore,bool bTex);//スコア書き換え
void AddScore(int nValue);//スコア加算
int GetScore(void);//スコア取得
int Digit(int nData, int nDigit);//分割処理

#endif _SCORE_H_
