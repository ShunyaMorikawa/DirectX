//
//
//弾の処理[bullet.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //作成したmain.hをインクルードする
#include "bullet.h"      //作成したbullet.hをインクルードする
#include "effect.h"
#include "shadow.h"
#include "explosion.h"

//=======================================
//マクロ定義
//=======================================
#define MAX_BULLET      (1024)   //弾の最大数
#define BULLET_SIZE     (10)     //弾のサイズ
#define BULLET_TEX      "data\\Texture\\bullet.png"  //自機の弾のテクスチャ
#define BULLET_SPEED    (4)      //弾の速度
#define EFFECTLIFE      (100)    //エフェクトの寿命
#define BULLET_LIFE     (100)    //弾の寿命
#define NUM_BULLET      (2)      //敵の弾のテクスチャ

#define SCREEN_DEPTH	(500)

//=======================================
//グローバル変数宣言
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;   //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];                      //弾の情報

//===========================================
//弾の初期化処理
//===========================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 50;
		g_aBullet[nCntBullet].BulletShadow = -1;
		g_aBullet[nCntBullet].bUse = false;  //使用してない状態にする
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TEX, &g_pTextureBullet);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBullet,
								NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(- BULLET_SIZE, - BULLET_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, - BULLET_SIZE, 0.0f);

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

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//===========================================
//弾の終了処理
//===========================================
void UninitBullet(void)
{
	//テクスチャ破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===========================================
//弾の更新処理
//===========================================
void UpdateBullet(void)
{
	int nCntBullet;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.6f), BULLET_SIZE, EFFECTLIFE);

			//弾の位置を更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//影の位置を設定
			SetPositionShadow(g_aBullet[nCntBullet].BulletShadow, g_aBullet[nCntBullet].pos);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, -BULLET_SIZE, 0.0f);

			//寿命カウントダウン
			g_aBullet[nCntBullet].nLife--;

			//寿命が尽きた
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				//使用してない状態にする
				g_aBullet[nCntBullet].bUse = false;
			}
		}

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//===========================================
//弾の描画処理
//===========================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBullet[nCntBullet].g_mtxWorldBullet);

		//ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &g_aBullet[nCntBullet].mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aBullet[nCntBullet].g_mtxWorldBullet, NULL, &g_aBullet[nCntBullet].mtxView);	//逆行列を求める
		g_aBullet[nCntBullet].g_mtxWorldBullet._41 = 0.0f;
		g_aBullet[nCntBullet].g_mtxWorldBullet._42 = 0.0f;
		g_aBullet[nCntBullet].g_mtxWorldBullet._43 = 0.0f;

		D3DXMatrixTranslation(&g_aBullet[nCntBullet].mtxTrans, 
								g_aBullet[nCntBullet].pos.x, 
								g_aBullet[nCntBullet].pos.y, 
								g_aBullet[nCntBullet].pos.z);

		D3DXMatrixMultiply(&g_aBullet[nCntBullet].g_mtxWorldBullet, 
							&g_aBullet[nCntBullet].g_mtxWorldBullet, 
							&g_aBullet[nCntBullet].mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].g_mtxWorldBullet);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureBullet);

		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用される
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}

		//テクスチャ設定
		pDevice->SetTexture(0, NULL);
	}
}

//===========================================
//弾の設定
//===========================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されない
			g_aBullet[nCntBullet].pos = pos;

			g_aBullet[nCntBullet].type = type;

			//弾の速度
			g_aBullet[nCntBullet].move = move * BULLET_SPEED;

			//弾の寿命
			g_aBullet[nCntBullet].nLife = BULLET_LIFE;

			//使用している状態にする
			g_aBullet[nCntBullet].bUse = true;

			//影を設定
			g_aBullet[nCntBullet].BulletShadow = SetShadow();

			break;
		}
	}
}

//===========================================
//弾の取得
//===========================================
Bullet *GetBullet(void)
{
	//弾の情報の先頭アドレスを返す
	return &g_aBullet[0];
}