//---------------------------------------
//
//入力処理の定義・宣言[input.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_
#include"main.h"

#define STICK_DED (0.8f)//デッドゾーン
#define VIBRATION_MAX (65535)//バイブレーション値

//POV
#define POV_UP (0.0f)//上
#define POV_RIGHTUP (4500.0f)//右上
#define POV_RIGHT (9000.0f)//右
#define POV_RIGHTDOWN (13500.0f)//右下
#define POV_DOWN (18000.0f)//下
#define POV_LEFTDOWN (22500.0f)//左下
#define POV_LEFT (27000.0f)//左
#define POV_LEFTUP (31500.0f)//左上

//マウスボタンの種類
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_SENTER,
	MOUSE_B1,
	MOUSE_B2,
	MOUSE_B3,
	MOUSE_B4,
	MOUSE_B5,
	MOUSE_MAX
}MOUSE;

//コントローラーの種類
typedef enum
{
	CONTROLLER_1 = 0,
	CONTROLLER_2,
	CONTROLLER_3,
	CONTROLLER_4,
	CONTROLLER_MAX
}CONTROLLER;

//コントローラータイプ
typedef enum
{
	CONTYPE_X = 0,
	CONTYPE_D,
	CONTYPE_MAX
}CONTYPE;

//Xパッドボタンの種類
typedef enum
{
	JOYKEY_UP = 0,
	JOYKEY_DOWN,
	JOYKEY_LEFT,
	JOYKEY_RIGHT,
	JOYKEY_START,
	JOYKEY_BACK,
	JOYKEY_L3,
	JOYKEY_R3,
	JOYKEY_LB,
	JOYKEY_RB,
	JOYKEY_LT,
	JOYKEY_RT,
	JOYKEY_A,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,
	JOYKEY_MAX
}JOYKEY;

//ELEパッドボタンの種類
typedef enum
{
	ELEKEY_A = 0,
	ELEKEY_B,
	ELEKEY_X,
	ELEKEY_Y,
	ELEKEY_LB,
	ELEKEY_RB,
	ELEKEY_LT,
	ELEKEY_RT,
	ELEKEY_L3,
	ELEKEY_R3,
	ELEKEY_BACK,
	ELEKEY_START,
	ELEKEY_SENTER,
	ELEKEY_MAX
}ELEKEY;

//PSパッドボタンの種類
typedef enum
{
	PSKEY_SQ = 0,
	PSKEY_CR,
	PSKEY_CI,
	PSKEY_TRA,
	PSKEY_LB,
	PSKEY_RB,
	PSKEY_LT,
	PSKEY_RT,
	PSKEY_BACK,
	PSKEY_START,
	PSKEY_L3,
	PSKEY_R3,
	PSKEY_SENTER,
	PSKEY_PAD,
	PSKEY_MAX
}PSKEY;

//NINパッドボタンの種類
typedef enum
{
	NINKEY_B = 0,
	NINKEY_A,
	NINKEY_Y,
	NINKEY_X,
	NINKEY_LB,
	NINKEY_RB,
	NINKEY_LT,
	NINKEY_RT,
	NINKEY_－,
	NINKEY_＋,
	NINKEY_L3,
	NINKEY_R3,
	NINKEY_HOME,
	NINKEY_CAP,
	NINKEY_MAX
}NINKEY;

//Dパッドボタンの種類
typedef enum
{
	DKEY_A = 0,
	DKEY_B,
	DKEY_X,
	DKEY_Y,
	DKEY_LB,
	DKEY_RB,
	DKEY_LT,
	DKEY_RT,
	DKEY_L3,
	DKEY_R3,
	DKEY_BACK,
	DKEY_START,
	DKEY_SENTER,
	DKEY_B1,
	DKEY_B2,
	DKEY_MAX
}DKEY;

//パッドスティックの種類
typedef enum
{
	STICK_LEFT = 0,
	STICK_RIGHT,
	STICK_MAX
}STICK;

//スティック方向の種類
typedef enum
{
	DIRESTICK_UP = 0,
	DIRESTICK_DOWN,
	DIRESTICK_LEFT,
	DIRESTICK_RIGHT,
	DIRESTICK_MAX
}DIRESTICK;

//キーボード
HRESULT InitKeyboard(HINSTANCE hInstanse, HWND hWnd);//初期化処理
void UninitKeyborad(void);//終了処理
void UpdateKeyborad(void);//更新処理
bool GetKeyboradPress(int nkey);//キー押下処理
bool GetKeyboradTrigger(int nkey);//キー入力処理
bool GetKeyboradRelease(int nkey);//キー離し処理
bool GetKeyboradRepeat(int nkey);//キー長押し処理

//マウス
HRESULT InitMouse(HINSTANCE hInstanse, HWND hWnd);//初期化処理
void UninitMouse(void);//終了処理
void UpdateMouse(void);//更新処理
bool GetMousePress(int nkey);//キー押下処理
bool GetMouseTrigger(int nkey);//キー入力処理
bool GetMouseRelease(int nkey);//キー離し処理
bool GetMouseRepeat(int nkey);//キー長押し処理
float *GetMouseMove(void);//マウス処理
float GetMouseWheel(void);//ホイール処理

//Xinput
HRESULT InitJoypad(void);//初期化処理
void UninitJoypad(void);//終了処理
void UpdateJoypad(void);//更新処理
bool GetJoykeyPress(JOYKEY key, CONTROLLER Controller);//キー押下処理
bool GetJoykeyTrigger(JOYKEY key, CONTROLLER Controller);//キー入力処理
bool GetJoykeyRelease(JOYKEY key, CONTROLLER Controller);//キー離し処理
bool GetJoykeyRepeat(JOYKEY key, CONTROLLER Controller);//キー長押し処理
float *GetJoyStick(STICK Stick, CONTROLLER Controller);//スティック処理
bool JoyStickPress(DIRESTICK DireStick ,STICK Stick, CONTROLLER Controller);//スティック押下処理
bool JoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック入力処理
bool JoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック離し処理
bool JoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック長押し処理
void VibrateController(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, CONTROLLER Controller);//バイブレーション処理

//dinputパッド
HRESULT InitdJoypad(HINSTANCE hInstanse, HWND hWnd);//初期化処理
void UninitdJoypad(void);//終了処理
void UpdatedJoypad(void);//更新処理
bool GetdJoykeyPress(int nkey, CONTROLLER Controller);//キー押下処理
bool GetdJoykeyTrigger(int nkey, CONTROLLER Controller);//キー入力処理
bool GetdJoykeyRelease(int nkey, CONTROLLER Controller);//キー離し処理
bool GetdJoykeyRepeat(int nkey, CONTROLLER Controller);//キー長押し処理
float* GetdJoyStick(STICK Stick, CONTROLLER Controller);//スティック処理
bool dJoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
bool dJoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック入力処理
bool dJoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック離し処理
bool dJoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック長押し処理
float GetdJoySlider(int nSlider, CONTROLLER Controller);//スライダー軸処理
bool GetdJoyPov(float fData,int nPov, CONTROLLER Controller);//POV押下処理
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller);//POV入力処理
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller);//POV離し処理
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller);//POV長押し処理
void VibratedController(LONG lMotorPower, CONTROLLER Controller);//バイブレーション処理
void VibratedControllerStop(CONTROLLER Controller);//バイブレーション停止処理

void SetVibrate(float fPower, CONTROLLER Controller);//バイブレーション設定

//接続確認
bool IsXInputControllerConnected(CONTROLLER Controller);//Xinput接続確認
bool IsDirectInputControllerConnected(CONTROLLER Controller);//dinpit接続確認
char *ControllerName(CONTROLLER Controller);//コントローラーインスタンスネーム取得
int ControllerNum(CONTYPE Contype);//コントローラー数取得
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);//コントローラー列挙コールバック関数
int XNum(void);//Xinput接続数取得

LPDIRECTINPUT8 GetInput(void);//入力ポインタ取得
void SetUnCon(bool bUnCon);

void InputReSet(void);

const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller);
XINPUT_STATE GetXstate(CONTROLLER Controller);
DIJOYSTATE Getdstate(CONTROLLER Controller);

#endif _INPUT_H_

