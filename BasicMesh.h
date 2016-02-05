/*=======================================================================================================
概　略：四角いポリゴンを作るための関数を作る
作成日：2015.09.07
更新日：2015.09.15
制作者：藤原順基
=======================================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"

//--------------ベーシックメッシュクラスの定義----------------
class BasicMesh
{
	//メッシュの情報を入れる構造体(LPなのでポインタとして扱う)
	LPD3DXMESH   pMesh;

	//マテリアルの情報を扱う構造体
	D3DMATERIAL9 material;

	//Createの中で呼ぶので、プライベートにしておく
	HRESULT InitMaterial();
public:
	BasicMesh();
	~BasicMesh();

	HRESULT Create(DWORD type);
	HRESULT Draw(D3DXMATRIX* matWorld);

};

//基本となる形をまとめたもの
enum
{
   BMESH_BOX,
   BMESH_SPHERE,
   BMESH_CYLINDER,
   BMESH_TORUS,
   BMESH_TEAPOT
};