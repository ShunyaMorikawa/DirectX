//
//
//�|���S������[polygon.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "billboard.h"    //�쐬����fade.h���C���N���[�h����

//=======================================
//�}�N����`
//=======================================
#define POLYGON_TEX	"data\\TEXTURE\\face.png"

#define BILL_WIDTH		(20)	//�r���{�[�h�̕�
#define BILL_HEIGHT		(20)	//�r���{�[�h�̍���

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureBillBoard = NULL;        //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillBoard = NULL;   //���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posBillBoard;		//�ʒu
D3DXMATRIX g_mtxWorldBillBoard;	//���[���h�}�g���b�N�X

//=======================================
//�r���{�[�h�̏���������
//=======================================
void InitBillBoard(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�r���{�[�h�̈ʒu��������
	g_posBillBoard = D3DXVECTOR3(50.0f, 20.0f, 50.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEX, &g_pTextureBillBoard);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillBoard,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillBoard->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(- BILL_WIDTH, BILL_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILL_WIDTH, BILL_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- BILL_WIDTH, - BILL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILL_WIDTH, - BILL_HEIGHT, 0.0f);

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

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillBoard->Unlock();
}

//=======================================
//�r���{�[�h�̏I������
//=======================================
void UninitBillBoard(void)
{
	//�e�N�X�`���j��
	if (g_pTextureBillBoard != NULL)
	{
		g_pTextureBillBoard->Release();
		g_pTextureBillBoard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillBoard != NULL)
	{
		g_pVtxBuffBillBoard->Release();
		g_pVtxBuffBillBoard = NULL;
	}
}

//=======================================
//�r���{�[�h�̍X�V����
//=======================================
void UpdateBillBoard(void)
{

}

//=======================================
//�r���{�[�h�̕`�揈��
//=======================================
void DrawBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���N�X�擾�p

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	////�A���t�@�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	////Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBillBoard);

	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&g_mtxWorldBillBoard, NULL, &mtxView);	//�t�s������߂�
	g_mtxWorldBillBoard._41 = 0.0f;
	g_mtxWorldBillBoard._42 = 0.0f;
	g_mtxWorldBillBoard._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posBillBoard.x, g_posBillBoard.y, g_posBillBoard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillBoard, &g_mtxWorldBillBoard, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillBoard);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBillBoard, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBillBoard); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL

	////�A���t�@�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////z�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}