//---------------------------------------
//
//エフェクト表示処理[effect.cpp]
//Author fuma sato
//
//---------------------------------------

#include"effect.h"
#include"player.h"

//弾構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;//寿命
	bool bSenter;
	bool bUse;//使用しているかどうか
}Effect;

//グローバル
LPDIRECT3DTEXTURE9 g_apTextureEffect = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//バッファのポインタ
Effect g_aEffect[MAX_EFFECT];//弾の情報
int g_nLifeDef[MAX_EFFECT] = { 0 };

//-----------------
//弾の初期化処理
//-----------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスを取得
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		g_aEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[i].fRadius = 0.0f;
		g_aEffect[i].nLife = 0;
		g_aEffect[i].bUse = false;//使用していない状態にする
	}

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureEffect
	);

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;//ポリゴン1つ分進める
	}
	g_pVtxBuffEffect->Unlock();//バッファのアンロック
}

//----------------
//弾の終了処理
//----------------
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_apTextureEffect != NULL)
	{
		g_apTextureEffect->Release();
		g_apTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//----------------
//弾の更新処理
//----------------
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;
	static D3DXVECTOR3 Oldpos[MAX_EFFECT];

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == true)
		{//弾が使用されている

			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//座標設定
			pVtx += i * VT_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);

			BYTE alpha = (BYTE)(((float)g_aEffect[i].nLife) / ((float)g_nLifeDef[i]) * 255.0f);
			pVtx[0].col = (pVtx[0].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[1].col = (pVtx[1].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[2].col = (pVtx[2].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[3].col = (pVtx[3].col & 0x00FFFFFF) | (alpha << 24);

			g_pVtxBuffEffect->Unlock();//バッファのアンロック

			Oldpos[i] = g_aEffect[i].pos;

			g_aEffect[i].pos.x += g_aEffect[i].move.x;
			g_aEffect[i].pos.y += g_aEffect[i].move.y;

			g_aEffect[i].nLife--;
			g_aEffect[i].fRadius *= ((float)g_aEffect[i].nLife) / ((float)g_nLifeDef[i]);

			if (g_aEffect[i].nLife <= 0)
			{
				g_aEffect[i].bUse = false;
			}

			if (g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y <= 0 || g_aEffect[i].pos.y >= SCREEN_HEIGHT)
			{
				g_aEffect[i].bUse = false;
			}
		}
	}
}

//----------------
//弾の描画処理
//----------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DRS_DESTBLENDALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == true)
		{//弾が使用されている
			if (GetMode()!=MODE_VS||!g_aEffect[i].bSenter)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEffect);

				//弾の描画
				pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,//タイプ
					i * VT_MAX,//始まりの番号
					2//ポリゴンの個数
				);
			}
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DRS_DESTBLENDALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリング
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//透明度
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-------------------
//発通常弾
//-------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool bSenter)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == false)
		{//弾が使用されていない
			g_aEffect[i].pos = pos;
			g_aEffect[i].move = move;
			g_aEffect[i].col = col;
			g_aEffect[i].fRadius = fRadius;

			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標設定
			pVtx += i * VT_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);

			pVtx[0].col = g_aEffect[i].col;
			pVtx[1].col = g_aEffect[i].col;
			pVtx[2].col = g_aEffect[i].col;
			pVtx[3].col = g_aEffect[i].col;

			g_pVtxBuffEffect->Unlock();//バッファのアンロック

			g_aEffect[i].nLife = nLife;
			g_nLifeDef[i] = g_aEffect[i].nLife;
			g_aEffect[i].bSenter = bSenter;
			g_aEffect[i].bUse = true;

			break;
		}
	}
}