//
//
//ポリゴン処理[polygon.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "wall.h"
//=======================================
//マクロ定義
//=======================================
#define POLYGON_TEX	"data\\TEXTURE\\grass000.png"

#define WALL_POSY		(200.0f)

#define MAX_WALL		(4)		//壁の最大数

//=======================================
//グローバル変数宣言
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;   //頂点バッファへのポインタ
Wall g_Wall[MAX_WALL];
int g_nCntWall;		//壁の数
int g_NumWall;		//壁の総数

//=======================================
//壁の初期化処理
//=======================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Wall[nCntWall].nType = 0;

		g_Wall[nCntWall].bUse = false;
	}

	//壁の数の初期化
	g_nCntWall = 0;

	//壁の総数の初期化
	g_NumWall = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEX, &g_pTextureWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntW = 0; nCntW < MAX_WALL; nCntW++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-1000.0f, 500.0f, 1000.0f);
		pVtx[1].pos = D3DXVECTOR3(1000.0f, 500.0f, 1000.0f);
		pVtx[2].pos = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);
		pVtx[3].pos = D3DXVECTOR3(1000.0f, 0.0f, 1000.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}

//=======================================
//壁の終了処理
//=======================================
void UninitWall(void)
{
	//テクスチャ破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=======================================
//壁の更新処理
//=======================================
void UpdateWall(void)
{

}

//=======================================
//壁の描画処理
//=======================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorldWall);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorldWall, &g_Wall[nCntWall].mtxWorldWall, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorldWall, &g_Wall[nCntWall].mtxWorldWall, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorldWall);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		g_nCntWall = 0;

		if (g_Wall[nCntWall].bUse == true)
		{
			g_nCntWall++;

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall); //テクスチャを使用してないときはNULL

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
		//テクスチャの設定
		pDevice->SetTexture(0, NULL); //テクスチャを使用してないときはNULL
	}
}

//=======================================
//壁の設定
//=======================================
void SetWall(D3DXVECTOR3 pos, float angle)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bUse == false)
		{//敵が使用されない
			g_Wall[nCntWall].pos = pos;

			g_Wall[nCntWall].rot.y = angle;

			//頂点座標を更新
			pVtx[0].pos = D3DXVECTOR3(-1000.0f, 500.0f, 1000.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 500.0f, 1000.0f);
			pVtx[2].pos = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 0.0f, 1000.0f);

			//使用している状態にする
			g_Wall[nCntWall].bUse = true;

			//壁の総数を加算する
			g_NumWall++;

			break;
		}

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}