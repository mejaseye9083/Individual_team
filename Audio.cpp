/*===========================================================================================
�T�@���F����炷�v���O����
�쐬���F2015.08.25
�X�V���F2015.08.25
����ҁF��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Audio.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Audio::Audio()
{
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Audio::~Audio()
{
	//XACT�V���b�g�_�E��
	//��ɃV���b�g�_�E�����Ă����Ȃ��ƃG���[�ɂȂ�
	xactEngine->ShutDown();

	//�T�E���h�o���N�J��
	SAFE_DELETE_ARRAY(soundBankData);

	//�E�F�[�u�o���N�J��
	UnmapViewOfFile(mapWaveBank);

	//�G���W���J��
	SAFE_RELEASE(xactEngine);

	//COM���C�u�����J��
	CoUninitialize();
}

//----------------------------------
//�@�\�F�I�[�f�B�I�̃G���W���𐶐�
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Audio::InitEngine()
{
	//COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//�G���W���쐬
	if (FAILED(XACT3CreateEngine(0, &xactEngine)))
	{
		MessageBox(0, "XACT�G���W���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	//�p�����[�^�ݒ�
	XACT_RUNTIME_PARAMETERS xactParam = { 0 };
	xactParam.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	if (FAILED(xactEngine->Initialize(&xactParam)))
	{
		MessageBox(0, "XACT�G���W���̃p�����[�^�ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�E�F�[�u�o���N�̓ǂݍ���
//�����FfileName	�E�F�[�u�o���N�̖��O
//�ߒl�F����or���s
//----------------------------------
HRESULT Audio::LoadWaveBank(char* fileName)
{
	//�t�@�C�����J��
	HANDLE  hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	//�t�@�C���T�C�Y�𒲂ׂ�
	DWORD  fileSize = GetFileSize(hFile, &fileSize);

	//�}�b�s���O
	HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
	mapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

	//�E�F�[�u�o���N�쐬
	if (FAILED(xactEngine->CreateInMemoryWaveBank(mapWaveBank, fileSize, 0, 0, &waveBank)))
	{
		MessageBox(0, "�E�F�[�u�o���N�쐬�Ɏ��s���܂���", fileName, MB_OK);
		return E_FAIL;
	}

	//�g�����n���h���͂����ƌ�Еt������
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	return S_OK;
}

//----------------------------------
//�@�\�F�T�E���h�o���N�̓ǂݍ���
//�����FfileName	�T�E���h�o���N�̖��O
//�ߒl�F����or���s
//----------------------------------
HRESULT Audio::LoadSoundBank(char* fileName)
{
	//�t�@�C�����J��
	HANDLE  hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	//�t�@�C���T�C�Y�𒲂ׂ�
	DWORD fileSize = GetFileSize(hFile, &fileSize);

	//�f�[�^��S���ǂ�
	soundBankData = new BYTE[fileSize];
	DWORD  byteRead;
	ReadFile(hFile, soundBankData, fileSize, &byteRead, NULL);

	//�T�E���h�o���N�쐬
	if (FAILED(xactEngine->CreateSoundBank(soundBankData, fileSize, 0, 0, &soundBank)))
	{
		MessageBox(0, "�T�E���h�o���N�쐬�Ɏ��s���܂���", fileName, MB_OK);
		return E_FAIL;
	}

	//�g���I������炵�܂�
	CloseHandle(hFile);
	return S_OK;
}

//----------------------------------
//�@�\�F�G���W�������E�E�F�[�u�o���Nand�T�E���h�o���N�̓ǂݍ��݂��܂Ƃ߂��֐�
//�����FwaveBankFileName		�E�F�[�u�o���N�̖��O
//�����FsoundBankFileName		�T�E���h�o���N�̖��O
//�ߒl�F����or���s
//----------------------------------
HRESULT Audio::Load(char* waveBankFileName, char* soundBankFileName)
{
	//XACT�G���W���̍쐬
	if (FAILED(InitEngine()))
	{
		return E_FAIL;
	}

	//WaveBank�ǂݍ���
	if (FAILED(LoadWaveBank(waveBankFileName)))
	{
		return E_FAIL;
	}

	//SoundBank�ǂݍ���
	if (FAILED(LoadSoundBank(soundBankFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//�@�\�F����炷
//�����FcueName		�w�肵���T�E���h
//�ߒl�F����or���s
//----------------------------------
HRESULT Audio::Play(char* cueName)
{
	//�L���[�̔ԍ����擾
	XACTINDEX cueIndex = soundBank->GetCueIndex(cueName);

	//�Đ�
	if (FAILED(soundBank->Play(cueIndex, 0, 0, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�炵�Ă��鉹���~�߂�
//�����FcueName		�w�肵���T�E���h
//�ߒl�F����or���s
//----------------------------------
HRESULT Audio::Stop(char* cueName)
{
	//�L���[�̔ԍ����擾
	XACTINDEX cueIndex = soundBank->GetCueIndex(cueName);

	//��~
	if (FAILED(soundBank->Stop(cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE)))
	{
		return E_FAIL;
	}
	return S_OK;
}