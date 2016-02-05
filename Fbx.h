/*===========================================================================================
�T�@���FMaya�ō�������f����\���ł���悤�ɂ���
�쐬���F2015.09.14
�X�V���F2015.10.14
����ҁF��������
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include <fbxsdk.h>

#pragma comment(lib,"libfbxsdk-mt.lib")

//���C�L���X�g�p�\����
struct RayCastData
{
	D3DXVECTOR3 start;	//���C���ˈʒu
	D3DXVECTOR3 dir;	//���C�̌����x�N�g��
	float		dist;	//�Փ˓_�܂ł̋���
	D3DXVECTOR3 normal;	//�Փ˓_�̖@��
	D3DXVECTOR3 pos;	//�Փˈʒu
	BOOL		hit;	//���C������������
};

//-------------Fbx�N���X�̒�`----------------------
class Fbx
{
	//
	struct Vertex
	{
		D3DXVECTOR3 pos;		//�\������ʒu�Ƒ傫��
		D3DXVECTOR3 normal;		//�@�����
		D3DXVECTOR2  uv;		//�摜�̂ǂ���\�����邩
	};


	FbxManager*		manager;		//FBX�̋@�\���Ǘ�����}�l�[�W���[
	FbxImporter*	importer;		//���f���̃C���|�[�^�[
	FbxScene*		scene;			//�J�����V�[�����Ǘ�����

	int	vertexCount;							//���_�̐��𐔂���ϐ�
	int polygonCount;							//
	int indexCount;								//
	int materialCount;							//
	int* polygonCountOfMaterial;				//

	IDirect3DVertexBuffer9		*pVertexBuffer;	//�o�b�t�@
	IDirect3DIndexBuffer9		**pIndexBuffer;
	D3DMATERIAL9			    *pMaterial;
	LPDIRECT3DTEXTURE9			*pTexture;

	HRESULT		CheckNode(FbxNode* pNode);		//�m�[�h�i���j�̃`�F�b�N
	HRESULT		CheckMesh(FbxMesh* pMesh);
	

public:
	Fbx();
	~Fbx();
	HRESULT Load(char* fileName);
	HRESULT Draw(D3DXMATRIX* matWorld);
	BOOL	RayCast(RayCastData *data);

};