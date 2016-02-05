/*===========================================================================================
概　要:ステージの設定
作成日:2015.10.21
更新日:2016.01.13
制作者:藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"
#include "UnitBase.h"
#include "Effect2D.h"
#include "Block.h"
#include "item.h"

//-------------------定数宣言-----------------------
#define GROUND_HEIGHT		400		//地面の高さ（着地する高さ）

#define STAGE_WIDTH		40		//画面の最大幅
#define STAGE_HEIGHT	45		//画面の最大高さ
/*これらにブロックサイズをかける*/

//現在のスクロールの位置(PlayScene.cppで宣言済み)
extern D3DXVECTOR3 g_stageScrollPosition;

class Stage : public UnitBase
{
	Sprite spt[3];			//スプライトクラスのポインタ宣言
	//Effect2D *fvx;		//エフェクト2Dクラスのポインタ宣言

	Audio *audio;

	BOOL Def = TRUE;		//BGMのループのため

	Sprite	pict;			//画像
	DWORD	table[STAGE_WIDTH][STAGE_HEIGHT];

public:
	Stage();
	~Stage();

	//----------------------------------
	//機能：画像・音楽データの読み込み
	//引数：なし
	//戻値：成功or失敗
	//----------------------------------
	HRESULT Load();

	//----------------------------------
	//機能：更新処理
	//引数：なし
	//戻値：成功
	//----------------------------------
	HRESULT Update();

	//----------------------------------
	//機能：当たり判定
	//引数：pTarget		敵の情報
	//戻値：成功
	//----------------------------------
	HRESULT Hit(UnitBase* pTarget);

	//----------------------------------
	//機能：描画処理
	//引数：なし
	//戻値：成功or失敗
	//----------------------------------
	HRESULT Render();

	//マップを生成
	void Create();
	int GetToComma(char data[], int* index);

	//--------------------アクセス関数--------------
	int GetChip(int x, int y){ return table[x][y]; }
};