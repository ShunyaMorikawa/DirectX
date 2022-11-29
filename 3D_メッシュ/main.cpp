//
//
//メイン処理[main.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//作成したmain.hをインクルードする
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "shadow.h"
#include "wall.h"
#include "camera.h"
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "meshfield.h"
#include "idxmeshfield.h"
#include "meshwall.h"
#include "explosion.h"
#include "obj.h"
#include "enemy.h"
#include <stdio.h>

//=======================================
//マクロ定義
//=======================================
#define CLASS_NAME       "WindowClass"            //ウィンドウクラスの名前
#define WINDOW_NAME      "3Dゲーム"	              //ウィンドウの名前
#define MAX_PARTEEN      (8)                      //アニメーションの最大値          
#define MAX_LETTER       (256)                    //デバッグ表示の最大文字数

//=======================================
//プロトタイプ宣言
//=======================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawBulletCoordinate(void);
void DrawCameraCoordinate(void);
void DrawCameraR(void);
void DrawCameraRot(void);
void DrawCameraMove(void);
void DrawPer(void);
void DrawGaze(void);
void DrawModelMove(void);
void DrawReset(void);
void DrawBulletOpe(void);
void DrawModelCoordinate(void);

//=======================================
//グローバル変数
//=======================================
LPDIRECT3D9 g_pD3D = NULL;                  //Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;      //Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;                  //フォントへのポインタ
int g_nCountFPS = 0;                        //FPSカウンタ
bool g_bDispDebug = false;                  //デバッグ状態のON/OFF
MODE g_mode = MODE_TITLE;                   //現在のモード

//========================================
//メイン関数
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//画面サイズの構造体
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//現在時刻
	DWORD dwCurrentTime;
	//最後に処理した時刻
	DWORD dwExecLastTime;

	//フレームカウント
	DWORD dwFrameCount;
	//最後にFPSを計測した時刻
	DWORD dwFPSLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),                 //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                         //ウィンドウのスタイル
		WindowProc,                         //ウィンドウプロシージャ
		0,                                  //0にする
		0,                                  //0にする
		hInstance,                          //インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),     //タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),         //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),         //クライアント領域の背景色
		NULL,                               //メニューバー
		CLASS_NAME,                         //ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)      //ファイルアイコン
	};

	//ウィンドウハンドル(識別子)
	HWND hWnd;

	//メッセージを格納する
	MSG msg;

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウ生成
	hWnd = CreateWindowEx
	(0,                                   //拡張ウィンドウスタイル
		CLASS_NAME,                       //ウィンドウクラスの名前
		WINDOW_NAME,                      //ウィンドウの名前
		WS_OVERLAPPEDWINDOW,              //ウィンドウスタイル
		CW_USEDEFAULT,                    //ウィンドウの左上X座標
		CW_USEDEFAULT,                    //ウィンドウの左上Y座標
		(rect.right - rect.left),         //ウィンドウの幅
		(rect.bottom - rect.top),         //ウィンドウの高さ
		NULL,                             //親ウィンドウのハンドル
		NULL,                             //メニューハンドルまたは子ウィンドウID
		hInstance,                        //インスタンスハンドル
		NULL);                            //ウィンドウ作成データ

										  //初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	//初期化する
	dwCurrentTime = 0;

	//現在時刻を取得(保存)
	dwExecLastTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
		 //現在の時刻を取得
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;
				//フレームレートをクリア
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
			 //処理開始の時刻[現在時刻]を保存
				dwExecLastTime = dwCurrentTime;

				//更新処理
				Update();

				//描画処理
				Draw();

				//フレームレートを計算
				dwFrameCount++;
			}
		}
	}

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
//ウィンドウプロシージャ
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID; //返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY://ウィンドウ破棄メッセージ
		//WM_QUITを送る
		PostQuitMessage(0);
		break;

		//キー押下のメッセージ
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		break;

	case  WM_CLOSE://閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{//0を返さないと終了してしまう
			return 0;
		}
		break;
	}
	//既定の処理を繰り返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===========================================
//初期化処理
//===========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	int nType = 0;

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));  // パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;                          //ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			               //ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;                         //バックバッファの形式
	d3dpp.BackBufferCount = 1;                                     //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                      //ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;                           //デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;                                      //ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			return E_FAIL;
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//パッドの初期化
	InitPad();

	//カメラの初期化
	InitCamera();

	//ライトの初期化
	InitLight();

	//ポリゴンの初期化
	InitPolygon();

	//メッシュフィールドの初期化
	InitMeshfield();

	//インデックスメッシュフィールドの初期化
	InitIdxMeshfield();

	//メッシュウォールの初期化
	InitMeshWall();

	//影の初期化
	InitShadow();

	//モデルの初期化
	InitModel();

	//オブジェクトの初期化
	InitObject();

	//敵の初期化処理
	InitEnemy();

	//弾の初期化
	InitBullet();

	//壁の初期化
	InitWall();
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f), float(D3DX_PI));			//奥の壁
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 2000.0f), float(D3DX_PI * 1.0f));//手前の壁
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f), float(D3DX_PI * 0.5f));	//左の壁
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f), float(-D3DX_PI * 0.5f));	//右の壁

	//ビルボードの初期化
	InitBillBoard();

	//エフェクトの初期化
	InitEffect();

	//爆発の初期化
	InitExplosion();


	return S_OK;
}

//===========================================
//終了処理
//===========================================
void Uninit(void)
{//各種オブジェクトの終了処理

	//キーボードの終了処理
	UninitKeyboard();

	//パッドの終了処理
	UninitPad();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//ポリゴンの終了処理
	UninitPolygon();

	//モデルの終了処理
	UninitModel();

	//オブジェクトの終了処理
	UninitObject();

	//敵の終了処理
	UninitEnemy();

	//メッシュフィールドの終了処理
	UninitMeshfield();

	//インデックスメッシュフィールドの終了処理
	UninitIdxMeshfield();

	//メッシュウォール
	UninitMeshWall();

	//影の終了処理
	UninitShadow();

	//壁の終了処理
	UninitWall();

	//ビルボードの終了処理
	UninitBillBoard();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//爆発の終了処理
	UninitExplosion();

	//モデルの終了処理
 //デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//分解能を戻す
	timeEndPeriod(1);

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//===========================================
//更新処理
//===========================================
void Update(void)
{//更新処理
	
	//ポリゴンの更新処理
	UpdatePolygon();
	
	//メッシュフィールドの更新処理
	UpdateMeshfield();

	//インデックスメッシュフィールドの更新処理
	UpdateIdxMeshfield();

	//メッシュウォール
	UpdateMeshWall();

	//モデルの更新処理
	UpdateModel();

	//オブジェクトの更新処理
	UpdateObject();

	//敵の更新処理
	UpdateEnemy();

	//影の更新処理
	UpdateShadow();

	//壁の更新処理
	UpdateWall();

	//ビルボードの更新処理
	UpdateBillBoard();

	//弾の更新処理
	UpdateBullet();

	//エフェクトの更新処理
	UpdateEffect();

	//爆発の更新処理
	UpdateExplosion();

	//カメラの更新処理
	UpdateCamera();

	//キーボードの更新処理
	UpdateKeyboard();

	//パッドの更新処理
	UpdatePad();

	//ライトの更新処理
	UpdateLight();

	//F1キーが押された
	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		// ワイヤーフレーム
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
}

//===========================================
//描画処理
//===========================================
void Draw(void)
{//各種オブジェクトの描画処理

 //画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//カメラの設定
		SetCamera();

		//ポリゴンの描画
		//DrawPolygon();

		//メッシュフィールドの描画処理
		//DrawMeshfield();

		//インデックスメッシュフィールドの描画処理
		DrawIdxMeshfield();

		//メッシュウォールの描画処理
		//DrawMeshWall();

		//モデルの描画処理
		DrawModel();

		//オブジェクトの描画処理
		DrawObject();

		//敵の描画処理
		DrawEnemy();

		//影の描画処理
		DrawShadow();

		//壁の描画処理
		DrawWall();

		//ビルボードの描画処理
		DrawBillBoard();

		//弾の描画処理
		DrawBullet();

		//エフェクトの描画処理
		DrawEffect();

		//爆発の描画処理
		DrawExplosion();

	 //デバッグ表示
#ifdef _DEBUG
		//FPSの表示
		DrawFPS();
		//弾の座標
		DrawBulletCoordinate();
		//視点の座標
		DrawCameraCoordinate();
		//注視点の座標
		DrawCameraR();
		//モデルの座標
		DrawModelCoordinate();
		//カメラの向き
		DrawCameraRot();
#endif

		//カメラの移動方法
		DrawCameraMove();

		//視点の移動方法
		DrawPer();

		//注視点の移動方法
		DrawGaze();

		//モデルの移動方法
		DrawModelMove();

		//弾の発射方法
		DrawBulletOpe();

		//カメラ・モデルリセット方法
		DrawReset();

		//描画終了
		g_pD3DDevice->EndScene();

		//バックバッファとフロントバッファの入れ替え
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//===========================================
//デバイスの取得
//===========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//===========================================
//FPS描画
//===========================================
void DrawFPS(void)
{
	RECT rect = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//プレイヤーの座標の描画
//===========================================
void DrawModelCoordinate(void)
{
	RECT rect = { 0, 70, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Model *pModel;

	pModel = GetModel();

	//文字列を代入
	sprintf(&aStr[0], "プレイヤーの座標(X:%.2f, Y:%.2f, Z:%.2f)\n", pModel->pos.x, pModel->pos.y, pModel->pos.z);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//弾の座標の描画
//===========================================
void DrawBulletCoordinate(void)
{
	RECT rect = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Bullet *pBullet;

	pBullet = GetBullet();

	//文字列を代入
	sprintf(&aStr[0], "弾の座標(X:%.2f, Y:%.2f, Z:%.2f)\n", pBullet->pos.x, pBullet->pos.y, pBullet->pos.z);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//カメラの座標の描画
//===========================================
void DrawCameraCoordinate(void)
{
	RECT rect = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Camera *nCamera;

	nCamera = GetCamera();

	//文字列を代入
	sprintf(&aStr[0], "カメラ座標(X:%.2f, Y:%.2f, Z:%.2f)\n", nCamera->posV.x, nCamera->posV.y, nCamera->posV.z);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//カメラの注視点座標の描画
//===========================================
void DrawCameraR(void)
{
	RECT rect = { 0,30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Camera *nCamera;

	nCamera = GetCamera();

	//文字列を代入
	sprintf(&aStr[0], "注視点座標(X:%.2f, Y:%.2f, Z:%.2f)\n", nCamera->posR.x, nCamera->posR.y, nCamera->posR.z);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//カメラの向き座標の描画
//===========================================
void DrawCameraRot(void)
{
	RECT rect = { 0,70, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Camera *nCamera;

	nCamera = GetCamera();

	//文字列を代入
	sprintf(&aStr[0], "向き座標(X:%.2f, Y:%.2f, Z:%.2f)\n", nCamera->rot.x, nCamera->rot.y, nCamera->rot.z);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//カメラ移動描画
//===========================================
void DrawCameraMove(void)
{
	RECT rect = { 0,110, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "----カメラ移動----\n上下(W / S)左右(A / D)\n");

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//視点操作方法の描画
//===========================================
void DrawPer(void)
{
	RECT rect = { 0,140, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "-------視点-------\n移動(Y / N)旋回(Z / C)\n");

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//注視点操作方法の描画
//===========================================
void DrawGaze(void)
{
	RECT rect = { 0,180, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "------注視点------\n移動(T / B)旋回(Q / E)\n");

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//モデル操作方法の描画
//===========================================
void DrawModelMove(void)
{
	RECT rect = { 0,220, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "----モデル移動----\n移動(↑ / ↓, ← / →)\n");

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//弾の発射方法の描画
//===========================================
void DrawBulletOpe(void)
{
	RECT rect = { 0,260, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "-----弾の発射-----\nSpaceキー\n");

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//カメラ・モデルリセット方法の描画
//===========================================
void DrawReset(void)
{
	RECT rect = { 0,300, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//文字列を代入
	wsprintf(&aStr[0], "カメラ・モデルリセット\nEnterキー");

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}