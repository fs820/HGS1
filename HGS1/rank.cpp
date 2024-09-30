//------------------------------------------
//
//ランキングの処理[rank.cpp]
//Author fuma sato
//
//------------------------------------------

#include"rank.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"data.h"
#include"cursor.h"
#include<assert.h>

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)
#define NUMBER_WIDTH (64)
#define NUMBER_HEIGHT (750)
#define RANK_MAX (6)
#define RANK_TIME (600)
#define NUMBER_SEPAESE (20)

typedef enum
{
	RANKSTATE_NORMAL = 0,
	RANKSTATE_SELECT,
	RANKSTATE_MAX
}RANKSTATE;

typedef enum
{
	SELECT_BACK = 0,
	SELECT_RETRY,
	SELECT_EXIT,
	SELECT_MAX,
}SELECT;

LPDIRECT3DTEXTURE9 g_apTextureRank[RANK_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;//バッファのポインタ
RANKSTATE g_RankState = RANKSTATE_NORMAL;
int g_nNewDataRank;
bool g_bRankin = false;

//--------------------
//初期化処理
//--------------------
void InitRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posScore;//スコアの位置
	D3DXVECTOR3 posScoreDef;//スコアの位置
	int nScore[MAX_DATA+1] = { 0 };
	int aPosTexUr[MAX_DATA+1][SCORE_MAX];
	int* pData;

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * (SCORE_MAX*MAX_DATA+SELECT_MAX+2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureRank[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureRank[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureRank[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureRank[3]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureRank[4]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureRank[5]
	);

	//過去のスコアの取得
	pData = LoadData();
	for (int i = 0; i < MAX_DATA; i++,pData++)
	{
		nScore[i] = *pData;
	}

	//今回のスコアの取得
	nScore[MAX_DATA] = GetScore();
	pData = Soat(&nScore[0]);
	for (int i = 0; i < MAX_DATA; i++, pData++)
	{
		nScore[i] = *pData;
	}
	//セーブ処理
	SaveData(&nScore[0]);

	//表示準備
	for (int i = 0; i < MAX_DATA; i++)
	{
		if (nScore[i] == GetScore()&&nScore[i]!=0)
		{
			g_nNewDataRank = i;
			g_bRankin=true;
			break;
		}
		else if (i==MAX_DATA-1)
		{
			g_nNewDataRank = 99;
			g_bRankin = false;
		}
	}
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			aPosTexUr[i][i2] = Digit(nScore[i], i2);
		}
	}

	SetScore(0, false);
	posScore = D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCORE_WIDTH / U_MAX_S) * (SCORE_MAX / 2), SCORE_HEIGHT/2, 0.0f);
	posScoreDef = posScore;
	D3DXVECTOR3 posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT, 0.0f);
	g_RankState = RANKSTATE_NORMAL;

	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += VT_MAX;
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			//座標設定
			pVtx[0].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[1].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[2].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[3].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2], UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2], UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S) + UV_DEF / V_MAX_S);

			posScore.x += SCORE_WIDTH / U_MAX_S;
			pVtx += VT_MAX;
		}
		posScore = posScoreDef;
		posScore.y += SCORE_HEIGHT * (i+1);
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{

		//座標設定
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	//座標設定
	pVtx[0].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2 - NUMBER_WIDTH- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 - NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 - NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2 - NUMBER_WIDTH- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 + NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 + NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);

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

	pVtx += VT_MAX;

	g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック
	PlaySound(SOUND_LABEL_BGM);
	if (g_bRankin)
	{
		if (g_nNewDataRank==0)
		{
			PlaySound(SOUND_LABEL_BGM);
		}
		else
		{
			PlaySound(SOUND_LABEL_BGM);
		}
	}
	else
	{
		PlaySound(SOUND_LABEL_BGM);
	}
}

//--------------------
//終了処理
//--------------------
void UninitRank(void)
{
	//サウンド
	StopSound(SOUND_LABEL_BGM);
	//テクスチャの破棄
	for (int i = 0; i < RANK_MAX; i++)
	{
		if (g_apTextureRank[i] != NULL)
		{
			g_apTextureRank[i]->Release();
			g_apTextureRank[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//--------------------
//更新処理
//--------------------
void UpdateRank(void)
{
	static SELECT SelectNew = SELECT_BACK;
	if (g_RankState==RANKSTATE_NORMAL)
	{
		static int nRankTime = 0;

		if (nRankTime % RANK_TIME == 0)
		{
			nRankTime = 0;
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				//サウンド
				StopSound(SOUND_LABEL_BGM);
				//切替
				SetFade(MODE_TITLE);
			}
		}

		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START,CONTROLLER_MAX) == true||GetMouseTrigger(MOUSE_LEFT)==true)
		{
			nRankTime = 0;
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_RankState = RANKSTATE_SELECT;
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック

			}
		}
		nRankTime++;
	}
	else
	{
		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true||GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_RankState = RANKSTATE_NORMAL;
		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_BACK:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_TITLE);
				}
				break;
			case SELECT_RETRY:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_GAME);
				}
				break;
			case SELECT_EXIT:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					HWND hWnd;
					hWnd = GethWnd();
					PostMessage(hWnd, WM_QUIT, 0, 0);
				}
				break;
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ
			CURSOR* pCursor;
			pCursor = GetCursorIn();

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1);

			for (int i = 0; i < SELECT_MAX; i++)
			{
				if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM);
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM);
							//切替
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM);
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
					break;
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック
		}
		if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP,STICK_LEFT,CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//カラー
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック

			switch (SelectNew)
			{
			case SELECT_BACK:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_RETRY:
				SelectNew = SELECT_BACK;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_RETRY;
				break;
			}

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//カラー
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック

			switch (SelectNew)
			{
			case SELECT_BACK:
				SelectNew = SELECT_RETRY;
				break;
			case SELECT_RETRY:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_BACK;
				break;
			}
			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック
		}
	}

	if (g_RankState == RANKSTATE_NORMAL&&g_bRankin)
	{
		static int nCount=0;
		nCount++;
		if (nCount%15==0)
		{
			static bool bP=false;
			VERTEX_2D* pVtx;//頂点情報ポインタ
			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * (SCORE_MAX * g_nNewDataRank + 1);

			for (int i = 0; i < SCORE_MAX; i++)
			{
				if (bP)
				{
					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffRank->Unlock();//プレイヤーバッファのアンロック
			bP = !bP;
		}
	}
}

//--------------------
//描画処理
//--------------------
void DrawRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureRank[0]);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);

	int nCount=0;
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			nCount++;
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRank[1]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				nCount * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}

	if (g_RankState==RANKSTATE_SELECT)
	{
		for (int i = 0; i < SELECT_MAX; i++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRank[i + 2]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX + (VT_MAX * (SCORE_MAX * MAX_DATA + 1)),//始まりの番号
				2//ポリゴンの個数
			);
		}
	}

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureRank[5]);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		VT_MAX * (SCORE_MAX * MAX_DATA + SELECT_MAX+1),//始まりの番号
		2//ポリゴンの個数
	);
}