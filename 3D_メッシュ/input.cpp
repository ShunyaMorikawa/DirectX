//
//
//入力処理[input.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "input.h"

//=======================================
//マクロ定義
//=======================================
#define NUM_KEY_MAX  (256)   //キーの最大数
#define MAX_PAD      (4)     //パッドの最大数
#define NUM_PAD_MAX  (13)    //パッドのボタンの最大数

//=======================================
//グローバル変数宣言
//=======================================
LPDIRECTINPUT8 g_pInput = NULL;               //DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;   //入力デバイス(キーボード)へのポインタ

BYTE g_aKeyState[NUM_KEY_MAX];                //キーボードのプレス機能
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];         //キーボードのトリガー情報

XINPUT_STATE g_aPadState[MAX_PAD];            //パッドのプレス機能
XINPUT_STATE g_aPadStateTrigger[MAX_PAD];     //パッドのトリガー情報

//=====================================
//キーボードの初期化処理
//=====================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//キーボードのアクセス権を獲得
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//=====================================
//パッドの初期化処理
//=====================================
HRESULT InitPad(void)
{
	int nCntPad;
	//有効化
	XInputEnable(true);

	for (nCntPad = 0; nCntPad < MAX_PAD; nCntPad++)
	{//メモリ初期化
		memset(&g_aPadState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&g_aPadStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//=====================================
//キーボードの終了処理
//=====================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//=====================================
//パッドの終了処理
//=====================================
void UninitPad(void)
{
	//無効化
	XInputEnable(false);
}

//=====================================
//キーボードの更新処理
//=====================================
void UpdateKeyboard(void)
{
	//キーボードの入力情報
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//キーボードのトリガー情報を保存
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//キーボードのプレス情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}

	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//=====================================
//パッドの更新処理
//=====================================
void UpdatePad(void)
{
	//パッドの入力情報
	XINPUT_STATE aPadState[MAX_PAD];

	int nCntPad;

	//入力デバイスからデータを取得
	for (nCntPad = 0; nCntPad < MAX_PAD; nCntPad++)
	{
		if (XInputGetState(nCntPad, &aPadState[nCntPad]) == ERROR_SUCCESS)
		{
			//パッドのトリガー情報を保存
			g_aPadStateTrigger[nCntPad].Gamepad.wButtons =
				(g_aPadState[nCntPad].Gamepad.wButtons
					^ aPadState[nCntPad].Gamepad.wButtons)
				& g_aPadState[nCntPad].Gamepad.wButtons;

			//パッドのプレス情報を保存
			g_aPadState[nCntPad] = aPadState[nCntPad];
		}
	}
}

//=====================================
//キーボードのプレス情報を取得
//=====================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================
//コントローラーのプレス情報を取得
//=====================================
bool GetPadPress(JOYKEY Key, int nPlayer)
{
	return (g_aPadState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//================================================
//コントローラーのLスティックのX軸プレス情報を取得
//================================================
SHORT GetPadLStickXPress(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadState[nPlayer].Gamepad.sThumbLX;
}

//================================================
//コントローラーのLスティックのY軸プレス情報を取得
//================================================
SHORT GetPadLStickYPress(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadState[nPlayer].Gamepad.sThumbLY;
}

//=====================================
//キーボードのトリガー情報を取得
//=====================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===========================================
//コントローラーのトリガー情報を取得
//===========================================
bool GetPadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_aPadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//=====================================================
//コントローラーのLスティックのX軸のトリガー情報を取得
//=====================================================
SHORT GetPadLStickXTrigger(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadStateTrigger[nPlayer].Gamepad.sThumbLX;
}

//=====================================================
//コントローラーのLスティックのY軸のトリガー情報を取得
//=====================================================
SHORT GetPadLStickYTrigger(JOYKEY Key, int nPlayer)
{
	return /*(float)*/ g_aPadStateTrigger[nPlayer].Gamepad.sThumbLY;
}