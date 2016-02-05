/*===========================================================================================
概　略：画像を読み込んで表示する
作成日：2015.08.25
更新日：2015.08.25
制作者：藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"

//クラスの中で作っちゃダメ(この構造体はスプライトを使うために作るから)
//----------画像の表示に使うデータ----------------
struct SpriteData
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR2 size;		//サイズ
	D3DXVECTOR2 cut;		//切り抜き位置
	D3DXVECTOR2 center;		//基準点
	D3DXVECTOR2 scale;		//拡大率

	float    angle;			//角度

	//色(struct構造体)
	struct
	{
		int a, r, g, b;
	}color;

	//コンストラクタ
	SpriteData()
	{
		pos	 =	 D3DXVECTOR3(0, 0, 0);
		size =	 D3DXVECTOR2(-999, -999);
		cut  =	 D3DXVECTOR2(0, 0);
		center = D3DXVECTOR2(-999, -999);
		color  = { 255, 255, 255, 255 };
		scale  = D3DXVECTOR2(1, 1);
		angle = 0;
	}
};

//----------スプライトクラスの定義----------------
class Sprite
{
	//スプライトオブジェクト
	LPD3DXSPRITE pSprite;
	
	//テクスチャオブジェクト
	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャのサイズ
	D3DXVECTOR2  texSize;
public:
	Sprite();
	~Sprite();

	//画像の読み込み,失敗したという情報は戻り値で返す
	HRESULT Load(char* fileName);
	//画像の表示
	void Draw(SpriteData* data);
	D3DXVECTOR2 GetSize(){ return texSize; }
};