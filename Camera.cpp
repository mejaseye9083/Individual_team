/*===========================================================================================
概　略：カメラ（視点）の設定
作成日：2015.08.25
更新日：2015.10.13
制作者：藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Camera.h"

//----------------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------------
Camera::Camera()
{
	pos = D3DXVECTOR3(0,5,-10);			//カメラの位置
	target = D3DXVECTOR3(0, 0, 0);		//焦点位置
	upVec = D3DXVECTOR3(0, 1, 0);		//上方向のベクトル

	angle = 60;									//画角(ラジアン)
	aspect = (float)WINDOW_WIDTH/WINDOW_HEIGHT;	//アスペクト比(画面の幅÷高さ)
	nearClip = 0.1f;							//近クリッピング
	farClip = 1000.0f;							//遠クリッピング
}

//----------------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------------
Camera::~Camera()
{
}

//----------------------------------------
//機能：ビュー行列（カメラの位置や向き）を作る
//引数：なし
//戻値：成功or失敗
//----------------------------------------
HRESULT Camera::SetView()
{
	//引数は全てポインタで渡す。カメラの位置、焦点位置、上方向ベクトルの順
	D3DXMatrixLookAtLH(&view, &pos, &target, &upVec);

	//失敗したらメッセージボックスを表示する
	if (FAILED(g_pDevice->SetTransform(D3DTS_VIEW, &view)))
	{
		MessageBox(0, "ビュー行列の設定に失敗しました", "Camera", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------------
//機能：射影行列
//引数：なし
//戻値：成功or失敗
//----------------------------------------
HRESULT Camera::SetProjection()
{
	//ラジアンは円周率のこと,円周率は3.14の事を指す。
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(angle), aspect, nearClip, farClip);
	
	//失敗したらメッセージボックスを表示する
	if (FAILED(g_pDevice->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		MessageBox(0, "射影行列の設定に失敗しました", "Camera", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------------
//機能：SetViewとSetProjectionを呼ぶ
//引数：なし
//戻値：成功or失敗
//----------------------------------------
HRESULT Camera::Update()
{
	//失敗したらエラーを返す
	if (FAILED(SetView()))
	{
		return E_FAIL;
	}

	//失敗したらエラーを返す
	if (FAILED(SetProjection()))
	{
		return E_FAIL;
	}

	//どちらも無事成功し、視点を変えることが出来たら成功を返す
	return S_OK;
}