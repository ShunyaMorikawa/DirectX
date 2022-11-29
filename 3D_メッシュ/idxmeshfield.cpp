//
//
//�|���S������[polygon.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "idxmeshfield.h"

//=======================================
//�}�N����`
//=======================================
#define IdxMESHFIELD_TEX	"data\\TEXTURE\\block001.jpg"
#define VTX (3)

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureIdxMeshfield = NULL;        //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxIdxBuffMeshfield = NULL;   //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		 //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posIdxMeshfield;
D3DXVECTOR3 g_rotIdxMeshfield;
D3DXMATRIX g_mtxWorldIdxMeshfield;	//���[���h�}�g���b�N�X

//=======================================
//�C���f�b�N�X���b�V���t�B�[���h�̏���������
//=======================================
void InitIdxMeshfield(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_posIdxMeshfield = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);

	g_rotIdxMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, IdxMESHFIELD_TEX, &g_pTextureIdxMeshfield);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxIdxBuffMeshfield,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxIdxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < VTX; nCntZ++)
	{
		for (int nCntX = 0; nCntX < VTX; nCntX++)
		{
			//���_���W�̐ݒ�
			pVtx[nCntX + nCntZ * 3].pos = D3DXVECTOR3(nCntX * 1000.0f, 0.0f, nCntZ * -1000.0f);
		
			//�@���̐ݒ�
			pVtx[nCntX + nCntZ * 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntX + nCntZ * 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCntX + nCntZ * 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxIdxBuffMeshfield->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshField,
								NULL);

	//�C���f�b�N�X���ւ̃|�C���^
	WORD * pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//	//���_�ԍ��f�[�^�̐ݒ�
	//	for (int nCntIdx = 0; nCntIdx < 3; nCntIdx++)
	//	{
	//		pIdx[nCntIdx * 2] = 3 + nCntIdx * 1;
	//		pIdx[(nCntIdx * 2) + 1] = 0 + nCntIdx * 1;
	//	}

	//	//pIdx[0] = 3;
	//	//pIdx[1] = 0;

	//	//pIdx[2] = 4;
	//	//pIdx[3] = 1;

	//	//pIdx[4] = 5;
	//	//pIdx[5] = 2;

	//	for (int nCntIdx2 = 0; nCntIdx2 < 2; nCntIdx2++)
	//	{
	//		pIdx[8] = 6;
	//		pIdx[9] = 3;

	//		pIdx[10] = 7;
	//		pIdx[11] = 4;

	//		pIdx[12] = 8;
	//		pIdx[13] = 5;

	//	}

	////�k��
	//pIdx[6] = 2;
	//pIdx[7] = 6;

	//���_�ԍ��f�[�^�̐ݒ�
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	g_pIdxBuffMeshField->Unlock();
}
 
//=======================================
//�C���f�b�N�X���b�V���t�B�[���h�̏I������
//=======================================
void UninitIdxMeshfield(void)
{
	//�e�N�X�`���j��
	if (g_pTextureIdxMeshfield != NULL)
	{
		g_pTextureIdxMeshfield->Release();
		g_pTextureIdxMeshfield = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxIdxBuffMeshfield != NULL)
	{
		g_pVtxIdxBuffMeshfield->Release();
		g_pVtxIdxBuffMeshfield = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=======================================
//�C���f�b�N�X���b�V���t�B�[���h�̍X�V����
//=======================================
void UpdateIdxMeshfield(void)
{

}

//=======================================
//�C���f�b�N�X���b�V���t�B�[���h�̕`�揈��
//=======================================
void DrawIdxMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldIdxMeshfield);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotIdxMeshfield.y, g_rotIdxMeshfield.x, g_rotIdxMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldIdxMeshfield, &g_mtxWorldIdxMeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posIdxMeshfield.x, g_posIdxMeshfield.y, g_posIdxMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldIdxMeshfield, &g_mtxWorldIdxMeshfield, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldIdxMeshfield);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxIdxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureIdxMeshfield); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL

	//�|���S���̕`��								 
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,//�p�ӂ������_�̐�
		0,
		12);//�|���S���̐�

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
}