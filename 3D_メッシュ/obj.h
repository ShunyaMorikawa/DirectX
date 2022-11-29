//
//
//���f������[model.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _OBJECT_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _OBJECT_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
//�I�u�W�F�N�g�\���̂̒�`
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXVECTOR3 rot;	//����
	float fDistance;	//����
	D3DXVECTOR3 RotDest;//�ړI�̌���
}Object;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
Object *GetObj(void);

#endif