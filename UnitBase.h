/*===========================================================================================
�T�@���F�e�Q�[�����j�b�g�̐e�N���X
�쐬���F2015.10.19
�X�V���F2016.01.28
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include <typeinfo.h>
#include "Sprite.h"
#include "Audio.h"

static int testFlg = NULL;

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�e�Q�[�����j�b�g�̐e�N���X
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class UnitBase
{
protected:
	//���j�b�g�̈ʒu
	D3DXVECTOR3	position;

	//�����蔻��p(�l�p)
	RECT HitZone;

	int anime;			//�A�j���[�V�����̊�_
	int direction;		//�����Ă����

public:
	virtual ~UnitBase() { };
	//�ǂݍ��ݏ���
	virtual HRESULT Load();

	//�X�V����
	virtual HRESULT Update();

	//�Փ˔��菈��
	//�����FpTarget	���肷�鑊�胆�j�b�g
	virtual HRESULT Hit(UnitBase* pTarget);

	//�`�揈��
	//�����F����	�f�o�C�X�R���e�L�X�g�n���h��
	virtual HRESULT Render();

	//���݂̈ʒu���擾
	//�ߒl�F���݂̈ʒu
	D3DXVECTOR3 GetPos()	{ return position; }

	//������������g��Ȃ�����
	RECT GetZone()	{ return HitZone; }
};

