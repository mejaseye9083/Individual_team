/*===========================================================================================
概　要:ゲームオーバー画面
作成日:2016.01.19
更新日:2016.01.19
制作者:藤原順基
===========================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "UnitBase.h"

//-----------ゲームオーバー-------------------------
class GameOver : public UnitBase
{
	Sprite spt;

public:
	GameOver();
	~GameOver();
	HRESULT Load();
	HRESULT Update();
	HRESULT Render();
};

