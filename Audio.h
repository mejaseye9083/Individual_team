/*===========================================================================================
�T�@���F����炷�v���O����
�쐬���F2015.08.25
�X�V���F2015.08.25
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include <xact3.h>

//------------�I�[�f�B�I�N���X�̒�`----------------
class Audio
{
	IXACT3Engine*		xactEngine;	//XACT�G���W���A���ꂪ�Ȃ��Ɖ���点�Ȃ�
	IXACT3WaveBank*		waveBank;	//�O���ō쐬�����T�E���h�o���N�̕ۑ��ʒu
	IXACT3SoundBank*	soundBank;	//�O���ō쐬�����E�F�[�u�o���N�̕ۑ��ʒu

	void*        mapWaveBank;	//.xwb�t�@�C��������ꏊ
	void*		 soundBankData;	//.xsb�t�@�C��������ꏊ

	HRESULT InitEngine();
	HRESULT LoadWaveBank(char* name);
	HRESULT LoadSoundBank(char* name);
	//���̎O��public��load�ł܂Ƃ߂ČĂ΂��悤�ɂ��Ă���̂�
	//�M���Ȃ��悤�Ƀv���C�x�[�g�ɂ��Ă����ƈ��S�Ɏ�舵���ł���
public:
	Audio();
	~Audio();

	HRESULT Load(char* wname, char* sname);	//�E�F�[�u�o���N�E�T�E���h�o���N��ǂݍ���
	HRESULT Play(char* cueName);			//����炷
	HRESULT Stop(char* cueName);			//�����~�߂�
};