/*===========================================================================================
�T�@���FMaya�ō�������f����\���ł���悤�ɂ���
�쐬���F2015.09.14
�X�V���F2015.10.14
����ҁF��������
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Fbx.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Fbx::Fbx()
{
	//�e�ϐ��̒��g������������
	vertexCount = 0;
	polygonCount = 0;
	indexCount = 0;
	materialCount = 0;
	polygonCountOfMaterial = NULL;
	pTexture = NULL;

	pVertexBuffer = NULL;
	pIndexBuffer = NULL;
	pMaterial = NULL;
}

//----------------------------------
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Fbx::~Fbx()
{
	//�Ō�ɍ���������烊���[�X���Ă���
	//����̃|�C���^��LP����n�܂�킯�ł͂Ȃ����A���̉���̎d��������
	for (int i = 0; i < materialCount; i++)
	{
		SAFE_RELEASE(pIndexBuffer[i]);
	}
	SAFE_DELETE_ARRAY(pIndexBuffer);

	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE_ARRAY(pMaterial);
	SAFE_DELETE_ARRAY(polygonCountOfMaterial);
	SAFE_DELETE_ARRAY(pTexture);

	//�}�l�[�W���[�̃f���[�g���������߂�
	manager->Destroy();
}

//----------------------------------
//�@�\�F�摜�̓ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Fbx::Load(char* fileName)
{
	//�}�l�[�W���[�̍쐬
	if (FAILED(manager = FbxManager::Create()))
	{
		MessageBox(0, "�}�l�[�W���[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}
	//�C���|�[�^�[�̍쐬
	if (FAILED(importer = FbxImporter::Create(manager, "")))
	{
		MessageBox(0, "�C���|�[�^�[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}
	//�V�[���̍쐬
	if (FAILED(scene = FbxScene::Create(manager, "")))
	{
		MessageBox(0, "FBX�V�[���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	importer->Initialize(fileName);			//�C���|�[�^�[���g����FBX�t�@�C�����J��
	importer->Import(scene);				//�J�����t�@�C�����V�[���ɓn��
	importer->Destroy();					//�C���|�[�^�[���폜����

	//�J�����g�f�B���N�g����ς���O�ɁA���݂̃J�����g�f�B���N�g�����擾���Ă���
	//�S�Ă̓ǂݍ��ݏ������I������猳�ɖ߂��i����Ȃ��Ƒ��ɉe�����o��j
	char  defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);	//�������ړ��O�̏ꏊ

	char    dir[MAX_PATH];
	_splitpath_s(fileName, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);
	SetCurrentDirectory(dir);

	FbxNode* rootNode = scene->GetRootNode();
	int childCount = rootNode->GetChildCount();


	//������CheckNode���ǂ܂�Ȃ��ꍇ�̓t�@�C�����������ĂȂ����Ƃ�����
	//maya�ō�����f�[�^���S�p�����̏ꍇ�͓ǂ܂�Ȃ����Ƃ���������
	for (int i = 0; childCount > i; i++)
	{
		CheckNode(rootNode->GetChild(i));
	}

	//���̃J�����g�f�B���N�g���ɖ߂�
	SetCurrentDirectory(defaultCurrentDir);
	return S_OK;
}

//----------------------------------
//�@�\�F���b�V���m�[�h�̃`�F�b�N
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Fbx::CheckNode(FbxNode* pNode)
{

	FbxNodeAttribute* attr = pNode->GetNodeAttribute();

	if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//���b�V���m�[�h������
		materialCount = pNode->GetMaterialCount();
		pMaterial = new D3DMATERIAL9[materialCount];
		pTexture = new LPDIRECT3DTEXTURE9[materialCount];

		//����}�e���A�������擾���Ă���
		//��������̂ŁA���[�v�����K�{
		for (int i = 0; i < materialCount; i++)
		{
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pLambert->Diffuse;
			FbxDouble3  ambient = pLambert->Ambient;

			//�������厖
			ZeroMemory(&pMaterial[i], sizeof(D3DMATERIAL9));

			//�f�B�t���[�Y�i�|���S���̐F�j�̓ǂݍ���
			pMaterial[i].Diffuse.r = (float)diffuse[0];
			pMaterial[i].Diffuse.g = (float)diffuse[1];
			pMaterial[i].Diffuse.b = (float)diffuse[2];
			pMaterial[i].Diffuse.a = 1.0f;

			//�A���r�G���g�i�����j�̓ǂݍ���
			pMaterial[i].Ambient.r = (float)ambient[0];
			pMaterial[i].Ambient.g = (float)ambient[1];
			pMaterial[i].Ambient.b = (float)ambient[2];
			pMaterial[i].Ambient.a = 1.0f;

			//�w�肵���摜�t�@�C����ǂݍ���Ŕz��ɓ����
			//�}�e���A�����ɏ��������B
			FbxProperty  lProperty = pNode->GetMaterial(i)->FindProperty(FbxSurfaceMaterial::sDiffuse);
			FbxFileTexture* textureFile = lProperty.GetSrcObject<FbxFileTexture>(0);

			if (textureFile == NULL)
			{
				pTexture[i] = NULL;
			}
			else
			{
				const char* textureFileName = textureFile->GetFileName();

				char    name[_MAX_FNAME];
				char    ext[_MAX_EXT];
				_splitpath_s(textureFileName, NULL, 0, NULL, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, name, 0, 0, 0, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture[i])))
				{
					MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", name, MB_OK);
					return E_FAIL;
				}
			}

		}

		CheckMesh(pNode->GetMesh());

	}
	else
	{
		int childCount = pNode->GetChildCount();
		for (int i = 0; childCount > i; i++)
		{
			CheckNode(pNode->GetChild(i));

		}
	}

	return S_OK;
}

//----------------------------------
//�@�\�F���b�V�����̂��̂̃`�F�b�N
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT  Fbx::CheckMesh(FbxMesh* pMesh)
{
	FbxVector4* pVertexPos = pMesh->GetControlPoints();

	vertexCount = pMesh->GetControlPointsCount();
	Vertex* pVertexList = new Vertex[vertexCount];

	//���_�����J��Ԃ���
	for (int i = 0; vertexCount > i; i++)
	{
		pVertexList[i].pos.x = (float)pVertexPos[i][0];
		pVertexList[i].pos.y = (float)pVertexPos[i][1];
		pVertexList[i].pos.z = (float)pVertexPos[i][2];
	}

	//���_��񂪈�Ƃ͌���Ȃ��A�Ȃ̂ŉ������Ă邩����o��
	FbxStringList uvSetName;
	pMesh->GetUVSetNames(uvSetName);
	int uvCount = uvSetName.GetCount();
	bool unMap = false;
	FbxVector2  uv;


	//�|���S���̐��𐔂���
	polygonCount = pMesh->GetPolygonCount();

	for (int i = 0; i < polygonCount; i++)
	{
		int   startIndex = pMesh->GetPolygonVertexIndex(i);
		for (int j = 0; j < 3; j++)
		{
			int index = pMesh->GetPolygonVertices()[startIndex + j];

			//�@���̎擾������ʒu
			FbxVector4  Normal;
			pMesh->GetPolygonVertexNormal(i, j, Normal);
			//PolygonVertexNormal�Ŏ擾�����̂��Ԃ�����
			//��1�����Ƀ|���S���̔ԍ��A��2���ɒ��_�̔ԍ����w�肷��ƁA���̖@�����3�����ɓ���Ă����B
			pVertexList[index].normal = D3DXVECTOR3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			//��ɃJ�E���g�������_�̐�������������
			for (int k = 0; k < uvCount; k++)
			{
				FbxString name = uvSetName.GetStringAt(k);
				pMesh->GetPolygonVertexUV(i, j, name, uv, unMap);
				pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));
			}

			/*�����Ŏ擾���Ă��̂𒸓_���̕ӂ�ɕς����A����Ă����̓R�����g�A�E�g����B
			//������UV�����擾����
			//int uvIndex = pMesh->GetTextureUVIndex(i, j, FbxLayerElement::eTextureDiffuse);
			//FbxVector2  uv = pMesh->GetLayer(0)->GetUVs()->GetDirectArray().GetAt(uvIndex);
			//pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));
			*/
		}
	}

	if (FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertex)* vertexCount, 0, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
		D3DPOOL_MANAGED, &pVertexBuffer, 0)))
	{
		MessageBox(0, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "Fbx", MB_OK);
		return E_FAIL;
	}

	//�����œ��ꂽ���̂����_�o�b�t�@�ɂȂ�
	Vertex *pVertexCopy;

	//�����̃��b�N��"������M��Ȃ��悤��"���b�N�������Ă���
	if (FAILED(pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0)))
	{
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", "Fbx", MB_OK);
		return E_FAIL;
	}

	memcpy(pVertexCopy, pVertexList, sizeof(Vertex)*vertexCount);

	//�I�������A�����b�N�����̏�Ԃ֖߂�
	if (FAILED(pVertexBuffer->Unlock()))
	{
		MessageBox(0, "���_�o�b�t�@�̃A�����b�N�Ɏ��s���܂���", "Fbx", MB_OK);
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(pVertexList);


	//�|���S���̒��_���𐔂���
	indexCount = pMesh->GetPolygonVertexCount();

	//�C���f�b�N�X����ǂݍ��ޑO�ɔz�񉻂���B
	pIndexBuffer = new IDirect3DIndexBuffer9*[materialCount];
	polygonCountOfMaterial = new int[materialCount];

	//�C���f�b�N�X����ǂݍ���ŃC���f�b�N�X�o�b�t�@���쐬����
	for (int i = 0; i < materialCount; i++)
	{
		int* pIndexList = new int[indexCount];

		//�C���f�b�N�X�ԍ���t����
		int count = 0;
		for (int polygon = 0; polygon < polygonCount; polygon++)
		{
			int materialID = pMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(polygon);
			if (materialID == i)
			{
				for (int vertex = 0; vertex < 3; vertex++)
				{
					pIndexList[count++] = pMesh->GetPolygonVertex(polygon, vertex);
				}
			}
		}

		polygonCountOfMaterial[i] = count / 3;

		//�C���f�b�N�X�o�b�t�@���̂��̂��쐬����
		if (FAILED(g_pDevice->CreateIndexBuffer(sizeof(int)* indexCount, 0, D3DFMT_INDEX32,
			D3DPOOL_MANAGED, &pIndexBuffer[i], 0)))
		{
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "Fbx", MB_OK);
			return E_FAIL;
		}

		DWORD *pIndexCopy;

		//�]�v�ȑ���������Ȃ����߂Ƀ��b�N����
		if (FAILED(pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0)))
		{
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̃��b�N�Ɏ��s���܂���", "Fbx", MB_OK);
			return E_FAIL;
		}

		memcpy(pIndexCopy, pIndexList, sizeof(int)* indexCount);

		//�A�����b�N���Č��̏�Ԃɖ߂�
		if (FAILED(pIndexBuffer[i]->Unlock()))
		{
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̃A�����b�N�Ɏ��s���܂���", "Fbx", MB_OK);
			return E_FAIL;
		}
		/*�����܂ł��Ă���ƃC���f�b�N�X�o�b�t�@�̊����I*/

		SAFE_DELETE_ARRAY(pIndexList);
	}
	return S_OK;
}

//----------------------------------
//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Fbx::Draw(D3DXMATRIX* matWorld)
{
	//���[���h�̈ʒu����ݒ肷��
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);
		return E_FAIL;
	}

	//���_�X�g���[���̐ݒ肷��
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)))
	{
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);
		return E_FAIL;
	}

	//���_�o�b�t�@�̐ݒ肷��
	//�֐���SetStreamSource(�Z�b�g�X�g���[���\�[�X)
	if (FAILED(g_pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(Vertex))))
	{
		MessageBox(0, "���_�o�b�t�@�̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < materialCount; i++)
	{
		//�C���f�b�N�X�o�b�t�@��ݒ肷��
		if (FAILED(g_pDevice->SetIndices(pIndexBuffer[i])))
		{
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);
			return E_FAIL;
		}

		if (FAILED(g_pDevice->SetMaterial(&pMaterial[i])))
		{
			MessageBox(0, "�}�e���A���̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);
			 return E_FAIL;
		}

		if (FAILED(g_pDevice->SetTexture(0, pTexture[i])))
		{
			MessageBox(0, "�e�N�X�`���̃Z�b�g�Ɏ��s���܂���", "Fbx", MB_OK);
			return E_FAIL;
		}

		//���b�V���̕`�揈�����s���Amaya�̏ꍇ��̎O�p�`���g���̂Ńg���C�A���O�����g��
		if (FAILED(g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, indexCount, 0, polygonCountOfMaterial[i])))
		{
			MessageBox(0, "���b�V���̕`��Ɏ��s���܂���", "Fbx", MB_OK);
			return E_FAIL;
		}
	}
	return S_OK;
}

BOOL Fbx::RayCast(RayCastData *data)
{
	data->dist = 99999.0f;
	data->hit = false;


	//���_�o�b�t�@�ɃA�N�Z�X�ł���悤��
	Vertex *pVertexCopy;
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);


	//�}�e���A����
	for (DWORD i = 0; i < materialCount; i++)
	{
		//�C���f�b�N�X�o�b�t�@�ɃA�N�Z�X�ł���悤��
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//���̃}�e���A���̃|���S����
		for (DWORD j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3���_��
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;

			//�Ԃ����Ă邩�`�F�b�N
			float dist, u, v;
			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], &data->start, &data->dir, &u, &v, &dist);

			//���܂ň�ԋ߂��H
			if (hit && dist < data->dist)
			{
				data->hit = true;
				data->dist = dist;
				data->pos = ver[0] + u * (ver[1] - ver[0]) + v * (ver[2] - ver[0]);
			}
		}

		//�C���f�b�N�X�o�b�t�@�g�p�I��
		pIndexBuffer[i]->Unlock();
	}

	//���_�o�b�t�@�g�p�I��
	pVertexBuffer->Unlock();

	return data->hit;
}