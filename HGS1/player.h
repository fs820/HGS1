//---------------------------------------
//
//プレイヤー表示処理の定義・宣言[player.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"main.h"

//プレイヤー情報
#define PLAYER_WIDTH (180)//幅
#define PLAYER_HEIGHT (90)//高さ
#define PLAYER_SPEED_DEF (0.0f)//基本スピード
#define PLAYER_SPEED_X (1.5f)//速さ
#define PLAYER_SPEED_Y (1.5f)//速さ
#define PLAYER_INA (0.05f)//慣性
#define PLAYER_ROT_SPEED (0.05f)//回転速度
#define PLAYER_SIZE_SPEED (3.0f)//拡大縮小速度
#define PLAYER_SIZE_MAX (300.0f)//大きさ最大
#define PLAYER_SIZE_MIN (50.0f)//大きさ最小
#define PLAYER_FIELD_INA (13.0f)//フィールド速度倍率
#define PLAYER_SPARSE_INA (5)//どれだけ離れられるか

#define U_MAX (1)//テクスチャの数(横)
#define V_MAX (1)//テクスチャの数(縦)

//プレイヤー状態の列挙型定義
typedef enum
{
	PLAYERSTATE_APPEAR = 0,//復活状態
	PLAYERSTATE_NORMAL,//通常
	PLAYERSTATE_DAMAGE,//ダメージ
	PLAYERSTATE_DIE,//死
	PLAYERSTATE_MAX//数
}PLAYERSTATE;

//プレイヤー構造体
typedef struct
{
	int nCounterAnim;//プレイヤーの更新間隔
	int nPatternAnim;//プレイヤーのアニメーションパターン管理
	D3DXVECTOR3 pos;//プレイヤー座標管理
	D3DXVECTOR3 Oldpos;//前フレームのpos
	D3DXVECTOR3 move;//プレイヤースピード管理
	D3DXVECTOR3 rot;//プレイヤー向き管理
	D3DXVECTOR3 rotDef;//プレイヤー向き管理
	float fLength;//対角線の長さ(大きさ)
	float fLengthDef;//対角線の長さ(大きさ)の初期値
	float fAngle;//対角線の角度
	int BulletCount;//弾数
	int nRema;//残機
	int nLife;//体力
	PLAYERSTATE state;//状態
	int nCounterState;//状態カウンター
	bool bDisp;//表示状態
}Player;

//プロトタイプ宣言
void InitPlayer(void);//プレイヤーの初期化処理
void UninitPlayer(void);//プレイヤーの終了処理
void UpdatePlayer(void);//プレイヤーの更新処理
void DrawPlayer(void);//プレイヤーの描画処理
void HitPlayer(int nDamage);//プレイヤーヒット処理
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void);//プレイヤーバッファの取得
Player* GetPlayer(void);//プレイヤー情報取得

#endif _PLAYER_H_