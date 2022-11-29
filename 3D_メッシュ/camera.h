//
//
//�J��������[camera.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _CAMERA_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _CAMERA_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;	//���_
	D3DXVECTOR3 posR;	//�����_
	D3DXVECTOR3 vecU;	//������x�N�g��
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 VDest;		//���_
	D3DXVECTOR3 RDest;		//�����_
	float fDistance;	//����
}Camera;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);

#endif