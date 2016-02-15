/*===========================================================================================
概　要:弾の発射したときの軌道
作成日:2015.06.26
更新日:2016.02.15
制作者:藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"
#include "Enemy.h"
#include "Effect2D.h"

//現在のスクロールの位置(PlayScene.cppで宣言済み)
extern D3DXVECTOR3 g_stageScrollPosition;

//--------------バレットクラスの定義----------------
class Bullet
{
	Sprite *spt;
	Effect2D *fvx;
	//位置
	D3DXVECTOR3 position;

	//弾の状態　（未発射かどうか）
	BOOL isShot;
	BOOL unControl;

	int bulletlife;		//弾だけに偶にフラグが消えないので意図的に消すための変数

	int dirflg = NULL;
	//--------------プレイヤーの向き------------------
	enum MOVE_SCENE
	{
		MS_DOWN,
		MS_LEFT,
		MS_RIGHT,
		MS_UP,
	};
	//------------------------------------------------

public:
	Bullet();
	~Bullet();

	HRESULT Load();
	
	//更新
	//HRESULT Update();

	//更新
	HRESULT Update();
	//衝突判定
	BOOL Hit(Enemy* pEnemy);

	//描画処理
	//引数HDC デバイスコンテキストハンドル
	//戻り値:なし
	HRESULT Render();

	//弾の発射処理
	BOOL Shot(D3DXVECTOR3 playerPos,int dir);

};