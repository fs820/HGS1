//------------------------------------------
//
//フェード処理[fade.cpp]
//Author fuma sato
//
//------------------------------------------
#include"fade.h"

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;//バッファのポインタ
FADE g_fade;
MODE g_modeNext;
D3DXCOLOR g_coloFade;

//-------------
//初期化処理
//-------------
void InitFade(MODE modeNext)
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
		&g_pVtxBuffFade,
		NULL
	);

	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_coloFade = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

    //座標設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー
	pVtx[0].col = g_coloFade;
	pVtx[1].col = g_coloFade;
	pVtx[2].col = g_coloFade;
	pVtx[3].col = g_coloFade;

	g_pVtxBuffFade->Unlock();//プレイヤーバッファのアンロック

	SetMode(g_modeNext);
}

//-----------------
//終了処理
//-----------------
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//-----------------
//更新処理
//-----------------
void UpdateFade(void)
{
	if (g_fade!=FADE_NONE)
	{
		if (g_fade==FADE_IN)
		{
			g_coloFade.a -= 0.01f;
			if (g_coloFade.a<=0.0f)
			{
				g_coloFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade==FADE_OUT)
		{
			g_coloFade.a += 0.01f;
			if (g_coloFade.a>=1.0f)
			{
				g_coloFade.a = 1.0f;
				g_fade = FADE_IN;

				//モード切替
				SetMode(g_modeNext);
			}
		}
		VERTEX_2D* pVtx;//頂点情報ポインタ

		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック
		//カラー
		pVtx[0].col = g_coloFade;
		pVtx[1].col = g_coloFade;
		pVtx[2].col = g_coloFade;
		pVtx[3].col = g_coloFade;

		g_pVtxBuffFade->Unlock();//プレイヤーバッファのアンロック
	}
}

//----------------
//描画処理
//----------------
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);
}

//-----------------
//フェード設定
//-----------------
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_coloFade = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
}

//-----------------
//フェード設定
//-----------------
void SetFade2(MODE modeNext)
{
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_coloFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

//------------------
//フェード取得
//------------------
FADE GetFade(void)
{
	return g_fade;
}