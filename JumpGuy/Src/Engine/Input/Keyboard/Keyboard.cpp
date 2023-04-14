#include "../../../Includes.h"

void Keyboard::GetInput()
{
	if (FAILED(_directInputDevice->GetDeviceState(sizeof(_buffer), (LPVOID)&_buffer)))
	{
		MessageBoxA(NULL, "Failed to get input from a direct input device!", NULL, NULL);
	}
}

bool Keyboard::IsPressed(int key)
{
	if (_buffer[key] & 0x80) return true;

	return false;
}

bool Keyboard::CreateDirectInputObject(HINSTANCE hInstance)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInputObj, NULL))) return false;

	return true;
}

bool Keyboard::CreateDirectInputDevice()
{
	if (FAILED(_directInputObj->CreateDevice(GUID_SysKeyboard, &_directInputDevice, NULL))) return false;

	return true;
}

bool Keyboard::SetDataFormat()
{
	if (FAILED(_directInputDevice->SetDataFormat(&c_dfDIKeyboard))) return false;

	return true;
}

bool Keyboard::AcquireAccess()
{
	HRESULT Aqcuire(VOID);

	if (FAILED(_directInputDevice->Acquire())) return false;

	return true;
}