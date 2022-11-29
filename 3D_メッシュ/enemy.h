//
//
//���f������[model.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _ENEMY_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _ENEMY_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
//�G�\���̂̒�`
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXVECTOR3 rot;	//����
	float fDistance;	//����
	D3DXVECTOR3 RotDest;//�ړI�̌���
}Enemy;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);

#endif