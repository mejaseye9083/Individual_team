/*=======================================================================================================
�T�@���F�l�p���|���S������邽�߂̊֐������
�쐬���F2015.09.07
�X�V���F2015.09.15
����ҁF��������
=======================================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"

//--------------�x�[�V�b�N���b�V���N���X�̒�`----------------
class BasicMesh
{
	//���b�V���̏�������\����(LP�Ȃ̂Ń|�C���^�Ƃ��Ĉ���)
	LPD3DXMESH   pMesh;

	//�}�e���A���̏��������\����
	D3DMATERIAL9 material;

	//Create�̒��ŌĂԂ̂ŁA�v���C�x�[�g�ɂ��Ă���
	HRESULT InitMaterial();
public:
	BasicMesh();
	~BasicMesh();

	HRESULT Create(DWORD type);
	HRESULT Draw(D3DXMATRIX* matWorld);

};

//��{�ƂȂ�`���܂Ƃ߂�����
enum
{
   BMESH_BOX,
   BMESH_SPHERE,
   BMESH_CYLINDER,
   BMESH_TORUS,
   BMESH_TEAPOT
};