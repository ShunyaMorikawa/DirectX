//
//
//���͏���[input.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"
#pragma comment (lib,"xinput.lib")

//=======================================
//�R���g���[���[�̏��
//=======================================
typedef enum
{
	BUTTON_UP,		//�\����
	BUTTON_DOWN,	//�\����
	BUTTON_LEFT,	//�\����
	BUTTON_RIGHT,	//�\����
	BUTTON_START,	//START
	BUTTON_BACK,	//BACK
	BUTTON_PUSHING_L,     //���X�e�B�b�N��������
	BUTTON_PUSHING_R,     //�E�X�e�B�b�N��������
	BUTTON_LB,		//LB
	BUTTON_RB,		//RB
	BUTTON_10,		//�s��(�g���K�[)
	BUTTON_11,		//�s��(�g���K�[)
	BUTTON_A,		//A�{�^��
	BUTTON_B,		//B�{�^��
	BUTTON_X,		//X�{�^��
	BUTTON_Y,		//Y�{�^��
	BUTTON_XSTICK,	//X�X�e�B�b�N
	BUTTON_17,		//�s��
	BUTTON_18,		//�s��
	BUTTON_19,		//�s��
	BUTTON_20,
	BUTTONSTATE_MAX
}JOYKEY;

//=======================================
//�v���g�^�C�v�錾
//=======================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
//�������܂ŃL�[�{�[�h

//=======================================
//����������R���g���[���[
//=======================================
HRESULT InitPad(void);
void UninitPad(void);
void UpdatePad(void);
bool GetPadPress(JOYKEY Key, int nPlayer);
bool GetPadTrigger(JOYKEY Key, int nPlayer);
SHORT GetPadLStickXPress(JOYKEY Key, int nPlayer);
SHORT GetPadLStickYPress(JOYKEY Key, int nPlayer);
SHORT GetPadLStickXTrigger(JOYKEY Key, int nPlayer);
SHORT GetPadLStickYTrigger(JOYKEY Key, int nPlayer);

#endif
