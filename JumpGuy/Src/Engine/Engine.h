#pragma once

class Engine
{
private:
	HINSTANCE _hInstance;
	DirectX9* _dx9;
	Graphics* _gfx;
	XAudio2* _audio;
	Keyboard* _kb;
	Mouse* _mouse;

public:
	Engine(HINSTANCE hInstance)
	{
		_hInstance = hInstance;

		if ((_dx9 = new DirectX9(_hInstance)) == NULL)
			MessageBoxA(NULL, "Failed to create DirectX9 obj!", NULL, NULL);

		if ((_gfx = new Graphics(_dx9->GetDevice())) == NULL)
			MessageBoxA(NULL, "Failed to create Graphics obj!", NULL, NULL);

		if ((_audio = new XAudio2()) == NULL)
			MessageBoxA(NULL, "Failed to create XAudio2 obj!", NULL, NULL);

		if ((_kb = new Keyboard(_hInstance)) == NULL)
			MessageBoxA(NULL, "Failed to create Keyboard obj!", NULL, NULL);

		if ((_mouse = new Mouse(_hInstance)) == NULL)
			MessageBoxA(NULL, "Failed to create Mouse obj!", NULL, NULL);
	}
	~Engine()
	{
		delete _dx9;
		delete _gfx;
		delete _audio;
		delete _kb;
		delete _mouse;
	}

public:
	DirectX9* GetDirectX9Ptr() { return _dx9; }
	Graphics* GetGraphicsPtr() { return _gfx; }
	XAudio2* GetAudioPtr() { return _audio; }
	Keyboard* GetKeyboardPtr() { return _kb; }
	Mouse* GetMousePtr() { return _mouse; }
};