//
//
//壁の処理[wall.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef __WALL_H_     //このマクロ定義が定義されていなかったら
#define __WALL_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//壁構造体の定義
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;     //位置
	D3DXVECTOR3 rot;	 //向き
	int nType;
	D3DXMATRIX mtxWorldWall;	//ワールドマトリックス
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリクス取得用
	bool bUse;
}Wall;

//=======================================
//プロトタイプ宣言
//=======================================
void InitWall(void);
void UninitWall(void);
void UpdateWall (void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, float angle);

#endif