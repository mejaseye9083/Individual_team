/*=======================================================================================================
�T�@���F�l�p���|���S������邽�߂̊֐������
�쐬���F2015.09.07
�X�V���F2015.09.08
����ҁF��������
=======================================================================================================*/

//---------------�C���N���[�h-----------------------
#include "BasicMesh.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
BasicMesh::BasicMesh()
{
	pMesh = 0;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
BasicMesh::~BasicMesh()
{
	SAFE_RELEASE(pMesh);
}

//----------------------------------
//�@�\�F���b�V�����쐬����֐�
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT BasicMesh::Create(DWORD type)
{
	/*------�����̂̃��b�V�����쐬����----------
	if (FAILED(D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL)))
	------------------------------------------*/


	/*------------�X�t�B�A�i���j�����----------
	�������͔��a�A���ʂ��猩���ʂ̐��A�ォ�猩���������A�ƂȂ�B
	������ݒ肵�ċ��̂̊��炩�������
	if (FAILED(D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL)))
	------------------------------------------*/

	/*---------------�~�������-----------------
	if (FAILED(D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 2, &pMesh, NULL)))
	------------------------------------------*/


	/*--------�g�[���X�i�h�[�i�c�^�j�����------
	�������̓h�[�i�c�̑����A�~�̑傫���A�����猩���������A���ʂ��猩��������
	if (FAILED(D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL)))
	------------------------------------------*/
	
	//�I�}�P�BDirectX�ɂ͌��̓������A�p�x�A�e����������֐������݂���
	//���ꂪ�e�B�[�|�b�g�ł���B
	//if (FAILED(D3DXCreateTeapot(g_pDevice, &pMesh, NULL)))
	//{
	//	MessageBox(0, "���b�V���̍쐬�Ɏ��s���܂���", "BasicMesh", MB_OK);
	//	return E_FAIL;
	//}

	//�G���[�����p�A��ɂ܂Ƃ߂鎖�ŗ]�v�Ȏ�Ԃ��Ȃ���
	HRESULT hr;


	//�e���f�����l�ߍ��킹���X�C�b�`��
	switch (type)
	{
	case BMESH_BOX:
		hr = D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL);
		break;

	case BMESH_SPHERE:
		hr = D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL);
		break;

	case BMESH_CYLINDER:
		hr = D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 2, &pMesh, NULL);
		break;

	case BMESH_TORUS:
		hr = D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL);
		break;

	case BMESH_TEAPOT:
		hr = D3DXCreateTeapot(g_pDevice, &pMesh, NULL);
		break;
	}

	//�ǂꂪ���s���Ă�hr�ɂ܂Ƃ܂��Ă���̂Ŋm�F���y
	if (FAILED(hr))
	{
		MessageBox(0, "���b�V���̍쐬�Ɏ��s���܂���", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//���̂ق��ō����InitMaterial���Ăяo���B�i�ĂіY��h�~�j
	InitMaterial();

	return S_OK;
}

//----------------------------------
//�@�\�F
//�����F���[���h���W
//�ߒl�F����or���s
//----------------------------------
HRESULT BasicMesh::Draw(D3DXMATRIX* matWorld)
{
	//���C���N���X�Ɠ��l�ɁA�e�N�X�`�����\����ƍ���̂œ\��Ȃ��悤�ɂ���B
	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		MessageBox(0, "�e�N�X�`���̃��Z�b�g�Ɏ��s���܂���", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//��������ʓr���[���h�s�񂪕K�v�ƂȂ�A���e�I�ɂ�Quad�Ɠ���
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//������ݒ肷��
	//���𔽎˂��Ȃ����͔̂F�����邱�Ƃ��o���Ȃ��̂ŁA������ݒ肷��
	if (FAILED(g_pDevice->SetMaterial(&material)))
	{
		MessageBox(0, "�}�e���A���̐ݒ�Ɏ��s���܂���", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//��L�ō쐬���������̃��b�V����`�悷��
	if (FAILED(pMesh->DrawSubset(0)))
	{
		MessageBox(0, "���b�V���̕`��Ɏ��s���܂���", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//�@�\�F�}�e���A���̐ݒ�(���������)
//�����F�Ȃ�
//�ߒl�F����or���s
//�ǋL�F�ĂіY���ƍ���̂ŁA��L�ɂ���Create�ŌĂяo���悤�ɂ���
//----------------------------------
HRESULT BasicMesh::InitMaterial()
{
	//�w�b�_�[�Ő錾�����}�e���A���\���̂�����������B
	ZeroMemory(&material, sizeof(D3DMATERIAL9));

	//�S��1.0f�ɂ���Ɛ^�����ɂȂ�
	//Diffuse�Ƃ͊g�U���˂̎����w���B�i�ڂɌ�������̐F�̂��Ɓj
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;
	/*
	�ь�͍g���A����͗ь炪�Ԃ̌����g�U���˂��邩��ڂɌ�����Ƃ��ɂ͍g��������B
	�܂蕨�̂̐F�Ɗg�U���˂����F�͎����悤�Ȃ��̂��Ƃ�����
	*/

	return S_OK;
}