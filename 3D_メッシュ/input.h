//
//
//入力処理[input.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"
#pragma comment (lib,"xinput.lib")

//=======================================
//コントローラーの状態
//=======================================
typedef enum
{
	BUTTON_UP,		//十字↑
	BUTTON_DOWN,	//十字↓
	BUTTON_LEFT,	//十字←
	BUTTON_RIGHT,	//十字→
	BUTTON_START,	//START
	BUTTON_BACK,	//BACK
	BUTTON_PUSHING_L,     //左スティック押し込み
	BUTTON_PUSHING_R,     //右スティック押し込み
	BUTTON_LB,		//LB
	BUTTON_RB,		//RB
	BUTTON_10,		//不明(トリガー)
	BUTTON_11,		//不明(トリガー)
	BUTTON_A,		//Aボタン
	BUTTON_B,		//Bボタン
	BUTTON_X,		//Xボタン
	BUTTON_Y,		//Yボタン
	BUTTON_XSTICK,	//Xスティック
	BUTTON_17,		//不明
	BUTTON_18,		//不明
	BUTTON_19,		//不明
	BUTTON_20,
	BUTTONSTATE_MAX
}JOYKEY;

//=======================================
//プロトタイプ宣言
//=======================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
//↑ここまでキーボード

//=======================================
//↓ここからコントローラー
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
