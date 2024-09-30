//---------------------------------------
//
//カーソル処理[cursor.cpp]
//Author fuma sato
//
//---------------------------------------

#include"cursor.h"
#include"input.h"
#include"fade.h"
#include"game.h"
#include"tuto.h"
#include"vs.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureCursor;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCursor = NULL;//バッファのポインタ
CURSOR g_Cursor;//プレイヤー情報

//-------------------
//初期化処理
//-------------------
void InitCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCursor,
		NULL
	);

	g_Cursor.nCounterAnim = 0;
	g_Cursor.nPatternAnim = 0;
	g_Cursor.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);//初期位置
	g_Cursor.move = D3DXVECTOR3(CURSOR_SPEED_DEF, CURSOR_SPEED_DEF, CURSOR_SPEED_DEF);//プレイヤースピード
	g_Cursor.state = CURSORSTATE_NONE;
	g_Cursor.bUse = true;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_CURSOR,
		&g_pTextureCursor
	);

	g_pVtxBuffCursor->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	//座標設定
	pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_CU);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU, UV_DEF / V_MAX_CU);

	g_pVtxBuffCursor->Unlock();//プレイヤーバッファのアンロック
}

//------------------
//終了処理
//------------------
void UninitCursor(void)
{
	//テクスチャの破棄
	if (g_pTextureCursor != NULL)
	{
		g_pTextureCursor->Release();
		g_pTextureCursor = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCursor != NULL)
	{
		g_pVtxBuffCursor->Release();
		g_pVtxBuffCursor = NULL;
	}
}

//-------------------
//更新処理
//-------------------
void UpdateCursor(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	float* pMouseMove;
	FADE fade;
	MODE mode;
	GAMESTATE state;
	GAMESTATE Tstate;
	VSSTATE Vstate;

	g_Cursor.move.x = 0;
	g_Cursor.move.y = 0;

	//移動処理
	pMouseMove = GetMouseMove();
	g_Cursor.move.x += *pMouseMove * CURSOR_SPEED_X;
	g_Cursor.move.y += *(pMouseMove+1) * CURSOR_SPEED_Y;

	//位置の更新
	g_Cursor.pos.x += g_Cursor.move.x;
	g_Cursor.pos.y += g_Cursor.move.y;

	g_Cursor.nCounterAnim++;
	if ((g_Cursor.nCounterAnim % UP_TIME) == 0)//更新期間
	{
		g_Cursor.nPatternAnim = (g_Cursor.nPatternAnim + 1) % (U_MAX_CU * V_MAX_CU);

		g_pVtxBuffCursor->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

	    //座標設定
		pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim + UV_DEF / U_MAX_CU, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU) + UV_DEF / V_MAX_CU);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim + UV_DEF / U_MAX_CU, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU) + UV_DEF / V_MAX_CU);


		if (pVtx[3].pos.x >= SCREEN_WIDTH+CURSOR_WIDTH)//画面右端
		{
			g_Cursor.pos.x = SCREEN_WIDTH + CURSOR_WIDTH;//画面左端
		}
		else if (pVtx[0].pos.x <= -CURSOR_WIDTH)//画面左端
		{
			g_Cursor.pos.x = -CURSOR_WIDTH;//画面右端
		}

		if (pVtx[3].pos.y >= SCREEN_HEIGHT+CURSOR_HEIGHT)//画面下端
		{
			g_Cursor.pos.y = SCREEN_HEIGHT + CURSOR_HEIGHT;//画面上端
		}
		else if (pVtx[0].pos.y <= -CURSOR_HEIGHT)//画面上端
		{
			g_Cursor.pos.y = -CURSOR_HEIGHT;//画面下端
		}

		fade = GetFade();
		mode = GetMode();
		state = GetGameState();
		Tstate = GetTutoState();
		Vstate = GetVsState();
		switch (g_Cursor.state)
		{
		case CURSORSTATE_NONE:
			if (fade == FADE_NONE && ((mode != MODE_GAME&&mode!=MODE_TUTO && mode != MODE_VS) || state == GAMESTATE_PAUSE || Tstate == GAMESTATE_PAUSE || Vstate == VSSTATE_PAUSE) && mode != MODE_DEMO)
			{
			    g_Cursor.state = CURSORSTATE_NORMAL;
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		case CURSORSTATE_NORMAL:
			if (fade != FADE_NONE|| ((mode==MODE_GAME||mode==MODE_TUTO || mode == MODE_VS)&&state != GAMESTATE_PAUSE&& Tstate != GAMESTATE_PAUSE && Vstate != VSSTATE_PAUSE) || mode == MODE_DEMO)
			{
				g_Cursor.state = CURSORSTATE_NONE;

				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			break;
		}

		g_pVtxBuffCursor->Unlock();//プレイヤーバッファのアンロック
	}
}

//------------------
//描画処理
//------------------
void DrawCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

    //デバイスの取得
	pDevice = GetDevice();

	if (g_Cursor.bUse)
	{
		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffCursor, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCursor);

		//プレイヤーの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			0,//始まりの番号
			2//ポリゴンの個数
		);
	}
}

//-------------------
//バッファの取得
//-------------------
PDIRECT3DVERTEXBUFFER9 GetCursorBuffer(void)
{
	return g_pVtxBuffCursor;
}

//----------------------
//情報の取得
//----------------------
CURSOR* GetCursorIn(void)
{
	return &g_Cursor;
}

//----
//使用
//----
void CrCursor(bool bUse)
{
	g_Cursor.bUse = bUse;
}