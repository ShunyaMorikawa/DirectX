//
//
//爆発処理[explosion.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EXPLOSION_H_     //このマクロ定義が定義されていなかったら
#define _EXPLOSION_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif