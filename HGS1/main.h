//------------------------------------------
//
//シューティングゲームプログラムのメインの処理の定義・宣言[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#include<windows.h>
#include<stdio.h>
#include<time.h>
#include <strsafe.h>
#include"d3dx9.h"
#include"dinput.h" // 入力に必要なファイル
#include"Xinput.h" // Xinput(パッド処理)
#include"xaudio2.h"

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")

// マクロ定義
#define FPS (1000/60) // 更新スピード
#define UP_TIME (3) // テクスチャスピード

// 名前
#define CLASS_NAME "WindowClass" // クラス名
#define WINDOW_NAME "Synchronize" // ウインドウ名

// 大きさ
#define SCREEN_WIDTH (1280) // 幅
#define SCREEN_HEIGHT (720) // 高さ

// 頂点
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 頂点フォーマット
#define VT_MAX (4)//頂点数
#define UV_DEF (1.0f) // テクスチャのデフォルトのサイズ

//デバッグ用
#define FPS_TIME (500)
#define FONT_SIZE (20)

// 頂点情報の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; // 座標
	float rhw; // 座標変換係数(1.0f)
	D3DCOLOR col; // カラー
	D3DXVECTOR2 tex; // テクスチャ座標
} VERTEX_2D;

// 画面の種類
typedef enum
{
	MODE_TITLE = 0,//タイトル
	MODE_DEMO,//デモ
	MODE_TUTO,//チュートリアル
	MODE_PLAYSELECT,//プレイセレクト
	MODE_GAME,//ゲーム
	MODE_VS,//バーサス
	MODE_RESULT,//リザルト
	MODE_RANK,//ランキング
	MODE_OPTION,//オプション
	MODE_MAX//モード数
} MODE;

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウインドウプロシージャ
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // 初期化処理
void Uninit(void); // 終了処理
void Update(void); // 更新処理
void Draw(void); // 描画処理
LPDIRECT3DDEVICE9 GetDevice(void); // デバイスの取得
void SetMode(MODE mode);//モード設定
MODE GetMode(void);//モード取得
HWND GethWnd(void);//ハンドル取得
HINSTANCE GethInstanse(void);//インスタンス取得

void ConfineCursorToWindow(HWND hwnd);//カーソル制御
void ReleaseCursor(void);//カーソル制御解除

void ToggleFullScreen(HWND hWnd);//フルスクリーン
//void ResetDevice(void);//デバイス再構築
//
////デバッグ用
//void DrawDebug(int nData, float* fData, char* cData);

#endif // _MAIN_H_