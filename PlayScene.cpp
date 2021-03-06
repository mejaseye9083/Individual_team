/*===========================================================================================
概　略：プレイシーンを扱う処理
作成日：2015.05.29
更新日：2015.10.22
制作者：藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"

//現在のスクロール位置
D3DXVECTOR3 g_stageScrollPosition;
BOOL	g_Stopflg = FALSE;

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
PlayScene::PlayScene()
{
	Create();
}

HRESULT PlayScene::Create()
{
	unit.push_back(new Stage);
	//ユニットを追加
	unit.push_back(new Enemy);
	unit.push_back(new Player);
	
	return S_OK;
}