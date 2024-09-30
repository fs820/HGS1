//------------------------------------------
//
//テクスチャ表示プログラムのメインの処理[main.cpp]
//Author fuma sato
//
//------------------------------------------

#include"main.h"

#define IDI_ICON1 (WORD)"IDI_ICON1"

LPDIRECT3D9 g_pD3D = NULL;//ダイレクトXオブジェクトのグローバルポインタを宣言
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//ダイレクトXデバイスのグローバルポインタを宣言
MODE g_mode = MODE_TITLE;
HINSTANCE g_hInstance;
HWND g_hWnd;
bool g_bPause = false;
bool g_bFullScreen = false;
LPD3DXFONT g_pFont = NULL;//フォントポインタ
int g_nCountFPS = 0;//FPSカウンタ
bool g_bDebug = true;
int g_DebugCon = 0;

//------------------------
//メイン関数
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	//ウインドウ設定
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//メモリサイズ
		CS_CLASSDC,//スタイル
		WindowProc,//プロシージャ
		0,//0
		0,//0
		hInstance,//インスタンスハンドル
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),//タスクバーアイコン
		LoadCursor(NULL,IDC_ARROW),//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 3),//背景色
		NULL,//メニューバー
		CLASS_NAME,//クラスの名前
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))//アイコン
	};

	srand((int)time(NULL));//乱数設定
	HWND hWnd = nullptr;//ハンドル
	MSG msg = { 0 };//メッセージ
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//ウインドウサイズの設定
	DWORD dwCurrntTime;//現在のシステム時刻
	DWORD dwExceLastTime;//前回のシステム時刻

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域のサイズ調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx
	(
		0,//スタイル
		CLASS_NAME,//ウインドウクラスの名前
		WINDOW_NAME,//ウインドウの名前
		WS_OVERLAPPEDWINDOW,//スタイル
		CW_USEDEFAULT,//左上座標
		CW_USEDEFAULT,//右下座標
		(rect.right - rect.left),//幅
		(rect.bottom - rect.top),//高さ
		NULL,//親
		NULL,//メニューID
		hInstance,//インスタンスハンドル
		NULL//作成データ
	);
	// ウィンドウのアイコンを設定
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));

	g_hInstance = hInstance;
	g_hWnd = hWnd;

	while (ShowCursor(FALSE) >= 0);
	ConfineCursorToWindow(hWnd);

	//表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (FAILED(Init(hInstance, hWnd, TRUE)))//初期化処理
	{
		return E_FAIL;
	}

	timeBeginPeriod(1);//分解能の設定
	dwCurrntTime = 0;//初期時間
	dwExceLastTime = timeGetTime();//開始時刻を記録

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)//終了メッセージ
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (!g_bPause)
			{
				dwCurrntTime = timeGetTime();//時刻を取得
				if ((dwCurrntTime - dwFPSLastTime) >= FPS_TIME)
				{//0.5秒
					//FPS算出
					g_nCountFPS = (dwFrameCount * 1000) / (dwCurrntTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrntTime;
					dwFrameCount = 0;
				}

				if ((dwCurrntTime - dwExceLastTime) >= FPS)//設定した間隔が空いたら
				{//60分の1
					dwExceLastTime = dwCurrntTime;//今回の更新時刻を記録

					Update();//更新処理
					Draw();//描画処理

					dwFrameCount++;
				}
			}
		}
	}

	Uninit();//終了処理

	//ウインドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);//分解能を戻す

	ReleaseCursor();
	while (ShowCursor(TRUE) < 0);

	//終了
	return (int)msg.wParam;
}

//--------------------------
//ウインドウプロシージャ
//--------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID = 0;//返り値の格納
	static bool bFreeMause = false;

	switch (uMsg)
	{
	case WM_DESTROY://破棄
		//WM_QUITメッセージ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://キー押下
		switch (wParam)
		{
		case VK_ESCAPE://ESCキー
			ReleaseCursor();
			while (ShowCursor(TRUE) < 0);
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				//破棄(WM_DESTROY)
				DestroyWindow(hWnd);
			}
			else
			{
				while (ShowCursor(FALSE) >= 0);
				ConfineCursorToWindow(hWnd);
			}
			break;
		}
	case WM_SETFOCUS:
		g_bPause = false;
		ConfineCursorToWindow(hWnd);
		break;
	case WM_KILLFOCUS:
		g_bPause = true;
		break;
	}
	//必要データを返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//------------------------------
//初期化処理
//------------------------------
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//ダイレクトXディスプレイモードの変数宣言
	D3DPRESENT_PARAMETERS d3dpp;//ダイレクトXパラメータの変数宣言

	//オブジェクト生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモード
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	ZeroMemory(&d3dpp, sizeof(d3dpp));//0を敷き詰める

	//バックバッファの設定
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//デバイス生成
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		if (FAILED(g_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{
			if (FAILED(g_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリング
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//透明度
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ用フォント生成
	D3DXCreateFont
	(
		g_pD3DDevice,
		FONT_SIZE, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont
	);

	//オブジェクトの初期化処理
	
	//初期化

	SetMode(g_mode);

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void Uninit(void)
{
	//オブジェクト終了
	//終了

	//デバッグフォント破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//デバイス破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//オブジェクト破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//------------------------------
//更新処理
//------------------------------
void Update(void)
{
	//UpdateKeyborad();//キー入力
	//UpdateMouse();//マウス入力
	//UpdateJoypad();//パッド入力
	switch (g_mode)
	{
	case MODE_TITLE:
		//UpdateTitle();
		break;
	case MODE_DEMO:
		//UpdateDemo();
		break;
	case MODE_TUTO:
		//UpdateTuto();
		break;
	case MODE_PLAYSELECT:
		//UpdatePlaySelect();
		break;
	case MODE_GAME:
		//UpdateGame();
		break;
	case MODE_VS:
		//UpdateVs();
		break;
	case MODE_RESULT:
		//UpdateResult();
		break;
	case MODE_RANK:
		//UpdateRank();
		break;
	case MODE_OPTION:
		//UpdateOption();
		break;
	}
	//フェード
	//UpdateFade();
}

//------------------------------
//描画処理
//------------------------------
void Draw(void)
{
	g_pD3DDevice->Clear//フロントバッファのクリア
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0
	);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))//描画開始
	{
		//オブジェクト描画
		switch (g_mode)
		{
		case MODE_TITLE:
			//DrawTitle();
			break;
		case MODE_DEMO:
			//DrawDemo();
			break;
		case MODE_TUTO:
			//DrawTuto();
			break;
		case MODE_PLAYSELECT:
			//DrawPlaySelect();
			break;
		case MODE_GAME:
			//DrawGame();
			break;
		case MODE_VS:
			//DrawVs();
			break;
		case MODE_RESULT:
			//DrawResult();
			break;
		case MODE_RANK:
			//DrawRank();
			break;
		case MODE_OPTION:
			//DrawOption();
			break;
		}
		////フェード
		//DrawFade();

		////カーソル
		//DrawCursor();

#ifdef _DEBUG

		if (g_bDebug)
		{
			//デバッグ表示
			//DrawDebug(ControllerNum(CONTYPE_D), GetdJoyStick(STICK_LEFT, CONTROLLER_1), ControllerName(CONTROLLER_1));
		}

#endif

		g_pD3DDevice->EndScene();//描画終了
	}
	//バックバッファに表示を切り替える
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//------------------------
//モードの切り替え
//------------------------
void SetMode(MODE mode)
{
	//現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		//UninitTitle();
		break;
	case MODE_DEMO:
		//UninitDemo();
		break;
	case MODE_TUTO:
		//UninitTuto();
		break;
	case MODE_PLAYSELECT:
		//UninitPlaySelect();
		break;
	case MODE_GAME:
		//UninitGame();
		break;
	case MODE_VS:
		//UninitVs();
		break;
	case MODE_RESULT:
		//UninitResult();
		break;
	case MODE_RANK:
		//UninitRank();
		break;
	case MODE_OPTION:
		//UninitOption();
		break;
	}

	g_mode = mode;

	//新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		//InitTitle();
		break;
	case MODE_DEMO:
		//InitDemo();
		break;
	case MODE_TUTO:
		//InitTuto();
		break;
	case MODE_PLAYSELECT:
		//InitPlaySelect();
		break;
	case MODE_GAME:
		//InitGame();
		break;
	case MODE_VS:
		//InitVs();
		break;
	case MODE_RESULT:
		//InitResult();
		break;
	case MODE_RANK:
		//InitRank();
		break;
	case MODE_OPTION:
		//InitOption();
		break;
	}
}

//------------------------
//デバイスの取得
//------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------
//モードの取得
//-----------------
MODE GetMode(void)
{
	return g_mode;
}

//-----------------
//ハンドルの取得
//-----------------
HWND GethWnd(void)
{
	return g_hWnd;
}

//-----------------
//インスタンスハンドル取得
//-----------------
HINSTANCE GethInstanse(void)
{
	return g_hInstance;
}

//------------------
//マウス制限
//------------------
void ConfineCursorToWindow(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	rect.left += 10;
	rect.top += 10;
	rect.right -= 10;
	rect.bottom -= 10;

	MapWindowPoints(hWnd, NULL, (POINT*)&rect, 2);
	ClipCursor(&rect);
}

//-----------------
//マウス解放
//-----------------
void ReleaseCursor(void)
{
	ClipCursor(NULL); // 制限を解除
}


//-------------------------------------------------
// フルスクリーンとウィンドウモードを切り替える関数
//-------------------------------------------------
void ToggleFullScreen(HWND hWnd)
{
	static WINDOWPLACEMENT wpPrev = { sizeof(wpPrev) };

	g_bFullScreen = !g_bFullScreen; // フラグを反転

	// ウィンドウスタイルを変更
	if (g_bFullScreen)
	{
		GetWindowPlacement(hWnd, &wpPrev);

		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_FRAMECHANGED);

		//ディスプレイ設定を変更
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = SCREEN_WIDTH;
		dmScreenSettings.dmPelsHeight = SCREEN_HEIGHT;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// フルスクリーン設定に失敗した場合、ウィンドウモードに戻す
			SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hWnd, &wpPrev);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
			g_bFullScreen = false;
		}
	}
	else
	{
		ChangeDisplaySettings(NULL, 0);

		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &wpPrev);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	// デバイスを再作成
	//ResetDevice();
}

////----------------------------
//// デバイスを再作成する関数
////----------------------------
//void ResetDevice(void)
//{
//	// デバイスを解放
//	if (g_pD3DDevice != NULL)
//	{
//		//リセット前にリソースを解放
//		UninitCursor();
//		UninitOption();
//		UninitRank();
//		UninitResult();
//		UninitVs();
//		UninitPlaySelect();
//		UninitGame();
//		UninitTuto();
//		UninitDemo();
//		UninitTitle();
//		UninitFade();
//		UninitSound();
//
//		// デバイスをリセット
//		D3DPRESENT_PARAMETERS d3dpp;
//		ZeroMemory(&d3dpp, sizeof(d3dpp));
//		d3dpp.Windowed = !g_bFullScreen;
//		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
//		d3dpp.EnableAutoDepthStencil = TRUE;
//		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
//
//		HRESULT hr = g_pD3DDevice->Reset(&d3dpp);
//		if (FAILED(hr))
//		{
//			MessageBox(NULL, "だめーーー", "何で？？？？？？？？？", MB_OK | MB_ICONERROR);
//			exit(1);
//		}
//
//		InitSound(g_hWnd);
//		InitFade(g_mode);
//		InitCursor();
//	}
//}

//--------------------
//
//デバッグ用
//
//--------------------

////--------------------
////デバッグ表示
////--------------------
//void DrawDebug(int nData, float* fData, char* cData)
//{
//	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[1024] = { '\0' };
//	const DIDEVICEINSTANCE* dIns[CONTROLLER_MAX] = { nullptr };
//	XINPUT_STATE XSta[CONTROLLER_MAX] = { NULL };
//	DIJOYSTATE dSta[CONTROLLER_MAX] = { NULL };
//	wchar_t gStr[39] = { '\0' }; // GUID文字列用バッファ
//	for (int i = 0; i < CONTROLLER_MAX; i++)
//	{
//		dIns[i] = GetDiInstance((CONTROLLER)i);
//		XSta[i] = GetXstate((CONTROLLER)i);
//		dSta[i] = Getdstate((CONTROLLER)i);
//	}
//
//	//文字列の代入
//	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
//
//	//テキストの描画
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//	rect.top += FONT_SIZE;
//
//	if (IsDirectInputControllerConnected(CONTROLLER_MAX))
//	{
//		switch (g_DebugCon % CONTROLLER_MAX)
//		{
//		case 0:
//			if (IsDirectInputControllerConnected(CONTROLLER_1))
//			{
//				//文字列の代入
//				wsprintf(&aStr[0], "ControllerName:%s\n", ControllerName(CONTROLLER_1));
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				//文字列の代入
//				wsprintf(&aStr[0], "IsX?:%s\n", IsXInputControllerConnected(CONTROLLER_1) ? "true" : "false");
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				if (IsXInputControllerConnected(CONTROLLER_1))
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_LEFT, CONTROLLER_1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_LEFT, CONTROLLER_1) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XRX:%f\n", *GetJoyStick(STICK_RIGHT, CONTROLLER_1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XRY:%f\n", -*(GetJoyStick(STICK_RIGHT, CONTROLLER_1) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < JOYKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetJoykeyPress((JOYKEY)i, CONTROLLER_1) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//				else
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "DLX:%f\n", *GetdJoyStick(STICK_LEFT, CONTROLLER_1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "DLY:%f\n", *(GetdJoyStick(STICK_LEFT, CONTROLLER_1) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "DRX:%f\n", *GetdJoyStick(STICK_RIGHT, CONTROLLER_1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "DRY:%f\n", *(GetdJoyStick(STICK_RIGHT, CONTROLLER_1) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < ELEKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetdJoykeyPress((ELEKEY)i, CONTROLLER_1) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVUP:%s\n", GetdJoyPov(POV_UP, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVRIGHTUP:%s\n", GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVRIGHT:%s\n", GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVRIGHTDOWN:%s\n", GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVDOWN:%s\n", GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVLEFTDOWN:%s\n", GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVLEFT:%s\n", GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "POVLEFTUP:%s\n", GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_1) ? "true" : "false");
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//				}
//
//			}
//			break;
//		case 1:
//			if (IsDirectInputControllerConnected(CONTROLLER_2))
//			{
//				//文字列の代入
//				wsprintf(&aStr[0], "ControllerName:%s\n", ControllerName(CONTROLLER_2));
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				//文字列の代入
//				wsprintf(&aStr[0], "IsX?:%s\n", IsXInputControllerConnected(CONTROLLER_2) ? "true" : "false");
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				if (IsXInputControllerConnected(CONTROLLER_2))
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_LEFT, CONTROLLER_2));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_LEFT, CONTROLLER_2) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_RIGHT, CONTROLLER_2));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_RIGHT, CONTROLLER_2) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < JOYKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetJoykeyPress((JOYKEY)i, CONTROLLER_2) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//				else
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetdJoyStick(STICK_LEFT, CONTROLLER_2));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", *(GetdJoyStick(STICK_LEFT, CONTROLLER_2) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetdJoyStick(STICK_RIGHT, CONTROLLER_2));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", *(GetdJoyStick(STICK_RIGHT, CONTROLLER_2) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < ELEKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetdJoykeyPress((ELEKEY)i, CONTROLLER_2) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//			}
//			break;
//		case 2:
//			if (IsDirectInputControllerConnected(CONTROLLER_3))
//			{
//				//文字列の代入
//				wsprintf(&aStr[0], "ControllerName:%s\n", ControllerName(CONTROLLER_3));
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				//文字列の代入
//				wsprintf(&aStr[0], "IsX?:%s\n", IsXInputControllerConnected(CONTROLLER_3) ? "true" : "false");
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				if (IsXInputControllerConnected(CONTROLLER_3))
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_LEFT, CONTROLLER_3));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_LEFT, CONTROLLER_3) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_RIGHT, CONTROLLER_3));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_RIGHT, CONTROLLER_3) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < JOYKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetJoykeyPress((JOYKEY)i, CONTROLLER_3) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//				else
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetdJoyStick(STICK_LEFT, CONTROLLER_3));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", *(GetdJoyStick(STICK_LEFT, CONTROLLER_3) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetdJoyStick(STICK_RIGHT, CONTROLLER_3));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", *(GetdJoyStick(STICK_RIGHT, CONTROLLER_3) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < ELEKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetdJoykeyPress((ELEKEY)i, CONTROLLER_3) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//			}
//			break;
//		case 3:
//			if (IsDirectInputControllerConnected(CONTROLLER_4))
//			{
//				//文字列の代入
//				wsprintf(&aStr[0], "ControllerName:%s\n", ControllerName(CONTROLLER_4));
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				//文字列の代入
//				wsprintf(&aStr[0], "IsX?:%s\n", IsXInputControllerConnected(CONTROLLER_4) ? "true" : "false");
//				//テキストの描画
//				g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//				rect.top += FONT_SIZE;
//				if (IsXInputControllerConnected(CONTROLLER_4))
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_LEFT, CONTROLLER_4));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_LEFT, CONTROLLER_4) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetJoyStick(STICK_RIGHT, CONTROLLER_4));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", -*(GetJoyStick(STICK_RIGHT, CONTROLLER_4) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < JOYKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetJoykeyPress((JOYKEY)i, CONTROLLER_4) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//				else
//				{
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetdJoyStick(STICK_LEFT, CONTROLLER_4));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", *(GetdJoyStick(STICK_LEFT, CONTROLLER_4) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLX:%f\n", *GetdJoyStick(STICK_RIGHT, CONTROLLER_4));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					//文字列の代入
//					snprintf(&aStr[0], sizeof(aStr), "XLY:%f\n", *(GetdJoyStick(STICK_RIGHT, CONTROLLER_4) + 1));
//					//テキストの描画
//					g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//					rect.top += FONT_SIZE;
//					for (int i = 0; i < ELEKEY_MAX; i++)
//					{
//						//文字列の代入
//						wsprintf(&aStr[0], "Key%d:%s\n", i, GetdJoykeyPress((ELEKEY)i, CONTROLLER_4) ? "true" : "false");
//						//テキストの描画
//						g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
//						rect.top += FONT_SIZE;
//					}
//				}
//			}
//			break;
//		}
//	}
//}