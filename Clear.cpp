/*===========================================================================================
�T�@�v:�N���A���
�쐬��:2016.02.08
�X�V��:2016.02.08
�����:��������
===========================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Clear.h"


//----------------------------------
//�@�\�F�R���X�g���N�^(���ɖ���)
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
Clear::Clear()
{
}


//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
Clear::~Clear()
{
}

//----------------------------------
//�@�\�F�摜��SE���̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Clear::Load()
{
	if (FAILED(back.Load("pictures\\clear.bmp")))
	{
		return E_FAIL;
	}
}

//----------------------------------
//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Clear::Update()
{
	//�L�[�{�[�h��E�L�[�ŃV�[���ύX
	if (g_pInput->IsKeyTap(DIK_E) /*|| g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_A)*/)
	{
		g_gameScene = SC_TITLE;
	}
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Clear::Render()
{
	//�w�i�Ȃ̂œ��ɘM�炸��ʂ����ς��ɕ\��������
	SpriteData backdata;
	//�F�Ɠ����x
	backdata.color.a = 255;
	backdata.color.r = 255;
	backdata.color.g = 255;
	backdata.color.b = 255;
	backdata.pos.z = 0;
	back.Draw(&backdata);
}