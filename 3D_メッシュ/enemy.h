//
//
//モデル処理[model.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _ENEMY_H_     //このマクロ定義が定義されていなかったら
#define _ENEMY_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//敵構造体の定義
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;   //移動量
	D3DXVECTOR3 rot;	//向き
	float fDistance;	//距離
	D3DXVECTOR3 RotDest;//目的の向き
}Enemy;

//=======================================
//プロトタイプ宣言
//=======================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);

#endif