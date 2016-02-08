/*===========================================================================================
概　要:クリア画面
作成日:2016.02.08
更新日:2016.02.08
制作者:藤原順基
===========================================================================================*/

//---------------インクルード-----------------------
#include "Clear.h"


//----------------------------------
//機能：コンストラクタ(特に無し)
//引数：なし
//戻値：成功
//----------------------------------
Clear::Clear()
{
}


//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：成功
//----------------------------------
Clear::~Clear()
{
}

//----------------------------------
//機能：画像やSE等の読み込み
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Clear::Load()
{
	if (FAILED(back.Load("pictures\\clear.bmp")))
	{
		return E_FAIL;
	}
}

//----------------------------------
//機能：更新処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Clear::Update()
{
	//キーボードのEキーでシーン変更
	if (g_pInput->IsKeyTap(DIK_E) /*|| g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_A)*/)
	{
		g_gameScene = SC_TITLE;
	}
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Clear::Render()
{
	//背景なので特に弄らず画面いっぱいに表示させる
	SpriteData backdata;
	//色と透明度
	backdata.color.a = 255;
	backdata.color.r = 255;
	backdata.color.g = 255;
	backdata.color.b = 255;
	backdata.pos.z = 0;
	back.Draw(&backdata);
}