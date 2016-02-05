/*===========================================================================================
概　略：プロジェクト全体で必要なものをまとめたファイル
作成日：2015.05.26
更新日：2016.01.19
制作者：藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include <windows.h>
#include <d3dx9.h>
#include "Input.h"
#include <vector>
#include <list>
#include "Audio.h"

//----------ライブラリファイルのロード--------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-------------------マクロ------------------
#define	SAFE_DELETE(p)		 {delete (p); (p) = NULL;}
#define	SAFE_DELETE_ARRAY(p) {delete[] (p);  (p) = NULL;}
#define SAFE_RELEASE(p)      { if(p != NULL) { (p)->Release(); (p) = NULL; } }

//-------------------定数宣言------------------
//ウィンドウサイズ
#define WINDOW_WIDTH	640	//ウィンドウの幅
#define WINDOW_HEIGHT	480	//ウィンドウ高さ

#define ENEMY_STATE		5	//敵の数
#define PLAYER_STATE	1	//自機
#define BULLET_SET		3	//出せる弾の最大数

//ブロック用の定数宣言
#define	BLOCK_CHIP	32

//プレイヤーのサイズ
#define PLAYER_SIZE		32

//-------------------グローバル変数------------------
//シーン
enum GAME_SCENE
{
	SC_PLAY,		//プレイシーン
	SC_TITLE,		//タイトルシーン
	SC_CLEAR,		//クリアシーン
	SC_GAMEOVER,	//ゲームオーバーシーン
	SC_MAX			//使うシーンは３つ、SC_MAXに入る値は3だからループ処理に使える
};

struct GLOBAL
{
	int mapW, mapH;
};
extern GLOBAL global;

extern GAME_SCENE	g_gameScene;		//実態はGame.cppで宣言


//Direct3Dデバイスオブジェクト
extern LPDIRECT3DDEVICE9 g_pDevice;

//入力処理（インプット）オブジェクト
extern Input* g_pInput;

//カメラオブジェクト
//カメラクラスはGlobal.hをインクルードしている。循環参照を避けるためにクラスを宣言する
class Camera;
extern Camera* g_pCamera;