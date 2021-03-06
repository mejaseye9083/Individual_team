/*===========================================================================================
概　要:敵の移動・攻撃パターン
作成日:2015.11.05
更新日:2017.01.27
制作者:藤原順基
=============================================================================================*/

#pragma once
//---------------インクルード-----------------------
#include "Global.h"
#include "UnitBase.h"
#include "Effect2D.h"
#include "Stage.h"

//現在のスクロールの位置(PlayScene.cppで宣言済み)
extern D3DXVECTOR3 g_stageScrollPosition;

extern BOOL g_Stopflg;

class Enemy : public UnitBase
{
	Sprite *spt;		//スプライトクラスのポインタ宣言
	Effect2D *fvx;		//エフェクト2Dクラスのポインタ宣言

	int anime;			//アニメーションの基点
	int direction;		//向いてる方向
	int asiba;

	//エネミーの移動値を入れる
	int moveE;

	//エネミーの初期地点を記憶させる変数
	D3DXVECTOR3 FixedPosition;
	BOOL ladderflg = FALSE;
	BOOL jumpBlock = FALSE;
	BOOL isGround = FALSE;


	int BMPIkun = 0;		//バグる原因を抑える番兵君


	//--------------プレイヤーの状態------------------
	enum
	{
		DEFAULT,		//通常
		LOOK_ENEMY,		//攻撃範囲に入った時
	}state;
	//------------------------------------------------

	//--------------プレイヤーの向き------------------
	enum MOVE_SCENE
	{
		MS_DOWN,
		MS_LEFT,
		MS_RIGHT,
		MS_UP,
	};
	//------------------------------------------------

	int jump;						//ジャンプする時に代入する変数
	int jcount;						//連続ジャンプ抑制

	HRESULT Move(Stage* stage);		//移動
	HRESULT Shot();					//攻撃

public:
	Enemy();
	~Enemy();

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

	void Kill();

};

