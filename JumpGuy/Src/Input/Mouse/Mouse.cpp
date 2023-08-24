#include "../../Includes.h"

void Mouse::GetInput()
{
	HRESULT hr = _directInputDevice->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState);

	if (FAILED(hr))
	{
		hr = _directInputDevice->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = _directInputDevice->Acquire();
			continue;
		}
	}
}

int Mouse::TravelX()
{
	return _mouseState.lX;
}

int Mouse::TravelY()
{
	return _mouseState.lY;
}

int Mouse::TravelZ()
{
	return _mouseState.lZ;
}

bool Mouse::IsPressed(int button)
{
	if (_mouseState.rgbButtons[button] & 0x80) return true;
	
	return false;
}

bool Mouse::CreateDirectInputObject(HINSTANCE hInstance)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInputObj, NULL))) return false;

	return true;
}

bool Mouse::CreateDirectInputDevice()
{
	if (FAILED(_directInputObj->CreateDevice(GUID_SysMouse, &_directInputDevice, NULL))) return false;

	return true;
}

bool Mouse::SetDataFormat()
{
	if (FAILED(_directInputDevice->SetDataFormat(&c_dfDIMouse))) return false;

	return true;
}

bool Mouse::AcquireAccess()
{
	HRESULT Aqcuire(VOID);

	if (FAILED(_directInputDevice->Acquire())) return false;

	return true;
}