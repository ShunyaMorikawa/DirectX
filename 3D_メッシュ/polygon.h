//
//
//ポリゴン処理[polygon.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _POLYGON_H_     //このマクロ定義が定義されていなかったら
#define _POLYGON_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//プロトタイプ宣言
//=======================================
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

#endif