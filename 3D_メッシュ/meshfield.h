//
//
//メッシュフィールド処理[meshfield.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _MESHFIELD_H_     //このマクロ定義が定義されていなかったら
#define _MESHFIELD_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//プロトタイプ宣言
//=======================================
void InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);

#endif