/*===========================================================================================
�T�@���F�摜��ǂݍ���ŕ\������
�쐬���F2015.08.25
�X�V���F2015.08.25
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"

//�N���X�̒��ō������_��(���̍\���̂̓X�v���C�g���g�����߂ɍ�邩��)
//----------�摜�̕\���Ɏg���f�[�^----------------
struct SpriteData
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR2 size;		//�T�C�Y
	D3DXVECTOR2 cut;		//�؂蔲���ʒu
	D3DXVECTOR2 center;		//��_
	D3DXVECTOR2 scale;		//�g�嗦

	float    angle;			//�p�x

	//�F(struct�\����)
	struct
	{
		int a, r, g, b;
	}color;

	//�R���X�g���N�^
	SpriteData()
	{
		pos	 =	 D3DXVECTOR3(0, 0, 0);
		size =	 D3DXVECTOR2(-999, -999);
		cut  =	 D3DXVECTOR2(0, 0);
		center = D3DXVECTOR2(-999, -999);
		color  = { 255, 255, 255, 255 };
		scale  = D3DXVECTOR2(1, 1);
		angle = 0;
	}
};

//----------�X�v���C�g�N���X�̒�`----------------
class Sprite
{
	//�X�v���C�g�I�u�W�F�N�g
	LPD3DXSPRITE pSprite;
	
	//�e�N�X�`���I�u�W�F�N�g
	LPDIRECT3DTEXTURE9 pTexture;

	//�e�N�X�`���̃T�C�Y
	D3DXVECTOR2  texSize;
public:
	Sprite();
	~Sprite();

	//�摜�̓ǂݍ���,���s�����Ƃ������͖߂�l�ŕԂ�
	HRESULT Load(char* fileName);
	//�摜�̕\��
	void Draw(SpriteData* data);
	D3DXVECTOR2 GetSize(){ return texSize; }
};