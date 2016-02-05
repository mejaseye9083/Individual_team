/*===========================================================================================
�T�@���F�J�����i���_�j�̐ݒ�
�쐬���F2015.08.25
�X�V���F2015.08.28
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"

//--------------�J�����N���X�̒�`------------------
class Camera
{
	D3DXVECTOR3     pos;     //�J�����̈ʒu
	D3DXVECTOR3		target;  //�œ_�ʒu
	D3DXVECTOR3     upVec;   //�����

	float angle;			 //��p
	float aspect;			 //�A�X�y�N�g��
	float nearClip;			 //�߃N���b�s���O
	float farClip;			 //���N���b�s���O

	D3DXMATRIX   view;		 //�r���[�s��
	D3DXMATRIX   proj;		 //�ˉe�s��

	//----------------------------------------
	//�@�\�F�r���[�s��i�J�����̈ʒu������j�����
	//�����F�Ȃ�
	//�ߒl�F����or���s
	//----------------------------------------
	HRESULT SetView();

	//----------------------------------------
	//�@�\�F�ˉe�s��
	//�����F�Ȃ�
	//�ߒl�F����or���s
	//----------------------------------------
	HRESULT SetProjection();


public:
	Camera();
	~Camera();

	//----------------------------------------
	//�@�\�FSetView��SetProjection���Ă�
	//�����F�Ȃ�
	//�ߒl�F����or���s
	//----------------------------------------
	HRESULT Update();

	//----------�A�N�Z�X�֐�(�J����)----------
	void SetPos(D3DXVECTOR3 v){ pos = v; }			//�J�����̃|�W�V������n��
	void SetTarget(D3DXVECTOR3 v){ target = v; }	//�J�����̏œ_�ʒu��n��
	void SetUp(D3DXVECTOR3 v){ upVec = v; }			//�J�����̃x�N�g����n��

	//---------�A�N�Z�X�֐�(�ˉe�s��)---------
	void SetAngle(float f){ angle = f; }			//��e�i���W�A���j
	void SetAspect(float f){ aspect = f; }			//�A�X�y�N�g��
	void SetNearClip(float f){ nearClip = f; }		//�߃N���b�s���O
	void SetFarClip(float f){ farClip = f; }		//���N���b�s���O
};

//�p�[�X�e�N�e�B�u�Ƃ������t������A����͉��ߊ���\�����t�ł���B
//Unity�ł��g���錾�t�Ȃ̂ŁA��ł�������Â���o���邱�ƁB
