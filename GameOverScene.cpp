/*===========================================================================================
概　要:ゲームオーバーシーン ~ゲームオーバーになったら終了(現時点)~
作成日:2016.01.19
更新日:2016.01.19
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "GameOverScene.h"
#include "GameOver.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
GameOverScene::GameOverScene()
{
	unit.push_back(new GameOver);
}
