/*===========================================================================================
概　略：エフェクトをアニメーションさせて表示する
作成日：2015.08.25
更新日：2015.08.25
制作者：藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Effect2D.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Effect2D::Effect2D()
{
	size = D3DXVECTOR2(128, 128);
	speed = 0.2f;
	spt = 0;
	spt = new Sprite;


}

//----------------------------------
//機能：デストラクタ（newの解放処理）
//引数：なし
//戻値：なし
//----------------------------------
Effect2D::~Effect2D()
{
	SAFE_DELETE(spt);
}

//----------------------------------
//機能：画像の読み込み処理
//引数：fileName	画像ファイルの名前
//引数：spd			アニメーション速度
//戻値：成功or失敗
//----------------------------------
HRESULT Effect2D::Load(char* fileName, D3DXVECTOR2 pctSize, float spd)
{
	if (FAILED(spt->Load(fileName)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：なし
//----------------------------------
void Effect2D::Draw()
{
	SpriteData data;
	data.size = size;

	//イテレーター（カーソル）の生成
	std::list<EffectData>::iterator	 itr;

	//イテレーターに最小値を当てはめ、最大値になるまで繰り返す
	for (itr = effectData.begin(); itr != effectData.end();)
	{

		//イテレーターの中にある番地のposition
		//(*itr) = effectDataとなる。*itr.positionだと(*itr.position)と同じ意味になる
		//(*itr).position == itr->positionとなる。ポインタはアロー演算子で指定できる
		data.pos = (*itr).position;
		data.cut.x = (int)(*itr).anm * data.size.x;
		spt->Draw(&data);
		(*itr).anm += speed;

		D3DXVECTOR2 pctSize = spt->GetSize();

		if ((*itr).anm > pctSize.x / size.y)
		{
			//次の値がないと分からないので、itrに結果をぶち込む
			itr = effectData.erase(itr);
			//消せたときは...
			continue;
		}
		//消せなかったら
		itr++;
		//elseでもいい
	}
}

//----------------------------------
//機能：アニメーション
//引数：自機が敵と衝突or弾が敵に衝突した時の場所
//戻値：なし
//----------------------------------
void Effect2D::Add(D3DXVECTOR3 pos)
{
	EffectData d;
	d.anm = 0;					//アニメーションの基点は0から
	d.position = pos;			//ポジションは引数posを渡す
	effectData.push_back(d);
}