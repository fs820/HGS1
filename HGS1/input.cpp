//---------------------------------------
//
//入力処理[input.cpp]
//Author fuma sato
//
//---------------------------------------

#include"input.h"

#define STICK_NUM (32767.0f)
#define DSTICK_NUM (-1.0f)
#define MOUSE_NUM (15.0f)
#define MOUSE_NUM_WHEEL (300.0f)
#define NUM_KEY_MAX (256)//キーの最大数
#define NUM_DIJOYKEY_MAX (32)//dinputボタンの最大数
#define POV_MAX (4)
#define POV_SYS_MAX (8)
#define POV_NUM (4500.0f)
#define RELEASE_TIME (50)//ため時間
#define REPEAT_TIME (40)//長押し判定

LPDIRECTINPUT8 g_pinput = NULL;//インプットポインタ
LPDIRECTINPUTDEVICE8 g_Devkeyboard = NULL;//キーボードデバイスポインタ
BYTE g_akeyState[NUM_KEY_MAX];//キー情報
LPDIRECTINPUTDEVICE8 g_DevMouse = NULL;//マウスデバイスポインタ
DIMOUSESTATE2 g_MouseState;//マウス情報
XINPUT_STATE g_joykeyState[CONTROLLER_MAX] = { NULL };//パッド情報
LPDIRECTINPUTDEVICE8 g_DevdJoypad[CONTROLLER_MAX] = { NULL };//パッドデバイスポインタ
DIJOYSTATE g_djoykeyState[CONTROLLER_MAX] = { NULL };//dパッド情報
LPDIRECTINPUTEFFECT g_djoyEffect[CONTROLLER_MAX] = { NULL };
char g_ConName[CONTROLLER_MAX][64] = { NULL };
int g_XcontrollerNum = 0;
int g_DcontrollerNum = 0;
bool g_bUnCon = false;
bool g_bX = false;
const DIDEVICEINSTANCE* g_pDiInstance[CONTROLLER_MAX];

//--------------------------
//
//キーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT InitKeyboard(HINSTANCE hInstanse, HWND hWnd)
{
	//インプットオブジェクトの作成
	if (FAILED(DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		NULL
	)))
	{
		return E_FAIL;
	}

	//インプットデバイスの作成
	if (FAILED(g_pinput->CreateDevice(GUID_SysKeyboard, &g_Devkeyboard, NULL)))
	{
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(g_Devkeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	if (FAILED(g_Devkeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	g_Devkeyboard->Acquire();//アクセス権限の取得

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void UninitKeyborad(void)
{
	//キーボードデバイスの破棄
	if (g_Devkeyboard != NULL)
	{
		g_Devkeyboard->Unacquire();
		g_Devkeyboard->Release();
		g_Devkeyboard = NULL;
	}
	//インプットの破棄
	if (g_pinput != NULL)
	{
		g_pinput->Release();
		g_pinput = NULL;
	}
}

//-------------------------
//更新処理
//-------------------------
void UpdateKeyborad(void)
{
	BYTE akeystate[NUM_KEY_MAX];//一時入力格納
	int nCntkey;//キー入力カウント

	if (SUCCEEDED(g_Devkeyboard->GetDeviceState(sizeof(akeystate), &akeystate[0])))
	{
		for (nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++)//キー入力分繰り返す
		{
			g_akeyState[nCntkey] = akeystate[nCntkey];//一時的な情報を正規情報に渡す
		}
	}
	else
	{
		g_Devkeyboard->Acquire();//アクセス権限の取得
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool GetKeyboradPress(int nkey)
{
	return (g_akeyState[nkey] & 0x80) ? true : false;
}

//------------------------------
//キー入力時の処理
//------------------------------
bool GetKeyboradTrigger(int nkey)
{
	static bool bOldkey[NUM_KEY_MAX] = { false };

	if (bOldkey[nkey]==false&&((g_akeyState[nkey] & 0x80)?true:false)==true)
	{
     	bOldkey[nkey] = true;
		return true;
	}
	else if(bOldkey[nkey] ==true&&((g_akeyState[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[nkey] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool GetKeyboradRelease(int nkey)
{
	static bool bOldkey[NUM_KEY_MAX] = { false };
	static int KeyPutTime[NUM_KEY_MAX] = { 0 };

	if (bOldkey[nkey] == true && ((g_akeyState[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[nkey] = false;
		if (KeyPutTime[nkey] >=RELEASE_TIME)
		{
			KeyPutTime[nkey] = 0;
			return true;
		}
		else
		{
			KeyPutTime[nkey] = 0;
			return false;
		}
	}
	else if (bOldkey[nkey] == false && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		bOldkey[nkey] = true;
		return false;
	}
	else if (bOldkey[nkey] == true && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[nkey]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー長押し処理
//------------------------------
bool GetKeyboradRepeat(int nkey)
{
	static bool bOldkey[NUM_KEY_MAX] = { false };
	static int KeyPutTime[NUM_KEY_MAX] = { 0 };

	if (bOldkey[nkey] == true && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[nkey] == false && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[nkey]++;

		if (KeyPutTime[nkey] >=REPEAT_TIME)
		{
			KeyPutTime[nkey] = 0;
			bOldkey[nkey] = true;
		}
		return false;
	}
	else
	{
		bOldkey[nkey] = false;
		KeyPutTime[nkey] = 0;
		return false;
	}
}

//--------------------------
//
//マウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT InitMouse(HINSTANCE hInstanse, HWND hWnd)
{
	//インプットオブジェクトの作成
	if (FAILED(DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		NULL
	)))
	{
		return E_FAIL;
	}

	//インプットデバイスの作成
	if (FAILED(g_pinput->CreateDevice(GUID_SysMouse, &g_DevMouse, NULL)))
	{
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(g_DevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	if (FAILED(g_DevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	g_DevMouse->Acquire();//アクセス権限の取得

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void UninitMouse(void)
{
	//キーボードデバイスの破棄
	if (g_DevMouse != NULL)
	{
		g_DevMouse->Unacquire();
		g_DevMouse->Release();
		g_DevMouse = NULL;
	}
	//インプットの破棄
	if (g_pinput != NULL)
	{
		g_pinput->Release();
		g_pinput = NULL;
	}
}

//-------------------------
//更新処理
//-------------------------
void UpdateMouse(void)
{
	DIMOUSESTATE2 MouseState;//マウス情報//一時入力格納

	if (SUCCEEDED(g_DevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		g_MouseState = MouseState;//一時的な情報を正規情報に渡す
	}
	else
	{
		g_DevMouse->Acquire();//アクセス権限の取得
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool GetMousePress(int nkey)
{
	return (g_MouseState.rgbButtons[nkey] & 0x80) ? true : false;
}

//------------------------------
//キー入力時の処理
//------------------------------
bool GetMouseTrigger(int nkey)
{
	static bool bOldMouse[MOUSE_MAX] = { false };

	if (bOldMouse[nkey] == false && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldMouse[nkey] = true;
		return true;
	}
	else if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldMouse[nkey] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool GetMouseRelease(int nkey)
{
	static bool bOldMouse[MOUSE_MAX] = { false };
	static int MousePutTime[MOUSE_MAX] = { 0 };

	if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldMouse[nkey] = false;
		if (MousePutTime[nkey] >= RELEASE_TIME)
		{
			MousePutTime[nkey] = 0;
			return true;
		}
		else
		{
			MousePutTime[nkey] = 0;
			return false;
		}
	}
	else if (bOldMouse[nkey] == false && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldMouse[nkey] = true;
		return false;
	}
	else if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		MousePutTime[nkey]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー長押し処理
//------------------------------
bool GetMouseRepeat(int nkey)
{
	static bool bOldMouse[MOUSE_MAX] = { false };
	static int MousePutTime[MOUSE_MAX] = { 0 };
	
	if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldMouse[nkey] == false && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		MousePutTime[nkey]++;

		if (MousePutTime[nkey] >= REPEAT_TIME)
		{
			MousePutTime[nkey] = 0;
			bOldMouse[nkey] = true;
		}
		return false;
	}
	else
	{
		bOldMouse[nkey] = false;
		MousePutTime[nkey] = 0;
		return false;
	}
}

//--------------
//マウス移動
//--------------
float* GetMouseMove(void)
{
	static float MouseMove[2] = { 0.0f };

	MouseMove[0]=(float)g_MouseState.lX;
	MouseMove[1]=(float)g_MouseState.lY;

	MouseMove[0] /= MOUSE_NUM;
	MouseMove[1] /= MOUSE_NUM;

	return &MouseMove[0];
}

//----------------
//キー押下処理
//----------------
float GetMouseWheel(void)
{
	float MouseWheel = 0.0f;

	MouseWheel = (float)g_MouseState.lZ;

	MouseWheel /= MOUSE_NUM_WHEEL;

	return MouseWheel;
}

//--------------------------
//
//パッド
//
//--------------------------

//--------------------------------
//初期化処理
//--------------------------------
HRESULT InitJoypad(void)
{
	memset(&g_joykeyState,0,sizeof(XINPUT_STATE)* CONTROLLER_MAX);

	XInputEnable(true);

	return S_OK;
}

//------------------------
//終了処理
//------------------------
void UninitJoypad(void)
{
	XInputEnable(false);
}

//------------------------
//更新処理
//------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
		{
			g_joykeyState[i] = joykeyState[i];
		}
		else
		{
			ControllerNum++;
		}
	}

	g_XcontrollerNum = CONTROLLER_MAX - ControllerNum;
}

//--------------------------
//キー押下処理
//--------------------------
bool GetJoykeyPress(JOYKEY key, CONTROLLER Controller)
{
	BYTE Trigger = 0;

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}
			
			if (g_joykeyState[i].Gamepad.wButtons & (0x01 << key) || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return true;
			}
		}

		return false;
	}

	if (key==JOYKEY_LT||key==JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	return (g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)|| Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true: false;
}

//--------------------------
//キー入力処理
//--------------------------
bool GetJoykeyTrigger(JOYKEY key, CONTROLLER Controller)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = true;
				return true;
			}
			else if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = false;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true|| Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = true;
		return true;
	}
	else if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == false&& Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = false;
		return false;
	}
	else
	{
		return false;
	}
}


//----------------------------
//キー離し処理
//----------------------------
bool GetJoykeyRelease(JOYKEY key, CONTROLLER Controller)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = false;
				if (KeyPutTime[i][key] >= RELEASE_TIME)
				{
					KeyPutTime[i][key] = 0;
					return true;
				}
				else
				{
					KeyPutTime[i][key] = 0;
				}
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = true;
			}
			else if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = false;
		if (KeyPutTime[Controller][key] >= RELEASE_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			return true;
		}
		else
		{
			KeyPutTime[Controller][key] = 0;
			return false;
		}
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = true;
		return false;
	}
	else if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;
		return false;
	}
	else
	{
		return false;
	}
}

//----------------------------
//キー長押し処理
//----------------------------
bool GetJoykeyRepeat(JOYKEY key, CONTROLLER Controller)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				return true;
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldJoy[i][key] = true;
				}
			}
			else
			{
				bOldJoy[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		return true;
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldJoy[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldJoy[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
//スティック処理
//-----------------
float *GetJoyStick(STICK Stick, CONTROLLER Controller)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------------------
//
//-----------------------------
bool JoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_DOWN:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_LEFT:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_RIGHT:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}

	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		return fStick > STICK_DED;
		break;
	case DIRESTICK_DOWN:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_LEFT:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_RIGHT:
		return fStick > STICK_DED;
		break;
	}

	return false;
}

//-----------------------------
//
//-----------------------------
bool JoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = {false};

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
					break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//-----------------------------
//
//-----------------------------
bool JoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//-----------------------------
//
//-----------------------------
bool JoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			}
		}
		return false;
	}

	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	}

	return false;
}

//----------------
//振動
//----------------
void VibrateController(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, CONTROLLER Controller)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = wLeftMotorSpeed;  // 左の振動モーターの速度 (0-65535)
	vibration.wRightMotorSpeed = wRightMotorSpeed; // 右の振動モーターの速度 (0-65535)

	XInputSetState((DWORD)Controller, &vibration);
}

//-----------------------------
//
//dinputパッド
//
//-----------------------------

//--------------------
//初期化処理
//--------------------
HRESULT InitdJoypad(HINSTANCE hInstanse, HWND hWnd)
{
	//インプットオブジェクトの作成
	DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		NULL
	);

	// DirectInputデバイスを列挙
	g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void UninitdJoypad(void)
{
	//エフェクトデバイスの破棄
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//キーボードデバイスの破棄
		if (g_DevdJoypad[i] != NULL)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = NULL;
		}
	}
	//インプットの破棄
	if (g_pinput != NULL)
	{
		g_pinput->Release();
		g_pinput = NULL;
	}
}

//-------------------------
//更新処理
//-------------------------
void UpdatedJoypad(void)
{
	DIJOYSTATE  djoykeyState[CONTROLLER_MAX];//一時入力格納
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (g_DevdJoypad[i] != NULL)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &djoykeyState[i])))
			{
				g_djoykeyState[i] = djoykeyState[i];//一時的な情報を正規情報に渡す
			}
			else
			{
				ControllerNum++;
			}
		}
		else
		{
			ControllerNum++;
		}
	}

	if (g_DcontrollerNum!=(CONTROLLER_MAX-ControllerNum))
	{
		SetUnCon(true);
		InputReSet();
		// DirectInputデバイスを列挙
		g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool GetdJoykeyPress(int nkey, CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false)
			{
				return true;
			}
		}

		return false;
	}

	return (g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false;
}

//------------------------------
//キー入力時の処理
//------------------------------
bool GetdJoykeyTrigger(int nkey, CONTROLLER Controller)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][nkey] == false && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				bOldkey[i][nkey] = true;
				return true;
			}
			else if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == false)
			{
				bOldkey[i][nkey] = false;
			}
		}

		return false;
	}

	if (bOldkey[Controller][nkey] == false && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldkey[Controller][nkey] = true;
		return true;
	}
	else if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[Controller][nkey] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool GetdJoykeyRelease(int nkey, CONTROLLER Controller)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == false)
			{
				bOldkey[i][nkey] = false;
				if (KeyPutTime[i][nkey] >= RELEASE_TIME)
				{
					KeyPutTime[i][nkey] = 0;
					return true;
				}
				else
				{
					KeyPutTime[i][nkey] = 0;
				}
			}
			else if (bOldkey[i][nkey] == false && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				bOldkey[i][nkey] = true;
			}
			else if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][nkey]++;
			}
		}

		return false;
	}

	if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[Controller][nkey] = false;
		if (KeyPutTime[Controller][nkey] >= RELEASE_TIME)
		{
			KeyPutTime[Controller][nkey] = 0;
			return true;
		}
		else
		{
			KeyPutTime[Controller][nkey] = 0;
			return false;
		}
	}
	else if (bOldkey[Controller][nkey] == false && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldkey[Controller][nkey] = true;
		return false;
	}
	else if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][nkey]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー長押し処理
//------------------------------
bool GetdJoykeyRepeat(int nkey, CONTROLLER Controller)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				return true;
			}
			else if (bOldkey[i][nkey] == false && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][nkey]++;

				if (KeyPutTime[i][nkey] >= REPEAT_TIME)
				{
					KeyPutTime[i][nkey] = 0;
					bOldkey[i][nkey] = true;
				}
			}
			else
			{
				bOldkey[i][nkey] = false;
				KeyPutTime[i][nkey] = 0;
			}
		}

		return false;
	}

	if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[Controller][nkey] == false && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][nkey]++;

		if (KeyPutTime[Controller][nkey] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][nkey] = 0;
			bOldkey[Controller][nkey] = true;
		}
		return false;
	}
	else
	{
		bOldkey[Controller][nkey] = false;
		KeyPutTime[Controller][nkey] = 0;
		return false;
	}
}

//-----------------
//スティック処理
//-----------------
float* GetdJoyStick(STICK Stick, CONTROLLER Controller)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_djoykeyState[Controller].lX;
		fStick[1] = (float)g_djoykeyState[Controller].lY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_djoykeyState[Controller].lZ;
		fStick[1] = (float)g_djoykeyState[Controller].lRz;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	fStick[0] += DSTICK_NUM;
	fStick[1] += DSTICK_NUM;

	return &fStick[0];
}

//
//
//
bool dJoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_DOWN:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_LEFT:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_RIGHT:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}
	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		return fStick > STICK_DED;
		break;
	case DIRESTICK_DOWN:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_LEFT:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_RIGHT:
		return fStick > STICK_DED;
		break;
	}

	return false;
}

//
//
//
bool dJoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//
//
//
bool dJoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//
//
//
bool dJoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			}
		}
		return false;
	}

	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	}
	return false;
}

//----------------------
//スライダー軸
//----------------------
float GetdJoySlider(int nSlider, CONTROLLER Controller)
{
	float fSlider = 0.0f;

	fSlider = (float)g_djoykeyState[Controller].rglSlider[nSlider];

	fSlider /= STICK_NUM;

	fSlider += DSTICK_NUM;

	return fSlider;
}

//------------------------
//ハットスイッチ
//------------------------
bool GetdJoyPov(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (fPov==fData)
			{
				return true;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	return (fPov == fData) ? true : false;
}

//-------------------------
//
//-------------------------
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//-----------------------
//
//-----------------------
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return true;
				}
				else
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return false;
				}
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return false;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return true;
		}
		else
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return false;
		}
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return false;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;
		return false;
	}
	else
	{
		return false;
	}
}

//---------------------
//
//---------------------
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;

				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				}
				return false;
			}
			else
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
			}


		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;

		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		}
		return false;
	}
	else
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
		return false;
	}
}

//-------------
//振動
//-------------
void VibratedController(LONG lMotorPower, CONTROLLER Controller)
{
	// フォースフィードバックのエフェクトを作成する
	DIEFFECT effect;
	ZeroMemory(&effect, sizeof(DIEFFECT));
	effect.dwSize = sizeof(DIEFFECT);
	effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	effect.dwDuration = INFINITE;  // 無限に続く振動
	effect.dwGain = DI_FFNOMINALMAX;
	effect.dwTriggerButton = DIEB_NOTRIGGER;

	// 軸と方向の設定
	DWORD rgdwAxes[1] = { DIJOFS_X };  // X軸の振動
	LONG rglDirection[1] = { 0 };      // 方向は正方向（0）
	effect.cAxes = 1;                  // 1軸
	effect.rgdwAxes = rgdwAxes;        // 使用する軸
	effect.rglDirection = rglDirection;// 振動の方向

	DICONSTANTFORCE cf;
	cf.lMagnitude = lMotorPower;  // 振動の強さ (0 - DI_FFNOMINALMAX)

	effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	effect.lpvTypeSpecificParams = &cf;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->CreateEffect(GUID_ConstantForce, &effect, &g_djoyEffect[i], NULL)))
			{
				if (FAILED(g_djoyEffect[i]->Start(1, 0)))
				{
					g_djoyEffect[i]->Release();
				}
			}
		}
	}
	else if (SUCCEEDED(g_DevdJoypad[Controller]->CreateEffect(GUID_ConstantForce, &effect, &g_djoyEffect[Controller], NULL)))
	{
		if (FAILED(g_djoyEffect[Controller]->Start(1, 0)))
		{
			g_djoyEffect[Controller]->Release();
		}
	}
}

//---------
//振動
//---------
void VibratedControllerStop(CONTROLLER Controller)
{
	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_djoyEffect[i] != NULL)
			{
				g_djoyEffect[i]->Stop();
				g_djoyEffect[i]->Release();
				g_djoyEffect[i] = NULL;
			}
		}
	}
	else if (g_djoyEffect[Controller] != NULL)
	{
		g_djoyEffect[Controller]->Stop();
		g_djoyEffect[Controller]->Release();
		g_djoyEffect[Controller] = NULL;
	}
}


//------------------------
//
//振動
//
//------------------------
void SetVibrate(float fPower, CONTROLLER Controller)
{
	static bool bVibate = false;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (g_DcontrollerNum>=(Controller+1)||Controller==CONTROLLER_MAX)
	{
		if (bVibate)
		{
			if (Controller == CONTROLLER_MAX)
			{
				for (int i = 0; i < g_DcontrollerNum; i++)
				{
					if (XInputGetState((CONTROLLER)i, &state) == ERROR_SUCCESS)
					{
						VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), (CONTROLLER)i);
					}
					else
					{
						VibratedControllerStop((CONTROLLER)i);
					}
				}
			}
			else
			{
				if (XInputGetState(Controller, &state) == ERROR_SUCCESS)
				{
					VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), Controller);
				}
				else
				{
					VibratedControllerStop(Controller);
				}
			}
			bVibate = false;
		}
		else
		{
			if (Controller == CONTROLLER_MAX)
			{
				for (int i = 0; i < g_DcontrollerNum; i++)
				{
					if (XInputGetState(i, &state) == ERROR_SUCCESS)
					{
						VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), (CONTROLLER)i);
					}
					else
					{
						VibratedController((LONG)(DI_FFNOMINALMAX * fPower), (CONTROLLER)i);
						bVibate = true;
					}
				}
			}

			if (XInputGetState(Controller, &state) == ERROR_SUCCESS)
			{
				VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), Controller);
			}
			else
			{
				VibratedController((LONG)(DI_FFNOMINALMAX * fPower), Controller);
				bVibate = true;
			}

		}
	}
}

//--------------------------------
//接続確認
//--------------------------------

//-----------
//Xinput
//-----------
bool IsXInputControllerConnected(CONTROLLER Controller)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(Controller, &joykeyState[Controller]) == ERROR_SUCCESS;
}

//------------
//dinput
//------------
bool IsDirectInputControllerConnected(CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_DevdJoypad[i] != NULL)
			{
				return true;
			}
		}

		return false;
	}

	return g_DevdJoypad[Controller] != NULL;
}

//------------------
//コントローラー名
//------------------
char *ControllerName(CONTROLLER Controller)
{
	return &g_ConName[Controller][0];
}

//------------------
//コントローラー数
//------------------
int ControllerNum(CONTYPE Contype)
{
	if (Contype==CONTYPE_X)
	{
		return g_XcontrollerNum;
	}
	else
	{
		return g_DcontrollerNum;
	}
}

//---------------------------
//デバイス情報取得
//---------------------------
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	static int nControllerNum = 0;
	static int XcontrollerNum = 0;

	if (g_bUnCon)
	{
		nControllerNum = 0;
		XcontrollerNum = 0;
		g_DcontrollerNum = 0;
		g_XcontrollerNum = 0;
		g_bX = false;
		g_bUnCon = false;
		//エフェクトデバイスの破棄
		VibratedControllerStop(CONTROLLER_MAX);
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			//キーボードデバイスの破棄
			if (g_DevdJoypad[i] != NULL)
			{
				g_DevdJoypad[i]->Unacquire();
				g_DevdJoypad[i]->Release();
				g_DevdJoypad[i] = NULL;
			}
		}
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			g_ConName[i][0] = '\0';
		}
	}

	if ((nControllerNum+XcontrollerNum)<=CONTROLLER_MAX)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		g_pDiInstance[nControllerNum] = pdidInstance;

		if (XInputGetState(XcontrollerNum, &state) == ERROR_SUCCESS)
		{
			if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[XcontrollerNum], NULL)))
			{//インプットデバイスの作成
				HINSTANCE hInstanse;
				HWND hWnd;

				hInstanse = GethInstanse();
				hWnd = GethWnd();

				//フォーマット作成
				g_DevdJoypad[nControllerNum]->SetDataFormat(&c_dfDIJoystick);

				g_DevdJoypad[nControllerNum]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

				g_DevdJoypad[nControllerNum]->Acquire();//アクセス権限の取得

				strcpy(&g_ConName[nControllerNum][0], pdidInstance->tszProductName);
				XcontrollerNum++;
			}
		}
		else if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[nControllerNum + XNum()], NULL)))
		{//インプットデバイスの作成
			HINSTANCE hInstanse;
			HWND hWnd;

			hInstanse = GethInstanse();
			hWnd = GethWnd();

			//フォーマット作成
			g_DevdJoypad[nControllerNum + XNum()]->SetDataFormat(&c_dfDIJoystick);

			g_DevdJoypad[nControllerNum + XNum()]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

			g_DevdJoypad[nControllerNum + XNum()]->Acquire();//アクセス権限の取得

			strcpy(&g_ConName[nControllerNum + XNum()][0], pdidInstance->tszProductName);
			nControllerNum++;
		}
	}
	g_DcontrollerNum = nControllerNum+XcontrollerNum;

	// 列挙を続行
	return DIENUM_CONTINUE;
}

//----------
//
//----------
int XNum(void)
{
	int XNum = 0;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			XNum++;
		}
	}
	return XNum;
}

LPDIRECTINPUT8 GetInput(void)
{
	return g_pinput;
}

void SetUnCon(bool bUnCon)
{
	g_bUnCon = bUnCon;
}

//-----------------------
//コントローラー管理初期化
//-----------------------
void InputReSet(void)
{
	g_bUnCon = false;
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		g_ConName[i][0] = '\0';
	}
	g_XcontrollerNum = 0;
	g_DcontrollerNum = 0;
    g_bX = false;
	//エフェクトデバイスの破棄
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//キーボードデバイスの破棄
		if (g_DevdJoypad[i] != NULL)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = NULL;
		}
	}
}

//------------------------------------------------
//コントローラー情報取得
//------------------------------------------------
const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller)
{
	return g_pDiInstance[Controller];
}

//----------------------------------------------
//Xinputコントローラー情報取得
//----------------------------------------------
XINPUT_STATE GetXstate(CONTROLLER Controller)
{
	return g_joykeyState[Controller];
}

//----------------------------------------------
//dinputコントローラー情報取得
//----------------------------------------------
DIJOYSTATE Getdstate(CONTROLLER Controller)
{
	return g_djoykeyState[Controller];
}