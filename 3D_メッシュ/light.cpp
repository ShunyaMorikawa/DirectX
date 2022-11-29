//
//
//ライト処理[light.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "light.h"    //作成したfade.hをインクルードする

//=======================================
//マクロ定義
//=======================================
#define MAX_LIGHT		(3)		//ライトの最大数

//=======================================
//グローバル変数宣言
//=======================================
D3DLIGHT9 g_alight[MAX_LIGHT];	//ライトの情報

//=======================================
//ライトの初期化処理
//=======================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];	//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
		g_alight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

		//ライトの方向を設定
		vecDir[0] = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
		vecDir[1] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
		vecDir[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

		//ベクトルを正規化する
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);

		g_alight[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=======================================
//ライトの終了処理
//=======================================
void UninitLight(void)
{

}

//=======================================
//ライトの更新処理
//=======================================
void UpdateLight(void)
{

}