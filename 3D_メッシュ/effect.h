//
//
//�G�t�F�N�g����[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EFECT_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _EFECT_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//===========================================
//�G�t�F�N�g�\���̂̒�`
//===========================================
typedef struct
{
	D3DXVECTOR3 pos;      //�ʒu
	D3DXVECTOR3 move;     //�ړ���
	D3DXVECTOR3 rot;      //����
	D3DXCOLOR col;        //�F
	float fRadius;        //���a(�傫��)
	int nLife;            //����(�\������)
	D3DXMATRIX g_mtxWorldEffectt;	//���[���h�}�g���b�N�X
	D3DXMATRIX mtxRot; 	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxView;		//�r���[�}�g���N�X�擾�p
	bool bUse;            //�g�p���Ă��邩�ǂ���
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife);

#endif