/*===========================================================================================
�T�@���F�v���W�F�N�g�S�̂ŕK�v�Ȃ��̂��܂Ƃ߂��t�@�C��
�쐬���F2015.05.26
�X�V���F2016.01.19
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include <windows.h>
#include <d3dx9.h>
#include "Input.h"
#include <vector>
#include <list>
#include "Audio.h"

//----------���C�u�����t�@�C���̃��[�h--------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-------------------�}�N��------------------
#define	SAFE_DELETE(p)		 {delete (p); (p) = NULL;}
#define	SAFE_DELETE_ARRAY(p) {delete[] (p);  (p) = NULL;}
#define SAFE_RELEASE(p)      { if(p != NULL) { (p)->Release(); (p) = NULL; } }

//-------------------�萔�錾------------------
//�E�B���h�E�T�C�Y
#define WINDOW_WIDTH	640	//�E�B���h�E�̕�
#define WINDOW_HEIGHT	480	//�E�B���h�E����

#define ENEMY_STATE		5	//�G�̐�
#define PLAYER_STATE	1	//���@
#define BULLET_SET		3	//�o����e�̍ő吔

//�u���b�N�p�̒萔�錾
#define	BLOCK_CHIP	32

//�v���C���[�̃T�C�Y
#define PLAYER_SIZE		32

//-------------------�O���[�o���ϐ�------------------
//�V�[��
enum GAME_SCENE
{
	SC_PLAY,		//�v���C�V�[��
	SC_TITLE,		//�^�C�g���V�[��
	SC_CLEAR,		//�N���A�V�[��
	SC_GAMEOVER,	//�Q�[���I�[�o�[�V�[��
	SC_MAX			//�g���V�[���͂R�ASC_MAX�ɓ���l��3�����烋�[�v�����Ɏg����
};

struct GLOBAL
{
	int mapW, mapH;
};
extern GLOBAL global;

extern GAME_SCENE	g_gameScene;		//���Ԃ�Game.cpp�Ő錾


//Direct3D�f�o�C�X�I�u�W�F�N�g
extern LPDIRECT3DDEVICE9 g_pDevice;

//���͏����i�C���v�b�g�j�I�u�W�F�N�g
extern Input* g_pInput;

//�J�����I�u�W�F�N�g
//�J�����N���X��Global.h���C���N���[�h���Ă���B�z�Q�Ƃ�����邽�߂ɃN���X��錾����
class Camera;
extern Camera* g_pCamera;