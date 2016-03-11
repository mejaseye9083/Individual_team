/*===========================================================================================
概　要:タイトル画面
作成日:2016.01.14
更新日:2016.01.19
制作者:藤原順基
===========================================================================================*/

//---------------インクルード-----------------------
#include "Title.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Title::Title()
{
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Title::~Title()
{
}

//----------------------------------
//機能：タイトル画像の読み込み処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Title::Load()
{
	if (FAILED(back.Load("pictures\\titleScene.jpg")))
	{
		return E_FAIL;
	}

	if (FAILED(manual.Load("pictures\\manual.png")))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//機能：更新処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Title::Update()
{
	//スペースキー及びコントローラーのAを押すとプレイシーンへ移行する
	if (g_pInput->IsKeyTap(DIK_E) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_A))
	{
		g_gameScene = SC_PLAY;
	}

	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Title::Render()
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

	SpriteData manualdata;
	manualdata.color.a = 255;
	manualdata.pos.x = 240;
	manualdata.pos.y = 400;
	manual.Draw(&manualdata);

	i++;
	return S_OK;
}
