#pragma once
#pragma comment (lib,"dxguid.lib") 
#pragma comment (lib,"dinput8.lib")
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

class Mouse
{
private:
	LPDIRECTINPUT8 _directInputObj;
	LPDIRECTINPUTDEVICE8 _directInputDevice;
	DIMOUSESTATE _mouseState;

public:
	Mouse(HINSTANCE hInstance)
	{
		// Create direct input object
		if (!CreateDirectInputObject(hInstance))
			MessageBoxA(NULL, "Failed to create Direct input object!", NULL, NULL);

		// Create direct input device
		if (!CreateDirectInputDevice())
			MessageBoxA(NULL, "Failed to create Direct input device!", NULL, NULL);

		// Set direct input format
		if (!SetDataFormat())
			MessageBoxA(NULL, "Failed to set data format!", NULL, NULL);

		// Acquire access
		if (!AcquireAccess())
			MessageBoxA(NULL, "Failed to acquire access!", NULL, NULL);
	}
	~Mouse()
	{
		_directInputObj->Release();
		_directInputDevice->Release();
	}

public:
	void GetInput();
	bool IsPressed(int button);
	int TravelX();
	int TravelY();
	int TravelZ();

private:
	bool CreateDirectInputObject(HINSTANCE hInstance);
	bool CreateDirectInputDevice();
	bool SetDataFormat();
	bool AcquireAccess();
};