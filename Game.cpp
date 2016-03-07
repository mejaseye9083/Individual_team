/*=======================================================================================================
�T�@���F�Q�[���S�̗̂������������
�쐬���F2015.05.28
�X�V���F2016.02.23
����ҁF��������
=======================================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Game.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "Camera.h"

//----------------�O���[�o���ϐ�----------------
GAME_SCENE			g_gameScene;	//���݂̃Q�[���V�[��
LPDIRECT3DDEVICE9	g_pDevice;		//Direct3D�f�o�C�X�I�u�W�F�N�g
Input*				g_pInput;		//���͏����I�u�W�F�N�g
Camera*				g_pCamera;		//�J�����I�u�W�F�N�g 

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�R���X�g���N�^
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
Game::Game()
{
	//�e�V�[���I�u�W�F�N�g�̐���
	scene[SC_TITLE] = new TitleScene;		//�^�C�g���V�[��
	scene[SC_PLAY] = new PlayScene;			//�v���C�V�[��
	scene[SC_CLEAR] = new ClearScene;
	scene[SC_GAMEOVER] = new GameOverScene;
	//�ŏ��̓^�C�g���V�[��
	g_gameScene = SC_TITLE;

	//�J����
	g_pCamera = new Camera;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�f�X�g���N�^
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
Game::~Game()
{
	for (int i = 0; i < SC_MAX; i++)
	{
		delete scene[i];
	}

	//DirectX ���
	SAFE_RELEASE(g_pDevice);
	SAFE_RELEASE(pD3d);

	//�J�����̃|�C���^�̉��
	SAFE_DELETE(g_pCamera);
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �ǂݍ��ݏ���
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Load()
{
	//�����őS���̃V�[����ǂݍ���
	for (int i = 0; i < SC_MAX; i++)
	{
		if (FAILED(scene[i]->Load()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// Direct3D������
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::InitD3d(HWND hWnd)
{
	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		//���b�Z�[�W�{�b�N�X���o��ƃG���[��������₷��
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}
	
	// �uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat =		D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount =			1;
	d3dpp.SwapEffect =				D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed =				TRUE;					//�J������TRUE�A����������FALSE�ɂ���
	d3dpp.EnableAutoDepthStencil =	TRUE;					//��邩���Ȃ����AFALSE�ɂ���ƑO�㊴�o�������Ȃ�
	d3dpp.AutoDepthStencilFormat =	D3DFMT_D16;				//16�ޯĂɂ��킹�Ă�

	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))
	{
		MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);

		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))
		{
			MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);

			return E_FAIL;
		}
	}

	//���C�g�̐ݒ�i�e�j�̐ݒ�A�������g��Ȃ��ƍ��ɂ����Ȃ�Ȃ��B
	//���̂Ƃ�����Ɏg��Ȃ��̂Ń��C�g�͐؂��Ă���
	/*if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}*/

	//�V�������C�g�̐ݒ肷��֐����ĂԁB
	if (FAILED(InitLight()))
	{
		return E_FAIL;
	}
	

	//�J�����O���Ă���Ɨ����������|���S���͕`�悳��Ȃ��Ȃ�
	//���ʂ�\���������ꍇ�̓J�����O���[�h�����Ȃ��ɐݒ肷��
	/*if (FAILED(g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		MessageBox(0, "�J�����O���[�h�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}*/

	//�����������̐ݒ���s���A�A���t�@�͓��߂���l��\���Ă���
	//SetRenderState�֐����g���ē����ȂƂ���͓����ɂ��邱�Ƃ��ł���
	if (FAILED(g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ||
		g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ||
		g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
	{
		MessageBox(0, "�A���t�@�u�����h�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}
	/*
	�����SetRenderState�֐���3��ĂԕK�v������B
	1�ڂ́u�A���t�@�u�����h��L���ɂ���v�Ƃ����ݒ�ŁA���Ƃ̂Q���u�����������̕\���̂������v�ɂȂ�B
	*/


	//�e�N�X�`���s��́A�\��t�����e�N�X�`���̑傫�������߂�ׂɕK�v�ȍs��B
	if (FAILED(g_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2)))
	{
		MessageBox(0, "�e�N�X�`���X�e�[�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	
	return S_OK;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�Փ˔���
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Hit()
{
	//�S�V�[���̏Փ˔���i�e�����͊e�N���X���j
	if (FAILED(scene[g_gameScene]->Hit()))
	{
		return FALSE;
	}
	return S_OK;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�X�V����
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Update()
{
	if (FAILED(g_pInput->Update()))
	{
		return E_FAIL;
	}

	if (FAILED(g_pCamera->Update()))
	{
		return E_FAIL;
	}

	if (FAILED(scene[g_gameScene]->Update()))
	{
		return FALSE;
	}
	return S_OK;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�`�揈��
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Render()
{

	//��ʂ��N���A
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(213, 183, 183), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pDevice->BeginScene()))
	{
		if (FAILED(scene[g_gameScene]->Render()))
		{
			return FALSE;
		}

		//�`��I��
		g_pDevice->EndScene();
	}

	//�t���b�v
	g_pDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT Game::InitLight()
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̌����̒���
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	//�f�B�t���[�Y��ݒ肷��
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	//�A���r�G���g�i�Ԑڌ��j��ǉ�����
	light.Ambient.r = 0.4f;
	light.Ambient.g = 0.4f;
	light.Ambient.b = 0.4f;

	if (FAILED(g_pDevice->SetLight(0, &light)))
	{
		MessageBox(0, "���C�g���Z�b�g�ł��܂���ł���", "Game", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->LightEnable(0, TRUE)))
	{
		MessageBox(0, "���C�g��L���ɂł��܂���ł���", "Game", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}