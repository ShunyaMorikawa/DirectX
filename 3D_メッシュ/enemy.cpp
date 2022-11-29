//
//
//���f������[model.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "obj.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "enemy.h"

//=======================================
//�}�N����`
//=======================================
#define ENEMY			"data\\MODEL\\Excalibur.x"

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPD3DXMESH g_pMeshEnemy = NULL;			//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatEnemy = NULL;		//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatEenmy = 0;					//�}�e���A���̐�
D3DXMATRIX g_mtxWorldEnemy;				//���[���h�}�g���b�N�X
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;	//�e�N�X�`���̃|�C���^
Enemy g_Enemy;						//���f���̏��

//=======================================
//�G�l�~�[�̏���������
//=======================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXMATERIAL *pMat;


	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃|�C���^
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

						//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���f���̈ʒu��������
	g_Enemy.pos = D3DXVECTOR3(150.0f, 0.0f, 0.0f);

	//���f���̌�����������
	g_Enemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Enemy.fDistance = 0.0f;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMY,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy,
		NULL,
		&g_dwNumMatEenmy,
		&g_pMeshEnemy);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

	//���_���̎擾
	nNumVtx = g_pMeshEnemy->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//���ׂĂ̒��_���r���ă��f���ŏ��l�E�ő�l�𔲂��o��

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshEnemy->UnlockVertexBuffer();
}

//=======================================
//�G�l�~�[�̏I������
//=======================================
void UninitEnemy(void)
{
	//���b�V���̔j��
	if (g_pMeshEnemy != NULL)
	{
		g_pMeshEnemy->Release();
		g_pMeshEnemy = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatEnemy != NULL)
	{
		g_pBuffMatEnemy->Release();
		g_pBuffMatEnemy = NULL;
	}
}

//=======================================
//�G�l�~�[�̍X�V����
//=======================================
void UpdateEnemy(void)
{

}

//=======================================
//�G�l�~�[�̕`�揈��
//=======================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldEnemy);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy.rot.y, g_Enemy.rot.x, g_Enemy.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Enemy.pos.x, g_Enemy.pos.y, g_Enemy.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatEenmy; nCntMat++)
	{
		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEnemy); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
												  //���f��(�p�[�c)�̕`��
		g_pMeshEnemy->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=======================================
//�G�l�~�[�̎擾
//=======================================
Enemy *GetEnemy(void)
{
	return &g_Enemy;
}
