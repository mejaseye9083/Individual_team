/*===========================================================================================
�T�@�v:�G�̈ړ��E�U���p�^�[��
�쐬��:2015.11.05
�X�V��:2016.03.01
�����:��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"

//------------------�萔�錾------------------------
#define GRAVITY			1		//�d��
#define	INERTIA			0.06
#define	JUMP			-20
#define	JUMP_COUNT		1


Stage *Estg;

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Enemy::Enemy()
{
	position.x = 6*BLOCK_CHIP;
	position.y = 20*BLOCK_CHIP;

	jump = 0;
	jcount = 0;

	anime = 0;
	moveE = 7;

	direction = MS_RIGHT;
	spt = new Sprite;
	fvx = new Effect2D;
	asiba = GROUND_HEIGHT;

	//�����������l�����̂܂܏����Ă����͂Ȃ�
	FixedPosition.x = position.x;
	FixedPosition.y = position.y;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Enemy::~Enemy()
{
	//���S�ɏ�������
	SAFE_DELETE(spt);
	SAFE_DELETE(Estg);
	SAFE_DELETE(fvx);
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Enemy::Load()
{
	//�v���C���[�摜�̓ǂݍ���
	if (FAILED(spt->Load("pictures\\Actor.png")))
	{
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Enemy::Update()
{
	//�v���C���[���X�g�b�v���Ă�Ԃ͓G���X�g�b�v�I
	if (g_Stopflg == FALSE)
	{
		Move(Estg);
		if (state == LOOK_ENEMY)
		{
			Shot();
		}
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpTarget		�G�̏��
//�ߒl�F����
//----------------------------------
HRESULT Enemy::Hit(UnitBase* pTarget)
{
	//-----------PlayScene�ŌĂяo�����X�e�[�W��stg�ɓ����---------
	if (typeid(*pTarget) == typeid(Stage))
	{
		Estg = (Stage*)pTarget;
	}

	if (typeid(*pTarget) == typeid(Player))
	{
		D3DXVECTOR3 playerPosition = pTarget->GetPos();
		RECT playerSize = pTarget->GetZone();

		//�v���C���[�ƃG�l�~�[�Ԃ̋����𑪂�
		if (((HitZone.left - 50 <= playerSize.right) && (HitZone.right + 50 >= playerSize.left)) 
			&& ((HitZone.top - 60 <= playerSize.bottom ) && (HitZone.bottom + 30 >= playerSize.top)))
		{
			state = LOOK_ENEMY;
		}
		else 
		{
			state = DEFAULT;
		}
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Enemy::Render()
{
	//-----------------------�v���C���[�̕`��-----------------------------
	SpriteData enemy;
	enemy.cut.x = ((anime / 10) % 4) * 32;
	enemy.cut.y = direction * 32;

	enemy.size = D3DXVECTOR2(32, 32);


	enemy.pos = position + g_stageScrollPosition;

	spt->Draw(&enemy);
	//--------------------------------------------------------------------
	return S_OK;
}

//----------------------------------
//�@�\�F�ړ�����
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Enemy::Move(Stage* stage)
{
	//-----------�����蔻���Ǐ]������------------
	HitZone.left = 2 + position.x;
	HitZone.top = 0 + position.y;

	HitZone.right = 30 + position.x;
	HitZone.bottom = 32 + position.y;
	//---------------------------------------------

	//----------------------�G�l�~�[�̈ړ�------------------------------
	switch (state)
	{

		//---------------------�ʏ�ړ�----------------------
	case DEFAULT:
		
		//�ړ������悪�����̏����n�_+40�����ړ������甽�]������
		if (position.x >= FixedPosition.x + 100)
		{
			//�������t�]������
			moveE = -7;
			direction = MS_LEFT;
		}
		//�ړ������悪�����̏����n�_-40�����ړ����͕ǂɂԂ������甽�]������
		if (position.x <= FixedPosition.x - 100)
		{
			//�������t�]������
			moveE = 7;
			direction = MS_RIGHT;
		}

		//�ړ��͏�ɂ�����̂�if���̊O�ɏ����Ă���
		position.x += moveE;

		break;

	case LOOK_ENEMY:
		//--------------------------�W�����v����------------------------------
		if ((jcount < JUMP_COUNT && isGround) && !jumpBlock)
		{
			jump = JUMP;
			jcount++;
			isGround = FALSE;
			ladderflg = FALSE;
		}

		//--------------------------------------------------------------------

		break;
	}
	//--------------------------------------------------------------------


	//isGround��FALSE�������Ƃ�
	if (!isGround)
	{
		if (jump >= 10)
		{
			jump = 10;
		}
		position.y += jump;
		BMPIkun = (int)position.y;
	}

	//�v���C���[�̍������E���̈ʒu�ɃW�����v�̒l�𑫂����ʒu�̃e�[�u���̒l��1���������̏���
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + jump) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + jump) / BLOCK_CHIP) == 1
		&& !isGround)
	{
		//�|�W�V���� ��������l�� (1�t���[���O�̎����̃|�W�V������32�Ŋ������]��) + 1
		position.y -= (BMPIkun % BLOCK_CHIP) + 1;
		isGround = TRUE;
		jump = 0;
		jcount = 0;
	}
	//isGround��TRUE�������ꍇ�̓W�����v�������ɑ��ꂪ�Ȃ��Ȃ����͂�(1�͑���or�ǃu���b�N)
	//�W�����v�����ɗ�������ɓ���ꍇ�̏��������L�̏����ƂȂ�(���R����)
	else if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0
		&& !ladderflg)
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//�V��i�Ǝv����ꏊ�j�ɂԂ�������
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top + jump - 3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top + jump - 3) / BLOCK_CHIP) == 1)
	{
		jump = (jump * -1);
		jump += GRAVITY;
	}

	//-----------------------------------------

	
	return S_OK;
}

//----------------------------------
//�@�\�F�U������
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Enemy::Shot()
{

	return S_OK;
}

//----------------------------------
//�@�\�F�U�����󂯂ē|���ꂽ��
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
void Enemy::Kill()
{
	g_gameScene = SC_CLEAR;
}
