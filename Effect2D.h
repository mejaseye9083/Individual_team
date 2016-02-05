/*===========================================================================================
�T�@���F�G�t�F�N�g���A�j���[�V���������ĕ\������
�쐬���F2015.08.24
�X�V���F2015.08.24
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Sprite.h"

//----------2D�G�t�F�N�g�N���X�̒�`----------------
class Effect2D
{
	Sprite *spt;			//�X�v���C�g�N���X�̃|�C���^�錾
	float speed;			//���[�V�����̑��x
	D3DXVECTOR2 size;		//�摜�̑傫��

	//�G�t�F�N�g�͕������݂���ꍇ�������A����ɑΉ��ł���悤�ɂ���
	struct EffectData
	{
		float anm;				//�A�j���[�V�����̊�_
		D3DXVECTOR3 position;
	};

	//���F�N�^�[���g���Δz��Ƃ��đ��₷���Ƃ��ł��A���𒲐��ł���B
	std::list<EffectData> effectData;


public:

	Effect2D();						//�R���X�g���N�^
	~Effect2D();					//�f�X�g���N�^
	HRESULT Load(char* fileName, D3DXVECTOR2 ptcSize, float spd);	//�G�t�F�N�g�摜��ǂݍ���
	void Draw();					//�G�t�F�N�g�̕\��
	void Add(D3DXVECTOR3 pos);		//�G�t�F�N�g��\��������ʒu������
};


