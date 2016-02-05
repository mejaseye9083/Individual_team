/*===========================================================================================
概　略：各シーンの親クラス
作成日：2015.05.29
更新日：2015.08.25
制作者：藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include <vector>
#include "UnitBase.h"

//―――――――――――――――――――――
//	各シーンの親クラス
//―――――――――――――――――――――
class SceneBase
{
protected:
	//シーンに登場するユニット
	std::vector<UnitBase*> unit;

public:
	//デストラクタ
	virtual ~SceneBase();

	//読み込み処理
	HRESULT Load();

	//衝突判定処理
	HRESULT Hit();

	//更新処理
	HRESULT Update();

	//描画処理
	//引数：hdc	デバイスコンテキストハンドル
	HRESULT Render();
};

