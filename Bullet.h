/*===========================================================================================
�T�@�v:�e�̔��˂����Ƃ��̋O��
�쐬��:2015.06.26
�X�V��:2016.02.15
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
class Bullet
{
protected:
	Sprite *spt;
	Effect2D *fvx;
	//�ʒu
	D3DXVECTOR3 position;
	D3DXVECTOR3 enemyPosition = {NULL,NULL,NULL};

	//�e�̏�ԁ@�i�����˂��ǂ����j
	BOOL isShot;
	BOOL unControl;

	BOOL SpecialOrNormal;

	int bulletlife;		//�e�����ɋ�Ƀt���O�������Ȃ��̂ňӐ}�I�ɏ������߂̕ϐ�

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
	Bullet();
	~Bullet();

	HRESULT Load();
	
	//�X�V
	//HRESULT Update();

	//�X�V
	HRESULT Update();
	//�Փ˔���
	BOOL Hit(UnitBase* pTarget);

	//�`�揈��
	//����HDC �f�o�C�X�R���e�L�X�g�n���h��
	//�߂�l:�Ȃ�
	HRESULT Render();

	//�e�̔��ˏ���
	BOOL Shot(D3DXVECTOR3 playerPos,int dir);

	BOOL SpecialA(D3DXVECTOR3 playerPos, int dir);

	void Reset();
};