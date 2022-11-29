//
//
//メッシュフィールド処理[meshfield.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _MESHWALL_H_     //このマクロ定義が定義されていなかったら
#define _MESHWALL_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//プロトタイプ宣言
//=======================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif