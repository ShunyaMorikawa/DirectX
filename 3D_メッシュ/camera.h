//
//
//カメラ処理[camera.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _CAMERA_H_     //このマクロ定義が定義されていなかったら
#define _CAMERA_H_     //2連インクルード防止のマクロを定義する

#include "main.h"

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;	//視点
	D3DXVECTOR3 posR;	//注視点
	D3DXVECTOR3 vecU;	//上方向ベクトル
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 VDest;		//視点
	D3DXVECTOR3 RDest;		//注視点
	float fDistance;	//距離
}Camera;

//=======================================
//プロトタイプ宣言
//=======================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);

#endif