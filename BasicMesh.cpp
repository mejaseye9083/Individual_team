/*=======================================================================================================
概　略：四角いポリゴンを作るための関数を作る
作成日：2015.09.07
更新日：2015.09.08
制作者：藤原順基
=======================================================================================================*/

//---------------インクルード-----------------------
#include "BasicMesh.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
BasicMesh::BasicMesh()
{
	pMesh = 0;
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
BasicMesh::~BasicMesh()
{
	SAFE_RELEASE(pMesh);
}

//----------------------------------
//機能：メッシュを作成する関数
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT BasicMesh::Create(DWORD type)
{
	/*------立方体のメッシュを作成する----------
	if (FAILED(D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL)))
	------------------------------------------*/


	/*------------スフィア（○）を作る----------
	第二引数は半径、正面から見た面の数、上から見た分割数、となる。
	これらを設定して球体の滑らかさを作る
	if (FAILED(D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL)))
	------------------------------------------*/

	/*---------------円柱を作る-----------------
	if (FAILED(D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 2, &pMesh, NULL)))
	------------------------------------------*/


	/*--------トーラス（ドーナツ型）を作る------
	第二引数はドーナツの太さ、円の大きさ、横から見た分割数、正面から見た分割数
	if (FAILED(D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL)))
	------------------------------------------*/
	
	//オマケ。DirectXには光の当たり具合、角度、影等を試せる関数が存在する
	//それがティーポットである。
	//if (FAILED(D3DXCreateTeapot(g_pDevice, &pMesh, NULL)))
	//{
	//	MessageBox(0, "メッシュの作成に失敗しました", "BasicMesh", MB_OK);
	//	return E_FAIL;
	//}

	//エラー処理用、一つにまとめる事で余計な手間を省ける
	HRESULT hr;


	//各モデルを詰め合わせたスイッチ文
	switch (type)
	{
	case BMESH_BOX:
		hr = D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL);
		break;

	case BMESH_SPHERE:
		hr = D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL);
		break;

	case BMESH_CYLINDER:
		hr = D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 2, &pMesh, NULL);
		break;

	case BMESH_TORUS:
		hr = D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL);
		break;

	case BMESH_TEAPOT:
		hr = D3DXCreateTeapot(g_pDevice, &pMesh, NULL);
		break;
	}

	//どれが失敗してもhrにまとまっているので確認が楽
	if (FAILED(hr))
	{
		MessageBox(0, "メッシュの作成に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//下のほうで作ったInitMaterialを呼び出す。（呼び忘れ防止）
	InitMaterial();

	return S_OK;
}

//----------------------------------
//機能：
//引数：ワールド座標
//戻値：成功or失敗
//----------------------------------
HRESULT BasicMesh::Draw(D3DXMATRIX* matWorld)
{
	//ラインクラスと同様に、テクスチャが貼られると困るので貼れないようにする。
	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		MessageBox(0, "テクスチャのリセットに失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//こちらも別途ワールド行列が必要となる、内容的にはQuadと同じ
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "ワールド行列の設定に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//質感を設定する
	//光を反射しないものは認識することが出来ないので、そこを設定する
	if (FAILED(g_pDevice->SetMaterial(&material)))
	{
		MessageBox(0, "マテリアルの設定に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	//上記で作成した立方体メッシュを描画する
	if (FAILED(pMesh->DrawSubset(0)))
	{
		MessageBox(0, "メッシュの描画に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//機能：マテリアルの設定(質感を作る)
//引数：なし
//戻値：成功or失敗
//追記：呼び忘れると困るので、上記にあるCreateで呼び出すようにする
//----------------------------------
HRESULT BasicMesh::InitMaterial()
{
	//ヘッダーで宣言したマテリアル構造体を初期化する。
	ZeroMemory(&material, sizeof(D3DMATERIAL9));

	//全部1.0fにすると真っ白になる
	//Diffuseとは拡散反射の事を指す。（目に見える光の色のこと）
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;
	/*
	林檎は紅い、それは林檎が赤の光を拡散反射するから目に見えるときには紅く見える。
	つまり物体の色と拡散反射される色は似たようなものだといえる
	*/

	return S_OK;
}