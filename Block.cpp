/*===========================================================================================
�T�@�v:�u���b�N�ƂȂ��{�̐ݒ�
�쐬��:2015.10.26
�X�V��:2015.12.03
�����:��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Block.h"
#include "Camera.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Block::Block()
{
	Number = 0;

	blPoint = BLOCK_CHIP;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Block::~Block()
{
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Block::Load(char* filename)
{

	Wall.Load(filename);
	return S_OK;
}

//----------------------------------
//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Block::Update()
{
	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpTarget		�G�̏��
//�ߒl�F����
//----------------------------------
HRESULT Block::Hit(UnitBase* pTarget)
{

	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Block::Render()
{
	return S_OK;
}

//----------------------------------
//�@�\�F�O������Ăԕ`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Block::Drow(D3DXVECTOR3 pos, int table)
{
	if (table == 1||table == 2)
	{
		HitZone.left = 0 + pos.x;
		HitZone.top = 0 + pos.y;

		HitZone.right = 32 + pos.x;
		HitZone.bottom = 32 + pos.y;
	}
	else if (table || 0)
	{
		HitZone = { 0, 0, 0, 0 };
	}

	//�X�e�[�W���\�z����u���b�N�̔z�u
	SpriteData WallData;
	/*
	WallData.size.x = size.x;
	WallData.size.y = size.y;
	*/

	//����F�\�����Ȃ������͏���
	
	WallData.size.x = blPoint;
	WallData.size.y = blPoint;
	
	WallData.cut.x = BLOCK_CHIP*table;

	//�L�����̓����ɍ��킹�ĉ�ʂ������̂ŁA�X�e�[�W�̈ʒu���ς���
	WallData.pos.x = pos.x + g_stageScrollPosition.x;
	WallData.pos.y = pos.y + g_stageScrollPosition.y;

	Wall.Draw(&WallData);
	return S_OK;
}
