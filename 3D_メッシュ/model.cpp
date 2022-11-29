//
//
//���f������[model.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//=======================================
//�}�N����`
//=======================================
#define MODEL			"data\\MODEL\\face.x"

#define MODEL_SPEED		(2.0f)	//���f���̈ړ����x
#define MOVEFAST		(0.2f)

#define MAX_TEX			(8)
#define MODELTEX		"data\\TEXTURE\\SaberUV.png"

#define CAMERA_SPEED	(5.0f)

#define BULLET_LIFE     (50)    //�e�̎���

#define WALL_RIMIT		(980)

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPD3DXMESH g_pMeshModel = NULL;				//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;		//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;					//�}�e���A���̐�
D3DXMATRIX g_mtxWorldModel;					//���[���h�}�g���b�N�X
LPDIRECT3DTEXTURE9 g_pTextureModel = NULL;	//�e�N�X�`���̃|�C���^

D3DXVECTOR3 g_moveModel;				//�ړ���
Model g_Model;							//���f���̏��
int g_nldxShadow = -1;					//�Ώۂɉe�̃C���f�b�N�X(�ԍ�)

D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;	//���f���̍ő�A�ŏ��l

//=======================================
//�v���g�^�C�v�錾
//=======================================
void PlayerMove(void);

//=======================================
//���f���̏���������
//=======================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXMATERIAL *pMat;

	int nCntTex;

	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃|�C���^
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���f���̈ʒu��������
	g_Model.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);

	//���f���̈ړ��ʂ�������
	g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���f���̌�����������
	g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Model.fDistance = 0.0f;

	//���f���̍ŏ��l��������
	g_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	//���f���̍ő�l��������
	g_vtxMaxModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_pBuffMatModel,
					NULL,
					&g_dwNumMatModel,
					&g_pMeshModel);


	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (nCntTex = 0; nCntTex < (int)g_dwNumMatModel; nCntTex++)
	{
		if (pMat[nCntTex].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C���������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, MODELTEX, &g_pTextureModel);
		}
	}

	//���_���̎擾
	nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		
		//���ׂĂ̒��_���r���ă��f���ŏ��l�E�ő�l�𔲂��o��
		
		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshModel->UnlockVertexBuffer();

	//�e��ݒ�
	g_nldxShadow = SetShadow();
}

//=======================================
//���f���̏I������
//=======================================
void UninitModel(void)
{
	//���b�V���̔j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=======================================
//���f���̍X�V����
//=======================================
void UpdateModel(void)
{
	PlayerMove();
}

//=======================================
//���f���̕`�揈��
//=======================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModel);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureModel); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=======================================
//���f���̎擾
//=======================================
Model *GetModel(void)
{
	return &g_Model;
}

//=======================================
//�v���C���[�̓���
//=======================================
void PlayerMove(void)
{
	Camera *pCamera;

	pCamera = GetCamera();

	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����󂪉����ꂽ
	if (GetKeyboardPress(DIK_LEFT) == true || GetPadPress(BUTTON_LEFT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) < 0)
	{
		//����
		if (GetKeyboardPress(DIK_UP) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.75f));
			g_Model.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.75f));

			//�ړ������Ƀ��f����������
			g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		//����
		else if (GetKeyboardPress(DIK_DOWN) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.25f));
			g_Model.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.25f));

			//�ړ������Ƀ��f����������
			g_Model.RotDest.y = -pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else
		{//��
			g_Model.move.x -= cosf(pCamera->rot.y + (D3DX_PI * 0.5f));
			g_Model.move.z -= sinf(pCamera->rot.y + (D3DX_PI * 0.5f));

			//�ړ������Ƀ��f����������
			g_Model.RotDest.y = -pCamera->rot.y;

			//�����_�����炷
			pCamera->posR - g_Model.move;
		}
	}
	//�E��󂪉����ꂽ
	else if (GetKeyboardPress(DIK_RIGHT) == true || GetPadPress(BUTTON_RIGHT, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		//�E��
		if (GetKeyboardPress(DIK_UP) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f));
			g_Model.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f));

			//�ړ������Ƀ��f����������
			g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		//�E��
		else if (GetKeyboardPress(DIK_DOWN) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
		{
			g_Model.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f));
			g_Model.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f));

			//�ړ������Ƀ��f����������
			g_Model.RotDest.y = -pCamera->rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//�E
			g_Model.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f));
			g_Model.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f));

			//�ړ������Ƀ��f����������
			g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI);
		}
	}
	//���󂪉����ꂽ
	else if (GetKeyboardPress(DIK_UP) == true || GetPadPress(BUTTON_UP, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_Model.move.x -= cosf(pCamera->rot.y);
		g_Model.move.z -= sinf(pCamera->rot.y);

		//�ړ������Ƀ��f����������
		g_Model.RotDest.y = -pCamera->rot.y + (D3DX_PI * 0.5f);
	}
	//����󂪉����ꂽ
	else if (GetKeyboardPress(DIK_DOWN) == true || GetPadPress(BUTTON_DOWN, 0) == true || GetPadLStickXPress(BUTTON_XSTICK, 0) > 0)
	{
		g_Model.move.x += cosf(pCamera->rot.y);
		g_Model.move.z += sinf(pCamera->rot.y);

		//�ړ������Ƀ��f����������
		g_Model.RotDest.y = -pCamera->rot.y + (-D3DX_PI * 0.5f);
	}

	DiffRot.y = g_Model.RotDest.y - g_Model.rot.y;

	//3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (DiffRot.y > D3DX_PI)
	{
		DiffRot.y -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2;
	}

	//Diff�ɕ␳�W����������
	g_Model.rot.y += DiffRot.y * 0.1f;

	//3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (g_Model.rot.y > D3DX_PI)
	{
		g_Model.rot.y -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (g_Model.rot.y < -D3DX_PI)
	{
		g_Model.rot.y += D3DX_PI * 2;
	}

	////�ړI�̋����ɃJ�����̃��b�g������
	//g_Model.RotDest.y = g_Model.rot.y;
	////�i�s�����Ɍ�����
	//g_Model.rot.y += (0.0f - g_Model.RotDest.y);

	//Enter�������ꂽ
	if (GetKeyboardPress(DIK_RETURN) == true || GetPadPress(BUTTON_START, 0) == true)
	{
		//���f���̈ʒu��������
		g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//���f���̌�����������
		g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//Space�L�[�������ꂽ
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetPadPress(BUTTON_RB, 0) == true)
	{
		SetBullet(g_Model.pos, D3DXVECTOR3(sinf(g_Model.rot.y) * -1.0f, 0.0f, cosf(g_Model.rot.y) * -1.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
	}

	//��ʂ̉����̐ݒ�
	if (g_Model.pos.x > WALL_RIMIT)
	{
		//�n�_(X���W)����ʂ̒[�ɓ�������
		g_Model.pos.x = WALL_RIMIT;
		g_Model.move.x = 0.0f;
	}
	else if (g_Model.pos.x < -WALL_RIMIT)
	{
		//�n�_(X���W)����ʂ̒[�ɓ�������
		g_Model.pos.x = -WALL_RIMIT;
		g_Model.move.x *= 0.0f;
	}

	//��ʂ̉��s�ݒ�
	if (g_Model.pos.z > WALL_RIMIT)
	{
		//�n�_(Y���W)����ʂ̒[�ɓ�������
		g_Model.pos.z = WALL_RIMIT;  //�n�_(Y���W)����ʂ̉��[�ɐݒ肷��
		g_Model.move.z = 0.0f;
	}
	else if (g_Model.pos.z < -WALL_RIMIT)
	{
		//�n�_(Y���W)����ʂ̒[�ɓ�������
		g_Model.pos.z = -WALL_RIMIT; 	    //�n�_(Y���W)����ʂ̉��[�ɐݒ肷��
		g_Model.move.z *= 0.0f;       //�ړ��������t�ɂ���
	}

	//�ʒu���X�V
	g_Model.pos.x += g_Model.move.x;
	g_Model.pos.z += g_Model.move.z;

	//�ړ��ʂ��X�V(����������)
	g_Model.move.x += (0.0f - g_Model.move.x) * MOVEFAST;
	g_Model.move.z += (0.0f - g_Model.move.z) * MOVEFAST;

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nldxShadow, g_Model.pos);
}