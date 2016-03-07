/*===========================================================================================
概　略：各ゲームユニットの親クラス
作成日：2015.10.19
更新日：2016.01.28
制作者：藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"
#include <typeinfo.h>
#include "Sprite.h"
#include "Audio.h"

static int testFlg = NULL;

//―――――――――――――――――――――
//	各ゲームユニットの親クラス
//―――――――――――――――――――――
class UnitBase
{
protected:
	//ユニットの位置
	D3DXVECTOR3	position;

	//当たり判定用(四角)
	RECT HitZone;

	int anime;			//アニメーションの基点
	int direction;		//向いてる方向

public:
	virtual ~UnitBase() { };
	//読み込み処理
	virtual HRESULT Load();

	//更新処理
	virtual HRESULT Update();

	//衝突判定処理
	//引数：pTarget	判定する相手ユニット
	virtual HRESULT Hit(UnitBase* pTarget);

	//描画処理
	//引数：無し	デバイスコンテキストハンドル
	virtual HRESULT Render();

	//現在の位置を取得
	//戻値：現在の位置
	D3DXVECTOR3 GetPos()	{ return position; }

	//もしかしたら使わないかも
	RECT GetZone()	{ return HitZone; }
};

