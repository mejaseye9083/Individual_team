/*===========================================================================================
概　要:自機の設定
作成日:2015.10.15
更新日:2016.02.08
制作者:藤原順基
=============================================================================================*/

#pragma once
//---------------インクルード-----------------------
#include "Global.h"
#include "UnitBase.h"
#include "Effect2D.h"
#include "Stage.h"
#include "Bullet.h"

//現在のスクロールの位置(PlayScene.cppで宣言済み)
extern D3DXVECTOR3 g_stageScrollPosition;

#define PLAYER_DM 8			//1メモリ8なので、メモリを減らす際にダメージ×PLAYER_DM　的な
#define PLAYER_HP 20		//ロックマンの体力、ライフアップとかはまだ実装しない

class Player : public UnitBase
{
	Sprite *ply;			//スプライトクラスのポインタ宣言
	Sprite *life;			//ライフメモリ表示用スプライトクラス

	//Effect2D *fvx;		//エフェクト2Dクラスのポインタ宣言
	Audio *audio;

	int anime;				//アニメーションの基点
	int direction;			//向いてる方向
	float asiba;

	int hp;					//ロックマンの体力	
	int BMPIkun = 0;		//バグる原因を抑える番兵君

	int memory_break = 0;	

	BOOL ladderflg = FALSE;
	BOOL jumpBlock = FALSE;
	BOOL isGround = FALSE;


	//--------------プレイヤーの状態------------------
	enum
	{
		DEFAULT,		//通常
		DOWN_SCROLL,	//下へスクロール中
		UP_SCROLL,		//上へスクロール中
		LEFT_SCROLL,
		RIGHT_SCROLL,
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

	int jump;			//ジャンプする時に代入する変数
	int jcount;			//連続ジャンプ抑制

	HRESULT Move(Stage* stage);		//移動
	HRESULT Shot();					//攻撃

	Bullet bullet[BULLET_SET];		//発射できる弾の段数

	BOOL isShotKeyFlg = FALSE;		//発射したかどうかの判定

	void BulletUpdate();	//弾の更新

public:
	Player();
	~Player();

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

	void Reset();
	
};

