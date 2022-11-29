//
//
//�|���S������[polygon.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "wall.h"
//=======================================
//�}�N����`
//=======================================
#define POLYGON_TEX	"data\\TEXTURE\\grass000.png"

#define WALL_POSY		(200.0f)

#define MAX_WALL		(4)		//�ǂ̍ő吔

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;        //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;   //���_�o�b�t�@�ւ̃|�C���^
Wall g_Wall[MAX_WALL];
int g_nCntWall;		//�ǂ̐�
int g_NumWall;		//�ǂ̑���

//=======================================
//�ǂ̏���������
//=======================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Wall[nCntWall].nType = 0;

		g_Wall[nCntWall].bUse = false;
	}

	//�ǂ̐��̏�����
	g_nCntWall = 0;

	//�ǂ̑����̏�����
	g_NumWall = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEX, &g_pTextureWall);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntW = 0; nCntW < MAX_WALL; nCntW++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-1000.0f, 500.0f, 1000.0f);
		pVtx[1].pos = D3DXVECTOR3(1000.0f, 500.0f, 1000.0f);
		pVtx[2].pos = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);
		pVtx[3].pos = D3DXVECTOR3(1000.0f, 0.0f, 1000.0f);

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffWall->Unlock();
}

//=======================================
//�ǂ̏I������
//=======================================
void UninitWall(void)
{
	//�e�N�X�`���j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=======================================
//�ǂ̍X�V����
//=======================================
void UpdateWall(void)
{

}

//=======================================
//�ǂ̕`�揈��
//=======================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorldWall);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorldWall, &g_Wall[nCntWall].mtxWorldWall, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorldWall, &g_Wall[nCntWall].mtxWorldWall, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorldWall);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		g_nCntWall = 0;

		if (g_Wall[nCntWall].bUse == true)
		{
			g_nCntWall++;

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
	}
}

//=======================================
//�ǂ̐ݒ�
//=======================================
void SetWall(D3DXVECTOR3 pos, float angle)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bUse == false)
		{//�G���g�p����Ȃ�
			g_Wall[nCntWall].pos = pos;

			g_Wall[nCntWall].rot.y = angle;

			//���_���W���X�V
			pVtx[0].pos = D3DXVECTOR3(-1000.0f, 500.0f, 1000.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 500.0f, 1000.0f);
			pVtx[2].pos = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 0.0f, 1000.0f);

			//�g�p���Ă����Ԃɂ���
			g_Wall[nCntWall].bUse = true;

			//�ǂ̑��������Z����
			g_NumWall++;

			break;
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffWall->Unlock();
}