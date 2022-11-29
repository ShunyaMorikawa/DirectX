//
//
//エフェクト処理[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EFECT_H_     //このマクロ定義が定義されていなかったら
#define _EFECT_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//===========================================
//エフェクト構造体の定義
//===========================================
typedef struct
{
	D3DXVECTOR3 pos;      //位置
	D3DXVECTOR3 move;     //移動量
	D3DXVECTOR3 rot;      //向き
	D3DXCOLOR col;        //色
	float fRadius;        //半径(大きさ)
	int nLife;            //寿命(表示時間)
	D3DXMATRIX g_mtxWorldEffectt;	//ワールドマトリックス
	D3DXMATRIX mtxRot; 	//計算用マトリックス
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxView;		//ビューマトリクス取得用
	bool bUse;            //使用しているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife);

#endif