//
//
//��������[explosion.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EXPLOSION_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _EXPLOSION_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�v���g�^�C�v�錾
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif