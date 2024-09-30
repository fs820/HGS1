//----------------------------------------
//
//�v���C���[�\������[player.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player.h"
#include"input.h"
#include"back.h"
#include"exef.h"
#include"score.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//�o�b�t�@�̃|�C���^
Player g_Player;//�v���C���[���
D3DXVECTOR3 g_moveBullet;//�e���ˌ����E�X�s�[�h�Ǘ�

//----------------------
//�v���C���[�̏���������
//----------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

	g_Player.nCounterAnim = 0;
	g_Player.nPatternAnim = 0;
	g_Player.pos = D3DXVECTOR3(PLAYER_WIDTH, SCREEN_HEIGHT / 3, 0);//�����ʒu
	g_Player.Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
	g_Player.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//�v���C���[�X�s�[�h
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
	g_Player.rotDef = g_Player.rot;//�����ۑ�
	g_Player.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//�Ίp���̒���
	g_Player.fLengthDef = g_Player.fLength;//�����ۑ�
	g_Player.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//�p�x
	g_Player.BulletCount = 0;
	g_Player.nRema = 3;
	g_Player.nLife = 100;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.nCounterState = 0;
	g_Player.bDisp = true;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		0,
		&g_pTexturePlayer
	);

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);

	g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//-------------------
//�v���C���[�I������
//-------------------
void UninitPlayer(void)
{
	SetVibrate(0.0f, CONTROLLER_1);
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------
//�v���C���[�X�V����
//-------------------
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	float* pStick;
	float* pMouseMove;
	float fMouseWheel;

	//�ߋ��̈ʒu�̕ۑ�
	g_Player.Oldpos = g_Player.pos;

	//�ړ�����

	//X
	if(IsXInputControllerConnected(CONTROLLER_1))
	{
		//���X�e�B�b�N����
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_1);
		g_Player.move.x += *pStick * PLAYER_SPEED_X;
		g_Player.move.y += -*(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player.rot.z = atan2f(*(pStick + 1), *pStick);
		}
	}

	//�}�E�X����
	pMouseMove = GetMouseMove();
	g_Player.move.x += *pMouseMove * PLAYER_SPEED_X;
	g_Player.move.y += *(pMouseMove + 1) * PLAYER_SPEED_Y;

	fMouseWheel = GetMouseWheel();
	g_Player.rot.z += fMouseWheel;

	if (GetKeyboradPress(DIK_A) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//A�L�[
	{
		if (GetKeyboradPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//����
			g_Player.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//����
			g_Player.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//��
			g_Player.move.x += -PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_D) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//D�L�[
	{
		if (GetKeyboradPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//�E��
			g_Player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//�E��
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//�E
			g_Player.move.x += PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
	{//��
		g_Player.move.y += -PLAYER_SPEED_Y;
	}
	else if (GetKeyboradPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
	{//��
		g_Player.move.y += PLAYER_SPEED_Y;
	}

	if (GetKeyboradPress(DIK_Q) == true || GetJoykeyPress(JOYKEY_B, CONTROLLER_1) == true)
	{
		g_Player.rot.z += PLAYER_ROT_SPEED;
	}

	if (GetKeyboradPress(DIK_E) == true || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1) == true)
	{
		g_Player.rot.z += -PLAYER_ROT_SPEED;
	}

	if (GetKeyboradTrigger(DIK_R) == true || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) == true)
	{
		g_Player.rot = g_Player.rotDef;
	}

	if (GetKeyboradPress(DIK_Z) == true ||  GetMousePress(MOUSE_SENTER) == true || GetJoykeyPress(JOYKEY_A, CONTROLLER_1) == true)
	{
		if (g_Player.fLength >= 0.0f && g_Player.fLength <= PLAYER_SIZE_MAX)
		{
			g_Player.fLength += PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradPress(DIK_C) == true|| GetMousePress(MOUSE_RIGHT) == true || GetJoykeyPress(JOYKEY_X, CONTROLLER_1) == true)
	{
		if (g_Player.fLength >= PLAYER_SIZE_MIN)
		{
			g_Player.fLength += -PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradTrigger(DIK_V) == true || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1) == true)
	{
		g_Player.fLength = g_Player.fLengthDef;
	}

	if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
	{
		if (GetKeyboradTrigger(DIK_SPACE) == true || GetMouseTrigger(MOUSE_LEFT) == true || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1) == true)
		{

		}
		else if (GetKeyboradRepeat(DIK_SPACE) == true || GetMouseRepeat(MOUSE_LEFT) == true || GetJoykeyRepeat(JOYKEY_RB, CONTROLLER_1) == true)
		{

		}
	}

	//�ʒu�̍X�V
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	//�ړ��ʂ̍X�V(����)
	g_Player.move.x += (PLAYER_SPEED_DEF - g_Player.move.x) * PLAYER_INA;
	g_Player.move.y += (PLAYER_SPEED_DEF - g_Player.move.y) * PLAYER_INA;

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	g_Player.nCounterAnim++;
	if ((g_Player.nCounterAnim % UP_TIME) == 0)//�X�V����
	{
		g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % (U_MAX * V_MAX);

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX) + UV_DEF / V_MAX);

		g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}

	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		SetVibrate(0.0f, CONTROLLER_1);

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		break;
	case PLAYERSTATE_APPEAR:
		g_Player.nCounterState++;
		if (g_Player.nCounterState >= 60)
		{
			g_Player.state = PLAYERSTATE_NORMAL;
			g_Player.bDisp = true;
		}
		else if (g_Player.nCounterState % 5 == 0)
		{
			g_Player.bDisp = !g_Player.bDisp;
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player.nCounterState--;
		if (g_Player.nCounterState <= 0)
		{
			g_Player.state = PLAYERSTATE_NORMAL;

			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		break;
	case PLAYERSTATE_DIE:
		g_Player.bDisp = false;
		break;
	}
}

//-------------------
//�v���C���[�`�揈��
//-------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player.bDisp)
	{
		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			0,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}

//-------------------
//�q�b�g����
//-------------------
void HitPlayer(int nDamage)
{
	if (g_Player.state!=PLAYERSTATE_DIE&& g_Player.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player.nLife -= nDamage;

		if (g_Player.nLife <= 0)
		{
			if (g_Player.nRema <= 0)
			{
				SetExef(g_Player.pos, g_Player.fLength, true);
				g_Player.state = PLAYERSTATE_DIE;
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
			}
			else
			{
				SetExef(g_Player.pos, g_Player.fLength, true);
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player.state = PLAYERSTATE_APPEAR;
				g_Player.nCounterState = 0;
				g_Player.nRema--;
				g_Player.nLife = 100;
			}
			SetVibrate(1.0f, CONTROLLER_1);
		}
		else
		{
			g_Player.state = PLAYERSTATE_DAMAGE;
			g_Player.nCounterState = 5;
			if (GetMode()==MODE_GAME)
			{
				AddScore(-nDamage * SCORE_MISS);
			}

			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffPlayer->Unlock();//�o�b�t�@�̃A�����b�N
		}
	}
}

//-------------------------
//�v���C���[�o�b�t�@�̎擾
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void)
{
	return g_pVtxBuffPlayer;
}

//-------------------------
//�v���C���[�擾
//-------------------------
Player* GetPlayer(void)
{
	return &g_Player;
}