//
//
//�J��������[camera.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "camera.h"    //�쐬����fade.h���C���N���[�h����
#include "input.h"
#include "model.h"

//=======================================
//�}�N����`
//=======================================
#define CAMERA_SPEED	(5.0f)
#define MOVEFAST		(0.2f)

//=======================================
//�O���[�o���ϐ��錾
//=======================================
Camera g_camera;	//�J�����̏��

//=======================================
//�v���g�^�C�v�錾
//=======================================
void CameraMove(void);
void CameraBack(void);
void MoveCamera(void);
void CameraMoveV(void);
void CameraMoveR(void);

//=======================================
//�J�����̏���������
//=======================================
void InitCamera(void)
{
	//���_
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -250.0f);

	//�����_
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//������x�N�g��
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//����
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		

	//����
	g_camera.fDistance = 200.0f;

	g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;
}

//=======================================
//�J�����̏I������
//=======================================
void UninitCamera(void)
{

}

//=======================================
//�J�����̍X�V����
//=======================================
void UpdateCamera(void)
{
	//�J�����̒Ǐ]
	CameraMove();

	//�w��
	CameraBack();

	//�J�����ړ�
	MoveCamera();

	//���_�̈ړ�
	CameraMoveV();

	//�����_�̈ړ�
	CameraMoveR();

	//���_�E�����_�E�����x�N�g���E���������Z�b�g����
	if (GetKeyboardPress(DIK_RETURN) == true || GetPadPress(BUTTON_START, 0) == true)
	{//Enter�L�[�������ꂽ
		g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -250.0f);
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=======================================
//�J�����̐ݒ菈��
//=======================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, 
								D3DXToRadian(45.0f), 
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,		//Z�l�̍ŏ��l
								1000.0f);	//Z�l�̍ő�l

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,		//���_
						&g_camera.posR,		//�����_
						&g_camera.vecU);	//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=======================================
//�J�����̎擾
//=======================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//=======================================
//�J�����̒Ǐ]
//=======================================
void CameraMove(void)
{
	D3DXVECTOR3 Diff;
	D3DXVECTOR3 VDiff;

	//���f���̏��
	Model *pModel = GetModel();

	//�����_
	g_camera.RDest = D3DXVECTOR3 (pModel->pos.x  + -sinf(pModel->rot.y) * 50.0f, pModel->pos.y, pModel->pos.z  + -cosf(pModel->rot.y) * 50.0f);

	g_camera.VDest.x = g_camera.RDest.x + cosf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.VDest.z = g_camera.RDest.z + sinf(g_camera.rot.y) * g_camera.fDistance;

	//�����_
	Diff = D3DXVECTOR3(
		g_camera.RDest.x - g_camera.posR.x, 0.0f, g_camera.RDest.z - g_camera.posR.z);

	//���_
	VDiff = D3DXVECTOR3(
		g_camera.VDest.x - g_camera.posV.x, 0.0f, g_camera.VDest.z - g_camera.posV.z);

	//�����_�̊���
	g_camera.posR += Diff * 0.3f;

	//���_�̊���
	g_camera.posV += VDiff * 0.2f;
}

//=======================================
//�J�����̔w��
//=======================================
void CameraBack(void)
{
	D3DXVECTOR3 CamDest;
	D3DXVECTOR3 CamDiff;

	//���f���̏��
	Model *pModel = GetModel();

	if (pModel->move.x > -0.1f && pModel->move.x < 0.1f && pModel->move.z > -0.1f && pModel->move.z < 0.1f)
	{

		//����
		CamDest = pModel->rot;

		CamDiff.y = (-CamDest.y + D3DX_PI * 0.5f) - g_camera.rot.y;

		//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (CamDiff.y > D3DX_PI)
		{
			CamDiff.y -= D3DX_PI * 2;
		}

		//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (CamDiff.y < -D3DX_PI)
		{
			CamDiff.y += D3DX_PI * 2;
		}

		//Diff�ɕ␳�W����������
		g_camera.rot.y += CamDiff.y * 0.06f;

		//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (CamDiff.y > D3DX_PI)
		{
			CamDiff.y -= D3DX_PI * 2;
		}

		//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (CamDiff.y < -D3DX_PI)
		{
			CamDiff.y += D3DX_PI * 2;
		}

	}

	g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;
}

//=======================================
//�J�����̈ړ�
//=======================================
void MoveCamera(void)
{
	//A�L�[�������ꂽ
	if (GetKeyboardPress(DIK_A) == true || GetPadPress(BUTTON_LEFT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) < 0)
	{
		if (GetKeyboardPress(DIK_W) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//����
			g_camera.posV.x += cosf(g_camera.rot.y + (-D3DX_PI * 0.75f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (-D3DX_PI * 0.75f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//����
			g_camera.posV.x += cosf(g_camera.rot.y + (-D3DX_PI * 0.25f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (-D3DX_PI * 0.25f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else
		{//��
			g_camera.posV.x += cosf(g_camera.rot.y + (-D3DX_PI * 0.5f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (-D3DX_PI * 0.5f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
	}
	//D�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_D) == true || GetPadPress(BUTTON_RIGHT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		if (GetKeyboardPress(DIK_W) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//�E��
			g_camera.posV.x += cosf(g_camera.rot.y + (D3DX_PI * 0.75f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (D3DX_PI * 0.75f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{//�E��
			g_camera.posV.x += cosf(g_camera.rot.y + (D3DX_PI * 0.25f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (D3DX_PI * 0.25f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
		else
		{//�E
			g_camera.posV.x += cosf(g_camera.rot.y + (+D3DX_PI * 0.5f)) * CAMERA_SPEED;
			g_camera.posV.z += sinf(g_camera.rot.y + (+D3DX_PI * 0.5f)) * CAMERA_SPEED;

			g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
		}
	}
	//W�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_W) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_camera.posV.x -= cosf(g_camera.rot.y) * CAMERA_SPEED;
		g_camera.posV.z -= sinf(g_camera.rot.y) * CAMERA_SPEED;

		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
	}
	//S�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_S) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_camera.posV.x += cosf(g_camera.rot.y) * CAMERA_SPEED;
		g_camera.posV.z += sinf(g_camera.rot.y) * CAMERA_SPEED;

		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
	}

	//�ʒu���X�V
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.y += g_camera.move.y;
	g_camera.posV.z += g_camera.move.z;

	g_camera.posR.x += g_camera.move.x;
	g_camera.posR.y += g_camera.move.y;
	g_camera.posR.z += g_camera.move.z;

	//�ړ��ʂ��X�V(����������)
	g_camera.move.x += (0.0f - g_camera.move.x) * MOVEFAST;
	g_camera.move.y += (0.0f - g_camera.move.y) * MOVEFAST;
	g_camera.move.z += (0.0f - g_camera.move.z) * MOVEFAST;
}

//=======================================
//���_�̈ړ�
//=======================================
void CameraMoveV(void)
{
	//C�L�[�������ꂽ
	if (GetKeyboardPress(DIK_C) == true || GetPadPress(BUTTON_LEFT, 0) == true)
	{
		g_camera.rot.y += 0.05f;

		//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}

		g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;
	}
	//Z�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_Z) == true || GetPadPress(BUTTON_RIGHT, 0) == true)
	{
		g_camera.rot.y -= 0.05f;

		//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}

		g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.y) * g_camera.fDistance;

	}
	//Y�L�[�������ꂽ
	if (GetKeyboardPress(DIK_Y) == true || GetPadPress(BUTTON_UP, 0) == true)
	{
		g_camera.posV.y += CAMERA_SPEED;
		g_camera.posV.z += CAMERA_SPEED;
	}
	//N�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_N) == true || GetPadPress(BUTTON_DOWN, 0) == true)
	{
		g_camera.posV.y -= CAMERA_SPEED;
		g_camera.posV.z -= CAMERA_SPEED;
	}

	//�ʒu���X�V
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.y += g_camera.move.y;
	g_camera.posV.z += g_camera.move.z;

	g_camera.posR.x += g_camera.move.x;
	g_camera.posR.y += g_camera.move.y;
	g_camera.posR.z += g_camera.move.z;

	//�ړ��ʂ��X�V(����������)
	g_camera.move.x += (0.0f - g_camera.move.x) * MOVEFAST;
	g_camera.move.y += (0.0f - g_camera.move.y) * MOVEFAST;
	g_camera.move.z += (0.0f - g_camera.move.z) * MOVEFAST;
}

//=======================================
//�����_�̈ړ�
//=======================================
void CameraMoveR(void)
{
	//Q�L�[�������ꂽ
	if (GetKeyboardPress(DIK_E) == true || GetPadPress(BUTTON_LEFT, 0) == true)
	{
		g_camera.rot.y -= 0.05f;

		//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}
		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;

	}
	//E�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_Q) == true || GetPadPress(BUTTON_RIGHT, 0) == true)
	{
		g_camera.rot.y += 0.05f;

		//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;

		}
		g_camera.posR.x = g_camera.posV.x - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - sinf(g_camera.rot.y) * g_camera.fDistance;
	}
	//T�L�[�������ꂽ
	if (GetKeyboardPress(DIK_T) == true || GetPadPress(BUTTON_UP, 0) == true)
	{
		g_camera.posR.y += CAMERA_SPEED;
		g_camera.posR.z += CAMERA_SPEED;
	}
	//B�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_B) == true || GetPadPress(BUTTON_DOWN, 0) == true)
	{
		g_camera.posR.y -= CAMERA_SPEED;
		g_camera.posR.z -= CAMERA_SPEED;
	}

	//�ʒu���X�V
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.y += g_camera.move.y;
	g_camera.posV.z += g_camera.move.z;

	g_camera.posR.x += g_camera.move.x;
	g_camera.posR.y += g_camera.move.y;
	g_camera.posR.z += g_camera.move.z;

	//�ړ��ʂ��X�V(����������)
	g_camera.move.x += (0.0f - g_camera.move.x) * MOVEFAST;
	g_camera.move.y += (0.0f - g_camera.move.y) * MOVEFAST;
	g_camera.move.z += (0.0f - g_camera.move.z) * MOVEFAST;
}