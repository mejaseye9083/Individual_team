/*===========================================================================================
概　略：ゲームの操作（入力処理）
作成日：2015.08.24
更新日：2015.08.24
制作者：藤原順基
=============================================================================================*/
#pragma once

#define DIRECTINPUT_VERSION 0x800 //DirectInput のバージョン設定

//---------------インクルード-----------------------
//#include "Global.h"
#include <dinput.h>
#include "XInput.h"

//----------ライブラリファイルのロード--------------
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"Xinput.lib")

//----------------------定数宣言--------------------
#define SAFE_RELEASE(p)        { if(p != NULL) { (p)->Release(); (p) = NULL; } }

//------------インプットクラスの定義----------------
class Input
{
	LPDIRECTINPUT8        pDinput;			//DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8  pKeyDevice;		//デバイスオブジェクト（キーボード）
	BYTE               keyState[256];		//各キーの状態
	BYTE               prevkeyState[256];

	XINPUT_STATE    controllerState[4];			//コントローラーの状態
	XINPUT_STATE    prevcontrollerState[4];		//コントローラーが刺さっているかの条件式に使う

	int c_error[4];

public:
	Input();
	~Input();
	HRESULT Init(HWND hWnd);				//DirectInputの準備

	HRESULT Update();						//各入力デバイスの状態を取得
	BOOL  IsKeyPush(DWORD keyCode);			//任意のキーが押されているかチェック
	BOOL  IsKeyTap(DWORD keyCode);			//任意のキーが押された瞬間をチェック
	BOOL  IsKeyRelease(DWORD keyCode);

	BOOL  IsPadButtonPush(DWORD buttonCode, int ID = 0);    //コントローラーのボタンが押されているか
	BOOL  IsPadButtonTap(DWORD buttonCode, int ID = 0);
	BOOL  IsPadButtonRelease(DWORD buttonCode, int ID = 0);

	float GetPadLStickX(int ID = 0);					//左スティックの左右の傾き
	float GetPadLStickY(int ID = 0);					//左スティックの上下の傾き
	float GetPadRStickX(int ID = 0);					//右スティックの左右の傾き
	float GetPadRStickY(int ID = 0);					//右スティックの上下の傾き

	float GetPadLTrigger(int ID = 0);					//左トリガーの押し込み
	float GetPadRTrigger(int ID = 0);					//右トリガーの押し込み

	void  Vibration(float, float);						//振動エフェクト
};