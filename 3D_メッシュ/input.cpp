//
//
//���͏���[input.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "input.h"

//=======================================
//�}�N����`
//=======================================
#define NUM_KEY_MAX  (256)   //�L�[�̍ő吔
#define MAX_PAD      (4)     //�p�b�h�̍ő吔
#define NUM_PAD_MAX  (13)    //�p�b�h�̃{�^���̍ő吔

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPDIRECTINPUT8 g_pInput = NULL;               //DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;   //���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

BYTE g_aKeyState[NUM_KEY_MAX];                //�L�[�{�[�h�̃v���X�@�\
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];         //�L�[�{�[�h�̃g���K�[���

XINPUT_STATE g_aPadState[MAX_PAD];            //�p�b�h�̃v���X�@�\
XINPUT_STATE g_aPadStateTrigger[MAX_PAD];     //�p�b�h�̃g���K�[���

//=====================================
//�L�[�{�[�h�̏���������
//=====================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//=====================================
//�p�b�h�̏���������
//=====================================
HRESULT InitPad(void)
{
	int nCntPad;
	//�L����
	XInputEnable(true);

	for (nCntPad = 0; nCntPad < MAX_PAD; nCntPad++)
	{//������������
		memset(&g_aPadState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&g_aPadStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//=====================================
//�L�[�{�[�h�̏I������
//=====================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//=====================================
//�p�b�h�̏I������
//=====================================
void UninitPad(void)
{
	//������
	XInputEnable(false);
}

//=====================================
//�L�[�{�[�h�̍X�V����
//=====================================
void UpdateKeyboard(void)
{
	//�L�[�{�[�h�̓��͏��
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//�L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//�L�[�{�[�h�̃v���X����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}

	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//=====================================
//�p�b�h�̍X�V����
//=====================================
void UpdatePad(void)
{
	//�p�b�h�̓��͏��
	XINPUT_STATE aPadState[MAX_PAD];

	int nCntPad;

	//���̓f�o�C�X����f�[�^���擾
	for (nCntPad = 0; nCntPad < MAX_PAD; nCntPad++)
	{
		if (XInputGetState(nCntPad, &aPadState[nCntPad]) == ERROR_SUCCESS)
		{
			//�p�b�h�̃g���K�[����ۑ�
			g_aPadStateTrigger[nCntPad].Gamepad.wButtons =
				(g_aPadState[nCntPad].Gamepad.wButtons
					^ aPadState[nCntPad].Gamepad.wButtons)
				& g_aPadState[nCntPad].Gamepad.wButtons;

			//�p�b�h�̃v���X����ۑ�
			g_aPadState[nCntPad] = aPadState[nCntPad];
		}
	}
}

//=====================================
//�L�[�{�[�h�̃v���X�����擾
//=====================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================
//�R���g���[���[�̃v���X�����擾
//=====================================
bool GetPadPress(JOYKEY Key, int nPlayer)
{
	return (g_aPadState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//================================================
//�R���g���[���[��L�X�e�B�b�N��X���v���X�����擾
//================================================
SHORT GetPadLStickXPress(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadState[nPlayer].Gamepad.sThumbLX;
}

//================================================
//�R���g���[���[��L�X�e�B�b�N��Y���v���X�����擾
//================================================
SHORT GetPadLStickYPress(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadState[nPlayer].Gamepad.sThumbLY;
}

//=====================================
//�L�[�{�[�h�̃g���K�[�����擾
//=====================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===========================================
//�R���g���[���[�̃g���K�[�����擾
//===========================================
bool GetPadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_aPadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//=====================================================
//�R���g���[���[��L�X�e�B�b�N��X���̃g���K�[�����擾
//=====================================================
SHORT GetPadLStickXTrigger(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadStateTrigger[nPlayer].Gamepad.sThumbLX;
}

//=====================================================
//�R���g���[���[��L�X�e�B�b�N��Y���̃g���K�[�����擾
//=====================================================
SHORT GetPadLStickYTrigger(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadStateTrigger[nPlayer].Gamepad.sThumbLY;
}