//
//
//エフェクト処理[effect.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //作成したmain.hをインクルードする
#include "effect.h"      //作成したeffect.hをインクルードする

//===========================================
//マクロ定義
//===========================================
#define MAX_EFFECT    (4096)    //エフェクトの最大数
#define EFFECT_TEX    "data\\TEXTURE\\effect000.jpg"   //爆発アニメーションのテクスチャ

//===========================================
//グローバル変数宣言
//===========================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;   //頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];                      //エフェクトの情報

//===========================================
//エフェクトの初期化処理
//===========================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX, &g_pTextureEffect);

	//爆発の情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     //位置
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); //色
		g_aEffect[nCntEffect].fRadius = 10;  //半径(大きさ)
		g_aEffect[nCntEffect].nLife = 10;	 //寿命(表示時間)
		g_aEffect[nCntEffect].bUse = false;  //使用してない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//===========================================
//エフェクトの終了処理
//===========================================
void UninitEffect(void)
{
	//テクスチャ破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//===========================================
//エフェクトの更新処理
//===========================================
void UpdateEffect(void)
{
	int nCntEffect;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//爆発が使用されている
		if (g_aEffect[nCntEffect].bUse == true)
		{
			g_aEffect[nCntEffect].col.a -= 0.01f;

			g_aEffect[nCntEffect].fRadius -= 0.5f;

			g_aEffect[nCntEffect].nLife--;

			//頂点座標を更新
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);

			if (g_aEffect[nCntEffect].nLife < 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

		}
		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//===========================================
//エフェクトの描画処理
//===========================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntE = 0; nCntE < MAX_EFFECT; nCntE++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aEffect[nCntE].g_mtxWorldEffectt);

		//ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &g_aEffect[nCntE].mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aEffect[nCntE].g_mtxWorldEffectt, NULL, &g_aEffect[nCntE].mtxView);	//逆行列を求める
		g_aEffect[nCntE].g_mtxWorldEffectt._41 = 0.0f;
		g_aEffect[nCntE].g_mtxWorldEffectt._42 = 0.0f;
		g_aEffect[nCntE].g_mtxWorldEffectt._43 = 0.0f;

		D3DXMatrixTranslation(&g_aEffect[nCntE].mtxTrans,
			g_aEffect[nCntE].pos.x,
			g_aEffect[nCntE].pos.y,
			g_aEffect[nCntE].pos.z);

		D3DXMatrixMultiply(&g_aEffect[nCntE].g_mtxWorldEffectt,
			&g_aEffect[nCntE].g_mtxWorldEffectt,
			&g_aEffect[nCntE].mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntE].g_mtxWorldEffectt);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		if (g_aEffect[nCntE].bUse == true)
		{//エフェクトが使用される
		 //ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntE * 4, 2);
		}
		
		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	}

}

//===========================================
//エフェクトの設定
//===========================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;         //位置
			g_aEffect[nCntEffect].move = move;       //移動量
			g_aEffect[nCntEffect].col = col;         //色
			g_aEffect[nCntEffect].fRadius = fRadius; //半径(大きさ)
			g_aEffect[nCntEffect].nLife = nLife;     //寿命(表示時間)
			g_aEffect[nCntEffect].bUse = true;       //使用しているかどうか

			break;
		}
	}
}