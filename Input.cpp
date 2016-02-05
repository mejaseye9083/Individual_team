/*===========================================================================================
�T�@���F�Q�[���̑���i���͏����j
�쐬���F2015.08.24
�X�V���F2015.08.24
����ҁF��������
=============================================================================================*/

//---------�C���N���[�h-------------
#include "Input.h"

//----------------------------------
//�@�\�F�R���X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
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
//�@�\�F�f�X�g���N�^
//�����F�Ȃ�
//�ߒl�F�Ȃ�
//----------------------------------
Input::~Input()
{
	//�L�[�{�[�h�̃A�N�Z�X�������
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	//DirectInput���
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(pDinput);
}

//----------------------------------
//�@�\�F�C���v�b�g�̏����ݒ�
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Input::Init(HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&pDinput, NULL)))

	{
		return E_FAIL;
	}

	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(pDinput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������x���̐ݒ�
	if (FAILED(pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F����or���s
//----------------------------------
HRESULT Input::Update()
{
	for (int i = 0; i < 4; i++)
	{
		memcpy(&prevcontrollerState[i], &controllerState[i], sizeof(controllerState[i]));
	}
	//�R���g���[���[�̏�Ԃ��擾
	for (int i = 0; i < 4; i++)
	{
		c_error[i] = XInputGetState(i, &controllerState[i]);
	}

	// �f�o�C�X�̃A�N�Z�X�����擾����
	HRESULT hr = pKeyDevice->Acquire();
	memcpy(prevkeyState, keyState, sizeof(keyState));
	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		//�S�ẴL�[�̏�Ԃ��擾
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		return S_OK;
	}
	return E_FAIL;
}

//------------------------�L�[�{�̉���������--------------------------

//----------------------------------
//�@�\�F���������ǂ����̔���
//�����FkeyCode			�L�[�����������ǂ����̔���
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Input::IsKeyPush(DWORD keyCode)
{
	if (keyState[keyCode] & 0x80)
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}

//----------------------------------
//�@�\�F�������u��
//�����FkeyCode			�L�[�����������ǂ����̔���
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Input::IsKeyTap(DWORD keyCode)
{
	//!�͔��΂̈Ӗ��������A�`����Ȃ�������̌`
	if (!(prevkeyState[keyCode] & 0x80) && (keyState[keyCode] & 0x80))
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}

//----------------------------------
//�@�\�F�������u��
//�����FkeyCode			�L�[�����������ǂ����̔���
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Input::IsKeyRelease(DWORD keyCode)
{
	if (!(keyState[keyCode] & 0x80) && (prevkeyState[keyCode] & 0x80))
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}


//--------------------------�p�b�h�̉���������------------------------

//----------------------------------
//�@�\�F���������ǂ����̔���
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Input::IsPadButtonPush(DWORD buttonCode, int ID)
{
	if (c_error[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	if (controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE; //�����Ă�
	}
	return FALSE;   //�����ĂȂ�
}

//----------------------------------
//�@�\�F�������u��
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Input::IsPadButtonTap(DWORD buttonCode, int ID)
{
	if (c_error[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	//!�͔��΂̈Ӗ��������A�`����Ȃ�������̌`
	if (!(prevcontrollerState[ID].Gamepad.wButtons & buttonCode) && (controllerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}

//----------------------------------
//�@�\�F�������u��
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�FTRUEorFALSE
//----------------------------------
BOOL Input::IsPadButtonRelease(DWORD buttonCode, int ID)
{
	if (c_error[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	if (!(controllerState[ID].Gamepad.wButtons & buttonCode) && (prevcontrollerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}


//-----------------------�X�e�B�b�N�̌X�����m-------------------------

//----------------------------------
//�@�\�F���X�e�B�b�N�̏㉺�̓���
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�F�X���
//----------------------------------
float Input::GetPadLStickX(int ID)
{

	float value = controllerState[ID].Gamepad.sThumbLX;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}

//----------------------------------
//�@�\�F���X�e�B�b�N�̍��E�̓���
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�F�X���
//----------------------------------
float Input::GetPadLStickY(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbLY;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}

//----------------------------------
//�@�\�F�E�X�e�B�b�N�̏㉺�̓���
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�F�X���
//----------------------------------
float Input::GetPadRStickX(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbRX;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}

//----------------------------------
//�@�\�F�E�X�e�B�b�N�̍��E�̓���
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�F�X���
//----------------------------------
float Input::GetPadRStickY(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbRY;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32768;
}


//---------------------�g���K�[�̉������݌��m--------------------------
//----------------------------------
//�@�\�F���g���K�[�̉������݋�̌��m
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�F�������݋
//----------------------------------
float Input::GetPadLTrigger(int ID)
{
	float value = controllerState[ID].Gamepad.bLeftTrigger;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;
	}

	return value / 255;
}

//----------------------------------
//�@�\�F�E�g���K�[�̉������݋�̌��m
//�����FID				�R���g���[���[�̔ԍ�
//�ߒl�F�������݋
//----------------------------------
float Input::GetPadRTrigger(int ID)
{
	float value = controllerState[ID].Gamepad.bRightTrigger;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;
	}

	return value / 255;
}

//----------------------------------
//�@�\�F�o�C�u���[�V��������
//�����FleftSpeed	���̃��[�^�[�̑���
//�����FrightSpeed	�E�̃��[�^�[�̑���
//�ߒl�F�X���
//----------------------------------
void Input::Vibration(float leftSpeed, float rightSpeed)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftSpeed * 065535;  // �����[�^�[
	vibration.wRightMotorSpeed = rightSpeed * 65535; // �E���[�^�[
	XInputSetState(0, &vibration);
}