//---------------------------------------
//
//スコア処理[score.cpp]
//Author fuma sato
//
//---------------------------------------

#include"score.h"

LPDIRECT3DTEXTURE9 g_pTextureScore=NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//バッファのポインタ
D3DXVECTOR3 g_posScore;//スコアの位置
int g_nScore;//スコアの値
//-------------------------
//ポリゴンの初期化処理
//-------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX*SCORE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_pTextureScore
	);

	g_posScore = D3DXVECTOR3(SCREEN_WIDTH-SCORE_MAX*(SCORE_WIDTH/U_MAX_S)+ (SCORE_WIDTH / U_MAX_S)/2,(SCORE_HEIGHT/V_MAX_S)/2,0.0f);
	g_nScore = 0;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);

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
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);

		g_posScore.x += SCORE_WIDTH/U_MAX_S;
		pVtx += VT_MAX;
	}

	g_pVtxBuffScore->Unlock();//プレイヤーバッファのアンロック
}

//--------------------------
//ポリゴンの終了処理
//--------------------------
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//--------------------------
//ポリゴンの更新処理
//--------------------------
void UpdateScore(void)
{

}

//----------------------------
//ポリゴンの描画処理
//----------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX * i,//始まりの番号
			2//ポリゴンの個数
		);
	}
}

//-----------------------------
//スコアの書き換え(保存)
//-----------------------------
void SetScore(int nScore, bool bTex)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	int aPosTexU[SCORE_MAX];
	g_nScore = nScore;

	//スコア設定
	for (int i = 0; i < SCORE_MAX; i++)
	{
		aPosTexU[i] = Digit(g_nScore,i);
	}

	if (bTex)
	{
		//座標
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

		for (int i = 0; i < SCORE_MAX; i++)
		{
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);

			pVtx += VT_MAX;
		}

		g_pVtxBuffScore->Unlock();//プレイヤーバッファのアンロック
	}
}

//------------------------------
//スコアの加算
//------------------------------
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	int aPosTexU[SCORE_MAX];
	g_nScore += nValue;

	//マイナススコアを除外
	if (g_nScore<0)
	{
		g_nScore = 0;
	}

	//スコア設定
	for (int i = 0; i < SCORE_MAX; i++)
	{
		aPosTexU[i] = Digit(g_nScore, i);
	}

    //座標
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);

		pVtx += VT_MAX;
	}

	g_pVtxBuffScore->Unlock();//プレイヤーバッファのアンロック
}

//------------------------
//スコアの取得
//------------------------
int GetScore(void)
{
	return g_nScore;
}

//--------------------
//桁分割処理
//--------------------
int Digit(int nData, int nDigit)
{
	int aNumber[SCORE_MAX] = { 0 };//分割用配列
	int nData2 = 1;//計算用

	for (int i = SCORE_MAX - 1; i >= 0; i--)
	{//桁数分
		if (i == 0)
		{
			aNumber[i] = nData / nData2;//最上位の桁
		}
		aNumber[i] = nData % (nData2 * DIGIT) / nData2;//計算
		nData2 *= DIGIT;//桁引き上げ
	}

	return aNumber[nDigit];//指定の桁を返す
}