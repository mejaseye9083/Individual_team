/*===========================================================================================
�T�@�v:�^�C�g�����
�쐬��:2016.01.14
�X�V��:2016.01.19
�����:��������
===========================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Title.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Title::Title()
{
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Title::~Title()
{
}

//----------------------------------
//�@�\�F�^�C�g���摜�̓ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Title::Load()
{
	if (FAILED(back.Load("pictures\\titleScene.jpg")))
	{
		return E_FAIL;
	}

	if (FAILED(manual.Load("pictures\\manual.png")))
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
HRESULT Title::Update()
{
	//�X�y�[�X�L�[�y�уR���g���[���[��A�������ƃv���C�V�[���ֈڍs����
	if (g_pInput->IsKeyTap(DIK_E) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_A))
	{
		g_gameScene = SC_PLAY;
	}

	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Title::Render()
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

	SpriteData manualdata;
	manualdata.color.a = 255;
	manualdata.pos.x = 240;
	manualdata.pos.y = 400;
	manual.Draw(&manualdata);

	i++;
	return S_OK;
}
