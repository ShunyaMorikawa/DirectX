//
//
//�G�t�F�N�g����[effect.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //�쐬����main.h���C���N���[�h����
#include "effect.h"      //�쐬����effect.h���C���N���[�h����

//===========================================
//�}�N����`
//===========================================
#define MAX_EFFECT    (4096)    //�G�t�F�N�g�̍ő吔
#define EFFECT_TEX    "data\\TEXTURE\\effect000.jpg"   //�����A�j���[�V�����̃e�N�X�`��

//===========================================
//�O���[�o���ϐ��錾
//===========================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;        //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;   //���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];                      //�G�t�F�N�g�̏��

//===========================================
//�G�t�F�N�g�̏���������
//===========================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX, &g_pTextureEffect);

	//�����̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     //�ʒu
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); //�F
		g_aEffect[nCntEffect].fRadius = 10;  //���a(�傫��)
		g_aEffect[nCntEffect].nLife = 10;	 //����(�\������)
		g_aEffect[nCntEffect].bUse = false;  //�g�p���ĂȂ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0�`1.0f�Őݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//===========================================
//�G�t�F�N�g�̏I������
//===========================================
void UninitEffect(void)
{
	//�e�N�X�`���j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//===========================================
//�G�t�F�N�g�̍X�V����
//===========================================
void UpdateEffect(void)
{
	int nCntEffect;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�������g�p����Ă���
		if (g_aEffect[nCntEffect].bUse == true)
		{
			g_aEffect[nCntEffect].col.a -= 0.01f;

			g_aEffect[nCntEffect].fRadius -= 0.5f;

			g_aEffect[nCntEffect].nLife--;

			//���_���W���X�V
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y + 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 20.0f, g_aEffect[nCntEffect].pos.y - 20.0f, 0.0f);

			if (g_aEffect[nCntEffect].nLife < 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

		}
		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//===========================================
//�G�t�F�N�g�̕`�揈��
//===========================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntE = 0; nCntE < MAX_EFFECT; nCntE++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aEffect[nCntE].g_mtxWorldEffectt);

		//�r���[�}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &g_aEffect[nCntE].mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_aEffect[nCntE].g_mtxWorldEffectt, NULL, &g_aEffect[nCntE].mtxView);	//�t�s������߂�
		g_aEffect[nCntE].g_mtxWorldEffectt._41 = 0.0f;
		g_aEffect[nCntE].g_mtxWorldEffectt._42 = 0.0f;
		g_aEffect[nCntE].g_mtxWorldEffectt._43 = 0.0f;

		D3DXMatrixTranslation(&g_aEffect[nCntE].mtxTrans,
			g_aEffect[nCntE].pos.x,
			g_aEffect[nCntE].pos.y,
			g_aEffect[nCntE].pos.z);

		D3DXMatrixMultiply(&g_aEffect[nCntE].g_mtxWorldEffectt,
			&g_aEffect[nCntE].g_mtxWorldEffectt,
			&g_aEffect[nCntE].mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntE].g_mtxWorldEffectt);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		if (g_aEffect[nCntE].bUse == true)
		{//�G�t�F�N�g���g�p�����
		 //�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntE * 4, 2);
		}
		
		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	}

}

//===========================================
//�G�t�F�N�g�̐ݒ�
//===========================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;         //�ʒu
			g_aEffect[nCntEffect].move = move;       //�ړ���
			g_aEffect[nCntEffect].col = col;         //�F
			g_aEffect[nCntEffect].fRadius = fRadius; //���a(�傫��)
			g_aEffect[nCntEffect].nLife = nLife;     //����(�\������)
			g_aEffect[nCntEffect].bUse = true;       //�g�p���Ă��邩�ǂ���

			break;
		}
	}
}