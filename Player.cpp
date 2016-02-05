/*===========================================================================================
�T�@�v:���@�̐ݒ�
�쐬��:2015.10.15
�X�V��:2016.02.02
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
	/*position.x = 100;
	position.y = 300;*/

	/*jump = 0;
	jcount = 0;

	anime = 0;
	direction = MS_RIGHT;*/
	ply = new Sprite;
	life = new Sprite;

	//stg = new Stage;		//������new�����Ⴂ���Ȃ��I�i��q��F�v���C���[��Hit�֐�)

	audio = new Audio;

	//asiba = 400;
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
	if (FAILED(ply->Load("SamplePict\\Actor.png")))
	{
		return E_FAIL;
	}

	//���C�t�������摜�̓ǂݍ���
	if (FAILED(life->Load("pictures\\LIFE.bmp")))
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
	//�ړ����ɓ񎟌��z��̃e�[�u���𗘗p����
	Move(stg);
	Shot();

	//�����R�}���h
	if (g_pInput->IsKeyTap(DIK_Y))
	{
		hp -= 2;
		memory_break = (PLAYER_HP - hp) * PLAYER_DM;
	}


	//�o�L�[�������ƃQ�[���I�[�o�[�Ɉڍs����(�m�F�p)
	if (hp <= 0)
	{
		Reset();
		g_gameScene = SC_GAMEOVER;
	}

	//�e�̍X�V
	BulletUpdate();
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

		}

		for (int i = 0; i < BULLET_SET; i++)
		{
			//�L���X�g���Z�q�𗘗p��UnitBase*����Enemy*�ɖ������ς��Ă��܂�
			BOOL b = bullet[i].Hit((Enemy*)pTarget);

		}

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
			hp += 8;
			if (hp >= 20)
			{
				hp = 20;
			}
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
	
	rock.pos = position + g_stageScrollPosition;

	ply->Draw(&rock);
	//--------------------------------------------------------------------

	//---------------------���C�t�������̕`��-----------------------------
	SpriteData Life;
	Life.pos.x = 10;
	Life.pos.y = 20 + memory_break;
	Life.size.y = 160 - memory_break;
	if (Life.size.y <= 0)
	{
		Life.size.y = 0;
	}

	life->Draw(&Life);
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
	HitZone.left = 0 + position.x;
	HitZone.top = 0 + position.y;

	HitZone.right = 32 + position.x;
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
		/*if (g_pInput->IsKeyPush(DIK_DOWN))
		{
			if (stage->GetChip((int)(position.x + PLAYER_SIZE) / BLOCK_CHIP, (int)(position.y + JUMP + PLAYER_SIZE) / BLOCK_CHIP) == 2
				|| stage->GetChip((int)position.x / BLOCK_CHIP, (int)(position.y - JUMP) / BLOCK_CHIP) == 2)
			{
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.bottom) / BLOCK_CHIP) == 1)
				{
					position.y += 0;
					direction = MS_UP;
					isGround = TRUE;
					jcount = 0;
				}
				else
				{
					position.y += moveS;
					direction = MS_UP;
					isGround = TRUE;
					jcount = 0;
				}
			}
		}*/

		
		//��q�ł̈ړ�(�b���)
		if (stage->GetChip((int)(HitZone.right - 12) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2
			|| stage->GetChip((int)(HitZone.left  + 12) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2)
		{
			if (g_pInput->IsKeyPush(DIK_DOWN))
			{

				ladderflg = TRUE;
				direction = MS_UP;
				isGround = TRUE;
				jcount = 0;

				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.bottom + moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;

				}
				
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.top) / BLOCK_CHIP) == 0)
				{
					
				}

				position.y += moveS;
				
			}

			if (g_pInput->IsKeyPush(DIK_UP))
			{
				direction = MS_UP;
				isGround = TRUE;
				ladderflg = TRUE;
				jcount = 0;

				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.top + moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;

				}

				position.y -= moveS;
			}
		}
		else
		{
			isGround = FALSE;
			ladderflg = FALSE;
		}

		/*
		 * ���ݒ�q�ړ��͕ʁX�ɔ��肳���Ă��邪�A���������Ȃ��Ă����if���ɂ܂Ƃ߂�����ǂ�����
		 * ��ŏC������̂ŗv�_�̂݋L��
		 *
		 */


		//�v���C���[���^�񒆂ɗ���悤�A���X�N���[���ʒu���Z�o
		g_stageScrollPosition.x = -position.x + (WINDOW_WIDTH / 2 - 32);


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
	if (g_pInput->IsKeyTap(DIK_SPACE) && (jcount < JUMP_COUNT)&&isGround)
	{
		jump = JUMP;
		jcount++;
		isGround = FALSE;
		ladderflg = FALSE;
	}

	//�ԕ��N���I�o�O�̌��������ǌ���̗��݂̍j
	int BMPIkun;

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
	else if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 1) / BLOCK_CHIP) == 0 
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 1) / BLOCK_CHIP) == 0 && !ladderflg)
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//�V��i�Ǝv����ꏊ�j�ɂԂ�������
	if (stage->GetChip((int)position.x / BLOCK_CHIP, (int)(position.y - JUMP) / BLOCK_CHIP) == 1)
	{
	}

	//--------------------------------------------------------------------

	return S_OK;
}

//----------------------------------
//�@�\�F�e�̔��˂ƋO��
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Player::Shot()
{
	//�e�𔭎˂��鑀��
	if (g_pInput->IsKeyTap(DIK_Z) || g_pInput->IsPadButtonRelease(XINPUT_GAMEPAD_A))
	{
		if (isShotKeyFlg == FALSE)
		{
			for (int i = 0; i < BULLET_SET; i++)
			{
				BOOL b;
				b = bullet[i].Shot(position + g_stageScrollPosition,direction);
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

	return S_OK;
}

//----------------------------------
//�@�\�F�e�̍X�V����
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
void Player::BulletUpdate()
{
	for (int i = 0; i < BULLET_SET; i++)
	{
		bullet[i].Update();
	}
}

void Player::Reset()
{
	position.x = 100;
	position.y = 300;

	jump = 0;
	jcount = 0;

	anime = 0;
	direction = MS_RIGHT;

	//asiba = 999;

	hp = PLAYER_HP;
}