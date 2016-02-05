/*===========================================================================================
�T�@�v:�X�e�[�W�̐ݒ�
�쐬��:2015.10.21
�X�V��:2016.01.13
�����:��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include "UnitBase.h"
#include "Effect2D.h"
#include "Block.h"
#include "item.h"

//-------------------�萔�錾-----------------------
#define GROUND_HEIGHT		400		//�n�ʂ̍����i���n���鍂���j

#define STAGE_WIDTH		40		//��ʂ̍ő啝
#define STAGE_HEIGHT	45		//��ʂ̍ő卂��
/*�����Ƀu���b�N�T�C�Y��������*/

//���݂̃X�N���[���̈ʒu(PlayScene.cpp�Ő錾�ς�)
extern D3DXVECTOR3 g_stageScrollPosition;

class Stage : public UnitBase
{
	Sprite spt[3];			//�X�v���C�g�N���X�̃|�C���^�錾
	//Effect2D *fvx;		//�G�t�F�N�g2D�N���X�̃|�C���^�錾

	Audio *audio;

	BOOL Def = TRUE;		//BGM�̃��[�v�̂���

	Sprite	pict;			//�摜
	DWORD	table[STAGE_WIDTH][STAGE_HEIGHT];

public:
	Stage();
	~Stage();

	//----------------------------------
	//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
	//�����F�Ȃ�
	//�ߒl�F����or���s
	//----------------------------------
	HRESULT Load();

	//----------------------------------
	//�@�\�F�X�V����
	//�����F�Ȃ�
	//�ߒl�F����
	//----------------------------------
	HRESULT Update();

	//----------------------------------
	//�@�\�F�����蔻��
	//�����FpTarget		�G�̏��
	//�ߒl�F����
	//----------------------------------
	HRESULT Hit(UnitBase* pTarget);

	//----------------------------------
	//�@�\�F�`�揈��
	//�����F�Ȃ�
	//�ߒl�F����or���s
	//----------------------------------
	HRESULT Render();

	//�}�b�v�𐶐�
	void Create();
	int GetToComma(char data[], int* index);

	//--------------------�A�N�Z�X�֐�--------------
	int GetChip(int x, int y){ return table[x][y]; }
};