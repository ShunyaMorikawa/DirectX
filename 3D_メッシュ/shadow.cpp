//
//
//�e�̏���[shadow.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "shadow.h"		//�쐬����fade.h���C���N���[�h����

//=======================================
//�}�N����`
//=======================================
#define MAX_SHADOW	(256)							//�e�̍ő吔
#define SHADOW_TEX	"data\\TEXTURE\\shadow000.jpg"	//�e�̃e�N�X�`��

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;        //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;   //���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];

//=======================================
//�e�̏���������
//=======================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEX, &g_pTextureShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);		//�e�̈ʒu
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�e�̌���
		g_aShadow[nCntShadow].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-25.0f, 0.0f, 25.0f);
		pVtx[1].pos = D3DXVECTOR3(25.0f, 0.0f, 25.0f);
		pVtx[2].pos = D3DXVECTOR3(-25.0f, 0.0f, -25.0f);
		pVtx[3].pos = D3DXVECTOR3(25.0f, 0.0f, -25.0f);

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//=======================================
//�e�̏I������
//=======================================
void UninitShadow(void)
{
	//�e�N�X�`���j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=======================================
//�e�̍X�V����
//=======================================
void UpdateShadow(void)
{

}

//=======================================
//�e�̕`�揈��
//=======================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot;
	D3DXMATRIX	mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].g_mtxWorldShadow);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 
										g_aShadow[nCntShadow].rot.y, 
										g_aShadow[nCntShadow].rot.x, 
										g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].g_mtxWorldShadow, 
								&g_aShadow[nCntShadow].g_mtxWorldShadow, 
								&mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, 
								g_aShadow[nCntShadow].pos.x, 
								g_aShadow[nCntShadow].pos.y,
								g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].g_mtxWorldShadow, 
								&g_aShadow[nCntShadow].g_mtxWorldShadow, 
								&mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].g_mtxWorldShadow);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
		}
	}

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=======================================
//�e�̐ݒ菈��
//=======================================
int SetShadow(void)
{
	int nCntShadow = -1;

	for (int nCntS = 0; nCntS < MAX_SHADOW; nCntS++)
	{
		if (g_aShadow[nCntS].bUse == false)
		{//�g���ĂȂ�������
			g_aShadow[nCntS].bUse = true;

			nCntShadow = nCntS;

			break;
		}
	}
	
	return nCntShadow;
}

//=======================================
//�e�̈ʒu�ݒ菈��
//=======================================
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos)
{
	//�w�肵���e�̈ʒu��ݒ�
	g_aShadow[nldxShadow].pos.x = pos.x;

	g_aShadow[nldxShadow].pos.z = pos.z;
}

//=======================================
//�J�����̎擾
//=======================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}
