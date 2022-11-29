//
//
//���f������[model.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _MODEL_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _MODEL_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
//���f���\���̂̒�`
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXVECTOR3 rot;	//����
	float fDistance;	//����
	D3DXVECTOR3 RotDest;//�ړI�̌���
}Model;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);

#endif