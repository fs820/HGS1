//------------------------------------------
//
//フェード処理の定義・宣言[fade.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _FADE_H_
#define _FADE_H_

#include"main.h"

//フェードの状態
typedef enum
{
	FADE_NONE=0,//通常
	FADE_IN,//フェードイン
	FADE_OUT,//フェードアウト
	FADE_MAX//数
}FADE;

void InitFade(MODE modeNext);//フェード初期化処理
void UninitFade(void);//フェード終了処理
void UpdateFade(void);//フェード更新処理
void DrawFade(void);//フェード描画処理
void SetFade(MODE modeNext);//フェード設定
void SetFade2(MODE modeNext);
FADE GetFade(void);//フェード状態取得

#endif  _FADE_H_

