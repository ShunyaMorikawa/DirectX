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

//=======================================
//マクロ定義
//=======================================
#define OBJECT			"data\\MODEL\\swing.x"

//=======================================
//グローバル変数宣言
//=======================================
LPD3DXMESH g_pMeshObject = NULL;			//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatObject = NULL;		//マテリアルへのポインタ
DWORD g_dwNumMatObject = 0;					//マテリアルの数
D3DXMATRIX g_mtxWorldObject;				//ワールドマトリックス
LPDIRECT3DTEXTURE9 g_pTextureObject = NULL;	//テクスチャのポインタ
Object g_Object;						//モデルの情報

//=======================================
//オブジェクトの初期化処理
//=======================================
void InitObject(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXMATERIAL *pMat;


	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのポインタ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

						//デバイスの取得
	pDevice = GetDevice();

	//モデルの位置を初期化
	g_Object.pos = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);

	//モデルの向きを初期化
	g_Object.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Object.fDistance = 0.0f;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(OBJECT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject,
		NULL,
		&g_dwNumMatObject,
		&g_pMeshObject);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	//頂点数の取得
	nNumVtx = g_pMeshObject->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObject->GetFVF());

	//頂点バッファをロック
	g_pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//すべての頂点を比較してモデル最小値・最大値を抜き出す

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	g_pMeshObject->UnlockVertexBuffer();
}

//=======================================
//オブジェクトの終了処理
//=======================================
void UninitObject(void)
{
	//メッシュの破棄
	if (g_pMeshObject != NULL)
	{
		g_pMeshObject->Release();
		g_pMeshObject = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatObject != NULL)
	{
		g_pBuffMatObject->Release();
		g_pBuffMatObject = NULL;
	}
}

//=======================================
//オブジェクトの更新処理
//=======================================
void UpdateObject(void)
{

}

//=======================================
//オブジェクトの描画処理
//=======================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldObject);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object.rot.y, g_Object.rot.x, g_Object.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldObject, &g_mtxWorldObject, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Object.pos.x, g_Object.pos.y, g_Object.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldObject, &g_mtxWorldObject, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObject);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject; nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureObject); //テクスチャを使用してないときはNULL
												 //モデル(パーツ)の描画
		g_pMeshObject->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================
//オブジェクトの取得
//=======================================
Object *GetObj(void)
{
	return &g_Object;
}
