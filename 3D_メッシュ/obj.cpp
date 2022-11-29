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

//=======================================
//�}�N����`
//=======================================
#define OBJECT			"data\\MODEL\\swing.x"

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPD3DXMESH g_pMeshObject = NULL;			//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatObject = NULL;		//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatObject = 0;					//�}�e���A���̐�
D3DXMATRIX g_mtxWorldObject;				//���[���h�}�g���b�N�X
LPDIRECT3DTEXTURE9 g_pTextureObject = NULL;	//�e�N�X�`���̃|�C���^
Object g_Object;						//���f���̏��

//=======================================
//�I�u�W�F�N�g�̏���������
//=======================================
void InitObject(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXMATERIAL *pMat;


	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃|�C���^
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

						//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���f���̈ʒu��������
	g_Object.pos = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);

	//���f���̌�����������
	g_Object.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Object.fDistance = 0.0f;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(OBJECT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject,
		NULL,
		&g_dwNumMatObject,
		&g_pMeshObject);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	//���_���̎擾
	nNumVtx = g_pMeshObject->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObject->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//���ׂĂ̒��_���r���ă��f���ŏ��l�E�ő�l�𔲂��o��

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshObject->UnlockVertexBuffer();
}

//=======================================
//�I�u�W�F�N�g�̏I������
//=======================================
void UninitObject(void)
{
	//���b�V���̔j��
	if (g_pMeshObject != NULL)
	{
		g_pMeshObject->Release();
		g_pMeshObject = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatObject != NULL)
	{
		g_pBuffMatObject->Release();
		g_pBuffMatObject = NULL;
	}
}

//=======================================
//�I�u�W�F�N�g�̍X�V����
//=======================================
void UpdateObject(void)
{

}

//=======================================
//�I�u�W�F�N�g�̕`�揈��
//=======================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldObject);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object.rot.y, g_Object.rot.x, g_Object.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldObject, &g_mtxWorldObject, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Object.pos.x, g_Object.pos.y, g_Object.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldObject, &g_mtxWorldObject, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObject);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject; nCntMat++)
	{
		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureObject); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
												 //���f��(�p�[�c)�̕`��
		g_pMeshObject->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=======================================
//�I�u�W�F�N�g�̎擾
//=======================================
Object *GetObj(void)
{
	return &g_Object;
}
