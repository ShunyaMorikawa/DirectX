//
//
//�ǂ̏���[wall.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef __WALL_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define __WALL_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
//�Ǎ\���̂̒�`
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;     //�ʒu
	D3DXVECTOR3 rot;	 //����
	int nType;
	D3DXMATRIX mtxWorldWall;	//���[���h�}�g���b�N�X
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���N�X�擾�p
	bool bUse;
}Wall;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitWall(void);
void UninitWall(void);
void UpdateWall (void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, float angle);

#endif