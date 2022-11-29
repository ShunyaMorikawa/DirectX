//
//
//�e�̏���[bullet.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //�쐬����main.h���C���N���[�h����
#include "bullet.h"      //�쐬����bullet.h���C���N���[�h����
#include "effect.h"
#include "shadow.h"
#include "explosion.h"

//=======================================
//�}�N����`
//=======================================
#define MAX_BULLET      (1024)   //�e�̍ő吔
#define BULLET_SIZE     (10)     //�e�̃T�C�Y
#define BULLET_TEX      "data\\Texture\\bullet.png"  //���@�̒e�̃e�N�X�`��
#define BULLET_SPEED    (4)      //�e�̑��x
#define EFFECTLIFE      (100)    //�G�t�F�N�g�̎���
#define BULLET_LIFE     (100)    //�e�̎���
#define NUM_BULLET      (2)      //�G�̒e�̃e�N�X�`��

#define SCREEN_DEPTH	(500)

//=======================================
//�O���[�o���ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;        //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;   //���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];                      //�e�̏��

//===========================================
//�e�̏���������
//===========================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 50;
		g_aBullet[nCntBullet].BulletShadow = -1;
		g_aBullet[nCntBullet].bUse = false;  //�g�p���ĂȂ���Ԃɂ���
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEX, &g_pTextureBullet);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBullet,
								NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(- BULLET_SIZE, - BULLET_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, - BULLET_SIZE, 0.0f);

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

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//===========================================
//�e�̏I������
//===========================================
void UninitBullet(void)
{
	//�e�N�X�`���j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===========================================
//�e�̍X�V����
//===========================================
void UpdateBullet(void)
{
	int nCntBullet;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���

			SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.6f), BULLET_SIZE, EFFECTLIFE);

			//�e�̈ʒu���X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_aBullet[nCntBullet].BulletShadow, g_aBullet[nCntBullet].pos);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, -BULLET_SIZE, 0.0f);

			//�����J�E���g�_�E��
			g_aBullet[nCntBullet].nLife--;

			//�������s����
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				//�g�p���ĂȂ���Ԃɂ���
				g_aBullet[nCntBullet].bUse = false;
			}
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//===========================================
//�e�̕`�揈��
//===========================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBullet[nCntBullet].g_mtxWorldBullet);

		//�r���[�}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &g_aBullet[nCntBullet].mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_aBullet[nCntBullet].g_mtxWorldBullet, NULL, &g_aBullet[nCntBullet].mtxView);	//�t�s������߂�
		g_aBullet[nCntBullet].g_mtxWorldBullet._41 = 0.0f;
		g_aBullet[nCntBullet].g_mtxWorldBullet._42 = 0.0f;
		g_aBullet[nCntBullet].g_mtxWorldBullet._43 = 0.0f;

		D3DXMatrixTranslation(&g_aBullet[nCntBullet].mtxTrans, 
								g_aBullet[nCntBullet].pos.x, 
								g_aBullet[nCntBullet].pos.y, 
								g_aBullet[nCntBullet].pos.z);

		D3DXMatrixMultiply(&g_aBullet[nCntBullet].g_mtxWorldBullet, 
							&g_aBullet[nCntBullet].g_mtxWorldBullet, 
							&g_aBullet[nCntBullet].mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].g_mtxWorldBullet);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureBullet);

		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p�����
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//===========================================
//�e�̐ݒ�
//===========================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ȃ�
			g_aBullet[nCntBullet].pos = pos;

			g_aBullet[nCntBullet].type = type;

			//�e�̑��x
			g_aBullet[nCntBullet].move = move * BULLET_SPEED;

			//�e�̎���
			g_aBullet[nCntBullet].nLife = BULLET_LIFE;

			//�g�p���Ă����Ԃɂ���
			g_aBullet[nCntBullet].bUse = true;

			//�e��ݒ�
			g_aBullet[nCntBullet].BulletShadow = SetShadow();

			break;
		}
	}
}

//===========================================
//�e�̎擾
//===========================================
Bullet *GetBullet(void)
{
	//�e�̏��̐擪�A�h���X��Ԃ�
	return &g_aBullet[0];
}