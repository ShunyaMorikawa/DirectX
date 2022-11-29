//
//
//�e�̔���[bullet.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _BULLET_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _BULLET_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
//�e�̎��
//=======================================
typedef enum
{
	BULLETTYPE_PLAYER = 0, //�v���C���[�̒e
	BULLETTYPE_ENEMY,      //�G�̒e
	BULLETTYPE_MAX
}BULLETTYPE;

//=======================================
//�e�\���̂̒�`
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;     //�ʒu
	D3DXVECTOR3 move;    //�ړ���
	int nLife;           //����
	BULLETTYPE type;     //���
	D3DXMATRIX g_mtxWorldBullet;	//���[���h�}�g���b�N�X
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���N�X�擾�p
	int BulletShadow;			//�Ώۂɉe�̃C���f�b�N�X(�ԍ�)
	bool bUse;           //�g�p���Ă��邩�ǂ���
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type);
Bullet *GetBullet(void);

#endif