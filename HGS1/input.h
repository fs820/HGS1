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

void SetVibrate(float fPower, CONTROLLER Controller);//バイブレーション設定

//接続確認
bool IsXInputControllerConnected(CONTROLLER Controller);//Xinput接続確認
int XNum(void);//Xinput接続数取得

LPDIRECTINPUT8 GetInput(void);//入力ポインタ取得

XINPUT_STATE GetXstate(CONTROLLER Controller);

#endif _INPUT_H_

