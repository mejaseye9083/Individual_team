/*===========================================================================================
�T�@�v:���@�̐ݒ�
�쐬��:2015.10.15
�X�V��:2016.02.15
�����:��������
=============================================================================================*/

#pragma once
//---------------�C���N���[�h-----------------------
#include "Global.h"
#include "UnitBase.h"
#include "Effect2D.h"
#include "Stage.h"
#include "Bullet.h"
#include "Bread.h"
//���݂̃X�N���[���̈ʒu(PlayScene.cpp�Ő錾�ς�)
extern D3DXVECTOR3 g_stageScrollPosition;

extern BOOL g_Stopflg;

#define PLAYER_DM 8			//1������8�Ȃ̂ŁA�����������炷�ۂɎg���B�_���[�W�Ɠ��ꕐ��B
#define PLAYER_HP 20		//���b�N�}���̗̑́A���C�t�A�b�v�Ƃ��͂܂��������Ȃ�
#define PLAYER_SP 20		//HP�Ɠ������l�����ǈꉞ��`

class Player : public UnitBase
{
	Sprite *ply;			//�X�v���C�g�N���X�̃|�C���^�錾
	Sprite *life;			//���C�t�������\���p�X�v���C�g�N���X
	Sprite *special;

	//Effect2D *fvx;		//�G�t�F�N�g2D�N���X�̃|�C���^�錾
	Audio *audio;

	int anime;				//�A�j���[�V�����̊�_
	int direction;			//�����Ă����
	float asiba;

	int hp = PLAYER_HP;		//���b�N�}���̗̑�
	int sp = PLAYER_SP;		//���ꕐ��̎c��
	int BMPIkun = 0;		//�o�O�錴����}����ԕ��N

	int invincibleTime;

	int memory_break_hp = 0;
	int memory_break_sp = 0;

	BOOL ladderflg = FALSE;
	BOOL jumpBlock = FALSE;
	BOOL isGround = FALSE;


	//--------------�J�����̈ʒu���------------------
	enum
	{
		DEFAULT,		//�ʏ�
		DOWN_SCROLL,	//���փX�N���[����
		UP_SCROLL,		//��փX�N���[����
		LEFT_SCROLL,
		RIGHT_SCROLL,
	}state;
	//------------------------------------------------

	//--------------�v���C���[�̏��------------------
	enum
	{
		PEASE,
		DAMAGE,
	}bonny;
	//------------------------------------------------

	//--------------�v���C���[�̌���------------------
	enum MOVE_SCENE
	{
		MS_DOWN,
		MS_LEFT,
		MS_RIGHT,
		MS_UP,
	};
	//------------------------------------------------

	int jump;			//�W�����v���鎞�ɑ������ϐ�
	int jcount;			//�A���W�����v�}��

	HRESULT Move(Stage* stage);		//�ړ�
	HRESULT Shot();					//�U��
	HRESULT Jump();

	Bullet bullet[BULLET_SET];		//���˂ł���e�̒i��
	Bread bread;

	BOOL isShotKeyFlg = FALSE;		//���˂������ǂ����̔���
	BOOL isSpecialFlg = FALSE;

	void BulletUpdate();	//�e�̍X�V

public:
	Player();
	~Player();

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

	void Reset();
	void Invincible();
};

