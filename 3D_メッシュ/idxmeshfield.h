//
//
//メッシュフィールド処理[meshfield.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _IDXMESHFIELD_H_     //このマクロ定義が定義されていなかったら
#define _IDXMESHFIELD_H_	 //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//プロトタイプ宣言
//=======================================
void InitIdxMeshfield(void);
void UninitIdxMeshfield(void);
void UpdateIdxMeshfield(void);
void DrawIdxMeshfield(void);

#endif