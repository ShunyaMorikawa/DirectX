//
//
//影の処理[shadow.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _SHADOW_H_     //このマクロ定義が定義されていなかったら
#define _SHADOW_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//影構造体の定義
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;		//影の位置
	D3DXVECTOR3 rot;		//影の向き
	D3DXMATRIX g_mtxWorldShadow;	//ワールドマトリックス
	bool bUse;           //使用しているかどうか
}Shadow;

//=======================================
//プロトタイプ宣言
//=======================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos);

#endif