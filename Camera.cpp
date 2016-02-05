/*===========================================================================================
�T�@���F�J�����i���_�j�̐ݒ�
�쐬���F2015.08.25
�X�V���F2015.10.13
����ҁF��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Camera.h"

//----------------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------------
Camera::Camera()
{
	pos = D3DXVECTOR3(0,5,-10);			//�J�����̈ʒu
	target = D3DXVECTOR3(0, 0, 0);		//�œ_�ʒu
	upVec = D3DXVECTOR3(0, 1, 0);		//������̃x�N�g��

	angle = 60;									//��p(���W�A��)
	aspect = (float)WINDOW_WIDTH/WINDOW_HEIGHT;	//�A�X�y�N�g��(��ʂ̕�������)
	nearClip = 0.1f;							//�߃N���b�s���O
	farClip = 1000.0f;							//���N���b�s���O
}

//----------------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------------
Camera::~Camera()
{
}

//----------------------------------------
//�@�\�F�r���[�s��i�J�����̈ʒu������j�����
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------------
HRESULT Camera::SetView()
{
	//�����͑S�ă|�C���^�œn���B�J�����̈ʒu�A�œ_�ʒu�A������x�N�g���̏�
	D3DXMatrixLookAtLH(&view, &pos, &target, &upVec);

	//���s�����烁�b�Z�[�W�{�b�N�X��\������
	if (FAILED(g_pDevice->SetTransform(D3DTS_VIEW, &view)))
	{
		MessageBox(0, "�r���[�s��̐ݒ�Ɏ��s���܂���", "Camera", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------------
//�@�\�F�ˉe�s��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------------
HRESULT Camera::SetProjection()
{
	//���W�A���͉~�����̂���,�~������3.14�̎����w���B
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(angle), aspect, nearClip, farClip);
	
	//���s�����烁�b�Z�[�W�{�b�N�X��\������
	if (FAILED(g_pDevice->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		MessageBox(0, "�ˉe�s��̐ݒ�Ɏ��s���܂���", "Camera", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------------
//�@�\�FSetView��SetProjection���Ă�
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------------
HRESULT Camera::Update()
{
	//���s������G���[��Ԃ�
	if (FAILED(SetView()))
	{
		return E_FAIL;
	}

	//���s������G���[��Ԃ�
	if (FAILED(SetProjection()))
	{
		return E_FAIL;
	}

	//�ǂ���������������A���_��ς��邱�Ƃ��o�����琬����Ԃ�
	return S_OK;
}