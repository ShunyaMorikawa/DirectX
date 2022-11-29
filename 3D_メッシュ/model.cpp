//
//
//モデル処理[model.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//=======================================
//マクロ定義
//=======================================
#define MODEL			"data\\MODEL\\face.x"

#define MODEL_SPEED		(2.0f)	//モデルの移動速度
#define MOVEFAST		(0.2f)

#define MAX_TEX			(8)
#define MODELTEX		"data\\TEXTURE\\SaberUV.png"

#define CAMERA_SPEED	(5.0f)

#define BULLET_LIFE     (50)    //弾の寿命

#define WALL_RIMIT		(980)

//=======================================
//グローバル変数宣言
//=======================================
LPD3DXMESH g_pMeshModel = NULL;				//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;		//マテリアルへのポインタ
DWORD g_dwNumMatModel = 0;					//マテリアルの数
D3DXMATRIX g_mtxWorldModel;					//ワールドマトリックス
LPDIRECT3DTEXTURE9 g_pTextureModel = NULL;	//テクスチャのポインタ

D3DXVECTOR3 g_moveModel;				//移動量
Model g_Model;							//モデルの情報
int g_nldxShadow = -1;					//対象に影のインデックス(番号)

D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;	//モデルの最大、最小値

//=======================================
//プロトタイプ宣言
//=======================================
void PlayerMove(void);

//=======================================
//モデルの初期化処理
//=======================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXMATERIAL *pMat;

	int nCntTex;

	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのポインタ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//モデルの位置を初期化
	g_Model.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);

	//モデルの移動量を初期化
	g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//モデルの向きを初期化
	g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Model.fDistance = 0.0f;

	//モデルの最小値を初期化
	g_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	//モデルの最大値を初期化
	g_vtxMaxModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_pBuffMatModel,
					NULL,
					&g_dwNumMatModel,
					&g_pMeshModel);


	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (nCntTex = 0; nCntTex < (int)g_dwNumMatModel; nCntTex++)
	{
		if (pMat[nCntTex].pTextureFilename != NULL)
		{//テクスチャファイル名が存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, MODELTEX, &g_pTextureModel);
		}
	}

	//頂点数の取得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		
		//すべての頂点を比較してモデル最小値・最大値を抜き出す
		
		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();

	//影を設定
	g_nldxShadow = SetShadow();
}

//=======================================
//モデルの終了処理
//=======================================
void UninitModel(void)
{
	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=======================================
//モデルの更新処理
//=======================================
void UpdateModel(void)
{
	PlayerMove();
}

//=======================================
//モデルの描画処理
//=======================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel); //テクスチャを使用してないときはNULL
		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================
//モデルの取得
//=======================================
Model *GetModel(void)
{
	return &g_Model;
}

//=======================================
//プレイヤーの動き
//=======================================
void PlayerMove(void)
{
	Camera *pCamera;

	pCamera = GetCamera();

	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//左矢印が押された
	if (GetKeyboardPress(DIK_LEFT) == true || GetPadPress(BUTTON_LEFT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) < 0)
	{
		//左上
		if (GetKeyboardPress(DIK_UP) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.75f));
			g_Model.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.75f));

			//移動方向にモデルを向ける
			g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		//左下
		else if (GetKeyboardPress(DIK_DOWN) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.25f));
			g_Model.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.25f));

			//移動方向にモデルを向ける
			g_Model.RotDest.y = -pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else
		{//左
			g_Model.move.x -= cosf(pCamera->rot.y + (D3DX_PI * 0.5f));
			g_Model.move.z -= sinf(pCamera->rot.y + (D3DX_PI * 0.5f));

			//移動方向にモデルを向ける
			g_Model.RotDest.y = -pCamera->rot.y;

			//注視点をずらす
			pCamera->posR - g_Model.move;
		}
	}
	//右矢印が押された
	else if (GetKeyboardPress(DIK_RIGHT) == true || GetPadPress(BUTTON_RIGHT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		//右上
		if (GetKeyboardPress(DIK_UP) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f));
			g_Model.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f));

			//移動方向にモデルを向ける
			g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		//右下
		else if (GetKeyboardPress(DIK_DOWN) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f));
			g_Model.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f));

			//移動方向にモデルを向ける
			g_Model.RotDest.y = -pCamera->rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//右
			g_Model.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f));
			g_Model.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f));

			//移動方向にモデルを向ける
			g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI);
		}
	}
	//上矢印が押された
	else if (GetKeyboardPress(DIK_UP) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_Model.move.x -= cosf(pCamera->rot.y);
		g_Model.move.z -= sinf(pCamera->rot.y);

		//移動方向にモデルを向ける
		g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI * 0.5f);
	}
	//下矢印が押された
	else if (GetKeyboardPress(DIK_DOWN) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_Model.move.x += cosf(pCamera->rot.y);
		g_Model.move.z += sinf(pCamera->rot.y);

		//移動方向にモデルを向ける
		g_Model.RotDest.y = -pCamera->rot.y + (-D3DX_PI * 0.5f);
	}

	DiffRot.y = g_Model.RotDest.y - g_Model.rot.y;

	//3.14を超えたときに反対にする
	if (DiffRot.y > D3DX_PI)
	{
		DiffRot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2;
	}

	//Diffに補正係数をかける
	g_Model.rot.y += DiffRot.y * 0.1f;

	//3.14を超えたときに反対にする
	if (g_Model.rot.y > D3DX_PI)
	{
		g_Model.rot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (g_Model.rot.y < -D3DX_PI)
	{
		g_Model.rot.y += D3DX_PI * 2;
	}

	////目的の距離にカメラのロットを入れる
	//g_Model.RotDest.y = g_Model.rot.y;
	////進行方向に向ける
	//g_Model.rot.y += (0.0f - g_Model.RotDest.y);

	//Enterが押された
	if (GetKeyboardPress(DIK_RETURN) == true || GetPadPress(BUTTON_START, 0) == true)
	{
		//モデルの位置を初期化
		g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//モデルの向きを初期化
		g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//Spaceキーが押された
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetPadPress(BUTTON_RB, 0) == true)
	{
		SetBullet(g_Model.pos, D3DXVECTOR3(sinf(g_Model.rot.y) * -1.0f, 0.0f, cosf(g_Model.rot.y) * -1.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
	}

	//画面の横幅の設定
	if (g_Model.pos.x > WALL_RIMIT)
	{
		//始点(X座標)が画面の端に当たった
		g_Model.pos.x = WALL_RIMIT;
		g_Model.move.x = 0.0f;
	}
	else if (g_Model.pos.x < -WALL_RIMIT)
	{
		//始点(X座標)が画面の端に当たった
		g_Model.pos.x = -WALL_RIMIT;
		g_Model.move.x *= 0.0f;
	}

	//画面の奥行設定
	if (g_Model.pos.z > WALL_RIMIT)
	{
		//始点(Y座標)が画面の端に当たった
		g_Model.pos.z = WALL_RIMIT;  //始点(Y座標)を画面の下端に設定する
		g_Model.move.z = 0.0f;
	}
	else if (g_Model.pos.z < -WALL_RIMIT)
	{
		//始点(Y座標)が画面の端に当たった
		g_Model.pos.z = -WALL_RIMIT; 	    //始点(Y座標)を画面の下端に設定する
		g_Model.move.z *= 0.0f;       //移動方向を逆にする
	}

	//位置を更新
	g_Model.pos.x += g_Model.move.x;
	g_Model.pos.z += g_Model.move.z;

	//移動量を更新(減衰させる)
	g_Model.move.x += (0.0f - g_Model.move.x) * MOVEFAST;
	g_Model.move.z += (0.0f - g_Model.move.z) * MOVEFAST;

	//影の位置を設定
	SetPositionShadow(g_nldxShadow, g_Model.pos);
}