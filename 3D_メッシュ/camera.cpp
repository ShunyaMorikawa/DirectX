//
//
//カメラ処理[camera.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "camera.h"    //作成したfade.hをインクルードする
#include "input.h"
#include "model.h"

//=======================================
//マクロ定義
//=======================================
#define CAMERA_SPEED	(5.0f)
#define MOVEFAST		(0.2f)

//=======================================
//グローバル変数宣言
//=======================================
Camera g_camera;	//カメラの情報

//=======================================
//プロトタイプ宣言
//=======================================
void CameraMove(void);
void CameraBack(void);
void MoveCamera(void);
void CameraMoveV(void);
void CameraMoveR(void);

//=======================================
//カメラの初期化処理
//=======================================
void InitCamera(void)
{
	//視点
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -250.0f);

	//注視点
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//上方向ベクトル
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//向き
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		

	//距離
	g_camera.fDistance = 200.0f;

	g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;
}

//=======================================
//カメラの終了処理
//=======================================
void UninitCamera(void)
{

}

//=======================================
//カメラの更新処理
//=======================================
void UpdateCamera(void)
{
	//カメラの追従
	CameraMove();

	//背後
	CameraBack();

	//カメラ移動
	MoveCamera();

	//視点の移動
	CameraMoveV();

	//注視点の移動
	CameraMoveR();

	//視点・注視点・方向ベクトル・向きをリセットする
	if (GetKeyboardPress(DIK_RETURN) == true || GetPadPress(BUTTON_START, 0) == true)
	{//Enterキーが押された
		g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -250.0f);
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=======================================
//カメラの設定処理
//=======================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, 
								D3DXToRadian(45.0f), 
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,		//Z値の最小値
								1000.0f);	//Z値の最大値

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,		//視点
						&g_camera.posR,		//注視点
						&g_camera.vecU);	//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=======================================
//カメラの取得
//=======================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//=======================================
//カメラの追従
//=======================================
void CameraMove(void)
{
	D3DXVECTOR3 Diff;
	D3DXVECTOR3 VDiff;

	//モデルの情報
	Model *pModel = GetModel();

	//注視点
	g_camera.RDest = D3DXVECTOR3 (pModel->pos.x  + -sinf(pModel->rot.y) * 50.0f, pModel->pos.y, pModel->pos.z  + -cosf(pModel->rot.y) * 50.0f);

	g_camera.VDest.x = g_camera.RDest.x + cosf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.VDest.z = g_camera.RDest.z + sinf(g_camera.rot.y) * g_camera.fDistance;

	//注視点
	Diff = D3DXVECTOR3(
		g_camera.RDest.x - g_camera.posR.x, 0.0f, g_camera.RDest.z - g_camera.posR.z);

	//視点
	VDiff = D3DXVECTOR3(
		g_camera.VDest.x - g_camera.posV.x, 0.0f, g_camera.VDest.z - g_camera.posV.z);

	//注視点の慣性
	g_camera.posR += Diff * 0.3f;

	//視点の慣性
	g_camera.posV += VDiff * 0.2f;
}

//=======================================
//カメラの背後
//=======================================
void CameraBack(void)
{
	D3DXVECTOR3 CamDest;
	D3DXVECTOR3 CamDiff;

	//モデルの情報
	Model *pModel = GetModel();

	if (pModel->move.x > -0.1f && pModel->move.x < 0.1f && pModel->move.z > -0.1f && pModel->move.z < 0.1f)
	{

		//向き
		CamDest = pModel->rot;

		CamDiff.y = (-CamDest.y + D3DX_PI * 0.5f) - g_camera.rot.y;

		//3.14を超えたときに反対にする
		if (CamDiff.y > D3DX_PI)
		{
			CamDiff.y -= D3DX_PI * 2;
		}

		//-3.14を超えたときに反対にする
		if (CamDiff.y < -D3DX_PI)
		{
			CamDiff.y += D3DX_PI * 2;
		}

		//Diffに補正係数をかける
		g_camera.rot.y += CamDiff.y * 0.06f;

		//3.14を超えたときに反対にする
		if (CamDiff.y > D3DX_PI)
		{
			CamDiff.y -= D3DX_PI * 2;
		}

		//-3.14を超えたときに反対にする
		if (CamDiff.y < -D3DX_PI)
		{
			CamDiff.y += D3DX_PI * 2;
		}

	}

	g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;
}

//=======================================
//カメラの移動
//=======================================
void MoveCamera(void)
{
	//Aキーが押された
	if (GetKeyboardPress(DIK_A) == true || GetPadPress(BUTTON_LEFT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) < 0)
	{
		if (GetKeyboardPress(DIK_W) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//左上
			g_camera.posV.x += cosf(g_camera.rot.y + (-D3DX_PI * 0.75f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (-D3DX_PI * 0.75f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//左下
			g_camera.posV.x += cosf(g_camera.rot.y + (-D3DX_PI * 0.25f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (-D3DX_PI * 0.25f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else
		{//左
			g_camera.posV.x += cosf(g_camera.rot.y + (-D3DX_PI * 0.5f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (-D3DX_PI * 0.5f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
	}
	//Dキーが押された
	else if (GetKeyboardPress(DIK_D) == true || GetPadPress(BUTTON_RIGHT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		if (GetKeyboardPress(DIK_W) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//右上
			g_camera.posV.x += cosf(g_camera.rot.y + (D3DX_PI * 0.75f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (D3DX_PI * 0.75f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//右下
			g_camera.posV.x += cosf(g_camera.rot.y + (D3DX_PI * 0.25f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (D3DX_PI * 0.25f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else
		{//右
			g_camera.posV.x += cosf(g_camera.rot.y + (+D3DX_PI * 0.5f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (+D3DX_PI * 0.5f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
	}
	//Wキーが押された
	else if (GetKeyboardPress(DIK_W) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_camera.posV.x -= cosf(g_camera.rot.y) * CAMERA_SPEED;
		g_camera.posV.z -= sinf(g_camera.rot.y) * CAMERA_SPEED;

		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
	}
	//Sキーが押された
	else if (GetKeyboardPress(DIK_S) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_camera.posV.x += cosf(g_camera.rot.y) * CAMERA_SPEED;
		g_camera.posV.z += sinf(g_camera.rot.y) * CAMERA_SPEED;

		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
	}

	//位置を更新
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.y += g_camera.move.y;
	g_camera.posV.z += g_camera.move.z;

	g_camera.posR.x += g_camera.move.x;
	g_camera.posR.y += g_camera.move.y;
	g_camera.posR.z += g_camera.move.z;

	//移動量を更新(減衰させる)
	g_camera.move.x += (0.0f - g_camera.move.x) * MOVEFAST;
	g_camera.move.y += (0.0f - g_camera.move.y) * MOVEFAST;
	g_camera.move.z += (0.0f - g_camera.move.z) * MOVEFAST;
}

//=======================================
//視点の移動
//=======================================
void CameraMoveV(void)
{
	//Cキーが押された
	if (GetKeyboardPress(DIK_C) == true || GetPadPress(BUTTON_LEFT, 0) == true)
	{
		g_camera.rot.y += 0.05f;

		//3.14を超えたときに反対にする
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}

		g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;
	}
	//Zキーが押された
	else if (GetKeyboardPress(DIK_Z) == true || GetPadPress(BUTTON_RIGHT, 0) == true)
	{
		g_camera.rot.y -= 0.05f;

		//3.14を超えたときに反対にする
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}

		g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;

	}
	//Yキーが押された
	if (GetKeyboardPress(DIK_Y) == true || GetPadPress(BUTTON_UP, 0) == true)
	{
		g_camera.posV.y += CAMERA_SPEED;
		g_camera.posV.z += CAMERA_SPEED;
	}
	//Nキーが押された
	else if (GetKeyboardPress(DIK_N) == true || GetPadPress(BUTTON_DOWN, 0) == true)
	{
		g_camera.posV.y -= CAMERA_SPEED;
		g_camera.posV.z -= CAMERA_SPEED;
	}

	//位置を更新
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.y += g_camera.move.y;
	g_camera.posV.z += g_camera.move.z;

	g_camera.posR.x += g_camera.move.x;
	g_camera.posR.y += g_camera.move.y;
	g_camera.posR.z += g_camera.move.z;

	//移動量を更新(減衰させる)
	g_camera.move.x += (0.0f - g_camera.move.x) * MOVEFAST;
	g_camera.move.y += (0.0f - g_camera.move.y) * MOVEFAST;
	g_camera.move.z += (0.0f - g_camera.move.z) * MOVEFAST;
}

//=======================================
//注視点の移動
//=======================================
void CameraMoveR(void)
{
	//Qキーが押された
	if (GetKeyboardPress(DIK_E) == true || GetPadPress(BUTTON_LEFT, 0) == true)
	{
		g_camera.rot.y -= 0.05f;

		//3.14を超えたときに反対にする
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}
		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;

	}
	//Eキーが押された
	else if (GetKeyboardPress(DIK_Q) == true || GetPadPress(BUTTON_RIGHT, 0) == true)
	{
		g_camera.rot.y += 0.05f;

		//3.14を超えたときに反対にする
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;

		}
		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
	}
	//Tキーが押された
	if (GetKeyboardPress(DIK_T) == true || GetPadPress(BUTTON_UP, 0) == true)
	{
		g_camera.posR.y += CAMERA_SPEED;
		g_camera.posR.z += CAMERA_SPEED;
	}
	//Bキーが押された
	else if (GetKeyboardPress(DIK_B) == true || GetPadPress(BUTTON_DOWN, 0) == true)
	{
		g_camera.posR.y -= CAMERA_SPEED;
		g_camera.posR.z -= CAMERA_SPEED;
	}

	//位置を更新
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.y += g_camera.move.y;
	g_camera.posV.z += g_camera.move.z;

	g_camera.posR.x += g_camera.move.x;
	g_camera.posR.y += g_camera.move.y;
	g_camera.posR.z += g_camera.move.z;

	//移動量を更新(減衰させる)
	g_camera.move.x += (0.0f - g_camera.move.x) * MOVEFAST;
	g_camera.move.y += (0.0f - g_camera.move.y) * MOVEFAST;
	g_camera.move.z += (0.0f - g_camera.move.z) * MOVEFAST;
}