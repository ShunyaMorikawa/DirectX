//
//
//モデル処理[model.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "obj.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "enemy.h"

//=======================================
//マクロ定義
//=======================================
#define ENEMY			"data\\MODEL\\Excalibur.x"

//=======================================
//グローバル変数宣言
//=======================================
LPD3DXMESH g_pMeshEnemy = NULL;			//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy = NULL;		//マテリアルへのポインタ
DWORD g_dwNumMatEenmy = 0;					//マテリアルの数
D3DXMATRIX g_mtxWorldEnemy;				//ワールドマトリックス
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;	//テクスチャのポインタ
Enemy g_Enemy;						//モデルの情報

//=======================================
//エネミーの初期化処理
//=======================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXMATERIAL *pMat;


	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのポインタ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

						//デバイスの取得
	pDevice = GetDevice();

	//モデルの位置を初期化
	g_Enemy.pos = D3DXVECTOR3(150.0f, 0.0f, 0.0f);

	//モデルの向きを初期化
	g_Enemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Enemy.fDistance = 0.0f;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy,
		NULL,
		&g_dwNumMatEenmy,
		&g_pMeshEnemy);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

	//頂点数の取得
	nNumVtx = g_pMeshEnemy->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy->GetFVF());

	//頂点バッファをロック
	g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//すべての頂点を比較してモデル最小値・最大値を抜き出す

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	g_pMeshEnemy->UnlockVertexBuffer();
}

//=======================================
//エネミーの終了処理
//=======================================
void UninitEnemy(void)
{
	//メッシュの破棄
	if (g_pMeshEnemy != NULL)
	{
		g_pMeshEnemy->Release();
		g_pMeshEnemy = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatEnemy != NULL)
	{
		g_pBuffMatEnemy->Release();
		g_pBuffMatEnemy = NULL;
	}
}

//=======================================
//エネミーの更新処理
//=======================================
void UpdateEnemy(void)
{

}

//=======================================
//エネミーの描画処理
//=======================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldEnemy);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy.rot.y, g_Enemy.rot.x, g_Enemy.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Enemy.pos.x, g_Enemy.pos.y, g_Enemy.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatEenmy; nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEnemy); //テクスチャを使用してないときはNULL
												  //モデル(パーツ)の描画
		g_pMeshEnemy->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================
//エネミーの取得
//=======================================
Enemy *GetEnemy(void)
{
	return &g_Enemy;
}
