//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM = 0,
	SOUND_LABEL_BGM2,
	SOUND_LABEL_BGM3,
	SOUND_LABEL_BGM4,
	SOUND_LABEL_BGM5,
	SOUND_LABEL_BGM6,
	SOUND_LABEL_TITLE,
	SOUND_LABEL_EXPLOSION,
	SOUND_LABEL_RANKIN,
	SOUND_LABEL_NORANKIN,
	SOUND_LABEL_BEST,
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
