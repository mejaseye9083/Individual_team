/*===========================================================================================
�T�@�v:�񕜃A�C�e����c�@�A�b�v�A�C�e����
�쐬��:2016.01.26
�X�V��:2016.01.26
�����:��������
===========================================================================================*/

//---------------�C���N���[�h-----------------------
#include "RecoveryItem.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
RecoveryItem::RecoveryItem()
{
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
RecoveryItem::~RecoveryItem()
{
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT RecoveryItem::Load(char *filename)
{
	if (FAILED(Recover_Aid.Load(filename)))
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
HRESULT RecoveryItem::Update()
{
	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpTarget		�G�̏��
//�ߒl�F����
//----------------------------------
HRESULT RecoveryItem::Hit(UnitBase* pTarget)
{
	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT RecoveryItem::Render()
{
	return S_OK;
}

//----------------------------------
//�@�\�F�O������Ăԕ`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT RecoveryItem::Drow(D3DXVECTOR3 pos)
{

	SpriteData Recover;

	//�L�����̓����ɍ��킹�ĉ�ʂ������̂ŁA�X�e�[�W�̈ʒu���ς���
	Recover.pos.x = pos.x + g_stageScrollPosition.x;
	Recover.pos.y = pos.y + g_stageScrollPosition.y;

	Recover_Aid.Draw(&Recover);

	position = pos;

	return S_OK;
}

//testcom2