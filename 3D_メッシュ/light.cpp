//
//
//���C�g����[light.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "light.h"    //�쐬����fade.h���C���N���[�h����

//=======================================
//�}�N����`
//=======================================
#define MAX_LIGHT		(3)		//���C�g�̍ő吔

//=======================================
//�O���[�o���ϐ��錾
//=======================================
D3DLIGHT9 g_alight[MAX_LIGHT];	//���C�g�̏��

//=======================================
//���C�g�̏���������
//=======================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];	//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
		g_alight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

		//���C�g�̕�����ݒ�
		vecDir[0] = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
		vecDir[1] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
		vecDir[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

		//�x�N�g���𐳋K������
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);

		g_alight[nCntLight].Direction = vecDir[nCntLight];

		//���C�g��ݒ�
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=======================================
//���C�g�̏I������
//=======================================
void UninitLight(void)
{

}

//=======================================
//���C�g�̍X�V����
//=======================================
void UpdateLight(void)
{

}