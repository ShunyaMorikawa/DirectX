//
//
//ライト処理[light.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _LIGHT_H_     //このマクロ定義が定義されていなかったら
#define _LIGHT_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//プロトタイプ宣言
//=======================================
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);

#endif