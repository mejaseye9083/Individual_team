/*===========================================================================================
�T�@�v:�X�e�[�W�̐ݒ�
�쐬��:2015.10.21
�X�V��:2016.01.13
�����:��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Stage.h"
#include "Camera.h"


Block block;
item  recover;

//------------------�萔�錾------------------------
#define GRAVITY			1		//�d��
#define	INERTIA			0.15
#define	JUMP			-15
#define	JUMP_COUNT		1

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Stage::Stage()
{
	global.mapW = STAGE_WIDTH;
	global.mapH = STAGE_HEIGHT;

	audio = new Audio;

	Create();
	
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Stage::~Stage()
{
	SAFE_DELETE(audio);
}

//----------------------------------
//�@�\�F�摜�E���y�f�[�^�̓ǂݍ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Stage::Load()
{
	//SE�֘A�̓ǂݍ���(�X�e�[�W��StageBGM�ނ̂�)
	if (FAILED(audio->Load("songs\\SE\\Rock_BGM.xwb", "songs\\SE\\Rock_BGM.xsb")))
	{
		return E_FAIL;
	}

	block.Load("pictures\\modelA.bmp");
	recover.Load("pictures\\RecoverAid.png");

	return S_OK;
}

//----------------------------------
//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F����
//----------------------------------
HRESULT Stage::Update()
{
	//BGM:Techno�̓��[�v�������Ă���
	//BOOL�����ōŏ���TRUE�ɂ��Ă����A����FALSE�ɕς��ē�x�Ɠǂݍ��܂�Ȃ��悤�ɂ���
	if (Def == TRUE&&g_gameScene == SC_PLAY)
	{
		audio->Play("StageALL");
		Def = FALSE;
	}

	return S_OK;
}

//----------------------------------
//�@�\�F�����蔻��
//�����FpTarget		�G�̏��
//�ߒl�F����
//----------------------------------
HRESULT Stage::Hit(UnitBase* pTarget)
{

	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Stage::Render()
{

	//�����\��������ʒu�F�񎟌��z�񓙌�����
	/*D3DXVECTOR3 Bpos(100, 350, 0);
	block.Drow(Bpos);*/
	for (int x = 0; x < global.mapW; x++)
	{
		for (int y = 0; y < global.mapH; y++)
		{

			int px = x * BLOCK_CHIP;
			int py = y * BLOCK_CHIP;
			D3DXVECTOR3 Bpos(px, py, 0);
			block.Drow(Bpos, table[x][y]);
		}
	}

	recover.Drow(D3DXVECTOR3((25*BLOCK_CHIP), (33*BLOCK_CHIP), 0));
	return S_OK;
}

//�����_���}�b�v�쐬
void Stage::Create()
{
	//�Ƃ肠�����S���ʂ��悤�ɂ���
	ZeroMemory(table, sizeof(table));

	//0:�w�i,1:��,2~6:�܂��w�肵�ĂȂ�
	char szFile[MAX_PATH] = "Stage\\stage2.csv";


	HANDLE hFile;
	hFile = CreateFile(
		szFile,					//�t�@�C����
		GENERIC_READ,			//�A�N�Z�X���[�h(�������ݗp)
		0,						//���L(�Ȃ�)
		NULL,					//�Z�L�����e�B����(�p�����Ȃ�)
		OPEN_EXISTING,			//�쐬���@(�����t�@�C���͏㏑��)
		FILE_ATTRIBUTE_NORMAL,	//�����ƃt���O(�ݒ�Ȃ�)
		NULL					//�g�������Ȃ�(�Ȃ�)
		);

	//�J���Ȃ������璆�f
	if (hFile == NULL)	return;

	//�t�@�C���̃T�C�Y���擾
	DWORD fileSize = GetFileSize(hFile, NULL);

	//�t�@�C���̃T�C�Y�����������m��
	char* data;
	data = new char[fileSize];

	//�S���ǂݍ���
	DWORD dwBytes = 0;
	ReadFile(
		hFile,					//�t�@�C���n���h��
		data,					//�f�[�^������ϐ�
		(DWORD)strlen(data),	//�ǂݍ��ރT�C�Y
		&dwBytes,				//�ǂݍ��񂾃T�C�Y
		NULL);					//�I�[�o�[���b�v�h�\����

	//�ǂݍ��񂾃f�[�^����}�b�v�̕��ƍ������擾
	int index = 0;
	//global.mapW = 20;
	//global.mapH = GetToComma(data, &index);

	//���ԂɊe�}�X�̒l���擾
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{

			table[x][y] = GetToComma(data, &index);
		}
	}

	/*y=�����A�����x���l�X�g�Ń��[�v������y���v���X����Ɖ��s�Ɠ��������ɂȂ�*/

	//new�ō����data���������
	delete data;
}

//-------------------------------------------------
//�@�\�F�R���}��؂�̃f�[�^�𐮐��Ŏ擾
//�����Fdata[]		�R���}��؂�̕�����f�[�^
//�����F*index		�������J�n����ʒu
//�ߒl�F���̃R���}�܂ł̕�����𐮐��ɕϊ������l
//-------------------------------------------------
int Stage::GetToComma(char data[], int* index)
{
	char chr[600];
	int i = 0;

	//[,]�܂ł̈ꕶ������chr�ɓ����
	while (data[*index] != ',')
	{
		chr[i] = data[*index];
		i++;
		(*index)++;
	}

	//�Ō�Ɂu\0�v������
	chr[i] = '\0';
	i++;
	(*index)++;

	//�����̕ϊ�
	int value = atoi(chr);
	return value;
}