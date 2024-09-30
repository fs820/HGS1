//------------------------------------------
//
//タイトルの処理[title.cpp]
//Author fuma sato
//
//------------------------------------------

#include"title.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"cursor.h"
#include"movie.h"

#define DEMO_TIME (600)
#define TITLE_MAX (4)
#define TITLE_TEX_MAX (5)
#define SELECT_WIDTH (512)//幅
#define SELECT_HEIGHT (128)//高さ
#define TITLE_WIDTH (640)
#define TITLE_HEIGHT (360)
#define ROGO_WIDTH (512)
#define ROGO_HEIGHT (128)
#define U_MAX_T (1)
#define V_MAX_T (6)

typedef enum
{
	TITLESTATE_NONE = 0,
	TITLESTATE_NORMAL,
	TITLESTATE_SELECT,
	TITLESTATE_MAX
}TITLESTATE;

typedef enum
{
	SELECT_PLAY = 0,
	SELECT_VERSUS,
	SELECT_TUTO,
	SELECT_RANK,
	SELECT_OPTION,
	SELECT_EXIT,
	SELECT_MAX
}SELECT;

LPDIRECT3DTEXTURE9 g_apTextureTitle[TITLE_TEX_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//バッファのポインタ
TITLESTATE g_TitleState=TITLESTATE_NONE;
D3DXVECTOR3 g_RogoPos;

//--------------------
//初期化処理
//--------------------
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posSelect;//スコアの位置

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX*TITLE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureTitle[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureTitle[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureTitle[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureTitle[3]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_apTextureTitle[4]
	);

	g_TitleState = TITLESTATE_NONE;
	g_RogoPos = D3DXVECTOR3(SCREEN_WIDTH/2,-TITLE_HEIGHT,0.0f);
	posSelect= D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * (3.0f/4.0f), 0.0f);

	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

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

	//座標設定
	pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);

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

	//座標設定
	pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);

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

	//座標設定
	pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2, 0.0f);

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
	if (g_TitleState == TITLESTATE_SELECT)
	{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_T, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_T);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_T, UV_DEF / V_MAX_T);
	}
	else
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	PlaySound(SOUND_LABEL_BGM);

	g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック
}

//------------------
//終了処理
//------------------
void UninitTitle(void)
{
	//サウンド
	StopSound(SOUND_LABEL_BGM);
	//テクスチャの破棄
	for (int i = 0; i < TITLE_TEX_MAX; i++)
	{
		if (g_apTextureTitle[i] != NULL)
		{
			g_apTextureTitle[i]->Release();
			g_apTextureTitle[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//--------------
//更新処理
//--------------
void UpdateTitle(void)
{
	static int DemoCnt = 0;
	static SELECT SelectNew = SELECT_PLAY;
	VERTEX_2D* pVtx;//頂点情報ポインタ

	if (g_TitleState==TITLESTATE_NONE)
	{
		g_RogoPos.y += 3;

		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック
		pVtx += VT_MAX;

		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);


		pVtx += VT_MAX;

		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
		g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック

		if (g_RogoPos.y >= SCREEN_HEIGHT / 4)
		{
			g_TitleState = TITLESTATE_NORMAL;
			PlaySound(SOUND_LABEL_TITLE);
		}

		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true)
		{
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_RogoPos.y = SCREEN_HEIGHT / 4;
				g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック
				pVtx += VT_MAX;

				//座標設定
				pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);


				pVtx += VT_MAX;

				//座標設定
				pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
				g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック
				g_TitleState = TITLESTATE_NORMAL;
				PlaySound(SOUND_LABEL_TITLE);
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_RogoPos.y = SCREEN_HEIGHT / 4;
				g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック
				pVtx += VT_MAX;

				//座標設定
				pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y - TITLE_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + TITLE_WIDTH / 2, g_RogoPos.y + TITLE_HEIGHT / 2, 0.0f);


				pVtx += VT_MAX;

				//座標設定
				pVtx[0].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 - ROGO_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RogoPos.x - ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RogoPos.x + ROGO_WIDTH / 2, g_RogoPos.y + ROGO_WIDTH / 2 + ROGO_HEIGHT / 2, 0.0f);
				g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック
				g_TitleState = TITLESTATE_NORMAL;
				PlaySound(SOUND_LABEL_TITLE);
			}
		}
	}

	if (g_TitleState == TITLESTATE_NORMAL)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START,CONTROLLER_MAX) == true)
			{
				g_TitleState = TITLESTATE_SELECT;
				DemoCnt = 0;

				g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX*3;
				//テクスチャ
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);

				g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック
			}
			else if (GetMouseTrigger(MOUSE_LEFT) == true)
			{
				CURSOR* pCursor;

				g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pCursor = GetCursorIn();
				if (pCursor->pos.x >= (pVtx + VT_MAX*3)[0].pos.x && pCursor->pos.x <= (pVtx + VT_MAX*3)[3].pos.x && pCursor->pos.y >= (pVtx + VT_MAX*3)[0].pos.y && pCursor->pos.y <= (pVtx + VT_MAX*3)[3].pos.y)
				{
					g_TitleState = TITLESTATE_SELECT;
					DemoCnt = 0;

					pVtx += VT_MAX*3;
					//テクスチャ
					pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
					pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
					pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);
					pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);
				}
				g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック
			}

			DemoCnt++;
		}
	}
	else if (g_TitleState == TITLESTATE_SELECT)
	{
		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_BACK, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_TitleState = TITLESTATE_NORMAL;

			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX*3;
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック

		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_PLAY:
				DemoCnt = 0;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_PLAYSELECT);
				}
				break;
			case SELECT_VERSUS:
				DemoCnt = 0;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_VS);
				}
				break;
			case SELECT_TUTO:
				DemoCnt = 0;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_TUTO);

				}
				break;
			case SELECT_RANK:
				DemoCnt = 0;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_RANK);

				}
				break;
			case SELECT_OPTION:
				DemoCnt = 0;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM);
					//切替
					SetFade(MODE_OPTION);

				}
				break;
			case SELECT_EXIT:
				DemoCnt = 0;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
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
			CURSOR* pCursor;

			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pCursor = GetCursorIn();
			if (pCursor->pos.x >= (pVtx + VT_MAX*3)[0].pos.x && pCursor->pos.x <= (pVtx + VT_MAX * 3)[3].pos.x && pCursor->pos.y >= (pVtx + VT_MAX * 3)[0].pos.y && pCursor->pos.y <= (pVtx + VT_MAX * 3)[3].pos.y)
			{
				FADE fade;
				switch (SelectNew)
				{
				case SELECT_PLAY:
					DemoCnt = 0;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM);
						//切替
						SetFade(MODE_PLAYSELECT);
					}

					break;
				case SELECT_VERSUS:
					DemoCnt = 0;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM);
						//切替
						SetFade(MODE_VS);
					}
					break;
				case SELECT_TUTO:
					DemoCnt = 0;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM);
						//切替
						SetFade(MODE_TUTO);

					}
					break;
				case SELECT_RANK:
					DemoCnt = 0;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM);
						//切替
						SetFade(MODE_RANK);

					}
					break;
				case SELECT_OPTION:
					DemoCnt = 0;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM);
						//切替
						SetFade(MODE_OPTION);

					}
					break;
				case SELECT_EXIT:
					DemoCnt = 0;
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
			g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック

		}
		else if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP,STICK_LEFT,CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
		{
			switch (SelectNew)
			{
			case SELECT_PLAY:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_VERSUS:
				SelectNew = SELECT_PLAY;
				break;
			case SELECT_TUTO:
				SelectNew = SELECT_VERSUS;
				break;
			case SELECT_RANK:
				SelectNew = SELECT_TUTO;
				break;
			case SELECT_OPTION:
				SelectNew = SELECT_RANK;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_OPTION;
				break;
			}

			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * 3;
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);

			g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック

		}
		else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
		{
			switch (SelectNew)
			{
			case SELECT_PLAY:
				SelectNew = SELECT_VERSUS;
				break;
			case SELECT_VERSUS:
				SelectNew = SELECT_TUTO;
				break;
			case SELECT_TUTO:
				SelectNew = SELECT_RANK;
				break;
			case SELECT_RANK:
				SelectNew = SELECT_OPTION;
				break;
			case SELECT_OPTION:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_PLAY;
				break;
			}

			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * 3;
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_T * SelectNew + UV_DEF / U_MAX_T, UV_DEF / V_MAX_T * (SelectNew / U_MAX_T) + UV_DEF / V_MAX_T);

			g_pVtxBuffTitle->Unlock();//プレイヤーバッファのアンロック

		}
	}

	if (DemoCnt >= DEMO_TIME)
	{
		DemoCnt = 0;
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound(SOUND_LABEL_BGM);
			HWND hWnd = GethWnd();
			HRESULT hr = PlayVideo(GethWnd(), MOVIE_OP);
			if (FAILED(hr))
			{
				MessageBox(NULL, "動画の再生に失敗しました。", "エラー", MB_OK);
			}
			SetFade2(MODE_TITLE);
			PlaySound(SOUND_LABEL_BGM);
		}
	}
}

//-------------------
//描画処理
//-------------------
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

    //デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTitle[0]);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTitle[1]);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		VT_MAX,//始まりの番号
		2//ポリゴンの個数
	);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTitle[2]);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		VT_MAX*2,//始まりの番号
		2//ポリゴンの個数
	);

	if (g_TitleState != TITLESTATE_NONE)
	{
		if (g_TitleState == TITLESTATE_SELECT)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitle[4]);
		}
		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitle[3]);
		}

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX * 3,//始まりの番号
			2//ポリゴンの個数
		);
	}
}
