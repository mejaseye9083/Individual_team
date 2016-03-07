/*===========================================================================================
�T�@�v:���ōU������N���X
�쐬��:2016.02.24
�X�V��:2016.02.24
�����:��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include "Enemy.h"
#include "Effect2D.h"

//���݂̃X�N���[���̈ʒu(PlayScene.cpp�Ő錾�ς�)
extern D3DXVECTOR3 g_stageScrollPosition;

//--------------�o���b�g�N���X�̒�`----------------
class Bread
{
protected:
	Sprite *spt;
	Effect2D *fvx;
	//�ʒu
	D3DXVECTOR3 position;
	D3DXVECTOR3 enemyPosition = { NULL, NULL, NULL };

	//�e�̏�ԁ@�i�����˂��ǂ����j
	BOOL isbread;

	int breadlife;			//�e�����ɋ�Ƀt���O�������Ȃ��̂ňӐ}�I�ɏ������߂̕ϐ�
	int breadDamage = 4;	//�u���[�h�̃_���[�W��(�G�l�~�[��HP�������Ɋ���)

	int dirflg = NULL;
	//--------------�v���C���[�̌���------------------
	enum MOVE_SCENE
	{
		MS_DOWN,
		MS_LEFT,
		MS_RIGHT,
		MS_UP,
	};
	//------------------------------------------------

public:
	Bread();
	~Bread();

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
	HRESULT Update(D3DXVECTOR3 playerPos);
	
	//----------------------------------
	//�@�\�F�����蔻��
	//�����FpTarget		�G�̏��
	//�ߒl�F����
	//----------------------------------
	BOOL Hit(UnitBase* pTarget);	

	//----------------------------------
	//�@�\�F�`�揈��
	//�����F�Ȃ�
	//�ߒl�F����or���s
	//----------------------------------
	HRESULT Render();

	//�e�̔��ˏ���
	BOOL bread(D3DXVECTOR3 playerPos, int dir);

	void Reset();
};