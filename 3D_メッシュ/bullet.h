//
//
//弾の発射[bullet.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _BULLET_H_     //このマクロ定義が定義されていなかったら
#define _BULLET_H_    //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//弾の種類
//=======================================
typedef enum
{
	BULLETTYPE_PLAYER = 0, //プレイヤーの弾
	BULLETTYPE_ENEMY,      //敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;

//=======================================
//弾構造体の定義
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;     //位置
	D3DXVECTOR3 move;    //移動量
	int nLife;           //寿命
	BULLETTYPE type;     //種類
	D3DXMATRIX g_mtxWorldBullet;	//ワールドマトリックス
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリクス取得用
	int BulletShadow;			//対象に影のインデックス(番号)
	bool bUse;           //使用しているかどうか
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type);
Bullet *GetBullet(void);

#endif