#pragma once

class Keyboard
{
private:
	LPDIRECTINPUT8 _directInputObj;
	LPDIRECTINPUTDEVICE8 _directInputDevice;
	char _buffer[256];

public:
	Keyboard(HINSTANCE hInstance)
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
	~Keyboard()
	{
		_directInputObj->Release();
		_directInputDevice->Release();
	}

public:
	void GetInput();
	bool IsPressed(int key);

private:
	bool CreateDirectInputObject(HINSTANCE hInstance);
	bool CreateDirectInputDevice();
	bool SetDataFormat();
	bool AcquireAccess();
};