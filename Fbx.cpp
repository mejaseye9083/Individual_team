/*===========================================================================================
概　略：Mayaで作ったモデルを表示できるようにする
作成日：2015.09.14
更新日：2015.10.14
制作者：藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Fbx.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Fbx::Fbx()
{
	//各変数の中身を初期化する
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
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Fbx::~Fbx()
{
	//最後に作った方からリリースしていく
	//今回のポインタはLPから始まるわけではないが、この解放の仕方をする
	for (int i = 0; i < materialCount; i++)
	{
		SAFE_RELEASE(pIndexBuffer[i]);
	}
	SAFE_DELETE_ARRAY(pIndexBuffer);

	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE_ARRAY(pMaterial);
	SAFE_DELETE_ARRAY(polygonCountOfMaterial);
	SAFE_DELETE_ARRAY(pTexture);

	//マネージャーのデリート処理を決める
	manager->Destroy();
}

//----------------------------------
//機能：画像の読み込み処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Fbx::Load(char* fileName)
{
	//マネージャーの作成
	if (FAILED(manager = FbxManager::Create()))
	{
		MessageBox(0, "マネージャーの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}
	//インポーターの作成
	if (FAILED(importer = FbxImporter::Create(manager, "")))
	{
		MessageBox(0, "インポーターの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}
	//シーンの作成
	if (FAILED(scene = FbxScene::Create(manager, "")))
	{
		MessageBox(0, "FBXシーンの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}

	importer->Initialize(fileName);			//インポーターを使ってFBXファイルを開く
	importer->Import(scene);				//開いたファイルをシーンに渡す
	importer->Destroy();					//インポーターを削除する

	//カレントディレクトリを変える前に、現在のカレントディレクトリを取得しておく
	//全ての読み込み処理が終わったら元に戻す（じゃないと他に影響が出る）
	char  defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);	//ここが移動前の場所

	char    dir[MAX_PATH];
	_splitpath_s(fileName, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);
	SetCurrentDirectory(dir);

	FbxNode* rootNode = scene->GetRootNode();
	int childCount = rootNode->GetChildCount();


	//此処のCheckNodeが読まれない場合はファイルが見つかってないことがある
	//mayaで作ったデータが全角文字の場合は読まれないことも分かった
	for (int i = 0; childCount > i; i++)
	{
		CheckNode(rootNode->GetChild(i));
	}

	//元のカレントディレクトリに戻す
	SetCurrentDirectory(defaultCurrentDir);
	return S_OK;
}

//----------------------------------
//機能：メッシュノードのチェック
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Fbx::CheckNode(FbxNode* pNode)
{

	FbxNodeAttribute* attr = pNode->GetNodeAttribute();

	if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//メッシュノードだった
		materialCount = pNode->GetMaterialCount();
		pMaterial = new D3DMATERIAL9[materialCount];
		pTexture = new LPDIRECT3DTEXTURE9[materialCount];

		//一つずつマテリアル情報を取得していく
		//複数あるので、ループ処理必須
		for (int i = 0; i < materialCount; i++)
		{
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pLambert->Diffuse;
			FbxDouble3  ambient = pLambert->Ambient;

			//初期化大事
			ZeroMemory(&pMaterial[i], sizeof(D3DMATERIAL9));

			//ディフューズ（ポリゴンの色）の読み込み
			pMaterial[i].Diffuse.r = (float)diffuse[0];
			pMaterial[i].Diffuse.g = (float)diffuse[1];
			pMaterial[i].Diffuse.b = (float)diffuse[2];
			pMaterial[i].Diffuse.a = 1.0f;

			//アンビエント（環境光）の読み込み
			pMaterial[i].Ambient.r = (float)ambient[0];
			pMaterial[i].Ambient.g = (float)ambient[1];
			pMaterial[i].Ambient.b = (float)ambient[2];
			pMaterial[i].Ambient.a = 1.0f;

			//指定した画像ファイルを読み込んで配列に入れる
			//マテリアル毎に処理される。
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
					MessageBox(0, "テクスチャの作成に失敗しました", name, MB_OK);
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
//機能：メッシュそのもののチェック
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT  Fbx::CheckMesh(FbxMesh* pMesh)
{
	FbxVector4* pVertexPos = pMesh->GetControlPoints();

	vertexCount = pMesh->GetControlPointsCount();
	Vertex* pVertexList = new Vertex[vertexCount];

	//頂点数文繰り返しす
	for (int i = 0; vertexCount > i; i++)
	{
		pVertexList[i].pos.x = (float)pVertexPos[i][0];
		pVertexList[i].pos.y = (float)pVertexPos[i][1];
		pVertexList[i].pos.z = (float)pVertexPos[i][2];
	}

	//頂点情報が一つとは限らない、なので何個入ってるか割り出す
	FbxStringList uvSetName;
	pMesh->GetUVSetNames(uvSetName);
	int uvCount = uvSetName.GetCount();
	bool unMap = false;
	FbxVector2  uv;


	//ポリゴンの数を数える
	polygonCount = pMesh->GetPolygonCount();

	for (int i = 0; i < polygonCount; i++)
	{
		int   startIndex = pMesh->GetPolygonVertexIndex(i);
		for (int j = 0; j < 3; j++)
		{
			int index = pMesh->GetPolygonVertices()[startIndex + j];

			//法線の取得をする位置
			FbxVector4  Normal;
			pMesh->GetPolygonVertexNormal(i, j, Normal);
			//PolygonVertexNormalで取得したのをぶち込む
			//第1引数にポリゴンの番号、第2数に頂点の番号を指定すると、その法線を第3引数に入れてくれる。
			pVertexList[index].normal = D3DXVECTOR3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			//先にカウントした頂点の数だけ生成する
			for (int k = 0; k < uvCount; k++)
			{
				FbxString name = uvSetName.GetStringAt(k);
				pMesh->GetPolygonVertexUV(i, j, name, uv, unMap);
				pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));
			}

			/*ここで取得してたのを頂点情報の辺りに変えた、よってここはコメントアウトする。
			//ここでUV情報を取得する
			//int uvIndex = pMesh->GetTextureUVIndex(i, j, FbxLayerElement::eTextureDiffuse);
			//FbxVector2  uv = pMesh->GetLayer(0)->GetUVs()->GetDirectArray().GetAt(uvIndex);
			//pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));
			*/
		}
	}

	if (FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertex)* vertexCount, 0, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
		D3DPOOL_MANAGED, &pVertexBuffer, 0)))
	{
		MessageBox(0, "頂点バッファの作成に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	//ここで入れたものが頂点バッファになる
	Vertex *pVertexCopy;

	//ここのロックは"他から弄れないように"ロックをかけている
	if (FAILED(pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0)))
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	memcpy(pVertexCopy, pVertexList, sizeof(Vertex)*vertexCount);

	//終わったらアンロックし元の状態へ戻す
	if (FAILED(pVertexBuffer->Unlock()))
	{
		MessageBox(0, "頂点バッファのアンロックに失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(pVertexList);


	//ポリゴンの頂点数を数える
	indexCount = pMesh->GetPolygonVertexCount();

	//インデックス情報を読み込む前に配列化する。
	pIndexBuffer = new IDirect3DIndexBuffer9*[materialCount];
	polygonCountOfMaterial = new int[materialCount];

	//インデックス情報を読み込んでインデックスバッファを作成する
	for (int i = 0; i < materialCount; i++)
	{
		int* pIndexList = new int[indexCount];

		//インデックス番号を付ける
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

		//インデックスバッファそのものを作成する
		if (FAILED(g_pDevice->CreateIndexBuffer(sizeof(int)* indexCount, 0, D3DFMT_INDEX32,
			D3DPOOL_MANAGED, &pIndexBuffer[i], 0)))
		{
			MessageBox(0, "インデックスバッファの作成に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		DWORD *pIndexCopy;

		//余計な操作をさせないためにロックする
		if (FAILED(pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0)))
		{
			MessageBox(0, "インデックスバッファのロックに失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		memcpy(pIndexCopy, pIndexList, sizeof(int)* indexCount);

		//アンロックして元の状態に戻す
		if (FAILED(pIndexBuffer[i]->Unlock()))
		{
			MessageBox(0, "インデックスバッファのアンロックに失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}
		/*ここまでしてやっとインデックスバッファの完成！*/

		SAFE_DELETE_ARRAY(pIndexList);
	}
	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Fbx::Draw(D3DXMATRIX* matWorld)
{
	//ワールドの位置情報を設定する
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "ワールド行列の設定に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	//頂点ストリームの設定する
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)))
	{
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	//頂点バッファの設定する
	//関数はSetStreamSource(セットストリームソース)
	if (FAILED(g_pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(Vertex))))
	{
		MessageBox(0, "頂点バッファの設定に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < materialCount; i++)
	{
		//インデックスバッファを設定する
		if (FAILED(g_pDevice->SetIndices(pIndexBuffer[i])))
		{
			MessageBox(0, "インデックスバッファの設定に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		if (FAILED(g_pDevice->SetMaterial(&pMaterial[i])))
		{
			MessageBox(0, "マテリアルの設定に失敗しました", "Fbx", MB_OK);
			 return E_FAIL;
		}

		if (FAILED(g_pDevice->SetTexture(0, pTexture[i])))
		{
			MessageBox(0, "テクスチャのセットに失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		//メッシュの描画処理を行う、mayaの場合大体三角形を使うのでトライアングルを使う
		if (FAILED(g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, indexCount, 0, polygonCountOfMaterial[i])))
		{
			MessageBox(0, "メッシュの描画に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}
	}
	return S_OK;
}

BOOL Fbx::RayCast(RayCastData *data)
{
	data->dist = 99999.0f;
	data->hit = false;


	//頂点バッファにアクセスできるように
	Vertex *pVertexCopy;
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);


	//マテリアル毎
	for (DWORD i = 0; i < materialCount; i++)
	{
		//インデックスバッファにアクセスできるように
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//そのマテリアルのポリゴン毎
		for (DWORD j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3頂点分
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;

			//ぶつかってるかチェック
			float dist, u, v;
			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], &data->start, &data->dir, &u, &v, &dist);

			//今まで一番近い？
			if (hit && dist < data->dist)
			{
				data->hit = true;
				data->dist = dist;
				data->pos = ver[0] + u * (ver[1] - ver[0]) + v * (ver[2] - ver[0]);
			}
		}

		//インデックスバッファ使用終了
		pIndexBuffer[i]->Unlock();
	}

	//頂点バッファ使用終了
	pVertexBuffer->Unlock();

	return data->hit;
}