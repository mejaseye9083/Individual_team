/*===========================================================================================
�T�@�v:���ōU������N���X
�쐬��:2016.02.24
�X�V��:2016.02.24
�����:��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Bread.h"
#include "Camera.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Bread::Bread()
{
	//�����˒i�K
	isbread = FALSE;

	breadlife = NULL;

	//�X�v���C�g�A2D�G�t�F�N�g�A�I�[�f�B�I�N���X�̐錾
	spt = new Sprite;
	fvx = new Effect2D;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Bread::~Bread()
{

	//�|�C���^�̃f���[�g�����A�ŗD��ŏ���
	//new���������炷���������炢�Ɉӎ�����
	SAFE_DELETE(spt);
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Bread::Load()
{
	//�e��摜�A�G�t�F�N�g�A���y�ǂݍ���
	if (FAILED(spt->Load("pictures\\bread.png")))
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
HRESULT Bread::Update(D3DXVECTOR3 playerPos)
{

	if (isbread == TRUE)
	{	//�����̈ʒu = �n���ꂽ�v���C���[�̈ʒu
		switch (dirflg)
		{
		case MS_RIGHT:
			position.x = playerPos.x + 25;
			break;

		case MS_LEFT:
			position.x = playerPos.x - 25;
			break;
		}
		position.y = playerPos.y - 8;
		breadlife++;
	}

	//�e�̐�������:4�b��
	if (breadlife == 0xF)
	{
		Reset();
	}

	//�s��ł��������ǂƂ肠����ok
	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpTarget		�G�̈ʒu���
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Bread::Hit(UnitBase *pTarget)
{
	if (typeid(*pTarget) == typeid(Enemy))
	{
		Enemy *pEnemy = (Enemy*)pTarget;
		if (isbread == TRUE)
		{
			//�G�̈ʒu��GetPos��ʂ��Ď󂯎��
			enemyPosition = pEnemy->GetPos();
			//�ʒu�̌v�Z���s��
			int distance =
				(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) *
				(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) +
				(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y)) *
				(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y));			//(0-0)*(0-0)+(0-0)*(0-0) =((0-0)*(0-0)) + ((0-0)*(0-0)) 

			if (distance <= (28 * 48))
			{
				//�e������������e������
				Reset();
				pEnemy->Kill();
				return TRUE;
			}
		}
	}

	return FALSE;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Bread::Render()
{
	SpriteData data;
	//�e�̕`����s��
	if (isbread == TRUE)
	{
		data.pos = position;

		data.size = D3DXVECTOR2(28, 48);

		switch (dirflg)
		{
		case MS_RIGHT:
			data.cut.x = 0*28;
			break;

		case MS_LEFT:
			data.cut.x = 1 * 28;
		}

		spt->Draw(&data);
	}
	return S_OK;
}

//----------------------------------
//�@�\�F���Ő؂铮��
//�����F�Ȃ�
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Bread::bread(D3DXVECTOR3 playerPos, int dir)
{
	//�����˂̂Ƃ�
	if (isbread == FALSE)
	{
		isbread = TRUE;
		dirflg = dir;

		//�����̈ʒu = �n���ꂽ�v���C���[�̈ʒu
		switch (dir)
		{
		case MS_RIGHT:
			position.x = playerPos.x + 25;
			break;

		case MS_LEFT:
			position.x = playerPos.x - 25;
			break;

		default:
			isbread = FALSE;
			break;
		}

		position.y = playerPos.y-8;
		return TRUE;
	}


	return FALSE;
}

/*-----------------------------���_------------------------
����|�W�V�������󂯎���Ă���ʒu����o���b�g����Ԃ悤�ɂȂ��Ă���
�܂�J�������ړ����Ă�Ƃ��ɒ����ɃL�����𑨂��Ă���ۂɒǏ]���ď�ɒ���
���̂܂܂��ƃo���b�g���L�����Ɠ������������Ȃ̂ł������s���R
------------------------------------------------------------*/

//----------------------------------
//�@�\�F���˂����e�̃t���O�������l�ɖ߂��֐�
//�����F�Ȃ�
//�ߒl�FTRUEorFALSE
//----------------------------------
void Bread::Reset()
{
	isbread = FALSE;
	breadlife = NULL;
}