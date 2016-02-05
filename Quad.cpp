/*=======================================================================================================
概　略：四角いポリゴンを作るための関数を作る
作成日：2015.09.01
更新日：2015.09.07
制作者：藤原順基
=======================================================================================================*/

//---------------インクルード-----------------------
#include "Quad.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Quad::Quad()
{
	pTexture = 0;
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Quad::~Quad()
{
	//-----LP~とついてる関数は必ず解放処理を書く
	SAFE_RELEASE(pTexture);
}

//----------------------------------
//機能：画像データの読み込み
//引数：fileName	画像ファイルの名前
//戻値：成功or失敗
//----------------------------------
HRESULT Quad::Load(char* fileName)
{
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		NULL, NULL, NULL, &pTexture)))
	{
		MessageBox(0, "テクスチャの作成に失敗しました", fileName, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


//----------------------------------
//機能：三角形を二つ組み合わせて四角を作る
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Quad::Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex)
{
	//D3DXVECTOR3じゃ色までは入らない、そこでひとつの構造体にする
	struct Vertex
	{
		D3DXVECTOR3 pos;
		DWORD color;
		D3DXVECTOR2    uv;
	};

	//三角形二つで四角形を作る
	Vertex point[] =
	{
		//一つ目の三角形
		D3DXVECTOR3(-0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 0),	//左上
		D3DXVECTOR3(0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 0),		//右上
		D3DXVECTOR3(-0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 1),	//左下
		//二つ目の三角形
		//D3DXVECTOR3(-0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255),		//左下
		//D3DXVECTOR3(0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255),			//右上
		D3DXVECTOR3(0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 1),	//右下
	};

	/*
	//三角形を最初に作るので頂点は三つ必要
	Vertex point[3] =
	{
		//各頂点の情報をセットする
		D3DXVECTOR3(-2, -1, 0), D3DCOLOR_ARGB(255, 255, 0, 0),
		D3DXVECTOR3(0, 1, 2), D3DCOLOR_ARGB(255, 0, 255, 0),
		D3DXVECTOR3(2, -1, 0), D3DCOLOR_ARGB(255, 0, 0, 255),
	};
	*/

	//------------------------------------------------------------------------
	//通常設定の場合、時計回りの順番で設定したらそれが表となる
	//上の頂点を下げた場合、このとき逆時計回りになるので裏側になる
	//裏側は基本的に見ることはないのが、カメラの位置を変えれば逆三角形に見える
	//------------------------------------------------------------------------

	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "ワールド行列の設定に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}


	//頂点ストリーム（各頂点がどのような情報を持ってるか）を設定する
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)))
	{
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	//テクスチャをセットする関数
	if (FAILED(g_pDevice->SetTexture(0, pTexture)))
	{
		MessageBox(0, "テクスチャのセットに失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	if (matTex != NULL)
	{
		//テクスチャ行列をセットする関数
		if (FAILED(g_pDevice->SetTransform(D3DTS_TEXTURE0, matTex)))
		{
			MessageBox(0, "テクスチャ行列の設定に失敗しました", "Quad", MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "ライトの設定に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	//描画する（DrawPrimitiveUP）プリミティブの描画ともいう
	//引数は「種類」「表示する数」「頂点情報」「構造体のデータサイズ」
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(Vertex))))
	{
		MessageBox(0, "ポリゴンの描画に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}
	//-----------------------------------------------------
	//Zの形に頂点を指定しても四角形が作れる
	//D3DPT_TRIANGLESTRIPを使う事でうまいこと表示してくれる
	//-----------------------------------------------------

	//ラインの時と同じ、ポリゴン表示の際に黒くならないようにライトを切る
	//描画がされたらライトを再度付けるようにする
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "ライトの設定に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}