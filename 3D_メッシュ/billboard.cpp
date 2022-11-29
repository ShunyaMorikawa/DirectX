//
//
//ポリゴン処理[polygon.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "billboard.h"    //作成したfade.hをインクルードする

//=======================================
//マクロ定義
//=======================================
#define POLYGON_TEX	"data\\TEXTURE\\face.png"

#define BILL_WIDTH		(20)	//ビルボードの幅
#define BILL_HEIGHT		(20)	//ビルボードの高さ

//=======================================
//グローバル変数宣言
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureBillBoard = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillBoard = NULL;   //頂点バッファへのポインタ
D3DXVECTOR3 g_posBillBoard;		//位置
D3DXMATRIX g_mtxWorldBillBoard;	//ワールドマトリックス

//=======================================
//ビルボードの初期化処理
//=======================================
void InitBillBoard(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//ビルボードの位置を初期化
	g_posBillBoard = D3DXVECTOR3(50.0f, 20.0f, 50.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEX, &g_pTextureBillBoard);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillBoard,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillBoard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(- BILL_WIDTH, BILL_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILL_WIDTH, BILL_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- BILL_WIDTH, - BILL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILL_WIDTH, - BILL_HEIGHT, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBillBoard->Unlock();
}

//=======================================
//ビルボードの終了処理
//=======================================
void UninitBillBoard(void)
{
	//テクスチャ破棄
	if (g_pTextureBillBoard != NULL)
	{
		g_pTextureBillBoard->Release();
		g_pTextureBillBoard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillBoard != NULL)
	{
		g_pVtxBuffBillBoard->Release();
		g_pVtxBuffBillBoard = NULL;
	}
}

//=======================================
//ビルボードの更新処理
//=======================================
void UpdateBillBoard(void)
{

}

//=======================================
//ビルボードの描画処理
//=======================================
void DrawBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリクス取得用

	//デバイスの取得
	pDevice = GetDevice();

	////アルファテストを有効にする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	////Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillBoard);

	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldBillBoard, NULL, &mtxView);	//逆行列を求める
	g_mtxWorldBillBoard._41 = 0.0f;
	g_mtxWorldBillBoard._42 = 0.0f;
	g_mtxWorldBillBoard._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBillBoard.x, g_posBillBoard.y, g_posBillBoard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillBoard, &g_mtxWorldBillBoard, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillBoard);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillBoard, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillBoard); //テクスチャを使用してないときはNULL

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL); //テクスチャを使用してないときはNULL

	////アルファテストを無効にする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}