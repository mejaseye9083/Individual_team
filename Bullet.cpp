/*===========================================================================================
�T�@�v:�e�̔��˂����Ƃ��̋O��
�쐬��:2015.06.26
�X�V��:2016.02.15
�����:��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Bullet.h"
#include "Camera.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Bullet::Bullet()
{
	//�����˒i�K
	isShot = FALSE;

	bulletlife = NULL;

	//�X�v���C�g�A2D�G�t�F�N�g�A�I�[�f�B�I�N���X�̐錾
	spt = new Sprite;
	fvx = new Effect2D;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Bullet::~Bullet()
{

	//�|�C���^�̃f���[�g�����A�ŗD��ŏ���
	//new���������炷���������炢�Ɉӎ�����
	SAFE_DELETE(spt);
	SAFE_DELETE(fvx);
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Bullet::Load()
{
	//�e��摜�A�G�t�F�N�g�A���y�ǂݍ���
	if (FAILED(spt->Load("pictures\\RockB.png")))
	{
		return E_FAIL;
	}

	if (FAILED(fvx->Load("SamplePict\\bakuhatu.png", D3DXVECTOR2(128, 128), 0.2f)))
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
HRESULT Bullet::Update()
{
	//�E�����̎��͉E�ɔ�΂����ςȂ��ɂ�����
	if (dirflg == MS_RIGHT)
	{
		//�����ł��̒e�������̉e�����󂯂Ȃ��悤�ɂ�����
		unControl = TRUE;
		if (unControl == TRUE)
		{
			//�e�̔�ԑ��x
			position.x += 8;
			//�e�̏����ʒu
			//if (position.x > 640)

			//{
			//	//�e�𖢔��˂ɖ߂�
			//	isShot = FALSE;
			//	//�t���O���������čēx���͂�҂�
			//	unControl = FALSE;
			//}
			bulletlife++;

		}
	}

	//�������l�ɔ�΂����ςȂ��ɂ���I
	if (dirflg == MS_LEFT)
	{
		//�����ł��̒e�������̉e�����󂯂Ȃ��悤�ɂ�����
		unControl = TRUE;
		if (unControl == TRUE)
		{
			//�e�̔�ԑ��x
			position.x -= 8;
			//�e�̏����ʒu
			//if (position.x < -40)

			//{
			//	//�e�𖢔��˂ɖ߂�
			//	isShot = FALSE;
			//	//�t���O���������čēx���͂�҂�
			//	unControl = FALSE;
			//}

			bulletlife++;

		}
	}
	if (bulletlife == 0xF0)
	{
 	 	isShot = FALSE;
		unControl = FALSE;
		bulletlife = NULL;

	}
	//�s��ł��������ǂƂ肠����ok
	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpEnemy		�G�̈ʒu���
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Bullet::Hit(Enemy* pEnemy)
{
	if (isShot == TRUE)
	{
		//�G�̈ʒu��GetPos��ʂ��Ď󂯎��
		D3DXVECTOR3 enemyPosition = pEnemy->GetPos();
		//�ʒu�̌v�Z���s��
		int distance =
			(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) *
			(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) +
			(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y)) *
			(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y));			//(0-0)*(0-0)+(0-0)*(0-0) =((0-0)*(0-0)) + ((0-0)*(0-0)) 

		if (distance <= (24 * 24))
		{
			//�e������������e������
			isShot = FALSE;
			bulletlife = NULL;
			unControl = FALSE;

			fvx->Draw();
			pEnemy->Kill();
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Bullet::Render()
{
	SpriteData data;
	//�e�̕`����s��
	if (isShot == TRUE)
	{
		data.pos = position;
 		spt->Draw(&data);
	}

	fvx->Draw();
	return S_OK;
}

//----------------------------------
//�@�\�F�e����ԏ���
//�����F�Ȃ�
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Bullet::Shot(D3DXVECTOR3 playerPos, int dir)
{
	//�����˂̂Ƃ�
	if (isShot == FALSE)
	{
		isShot = TRUE;
		dirflg = dir;

		//�����̈ʒu = �n���ꂽ�v���C���[�̈ʒu
		switch (dir)
		{
		case MS_RIGHT:
			position.x = playerPos.x +10;
			break;

		case MS_LEFT:
			position.x = playerPos.x -10;
			break;

		default:
			break;
		}
		position.y = playerPos.y+16;

		return TRUE;
	}
	return FALSE;
}

/*-----------------------------���_------------------------
����|�W�V�������󂯎���Ă���ʒu����o���b�g����Ԃ悤�ɂȂ��Ă���
�܂�J�������ړ����Ă�Ƃ��ɒ����ɃL�����𑨂��Ă���ۂɒǏ]���ď�ɒ���
���̂܂܂��ƃo���b�g���L�����Ɠ������������Ȃ̂ł������s���R
------------------------------------------------------------*/