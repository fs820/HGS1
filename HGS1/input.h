//---------------------------------------
//
//���͏����̒�`�E�錾[input.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_
#include"main.h"

#define STICK_DED (0.8f)//�f�b�h�]�[��
#define VIBRATION_MAX (65535)//�o�C�u���[�V�����l

//�}�E�X�{�^���̎��
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

//�R���g���[���[�̎��
typedef enum
{
	CONTROLLER_1 = 0,
	CONTROLLER_2,
	CONTROLLER_3,
	CONTROLLER_4,
	CONTROLLER_MAX
}CONTROLLER;

//X�p�b�h�{�^���̎��
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

//�p�b�h�X�e�B�b�N�̎��
typedef enum
{
	STICK_LEFT = 0,
	STICK_RIGHT,
	STICK_MAX
}STICK;

//�X�e�B�b�N�����̎��
typedef enum
{
	DIRESTICK_UP = 0,
	DIRESTICK_DOWN,
	DIRESTICK_LEFT,
	DIRESTICK_RIGHT,
	DIRESTICK_MAX
}DIRESTICK;

//�L�[�{�[�h
HRESULT InitKeyboard(HINSTANCE hInstanse, HWND hWnd);//����������
void UninitKeyborad(void);//�I������
void UpdateKeyborad(void);//�X�V����
bool GetKeyboradPress(int nkey);//�L�[��������
bool GetKeyboradTrigger(int nkey);//�L�[���͏���
bool GetKeyboradRelease(int nkey);//�L�[��������
bool GetKeyboradRepeat(int nkey);//�L�[����������

//�}�E�X
HRESULT InitMouse(HINSTANCE hInstanse, HWND hWnd);//����������
void UninitMouse(void);//�I������
void UpdateMouse(void);//�X�V����
bool GetMousePress(int nkey);//�L�[��������
bool GetMouseTrigger(int nkey);//�L�[���͏���
bool GetMouseRelease(int nkey);//�L�[��������
bool GetMouseRepeat(int nkey);//�L�[����������
float *GetMouseMove(void);//�}�E�X����
float GetMouseWheel(void);//�z�C�[������

//Xinput
HRESULT InitJoypad(void);//����������
void UninitJoypad(void);//�I������
void UpdateJoypad(void);//�X�V����
bool GetJoykeyPress(JOYKEY key, CONTROLLER Controller);//�L�[��������
bool GetJoykeyTrigger(JOYKEY key, CONTROLLER Controller);//�L�[���͏���
bool GetJoykeyRelease(JOYKEY key, CONTROLLER Controller);//�L�[��������
bool GetJoykeyRepeat(JOYKEY key, CONTROLLER Controller);//�L�[����������
float *GetJoyStick(STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����
bool JoyStickPress(DIRESTICK DireStick ,STICK Stick, CONTROLLER Controller);//�X�e�B�b�N��������
bool JoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N���͏���
bool JoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N��������
bool JoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����������
void VibrateController(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, CONTROLLER Controller);//�o�C�u���[�V��������

void SetVibrate(float fPower, CONTROLLER Controller);//�o�C�u���[�V�����ݒ�

//�ڑ��m�F
bool IsXInputControllerConnected(CONTROLLER Controller);//Xinput�ڑ��m�F
int XNum(void);//Xinput�ڑ����擾

LPDIRECTINPUT8 GetInput(void);//���̓|�C���^�擾

XINPUT_STATE GetXstate(CONTROLLER Controller);

#endif _INPUT_H_

