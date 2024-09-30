//------------------------------------------
//
//�V���[�e�B���O�Q�[���v���O�����̃��C���̏����̒�`�E�錾[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#include<windows.h>
#include<stdio.h>
#include<time.h>
#include <strsafe.h>
#include"d3dx9.h"
#include"dinput.h" // ���͂ɕK�v�ȃt�@�C��
#include"Xinput.h" // Xinput(�p�b�h����)
#include"xaudio2.h"

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")

// �}�N����`
#define FPS (1000/60) // �X�V�X�s�[�h
#define UP_TIME (3) // �e�N�X�`���X�s�[�h

// ���O
#define CLASS_NAME "WindowClass" // �N���X��
#define WINDOW_NAME "Synchronize" // �E�C���h�E��

// �傫��
#define SCREEN_WIDTH (1280) // ��
#define SCREEN_HEIGHT (720) // ����

// ���_
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define VT_MAX (4)//���_��
#define UV_DEF (1.0f) // �e�N�X�`���̃f�t�H���g�̃T�C�Y

//�f�o�b�O�p
#define FPS_TIME (500)
#define FONT_SIZE (20)

// ���_���̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos; // ���W
	float rhw; // ���W�ϊ��W��(1.0f)
	D3DCOLOR col; // �J���[
	D3DXVECTOR2 tex; // �e�N�X�`�����W
} VERTEX_2D;

// ��ʂ̎��
typedef enum
{
	MODE_TITLE = 0,//�^�C�g��
	MODE_DEMO,//�f��
	MODE_TUTO,//�`���[�g���A��
	MODE_PLAYSELECT,//�v���C�Z���N�g
	MODE_GAME,//�Q�[��
	MODE_VS,//�o�[�T�X
	MODE_RESULT,//���U���g
	MODE_RANK,//�����L���O
	MODE_OPTION,//�I�v�V����
	MODE_MAX//���[�h��
} MODE;

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�C���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // ����������
void Uninit(void); // �I������
void Update(void); // �X�V����
void Draw(void); // �`�揈��
LPDIRECT3DDEVICE9 GetDevice(void); // �f�o�C�X�̎擾
void SetMode(MODE mode);//���[�h�ݒ�
MODE GetMode(void);//���[�h�擾
HWND GethWnd(void);//�n���h���擾
HINSTANCE GethInstanse(void);//�C���X�^���X�擾

void ConfineCursorToWindow(HWND hwnd);//�J�[�\������
void ReleaseCursor(void);//�J�[�\���������

void ToggleFullScreen(HWND hWnd);//�t���X�N���[��
//void ResetDevice(void);//�f�o�C�X�č\�z
//
////�f�o�b�O�p
//void DrawDebug(int nData, float* fData, char* cData);

#endif // _MAIN_H_