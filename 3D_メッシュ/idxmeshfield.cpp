//
//
//ポリゴン処理[polygon.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "idxmeshfield.h"

//=======================================
//マクロ定義
//=======================================
#define IdxMESHFIELD_TEX	"data\\TEXTURE\\block001.jpg"
#define VTX (3)

//=======================================
//グローバル変数宣言
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureIdxMeshfield = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxIdxBuffMeshfield = NULL;   //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		 //インデックスバッファへのポインタ
D3DXVECTOR3 g_posIdxMeshfield;
D3DXVECTOR3 g_rotIdxMeshfield;
D3DXMATRIX g_mtxWorldIdxMeshfield;	//ワールドマトリックス

//=======================================
//インデックスメッシュフィールドの初期化処理
//=======================================
void InitIdxMeshfield(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_posIdxMeshfield = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);

	g_rotIdxMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, IdxMESHFIELD_TEX, &g_pTextureIdxMeshfield);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxIdxBuffMeshfield,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxIdxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < VTX; nCntZ++)
	{
		for (int nCntX = 0; nCntX < VTX; nCntX++)
		{
			//頂点座標の設定
			pVtx[nCntX + nCntZ * 3].pos = D3DXVECTOR3(nCntX * 1000.0f, 0.0f, nCntZ * -1000.0f);
		
			//法線の設定
			pVtx[nCntX + nCntZ * 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntX + nCntZ * 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[nCntX + nCntZ * 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
	}

	//頂点バッファをアンロック
	g_pVtxIdxBuffMeshfield->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshField,
								NULL);

	//インデックス情報へのポインタ
	WORD * pIdx;

	//インデックスバッファをロックし、頂点情報へのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//	//頂点番号データの設定
	//	for (int nCntIdx = 0; nCntIdx < 3; nCntIdx++)
	//	{
	//		pIdx[nCntIdx * 2] = 3 + nCntIdx * 1;
	//		pIdx[(nCntIdx * 2) + 1] = 0 + nCntIdx * 1;
	//	}

	//	//pIdx[0] = 3;
	//	//pIdx[1] = 0;

	//	//pIdx[2] = 4;
	//	//pIdx[3] = 1;

	//	//pIdx[4] = 5;
	//	//pIdx[5] = 2;

	//	for (int nCntIdx2 = 0; nCntIdx2 < 2; nCntIdx2++)
	//	{
	//		pIdx[8] = 6;
	//		pIdx[9] = 3;

	//		pIdx[10] = 7;
	//		pIdx[11] = 4;

	//		pIdx[12] = 8;
	//		pIdx[13] = 5;

	//	}

	////縮退
	//pIdx[6] = 2;
	//pIdx[7] = 6;

	//頂点番号データの設定
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//インデックスバッファをアンロック
	g_pIdxBuffMeshField->Unlock();
}
 
//=======================================
//インデックスメッシュフィールドの終了処理
//=======================================
void UninitIdxMeshfield(void)
{
	//テクスチャ破棄
	if (g_pTextureIdxMeshfield != NULL)
	{
		g_pTextureIdxMeshfield->Release();
		g_pTextureIdxMeshfield = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxIdxBuffMeshfield != NULL)
	{
		g_pVtxIdxBuffMeshfield->Release();
		g_pVtxIdxBuffMeshfield = NULL;
	}

	//頂点バッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=======================================
//インデックスメッシュフィールドの更新処理
//=======================================
void UpdateIdxMeshfield(void)
{

}

//=======================================
//インデックスメッシュフィールドの描画処理
//=======================================
void DrawIdxMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldIdxMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotIdxMeshfield.y, g_rotIdxMeshfield.x, g_rotIdxMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldIdxMeshfield, &g_mtxWorldIdxMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posIdxMeshfield.x, g_posIdxMeshfield.y, g_posIdxMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldIdxMeshfield, &g_mtxWorldIdxMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldIdxMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxIdxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureIdxMeshfield); //テクスチャを使用してないときはNULL

	//ポリゴンの描画								 
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,//用意した頂点の数
		0,
		12);//ポリゴンの数

	//テクスチャの設定
	pDevice->SetTexture(0, NULL); //テクスチャを使用してないときはNULL
}