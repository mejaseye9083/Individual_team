/*===========================================================================================
�T�@�v:���@�̐ݒ�
�쐬��:2015.10.15
�X�V��:2016.02.15
�����:��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Block.h"

/*
 * ���݃G���[���������A�����͉��L�̃X�e�[�W�̃|�C���^���ۂ�
 *
 *
 */

Stage *stg;

//------------------�萔�錾------------------------
#define GRAVITY			1		//�d��
#define	INERTIA			0.06
#define	JUMP			-12
#define	JUMP_COUNT		1

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Player::Player()
{
	ply = new Sprite;
	life = new Sprite;
	special = new Sprite;
	audio = new Audio;

	Reset();
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Player::~Player()
{
	//���S�ɏ�������
	SAFE_DELETE(ply);
	SAFE_DELETE(life);
	SAFE_DELETE(special);
	SAFE_DELETE(stg);
	SAFE_DELETE(audio);
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Player::Load()
{
	//�v���C���[�摜�̓ǂݍ���
	if (FAILED(ply->Load("pictures\\Actor.png")))
	{
		return E_FAIL;
	}

	//���C�t�������摜�̓ǂݍ���
	if (FAILED(life->Load("pictures\\LIFE.bmp")))
	{
		return E_FAIL;
	}

	//���C�t�������摜�̓ǂݍ���
	if (FAILED(special->Load("pictures\\Special_ber.bmp")))
	{
		return E_FAIL;
	}

	
	//�ݒ肵�Ă鐔�܂Œe���o����悤�ɂ���
	for (int i = 0; i < BULLET_SET; i++)
	{
		if (FAILED(bullet[i].Load()))
		{
			return E_FAIL;
		}
	}

	//�v���C���[�摜�̓ǂݍ���
	if (FAILED(bread.Load()))
	{
		return E_FAIL;
	}

	//SE�֘A�̓ǂݍ���
	if (FAILED(audio->Load("songs\\SE\\Rock_BGM.xwb", "songs\\SE\\Rock_BGM.xsb")))
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
HRESULT Player::Update()
{
	//�X�g�b�v�{�^���i�|�[�Y�j
	if (g_pInput->IsKeyTap(DIK_J))
	{
		g_Stopflg = !g_Stopflg;
	}

	if (g_Stopflg == FALSE)
	{
		//�ړ����ɓ񎟌��z��̃e�[�u���𗘗p����
		Move(stg);
		Shot();

		//�q�b�g�|�C���g��0�ȉ��ɂȂ�����Q�[���I�[�o�[
		if (hp <= 0)
		{
			Reset();
			audio->Play("dead");
			g_gameScene = SC_GAMEOVER;
		}

		//0�̒i�K�œ��ꕐ��͑łĂȂ����A�摜�����������Ȃ�̂Ŏ~�߂�
		if (sp <= 0)
		{
			sp = 0;
		}

		//---------���G���Ԃ̊Ǘ�--------
		if (bonny == DAMAGE)
		{
			Invincible();
		}
		
		if (bonny == PEASE)
		{
			invincibleTime = 0;
		}
		//-------------------------------
		//�e�̍X�V
		BulletUpdate();


		//---------------------�f�o�b�O�p�R�}���h------------------
		//�����R�}���h
		if (g_pInput->IsKeyTap(DIK_Y))
		{
			hp -= 2;
			memory_break_hp = (PLAYER_HP - hp) * PLAYER_DM;
		}

		//�����R�}���h
		if (g_pInput->IsKeyTap(DIK_P))
		{
			hp -= 999;
			memory_break_hp = (PLAYER_HP - hp) * PLAYER_DM;
		}
		//---------------------------------------------------------

	}
	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpTarget		�G�̏��
//�ߒl�F����
//----------------------------------
HRESULT Player::Hit(UnitBase* pTarget)
{
	//--------------------�G�l�~�[�Ƃ̏Փ˔���----------------------
	//�����蔻������AX���EY���E���E�������K�v�ɂȂ�
	if (typeid(*pTarget) == typeid(Enemy))
	{
		D3DXVECTOR3 enemyPosition = pTarget->GetPos();
		RECT enemySize = pTarget->GetZone();
	
		//�����蔻����o�����߂̌v�Z
		int distance =
			(int)(position.x - enemyPosition.x) *
			(int)(position.x - enemyPosition.x) +
			(int)(position.y - enemyPosition.y) *
			(int)(position.y - enemyPosition.y);

		//�Փ˔���
		if (distance <= (PLAYER_SIZE * PLAYER_SIZE))
		{
			if (bonny == PEASE)
			{
				//�ڐG�_���[�W
				hp -= 3;
				memory_break_hp = (PLAYER_HP - hp) * PLAYER_DM;
				audio->Play("Player_Damage");
				bonny = DAMAGE;
			}
		}

		//�e�̃q�b�g����
		for (int i = 0; i < BULLET_SET; i++)
		{
			//�L���X�g���Z�q�𗘗p��UnitBase*����Enemy*�ɖ������ς��Ă��܂�
			BOOL b = bullet[i].Hit((Enemy*)pTarget);

		}

		//�u���[�h�̃q�b�g����
		bread.Hit((Enemy*)pTarget);
	}
	//--------------------------------------------------------------


	//--------------------�A�C�e���Ƃ̏Փ˔���----------------------
	//�����蔻������AX���EY���E���E�������K�v�ɂȂ�
	if (typeid(*pTarget) == typeid(item))
	{
		D3DXVECTOR3 itemPosition = pTarget->GetPos();
		RECT itemSize = pTarget->GetZone();

		//�����蔻����o�����߂̌v�Z
		int distance =
			(int)(position.x - itemPosition.x) *
			(int)(position.x - itemPosition.x) +
			(int)(position.y - itemPosition.y) *
			(int)(position.y - itemPosition.y);

		//�Փ˔���
		if (distance <= (PLAYER_SIZE * PLAYER_SIZE))
		{

			g_gameScene = SC_CLEAR;
		}

	}
	//--------------------------------------------------------------


	//-----------PlayScene�ŌĂяo�����X�e�[�W��stg�ɓ����---------
	if (typeid(*pTarget) == typeid(Stage))
	{
		/*
		 * pTarget��UnitBase�̃T�u�N���X�Ȃ�|�C���^��ʂ��Ċm�F���o����
		 * �܂�APlayScene��Stage���Ăяo���Ă���̂ŁA�����stg�ɓ����΂悢�I
		 * ���������2��ĂԂ��Ƃł̕s���Ȓl�̏K���͂Ȃ��Ȃ�B
		 *
		 * �`�`�`�`�`�`�`�`�`�`�`�I�}�P�`�`�`�`�`�`�`�`�`�`
		 * �V���O���g���Ƃ����@��������A�m�邱�Ƃ�new���������I�Ɏg����悤�ɂȂ�炵���I
		 */
		stg = (Stage*)pTarget;
	}
	//--------------------------------------------------------------

	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Player::Render()
{
	//-----------------------�v���C���[�̕`��-----------------------------
	SpriteData rock;
	rock.cut.x = ((anime / 10) % 4) * 32;
	rock.cut.y = direction * 32;

	rock.size = D3DXVECTOR2(32, 32);

	//----�_���[�W���󂯂��Ƃ��̓_�ŏ���----
	switch (bonny)
	{
	case PEASE:
		break;

	case DAMAGE:
		if (invincibleTime % 2 == 0)
		{
			rock.size.x = 0;
		}
		else
		{
			rock.size.x = 32;
		}
		break;
	}
	//--------------------------------------
	
	rock.pos = position + g_stageScrollPosition;

	ply->Draw(&rock);
	//--------------------------------------------------------------------

	//---------------------���C�t�������̕`��-----------------------------
	SpriteData Life;
	Life.pos.x = 10;
	Life.pos.y = 20 + memory_break_hp;
	Life.size.y = 160 - memory_break_hp;
	if (Life.size.y <= 0)
	{
		Life.size.y = 0;
	}

	life->Draw(&Life);
	//--------------------------------------------------------------------

	//-------------------���ꕐ��̃������̕`��---------------------------
	SpriteData Special;
	Special.pos.x = 25;
	Special.pos.y = 20 + memory_break_sp;
	Special.size.y = 160 - memory_break_sp;
	if (Special.size.y <= 0)
	{
		Special.size.y = 0;
	}

	special->Draw(&Special);
	//--------------------------------------------------------------------

	//------------------�e�������ƕ`�揈���������I----------------------
	//�e���ő吔�ɒB�������B���ĂȂ���
	for (int i = 0; i < BULLET_SET; i++)
	{
		//�B���ĂȂ������甭�ˉ\
		if (FAILED(bullet[i].Render()))
		{
			return E_FAIL;
		}
	}
	//--------------------------------------------------------------------

	//�u���[�h�̃����_�[
	bread.Render();
	return S_OK;
}

//----------------------------------
//�@�\�F���@�̈ړ�
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Player::Move(Stage* stage)
{
	//-----------�����蔻���Ǐ]������------------
	HitZone.left = 2 + position.x;
	HitZone.top = 2 + position.y;

	HitZone.right = 30 + position.x;
	HitZone.bottom = 32 + position.y;
	//---------------------------------------------

	int moveS = 4;
	//----------------------�v���C���[�̈ړ�------------------------------
	switch (state)
	{

		//---------------------�ʏ�ړ�----------------------
	case DEFAULT:
		/*
		 *�����Ŏ󂯎����Stage�̒��ɃX�e�[�W�̓񎟌��z�񂪂���̂ŁA������󂯎��
		 */

		//�E�ړ��̏���
		if (g_pInput->IsKeyPush(DIK_RIGHT) && !ladderflg)
		{

			if (stage->GetChip((int)(HitZone.right + moveS) / BLOCK_CHIP, (int)(HitZone.bottom) / BLOCK_CHIP) == 1
				|| stage->GetChip((int)(HitZone.right + moveS) / BLOCK_CHIP, (int)(HitZone.top) / BLOCK_CHIP) == 1)
			{
				moveS = 0;	//0�ɂ���
			}
			
			position.x += moveS;
			direction = MS_RIGHT;
			anime++;
		}

		//���ړ��̏���(������)
		if (g_pInput->IsKeyPush(DIK_LEFT) && !ladderflg)
		{

			if (stage->GetChip((int)(HitZone.left - moveS) / BLOCK_CHIP, (int)(HitZone.bottom) / BLOCK_CHIP) == 1 
				|| stage->GetChip((int)(HitZone.left - moveS) / BLOCK_CHIP, (int)(HitZone.top) / BLOCK_CHIP) == 1)
			{
				moveS = 0;	//�ړ����x��0�ɂ���
			}

			position.x -= moveS;
			direction = MS_LEFT;
			anime++;
		}

		//------------------�v����---------------------
		//��q�ł̈ړ�(�Ȃ�������)
		if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2
			|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2)
		{
			//��q���ړ�
			if (g_pInput->IsKeyPush(DIK_DOWN))
			{
				//�t���O���X�̗����Ō���̂Ō�ŗv�C��
				direction = MS_UP;
				isGround = TRUE;
				ladderflg = TRUE;
				jcount = 0;
				jumpBlock = FALSE;

				//�i�s��������(1)�������ꍇ�͈ړ��o���Ȃ��悤�ɂ���
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.bottom + moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;

				}

				position.y += moveS;
				
			}
		//-----------------------------------------------------------------------------------------------
			//��q��ړ�
			if (g_pInput->IsKeyPush(DIK_UP))
			{
				//�t���O���X�̗����Ō���̂Ō�ŗv�C��
				direction = MS_UP;
				isGround = TRUE;
				ladderflg = TRUE;
				jcount = 0;

				//�i�s��������(1)�������ꍇ�͈ړ��o���Ȃ��悤�ɂ���
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.top - moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;
					jumpBlock = TRUE;

				}
				//��L�̔����rect�̃g�b�v(���̈ʒu)�Ȃ̂ŁA�W�����v�ł��Ȃ��悤�ɂ��Ă���B
				//1�s�N�Z���ł����ꂽ��W�����v�\�Ƀt���O��؂�ւ���
				else
				{
					jumpBlock = FALSE;
				}
				
				//���̈ʒu-5�̍������烀�[�u�̒l���������l��0(�����Ȃ����)�ɓ���ꍇ
				if (stage->GetChip((int)HitZone.right/ BLOCK_CHIP, (int)(HitZone.bottom -5 -moveS) / BLOCK_CHIP) == 0
					&& stage->GetChip((int)HitZone.left/ BLOCK_CHIP, (int)(HitZone.bottom -5 -moveS) / BLOCK_CHIP) == 0)
				{
					moveS = 0;
					jumpBlock = TRUE;

				}

				position.y += -moveS;
			}
		}
		//����ȊO�͒�q��͂�ł��Ȃ��ƌ��Ȃ��A��q�t���O��FALSE�ɕς���
		else
		{
			ladderflg = FALSE;
		}
		

		//�v���C���[���^�񒆂ɗ���悤�A���X�N���[���ʒu���Z�o
		g_stageScrollPosition.x = -position.x + (WINDOW_WIDTH / 2 - BLOCK_CHIP);

		//-------���X�N���[���������̏ꍇ�̏���---------
		if (g_stageScrollPosition.x > 0)
			g_stageScrollPosition.x = 0;

		if (g_stageScrollPosition.x < -((STAGE_WIDTH * BLOCK_CHIP/* * 2*/) - WINDOW_WIDTH))
			g_stageScrollPosition.x = -((STAGE_WIDTH * BLOCK_CHIP/* * 2*/) - WINDOW_WIDTH);
		//----------------------------------------------


		//��ʉ��[�܂ōs�����Ƃ�
		if (position.y + g_stageScrollPosition.y > WINDOW_HEIGHT - BLOCK_CHIP)
		{
			state = DOWN_SCROLL;
		}

		//��ʏ�[�܂ōs�����Ƃ�
		if (position.y + g_stageScrollPosition.y < 0)
		{
			state = UP_SCROLL;
		}

		break;
		//----------------------------------------------------


		//------------------���փX�N���[����------------------
	case DOWN_SCROLL:
		g_stageScrollPosition.y -= 20;	//��ʂ��X�N���[��
		position.y += 3;				//�v���C���[���ړ�

		//�L���̗ǂ��Ƃ���܂ł�������ʏ��Ԃɖ߂�
		if ((int)g_stageScrollPosition.y % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------


		//------------------��փX�N���[����------------------
	case UP_SCROLL:
		g_stageScrollPosition.y += 20;	//��ʂ��X�N���[��
		position.y -= 3;				//�v���C���[���ړ�

		//�L���̗ǂ��Ƃ���܂ł�������ʏ��Ԃɖ߂�
		if ((int)g_stageScrollPosition.y % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------

		//------------------���փX�N���[����------------------
	case LEFT_SCROLL:
		g_stageScrollPosition.x -= 20;	//��ʂ��X�N���[��
		position.x += 3;				//�v���C���[���ړ�

		//�L���̗ǂ��Ƃ���܂ł�������ʏ��Ԃɖ߂�
		if ((int)g_stageScrollPosition.x % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------

		//------------------�E�փX�N���[����------------------
	case RIGHT_SCROLL:
		g_stageScrollPosition.x += 20;	//��ʂ��X�N���[��
		position.x -= 3;				//�v���C���[���ړ�

		//�L���̗ǂ��Ƃ���܂ł�������ʏ��Ԃɖ߂�
		if ((int)g_stageScrollPosition.x % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------

	}
	//--------------------------------------------------------------------

	//--------------------------�W�����v����------------------------------
	if ((g_pInput->IsKeyTap(DIK_SPACE) && (jcount < JUMP_COUNT)&&isGround) && !jumpBlock)
	{
		jump = JUMP;
		jcount++;
		isGround = FALSE;
		ladderflg = FALSE;
	}


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
	else if ((stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0 
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0) 
		||(stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 2
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 2)
		&& !ladderflg)
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//�V��i�Ǝv����ꏊ�j�ɂԂ�������
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top + jump -3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top + jump -3) / BLOCK_CHIP) == 1)
	{
		jump *= -1;
		jump += GRAVITY;
	}

	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top - 3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top - 3) / BLOCK_CHIP) == 1)
	{
		jumpBlock = TRUE;
	}
	else
	{
		jumpBlock = FALSE;
	}
	//--------------------------------------------------------------------

	/*
	 * �W�����v���̃e�[�u������Œ�q(2)�̕����̔�����s���Ă��Ȃ�
	 * ���̂��߂�jump�ɗ^�������l(JUMP = -12)��GRAVITY�̒l���v���X����Ȃ����ۂ��N���Ă���
	 * �����܂Ō���͎d�l�Ƃ��Ă邪�A��X����Ō���Ȃ��d�l�ɂ��Ă�������(���L������)
	 * 
	 */

	return S_OK;
}

//----------------------------------
//�@�\�F�e�̔��˂ƋO��
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Player::Shot()
{
	//��q�ɂ��܂��Ă���Ƃ��͍U���������Ȃ�
	if (ladderflg != TRUE)
	{
		//�u���[�h�U��
		if (g_pInput->IsKeyTap(DIK_X) && sp > 0)
		{

			BOOL c;
			c = bread.bread(position + g_stageScrollPosition, direction);
			//���˂ł����I
			if (c == TRUE)
			{
				sp -= 3;
				memory_break_sp = (PLAYER_SP - sp) * PLAYER_DM;
				audio->Play("shoot");
			}

			//�L�[�������Ă���
			isSpecialFlg = TRUE;
		}
		else
		{
			//�L�[�𗣂��Ă���
			isSpecialFlg = FALSE;
		}

		//�e�𔭎˂��鑀��
		if (g_pInput->IsKeyTap(DIK_Z) || g_pInput->IsPadButtonRelease(XINPUT_GAMEPAD_A))
		{
			if (isShotKeyFlg == FALSE)
			{
				for (int i = 0; i < BULLET_SET; i++)
				{
					BOOL b;
					b = bullet[i].Shot(position + g_stageScrollPosition, direction);
					//���˂ł����I
					if (b == TRUE)
					{
						audio->Play("shoot");
						break;
					}
				}
			}
			//�L�[�������Ă���
			isShotKeyFlg = TRUE;
		}
		else
		{
			//�L�[�𗣂��Ă���
			isShotKeyFlg = FALSE;
		}
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�e�̍X�V����
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
void Player::BulletUpdate()
{
	//�e�̌������[�v����
	for (int i = 0; i < BULLET_SET; i++)
	{
		bullet[i].Update();
	}

	//�㉺�ɃX�N���[������ʒu�ɍs������e��S�ď���(���Z�b�g)
	if (state == DOWN_SCROLL || state == UP_SCROLL)
	{
		for (int i = 0; i < BULLET_SET; i++)
		{
			bullet[i].Reset();
		}
	}

	//�Ǐ]�����邽�߂Ƀ|�W�V������n���Ă���
	bread.Update(position+g_stageScrollPosition);
}

//----------------------------------
//�@�\�F�����ݒ聕�ď���������
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
void Player::Reset()
{
	position.x = 100;
	position.y = 300;

	jump = 0;
	jcount = 0;

	anime = 0;

	invincibleTime = 0;
	direction = MS_RIGHT;

	//�_���[�W���󂯂����ۂ�
	bonny = PEASE;

	//asiba = 999;
	

	hp = PLAYER_HP;
	sp = PLAYER_SP;
}

//----------------------------------
//�@�\�F�Q�[���I�[�o�[��ʂɈړ����鏈��
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
void Player::Invincible()
{
	invincibleTime++;
	if (invincibleTime == 0x80)
	{
		bonny = PEASE;
	}
}