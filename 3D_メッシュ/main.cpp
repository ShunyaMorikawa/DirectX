//
//
//���C������[main.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
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
//�}�N����`
//=======================================
#define CLASS_NAME       "WindowClass"            //�E�B���h�E�N���X�̖��O
#define WINDOW_NAME      "3D�Q�[��"	              //�E�B���h�E�̖��O
#define MAX_PARTEEN      (8)                      //�A�j���[�V�����̍ő�l          
#define MAX_LETTER       (256)                    //�f�o�b�O�\���̍ő啶����

//=======================================
//�v���g�^�C�v�錾
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
//�O���[�o���ϐ�
//=======================================
LPDIRECT3D9 g_pD3D = NULL;                  //Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;      //Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;                  //�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;                        //FPS�J�E���^
bool g_bDispDebug = false;                  //�f�o�b�O��Ԃ�ON/OFF
MODE g_mode = MODE_TITLE;                   //���݂̃��[�h

//========================================
//���C���֐�
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//��ʃT�C�Y�̍\����
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//���ݎ���
	DWORD dwCurrentTime;
	//�Ō�ɏ�����������
	DWORD dwExecLastTime;

	//�t���[���J�E���g
	DWORD dwFrameCount;
	//�Ō��FPS���v����������
	DWORD dwFPSLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),                 //WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,                         //�E�B���h�E�̃X�^�C��
		WindowProc,                         //�E�B���h�E�v���V�[�W��
		0,                                  //0�ɂ���
		0,                                  //0�ɂ���
		hInstance,                          //�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),     //�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),         //�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),         //�N���C�A���g�̈�̔w�i�F
		NULL,                               //���j���[�o�[
		CLASS_NAME,                         //�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)      //�t�@�C���A�C�R��
	};

	//�E�B���h�E�n���h��(���ʎq)
	HWND hWnd;

	//���b�Z�[�W���i�[����
	MSG msg;

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E����
	hWnd = CreateWindowEx
	(0,                                   //�g���E�B���h�E�X�^�C��
		CLASS_NAME,                       //�E�B���h�E�N���X�̖��O
		WINDOW_NAME,                      //�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,              //�E�B���h�E�X�^�C��
		CW_USEDEFAULT,                    //�E�B���h�E�̍���X���W
		CW_USEDEFAULT,                    //�E�B���h�E�̍���Y���W
		(rect.right - rect.left),         //�E�B���h�E�̕�
		(rect.bottom - rect.top),         //�E�B���h�E�̍���
		NULL,                             //�e�E�B���h�E�̃n���h��
		NULL,                             //���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,                        //�C���X�^���X�n���h��
		NULL);                            //�E�B���h�E�쐬�f�[�^

										  //����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	//����������
	dwCurrentTime = 0;

	//���ݎ������擾(�ۑ�)
	dwExecLastTime = timeGetTime();

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
		 //���݂̎������擾
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPS�𑪒肵��������ۑ�
				dwFPSLastTime = dwCurrentTime;
				//�t���[�����[�g���N���A
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
			 //�����J�n�̎���[���ݎ���]��ۑ�
				dwExecLastTime = dwCurrentTime;

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				//�t���[�����[�g���v�Z
				dwFrameCount++;
			}
		}
	}

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
//�E�B���h�E�v���V�[�W��
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID; //�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY://�E�B���h�E�j�����b�Z�[�W
		//WM_QUIT�𑗂�
		PostQuitMessage(0);
		break;

		//�L�[�����̃��b�Z�[�W
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		break;

	case  WM_CLOSE://����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//�E�B���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{//0��Ԃ��Ȃ��ƏI�����Ă��܂�
			return 0;
		}
		break;
	}
	//����̏������J��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===========================================
//����������
//===========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	int nType = 0;

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));  // �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;                          //�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			               //�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;                         //�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;                                     //�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                      //�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;                           //�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;                                      //�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3D�f�o�C�X�̐���
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

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�p�b�h�̏�����
	InitPad();

	//�J�����̏�����
	InitCamera();

	//���C�g�̏�����
	InitLight();

	//�|���S���̏�����
	InitPolygon();

	//���b�V���t�B�[���h�̏�����
	InitMeshfield();

	//�C���f�b�N�X���b�V���t�B�[���h�̏�����
	InitIdxMeshfield();

	//���b�V���E�H�[���̏�����
	InitMeshWall();

	//�e�̏�����
	InitShadow();

	//���f���̏�����
	InitModel();

	//�I�u�W�F�N�g�̏�����
	InitObject();

	//�G�̏���������
	InitEnemy();

	//�e�̏�����
	InitBullet();

	//�ǂ̏�����
	InitWall();
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f), float(D3DX_PI));			//���̕�
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 2000.0f), float(D3DX_PI * 1.0f));//��O�̕�
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f), float(D3DX_PI * 0.5f));	//���̕�
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f), float(-D3DX_PI * 0.5f));	//�E�̕�

	//�r���{�[�h�̏�����
	InitBillBoard();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�����̏�����
	InitExplosion();


	return S_OK;
}

//===========================================
//�I������
//===========================================
void Uninit(void)
{//�e��I�u�W�F�N�g�̏I������

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�p�b�h�̏I������
	UninitPad();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�|���S���̏I������
	UninitPolygon();

	//���f���̏I������
	UninitModel();

	//�I�u�W�F�N�g�̏I������
	UninitObject();

	//�G�̏I������
	UninitEnemy();

	//���b�V���t�B�[���h�̏I������
	UninitMeshfield();

	//�C���f�b�N�X���b�V���t�B�[���h�̏I������
	UninitIdxMeshfield();

	//���b�V���E�H�[��
	UninitMeshWall();

	//�e�̏I������
	UninitShadow();

	//�ǂ̏I������
	UninitWall();

	//�r���{�[�h�̏I������
	UninitBillBoard();

	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�����̏I������
	UninitExplosion();

	//���f���̏I������
 //�f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//����\��߂�
	timeEndPeriod(1);

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//===========================================
//�X�V����
//===========================================
void Update(void)
{//�X�V����
	
	//�|���S���̍X�V����
	UpdatePolygon();
	
	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshfield();

	//�C���f�b�N�X���b�V���t�B�[���h�̍X�V����
	UpdateIdxMeshfield();

	//���b�V���E�H�[��
	UpdateMeshWall();

	//���f���̍X�V����
	UpdateModel();

	//�I�u�W�F�N�g�̍X�V����
	UpdateObject();

	//�G�̍X�V����
	UpdateEnemy();

	//�e�̍X�V����
	UpdateShadow();

	//�ǂ̍X�V����
	UpdateWall();

	//�r���{�[�h�̍X�V����
	UpdateBillBoard();

	//�e�̍X�V����
	UpdateBullet();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�����̍X�V����
	UpdateExplosion();

	//�J�����̍X�V����
	UpdateCamera();

	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�p�b�h�̍X�V����
	UpdatePad();

	//���C�g�̍X�V����
	UpdateLight();

	//F1�L�[�������ꂽ
	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		// ���C���[�t���[��
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
}

//===========================================
//�`�揈��
//===========================================
void Draw(void)
{//�e��I�u�W�F�N�g�̕`�揈��

 //��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//�J�����̐ݒ�
		SetCamera();

		//�|���S���̕`��
		//DrawPolygon();

		//���b�V���t�B�[���h�̕`�揈��
		//DrawMeshfield();

		//�C���f�b�N�X���b�V���t�B�[���h�̕`�揈��
		DrawIdxMeshfield();

		//���b�V���E�H�[���̕`�揈��
		//DrawMeshWall();

		//���f���̕`�揈��
		DrawModel();

		//�I�u�W�F�N�g�̕`�揈��
		DrawObject();

		//�G�̕`�揈��
		DrawEnemy();

		//�e�̕`�揈��
		DrawShadow();

		//�ǂ̕`�揈��
		DrawWall();

		//�r���{�[�h�̕`�揈��
		DrawBillBoard();

		//�e�̕`�揈��
		DrawBullet();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�����̕`�揈��
		DrawExplosion();

	 //�f�o�b�O�\��
#ifdef _DEBUG
		//FPS�̕\��
		DrawFPS();
		//�e�̍��W
		DrawBulletCoordinate();
		//���_�̍��W
		DrawCameraCoordinate();
		//�����_�̍��W
		DrawCameraR();
		//���f���̍��W
		DrawModelCoordinate();
		//�J�����̌���
		DrawCameraRot();
#endif

		//�J�����̈ړ����@
		DrawCameraMove();

		//���_�̈ړ����@
		DrawPer();

		//�����_�̈ړ����@
		DrawGaze();

		//���f���̈ړ����@
		DrawModelMove();

		//�e�̔��˕��@
		DrawBulletOpe();

		//�J�����E���f�����Z�b�g���@
		DrawReset();

		//�`��I��
		g_pD3DDevice->EndScene();

		//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//===========================================
//�f�o�C�X�̎擾
//===========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//===========================================
//FPS�`��
//===========================================
void DrawFPS(void)
{
	RECT rect = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�v���C���[�̍��W�̕`��
//===========================================
void DrawModelCoordinate(void)
{
	RECT rect = { 0, 70, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Model *pModel;

	pModel = GetModel();

	//���������
	sprintf(&aStr[0], "�v���C���[�̍��W(X:%.2f, Y:%.2f, Z:%.2f)\n", pModel->pos.x, pModel->pos.y, pModel->pos.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�e�̍��W�̕`��
//===========================================
void DrawBulletCoordinate(void)
{
	RECT rect = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Bullet *pBullet;

	pBullet = GetBullet();

	//���������
	sprintf(&aStr[0], "�e�̍��W(X:%.2f, Y:%.2f, Z:%.2f)\n", pBullet->pos.x, pBullet->pos.y, pBullet->pos.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�J�����̍��W�̕`��
//===========================================
void DrawCameraCoordinate(void)
{
	RECT rect = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Camera *nCamera;

	nCamera = GetCamera();

	//���������
	sprintf(&aStr[0], "�J�������W(X:%.2f, Y:%.2f, Z:%.2f)\n", nCamera->posV.x, nCamera->posV.y, nCamera->posV.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�J�����̒����_���W�̕`��
//===========================================
void DrawCameraR(void)
{
	RECT rect = { 0,30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Camera *nCamera;

	nCamera = GetCamera();

	//���������
	sprintf(&aStr[0], "�����_���W(X:%.2f, Y:%.2f, Z:%.2f)\n", nCamera->posR.x, nCamera->posR.y, nCamera->posR.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�J�����̌������W�̕`��
//===========================================
void DrawCameraRot(void)
{
	RECT rect = { 0,70, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];
	Camera *nCamera;

	nCamera = GetCamera();

	//���������
	sprintf(&aStr[0], "�������W(X:%.2f, Y:%.2f, Z:%.2f)\n", nCamera->rot.x, nCamera->rot.y, nCamera->rot.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�J�����ړ��`��
//===========================================
void DrawCameraMove(void)
{
	RECT rect = { 0,110, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "----�J�����ړ�----\n�㉺(W / S)���E(A / D)\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//���_������@�̕`��
//===========================================
void DrawPer(void)
{
	RECT rect = { 0,140, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "-------���_-------\n�ړ�(Y / N)����(Z / C)\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�����_������@�̕`��
//===========================================
void DrawGaze(void)
{
	RECT rect = { 0,180, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "------�����_------\n�ړ�(T / B)����(Q / E)\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//���f��������@�̕`��
//===========================================
void DrawModelMove(void)
{
	RECT rect = { 0,220, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "----���f���ړ�----\n�ړ�(�� / ��, �� / ��)\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�e�̔��˕��@�̕`��
//===========================================
void DrawBulletOpe(void)
{
	RECT rect = { 0,260, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "-----�e�̔���-----\nSpace�L�[\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================================
//�J�����E���f�����Z�b�g���@�̕`��
//===========================================
void DrawReset(void)
{
	RECT rect = { 0,300, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_LETTER];

	//���������
	wsprintf(&aStr[0], "�J�����E���f�����Z�b�g\nEnter�L�[");

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}