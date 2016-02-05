/*===========================================================================================
概　略：ゲームの操作（入力処理）
作成日：2015.08.24
更新日：2015.08.24
制作者：藤原順基
=============================================================================================*/

//---------インクルード-------------
#include "Input.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Input::Input()
{
	ZeroMemory(keyState, sizeof(keyState));
	for (int i = 0; i < 4; i++)
	{
		c_error[i] = 0;
	}
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Input::~Input()
{
	//キーボードのアクセス権を解放
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	//DirectInput解放
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(pDinput);
}

//----------------------------------
//機能：インプットの初期設定
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Input::Init(HWND hWnd)
{
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&pDinput, NULL)))

	{
		return E_FAIL;
	}

	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(pDinput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL)))
	{
		return E_FAIL;
	}

	// デバイスをキーボードに設定
	if (FAILED(pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調レベルの設定
	if (FAILED(pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//機能：更新処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Input::Update()
{
	for (int i = 0; i < 4; i++)
	{
		memcpy(&prevcontrollerState[i], &controllerState[i], sizeof(controllerState[i]));
	}
	//コントローラーの状態を取得
	for (int i = 0; i < 4; i++)
	{
		c_error[i] = XInputGetState(i, &controllerState[i]);
	}

	// デバイスのアクセス権を取得する
	HRESULT hr = pKeyDevice->Acquire();
	memcpy(prevkeyState, keyState, sizeof(keyState));
	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		//全てのキーの状態を取得
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		return S_OK;
	}
	return E_FAIL;
}

//------------------------キーボの押した判定--------------------------

//----------------------------------
//機能：押したかどうかの判定
//引数：keyCode			キーを押したかどうかの判定
//戻値：TRUEorFALSE
//----------------------------------
BOOL Input::IsKeyPush(DWORD keyCode)
{
	if (keyState[keyCode] & 0x80)
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}

//----------------------------------
//機能：押した瞬間
//引数：keyCode			キーを押したかどうかの判定
//戻値：TRUEorFALSE
//----------------------------------
BOOL Input::IsKeyTap(DWORD keyCode)
{
	//!は反対の意味をさす、～じゃなかったらの形
	if (!(prevkeyState[keyCode] & 0x80) && (keyState[keyCode] & 0x80))
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}

//----------------------------------
//機能：離した瞬間
//引数：keyCode			キーを押したかどうかの判定
//戻値：TRUEorFALSE
//----------------------------------
BOOL Input::IsKeyRelease(DWORD keyCode)
{
	if (!(keyState[keyCode] & 0x80) && (prevkeyState[keyCode] & 0x80))
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}


//--------------------------パッドの押した判定------------------------

//----------------------------------
//機能：押したかどうかの判定
//引数：ID				コントローラーの番号
//戻値：TRUEorFALSE
//----------------------------------
BOOL Input::IsPadButtonPush(DWORD buttonCode, int ID)
{
	if (c_error[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	if (controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE; //押してる
	}
	return FALSE;   //押してない
}

//----------------------------------
//機能：押した瞬間
//引数：ID				コントローラーの番号
//戻値：TRUEorFALSE
//----------------------------------
BOOL Input::IsPadButtonTap(DWORD buttonCode, int ID)
{
	if (c_error[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	//!は反対の意味をさす、～じゃなかったらの形
	if (!(prevcontrollerState[ID].Gamepad.wButtons & buttonCode) && (controllerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}

//----------------------------------
//機能：離した瞬間
//引数：ID				コントローラーの番号
//戻値：TRUEorFALSE
//----------------------------------
BOOL Input::IsPadButtonRelease(DWORD buttonCode, int ID)
{
	if (c_error[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	if (!(controllerState[ID].Gamepad.wButtons & buttonCode) && (prevcontrollerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}


//-----------------------スティックの傾き検知-------------------------

//----------------------------------
//機能：左スティックの上下の動作
//引数：ID				コントローラーの番号
//戻値：傾き具合
//----------------------------------
float Input::GetPadLStickX(int ID)
{

	float value = controllerState[ID].Gamepad.sThumbLX;

	//デッドゾーン
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}

//----------------------------------
//機能：左スティックの左右の動作
//引数：ID				コントローラーの番号
//戻値：傾き具合
//----------------------------------
float Input::GetPadLStickY(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbLY;

	//デッドゾーン
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}

//----------------------------------
//機能：右スティックの上下の動作
//引数：ID				コントローラーの番号
//戻値：傾き具合
//----------------------------------
float Input::GetPadRStickX(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbRX;

	//デッドゾーン
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}

//----------------------------------
//機能：右スティックの左右の動作
//引数：ID				コントローラーの番号
//戻値：傾き具合
//----------------------------------
float Input::GetPadRStickY(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbRY;

	//デッドゾーン
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}


//---------------------トリガーの押し込み検知--------------------------
//----------------------------------
//機能：左トリガーの押し込み具合の検知
//引数：ID				コントローラーの番号
//戻値：押し込み具合
//----------------------------------
float Input::GetPadLTrigger(int ID)
{
	float value = controllerState[ID].Gamepad.bLeftTrigger;

	//デッドゾーン
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;
	}

	return value / 255;
}

//----------------------------------
//機能：右トリガーの押し込み具合の検知
//引数：ID				コントローラーの番号
//戻値：押し込み具合
//----------------------------------
float Input::GetPadRTrigger(int ID)
{
	float value = controllerState[ID].Gamepad.bRightTrigger;

	//デッドゾーン
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;
	}

	return value / 255;
}

//----------------------------------
//機能：バイブレーション処理
//引数：leftSpeed	左のモーターの速さ
//引数：rightSpeed	右のモーターの速さ
//戻値：傾き具合
//----------------------------------
void Input::Vibration(float leftSpeed, float rightSpeed)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftSpeed * 065535;  // 左モーター
	vibration.wRightMotorSpeed = rightSpeed * 65535; // 右モーター
	XInputSetState(0, &vibration);
}