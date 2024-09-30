//---------------------------------------
//
//カーソル処理の定義・宣言[cursor.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include"main.h"

//プレイヤー情報
#define CURSOR_WIDTH (64)//幅
#define CURSOR_HEIGHT (64)//高さ
#define CURSOR_SPEED_DEF (0.0f)//初期スピード
#define CURSOR_SPEED_X (20.0f)//速さ
#define CURSOR_SPEED_Y (20.0f)//速さ

#define U_MAX_CU (1)//テクスチャの数(横)
#define V_MAX_CU (1)//テクスチャの数(縦)

//カーソル状態の列挙型定義
typedef enum
{
	CURSORSTATE_NONE = 0,//非表示
	CURSORSTATE_NORMAL,//通常
	CURSORSTATE_POINT,//？
	CURSORSTATE_MAX//種類数
}CURSORSTATE;

//カーソル構造体
typedef struct
{
	int nCounterAnim;//カーソルの更新間隔
	int nPatternAnim;//カーソルのアニメーションパターン管理
	D3DXVECTOR3 pos;//カーソル座標管理
	D3DXVECTOR3 move;//カーソルスピード管理
	CURSORSTATE state;//状態
	bool bUse;//使用しているか
}CURSOR;

void InitCursor(void);//カーソルの初期化処理
void UninitCursor(void);//カーソルの終了処理
void UpdateCursor(void);//カーソルの更新処理
void DrawCursor(void);//カーソルの描画処理
PDIRECT3DVERTEXBUFFER9 GetCursorBuffer(void);//カーソルバッファの取得
CURSOR* GetCursorIn(void);//カーソル情報取得
void CrCursor(bool bUse);//表示切替

#endif _CURSOR_H_
