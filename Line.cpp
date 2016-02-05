/*===========================================================================================
�T�@���F�`��֘A�̍쐬(�J�����̌����Ă�����̃`�F�b�N��)
�쐬���F2015.08.31
�X�V���F2015.09.02
����ҁF��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Line.h"

//----------------------------------
//�@�\�F�`�揈��(�J���[����)
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Line::Draw(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	//���_�o�b�t�@
	D3DXVECTOR3 point[2];		//�n�_�ƏI�_�̔z��
	point[0] = start;			//�n�_
	point[1] = end;				//�I�_

	//���_�X�g���[���i�e���_���ǂ̂悤�ȏ��������Ă邩�j��ݒ肷��
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ)))				//D3DFVF_XYZ�͈ʒu����\��
	{
		//���s�����烁�b�Z�[�W�{�b�N�X��\������
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	//�P�ʍs��i�������Ȃ��s��j
	D3DXMATRIX mReset;
	//�����Őݒ肵�Ă������Ƃő��̍s��Ɋ������܂�Ȃ��Ȃ�
	D3DXMatrixIdentity(&mReset);
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, &mReset)))
	{
		MessageBox(0, "���[���h�s��̃��Z�b�g�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		MessageBox(0, "�e�N�X�`���̃��Z�b�g�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	//���C���������Ȃ�̂ł���̑΍�
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	//�`�悷��iDrawPrimitiveUP�j
	//�����́u��ށv�u�\�����鐔�v�u���_���v�u1���_�̃f�[�^�T�C�Y�v
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, point, sizeof(D3DXVECTOR3))))
	{
		MessageBox(0, "���C���̕`��Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	//�ŏ��Ƀ��C�g�������Ă����΃��C�����`�悳���ۂɂ͌��̉e�����󂯂Ȃ�
	//���̌�A���C�g��t����΃��C�����`�悳�ꂽ��Ȃ̂ŉ��̖����Ȃ�
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��(�J���[�t��)
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Line::Draw(D3DXVECTOR3 start, D3DXVECTOR3 end, DWORD color)
{
	//D3DXVECTOR3����F�܂ł͓���Ȃ��A�����łЂƂ̍\���̂ɂ���
	struct Vertex
	{
		D3DXVECTOR3 pos;
		DWORD color;
	};

	//�n�_�ƏI�_�Ȃ̂œ�K�v
	Vertex point[2];

	//�e���_�̏����Z�b�g����i�����Ŏ󂯎�������̂��j
	point[0].pos = start;			//�n�_�̈ʒu
	point[0].color = color;
	point[1].pos = end;				//�I�_�̈ʒu
	point[1].color = color;
	/*�ŏI�I�ɏ�L�̕������}���ō�������f����K��������*/

	//���_�X�g���[���i�e���_���ǂ̂悤�ȏ��������Ă邩�j��ݒ肷��
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE)))
	{
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	//�P�ʍs��i�������Ȃ��s��j
	D3DXMATRIX mReset;
	//�����Őݒ肵�Ă������Ƃő��̍s��Ɋ������܂�Ȃ��Ȃ�
	D3DXMatrixIdentity(&mReset);
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, &mReset)))
	{
		MessageBox(0, "���[���h�s��̃��Z�b�g�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	//�`�悷��iDrawPrimitiveUP�j
	//�����́u��ށv�u�\�����鐔�v�u���_���v�u�\���̂̃f�[�^�T�C�Y�v
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, point, sizeof(Vertex))))
	{
		MessageBox(0, "���C���̕`��Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

