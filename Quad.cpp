/*=======================================================================================================
�T�@���F�l�p���|���S������邽�߂̊֐������
�쐬���F2015.09.01
�X�V���F2015.09.07
����ҁF��������
=======================================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Quad.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Quad::Quad()
{
	pTexture = 0;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Quad::~Quad()
{
	//-----LP~�Ƃ��Ă�֐��͕K���������������
	SAFE_RELEASE(pTexture);
}

//----------------------------------
//�@�\�F�摜�f�[�^�̓ǂݍ���
//�����FfileName	�摜�t�@�C���̖��O
//�ߒl�F����or���s
//----------------------------------
HRESULT Quad::Load(char* fileName)
{
	//�u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		NULL, NULL, NULL, &pTexture)))
	{
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", fileName, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


//----------------------------------
//�@�\�F�O�p�`���g�ݍ��킹�Ďl�p�����
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Quad::Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex)
{
	//D3DXVECTOR3����F�܂ł͓���Ȃ��A�����łЂƂ̍\���̂ɂ���
	struct Vertex
	{
		D3DXVECTOR3 pos;
		DWORD color;
		D3DXVECTOR2    uv;
	};

	//�O�p�`��Ŏl�p�`�����
	Vertex point[] =
	{
		//��ڂ̎O�p�`
		D3DXVECTOR3(-0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 0),	//����
		D3DXVECTOR3(0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 0),		//�E��
		D3DXVECTOR3(-0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 1),	//����
		//��ڂ̎O�p�`
		//D3DXVECTOR3(-0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255),		//����
		//D3DXVECTOR3(0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255),			//�E��
		D3DXVECTOR3(0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 1),	//�E��
	};

	/*
	//�O�p�`���ŏ��ɍ��̂Œ��_�͎O�K�v
	Vertex point[3] =
	{
		//�e���_�̏����Z�b�g����
		D3DXVECTOR3(-2, -1, 0), D3DCOLOR_ARGB(255, 255, 0, 0),
		D3DXVECTOR3(0, 1, 2), D3DCOLOR_ARGB(255, 0, 255, 0),
		D3DXVECTOR3(2, -1, 0), D3DCOLOR_ARGB(255, 0, 0, 255),
	};
	*/

	//------------------------------------------------------------------------
	//�ʏ�ݒ�̏ꍇ�A���v���̏��ԂŐݒ肵���炻�ꂪ�\�ƂȂ�
	//��̒��_���������ꍇ�A���̂Ƃ��t���v���ɂȂ�̂ŗ����ɂȂ�
	//�����͊�{�I�Ɍ��邱�Ƃ͂Ȃ��̂��A�J�����̈ʒu��ς���΋t�O�p�`�Ɍ�����
	//------------------------------------------------------------------------

	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}


	//���_�X�g���[���i�e���_���ǂ̂悤�ȏ��������Ă邩�j��ݒ肷��
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)))
	{
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	//�e�N�X�`�����Z�b�g����֐�
	if (FAILED(g_pDevice->SetTexture(0, pTexture)))
	{
		MessageBox(0, "�e�N�X�`���̃Z�b�g�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	if (matTex != NULL)
	{
		//�e�N�X�`���s����Z�b�g����֐�
		if (FAILED(g_pDevice->SetTransform(D3DTS_TEXTURE0, matTex)))
		{
			MessageBox(0, "�e�N�X�`���s��̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	//�`�悷��iDrawPrimitiveUP�j�v���~�e�B�u�̕`��Ƃ�����
	//�����́u��ށv�u�\�����鐔�v�u���_���v�u�\���̂̃f�[�^�T�C�Y�v
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(Vertex))))
	{
		MessageBox(0, "�|���S���̕`��Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}
	//-----------------------------------------------------
	//Z�̌`�ɒ��_���w�肵�Ă��l�p�`������
	//D3DPT_TRIANGLESTRIP���g�����ł��܂����ƕ\�����Ă����
	//-----------------------------------------------------

	//���C���̎��Ɠ����A�|���S���\���̍ۂɍ����Ȃ�Ȃ��悤�Ƀ��C�g��؂�
	//�`�悪���ꂽ�烉�C�g���ēx�t����悤�ɂ���
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}