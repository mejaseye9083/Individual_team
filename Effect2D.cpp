/*===========================================================================================
�T�@���F�G�t�F�N�g���A�j���[�V���������ĕ\������
�쐬���F2015.08.25
�X�V���F2015.08.25
����ҁF��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Effect2D.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Effect2D::Effect2D()
{
	size = D3DXVECTOR2(128, 128);
	speed = 0.2f;
	spt = 0;
	spt = new Sprite;


}

//----------------------------------
//�@�\�F�f�X�g���N�^�inew�̉�������j
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Effect2D::~Effect2D()
{
	SAFE_DELETE(spt);
}

//----------------------------------
//�@�\�F�摜�̓ǂݍ��ݏ���
//�����FfileName	�摜�t�@�C���̖��O
//�����Fspd			�A�j���[�V�������x
//�ߒl�F����or���s
//----------------------------------
HRESULT Effect2D::Load(char* fileName, D3DXVECTOR2 pctSize, float spd)
{
	if (FAILED(spt->Load(fileName)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
void Effect2D::Draw()
{
	SpriteData data;
	data.size = size;

	//�C�e���[�^�[�i�J�[�\���j�̐���
	std::list<EffectData>::iterator	 itr;

	//�C�e���[�^�[�ɍŏ��l�𓖂Ă͂߁A�ő�l�ɂȂ�܂ŌJ��Ԃ�
	for (itr = effectData.begin(); itr != effectData.end();)
	{

		//�C�e���[�^�[�̒��ɂ���Ԓn��position
		//(*itr) = effectData�ƂȂ�B*itr.position����(*itr.position)�Ɠ����Ӗ��ɂȂ�
		//(*itr).position == itr->position�ƂȂ�B�|�C���^�̓A���[���Z�q�Ŏw��ł���
		data.pos = (*itr).position;
		data.cut.x = (int)(*itr).anm * data.size.x;
		spt->Draw(&data);
		(*itr).anm += speed;

		D3DXVECTOR2 pctSize = spt->GetSize();

		if ((*itr).anm > pctSize.x / size.y)
		{
			//���̒l���Ȃ��ƕ�����Ȃ��̂ŁAitr�Ɍ��ʂ��Ԃ�����
			itr = effectData.erase(itr);
			//�������Ƃ���...
			continue;
		}
		//�����Ȃ�������
		itr++;
		//else�ł�����
	}
}

//----------------------------------
//�@�\�F�A�j���[�V����
//�����F���@���G�ƏՓ�or�e���G�ɏՓ˂������̏ꏊ
//�ߒl�F�Ȃ�
//----------------------------------
void Effect2D::Add(D3DXVECTOR3 pos)
{
	EffectData d;
	d.anm = 0;					//�A�j���[�V�����̊�_��0����
	d.position = pos;			//�|�W�V�����͈���pos��n��
	effectData.push_back(d);
}