/*===========================================================================================
概　略：Mayaで作ったモデルを表示できるようにする
作成日：2015.09.14
更新日：2015.10.14
制作者：藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"
#include <fbxsdk.h>

#pragma comment(lib,"libfbxsdk-mt.lib")

//レイキャスト用構造体
struct RayCastData
{
	D3DXVECTOR3 start;	//レイ発射位置
	D3DXVECTOR3 dir;	//レイの向きベクトル
	float		dist;	//衝突点までの距離
	D3DXVECTOR3 normal;	//衝突点の法線
	D3DXVECTOR3 pos;	//衝突位置
	BOOL		hit;	//レイが当たったか
};

//-------------Fbxクラスの定義----------------------
class Fbx
{
	//
	struct Vertex
	{
		D3DXVECTOR3 pos;		//表示する位置と大きさ
		D3DXVECTOR3 normal;		//法線情報
		D3DXVECTOR2  uv;		//画像のどこを表示するか
	};


	FbxManager*		manager;		//FBXの機能を管理するマネージャー
	FbxImporter*	importer;		//モデルのインポーター
	FbxScene*		scene;			//開いたシーンを管理する

	int	vertexCount;							//頂点の数を数える変数
	int polygonCount;							//
	int indexCount;								//
	int materialCount;							//
	int* polygonCountOfMaterial;				//

	IDirect3DVertexBuffer9		*pVertexBuffer;	//バッファ
	IDirect3DIndexBuffer9		**pIndexBuffer;
	D3DMATERIAL9			    *pMaterial;
	LPDIRECT3DTEXTURE9			*pTexture;

	HRESULT		CheckNode(FbxNode* pNode);		//ノード（情報）のチェック
	HRESULT		CheckMesh(FbxMesh* pMesh);
	

public:
	Fbx();
	~Fbx();
	HRESULT Load(char* fileName);
	HRESULT Draw(D3DXMATRIX* matWorld);
	BOOL	RayCast(RayCastData *data);

};