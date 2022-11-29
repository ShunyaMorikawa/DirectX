//
//
//ビルボード処理[billboard.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _BILLBOARD_H_     //このマクロ定義が定義されていなかったら
#define _BILLBOARD_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//プロトタイプ宣言
//=======================================
void InitBillBoard(void);
void UninitBillBoard(void);
void UpdateBillBoard(void);
void DrawBillBoard(void);

#endif