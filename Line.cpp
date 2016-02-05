/*===========================================================================================
概　略：描画関連の作成(カメラの向いてる向きのチェック等)
作成日：2015.08.31
更新日：2015.09.02
制作者：藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Line.h"

//----------------------------------
//機能：描画処理(カラー無し)
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Line::Draw(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	//頂点バッファ
	D3DXVECTOR3 point[2];		//始点と終点の配列
	point[0] = start;			//始点
	point[1] = end;				//終点

	//頂点ストリーム（各頂点がどのような情報を持ってるか）を設定する
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ)))				//D3DFVF_XYZは位置情報を表す
	{
		//失敗したらメッセージボックスを表示する
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	//単位行列（何もしない行列）
	D3DXMATRIX mReset;
	//ここで設定しておくことで他の行列に巻き込まれなくなる
	D3DXMatrixIdentity(&mReset);
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, &mReset)))
	{
		MessageBox(0, "ワールド行列のリセットに失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		MessageBox(0, "テクスチャのリセットに失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	//ラインが黒くなるのでそれの対策
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "ライトの設定に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	//描画する（DrawPrimitiveUP）
	//引数は「種類」「表示する数」「頂点情報」「1頂点のデータサイズ」
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, point, sizeof(D3DXVECTOR3))))
	{
		MessageBox(0, "ラインの描画に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}

	//最初にライトをきっておけばラインが描画される際には光の影響を受けない
	//その後、ライトを付ければラインが描画された後なので何の問題もない
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "ライトの設定に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//機能：描画処理(カラー付き)
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Line::Draw(D3DXVECTOR3 start, D3DXVECTOR3 end, DWORD color)
{
	//D3DXVECTOR3じゃ色までは入らない、そこでひとつの構造体にする
	struct Vertex
	{
		D3DXVECTOR3 pos;
		DWORD color;
	};

	//始点と終点なので二つ必要
	Vertex point[2];

	//各頂点の情報をセットする（引数で受け取ったものも）
	point[0].pos = start;			//始点の位置
	point[0].color = color;
	point[1].pos = end;				//終点の位置
	point[1].color = color;
	/*最終的に上記の部分がマヤで作ったモデルを適応させる*/

	//頂点ストリーム（各頂点がどのような情報を持ってるか）を設定する
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE)))
	{
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	//単位行列（何もしない行列）
	D3DXMATRIX mReset;
	//ここで設定しておくことで他の行列に巻き込まれなくなる
	D3DXMatrixIdentity(&mReset);
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, &mReset)))
	{
		MessageBox(0, "ワールド行列のリセットに失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	//描画する（DrawPrimitiveUP）
	//引数は「種類」「表示する数」「頂点情報」「構造体のデータサイズ」
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, point, sizeof(Vertex))))
	{
		MessageBox(0, "ラインの描画に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

