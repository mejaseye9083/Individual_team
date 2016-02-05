/*===========================================================================================
�T�@���F�摜��ǂݍ���ŕ\������
�쐬���F2015.08.24
�X�V���F2015.08.25
����ҁF��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Sprite.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Sprite::Sprite()
{
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Sprite::~Sprite()
{
	//����菇�͍Ōォ�珇�ɏ����Ă���
	SAFE_RELEASE(pTexture);		//�e�N�X�`���I�u�W�F�N�g
	SAFE_RELEASE(pSprite);		//�X�v���C�g�I�u�W�F�N�g
}

//----------------------------------
//�@�\�F�摜�f�[�^�̓ǂݍ���
//�����FfileName	�摜�t�@�C���̖��O
//�ߒl�F����or���s
//----------------------------------
HRESULT Sprite::Load(char* fileName)
{
	// �u�X�v���C�g�I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateSprite(g_pDevice, &pSprite)))
	{
		MessageBox(0, "�X�v���C�g�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	//�u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 
		NULL, NULL, NULL, &pTexture)))
	{
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", fileName, MB_OK);
		return E_FAIL;
	}

	//�e�N�X�`���̃T�C�Y���擾
	D3DSURFACE_DESC  d3dds;
	pTexture->GetLevelDesc(0, &d3dds);
	texSize.x = d3dds.Width;
	texSize.y = d3dds.Height;

	return S_OK;
}

//----------------------------------
//�@�\�F�摜�̕\��
//�����Fdata	�摜�̕\���ʒu��T�C�Y���w�肷��
//�ߒl�F�Ȃ�
//----------------------------------
void Sprite::Draw(SpriteData* data)
{
	//�����n��
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//�T�C�Y���ȗ������ꍇ�͉摜�T�C�Y���g��
	if (data->size.x == -999) { data->size.x = texSize.x; }
	if (data->size.y == -999) { data->size.y = texSize.y; }

	//�`��ʒu
	D3DXVECTOR3 position = data->pos;

	//�؂蔲���͈�
	RECT cut = { data->cut.x, data->cut.y, data->cut.x+data->size.x, data->cut.y+data->size.y };

	//��_
	D3DXVECTOR3 center = data->center;
	center.z = 0;
	if (center.x == -999 && center.y == -999)      //���ݒ�̏ꍇ�͉摜�̒��S
	{
		center.x = data->size.x / 2;
		center.y = data->size.y / 2;
	}

	//�g��s��
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, data->scale.x, data->scale.y, 1);

	//��]�s��
	D3DXMATRIX matRotate;
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(data->angle));

	//�ړ��s��
	D3DXMATRIX matTrans;
	//pos�̒l��center�̒l�𑫂��A����ɃX�P�[�����{�ɂ���B
	D3DXMatrixTranslation(&matTrans, data->pos.x + center.x * data->scale.x, data->pos.y + center.y * data->scale.y, 0);


	//�����s��
	D3DXMATRIX matWorld = matScale * matRotate * matTrans;

	//�X�v���C�g�ɍs���K�p
	pSprite->SetTransform(&matWorld);

	center.x += data->pos.x;
	center.y += data->pos.y;

	//�`��
	//�\���ł��Ȃ��`��������̂Œ��ӂ��ׂ�
	pSprite->Draw(pTexture, &cut, &center, &position, 
		D3DCOLOR_ARGB(data->color.a, data->color.r, data->color.g, data->color.b));

	//�����I���
	pSprite->End();
}
