//
//
//爆発アニメーション[explosion.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //作成したmain.hをインクルードする
#include "explosion.h"   //作成したexplosion.hをインクルードする

//=======================================
//マクロ定義
//=======================================
#define MAX_EXPLOSION    (128)    //爆発の最大数
#define EXPLOSION_TEX    "data\\Texture\\explosion000.png"   //爆発アニメーションのテクスチャ

//=======================================
//爆発構造体の定義
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;      //位置
	D3DXCOLOR col;        //色
	int nCounterAnim;     //アニメーションカウンター
	int nPatternAnim;     //アニメーションパターンNo,
	D3DXMATRIX g_mtxWorldExplosion;	//ワールドマトリックス
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリクス取得用
	bool bUse;            //使用しているかどうか
}Explosion;

//=======================================
//グローバル変数宣言
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;   //頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];                //爆発の情報

//===========================================
//爆発の初期化処理
 //===========================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEX, &g_pTextureExplosion);

	//爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;  //使用してない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0～1.0fで設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//===========================================
//爆発の終了処理
//===========================================
void UninitExplosion(void)
{
	//テクスチャ破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//===========================================
//爆発の更新処理
//===========================================
void UpdateExplosion(void)
{
	int nCntExplosion;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//爆発が使用されている
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//頂点座標を更新
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, 0.0f);

			//カウンターを計算
			g_aExplosion[nCntExplosion].nCounterAnim++;

			if((g_aExplosion[nCntExplosion].nCounterAnim % 8) == 0)
			{
				//パターンNo.を更新する
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;
			}

			if (g_aExplosion[nCntExplosion].nPatternAnim >= 7)
			{
				//使用してない状態にする
				g_aExplosion[nCntExplosion].bUse = false;
			}

			//テクスチャ座標の再設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);
		}
		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//===========================================
//爆発の描画処理
//===========================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].g_mtxWorldExplosion);

		//ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &g_aExplosion[nCntExplosion].mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aExplosion[nCntExplosion].g_mtxWorldExplosion, NULL, &g_aExplosion[nCntExplosion].mtxView);	//逆行列を求める
		g_aExplosion[nCntExplosion].g_mtxWorldExplosion._41 = 0.0f;
		g_aExplosion[nCntExplosion].g_mtxWorldExplosion._42 = 0.0f;
		g_aExplosion[nCntExplosion].g_mtxWorldExplosion._43 = 0.0f;

		D3DXMatrixTranslation(&g_aExplosion[nCntExplosion].mtxTrans,
			g_aExplosion[nCntExplosion].pos.x,
			g_aExplosion[nCntExplosion].pos.y,
			g_aExplosion[nCntExplosion].pos.z);

		D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].g_mtxWorldExplosion,
			&g_aExplosion[nCntExplosion].g_mtxWorldExplosion,
			&g_aExplosion[nCntExplosion].mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].g_mtxWorldExplosion);

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		if (g_aExplosion[nCntExplosion].bUse == true)
		{//弾が使用される
			 //テクスチャ設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}

		//テクスチャ設定
		pDevice->SetTexture(0, NULL);
	}
}

//===========================================
//爆発の設定
//===========================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発アニメーションが使用されない
			g_aExplosion[nCntExplosion].pos = pos;

			//頂点情報へのポインタ
			VERTEX_3D *pVtx;

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntExplosion * 4);

			//頂点座標を更新
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, 0.0f);

			//使用している状態にする
			g_aExplosion[nCntExplosion].bUse = true;

			//カウンターを初期化する
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			//パターンNo,を初期化する
			g_aExplosion[nCntExplosion].nPatternAnim = 0;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0～1.0fで設定
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffExplosion->Unlock();

			break;
		}
	}
}