#pragma once
#include <Windows.h>
#include <memory>

#include "../DirectX/DirectX9.h"
#include "../Graphics/Graphics.h"
#include "../XAudio2/XAudio2.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../Input/Mouse/Mouse.h"

class Engine
{
private:
	HINSTANCE _hInstance;
	std::shared_ptr<DirectX9> _dx9;
	std::shared_ptr<Graphics> _gfx;
	std::shared_ptr<XAudio2> _audio;
	std::shared_ptr<Keyboard> _kb;
	std::shared_ptr<Mouse> _mouse;

public:
	Engine() {}
	Engine(HINSTANCE hInstance)
	{
		_hInstance = hInstance;

		_dx9 = std::make_shared<DirectX9>(hInstance);
		if (!_dx9)
			MessageBoxA(NULL, "Failed to create DirectX9 obj!", NULL, NULL);

		_gfx = std::make_shared<Graphics>(_dx9->GetDevice());
		if (!_gfx)
			MessageBoxA(NULL, "Failed to create Graphics obj!", NULL, NULL);

		_audio = std::make_shared<XAudio2>();
		if (!_audio)
			MessageBoxA(NULL, "Failed to create XAudio2 obj!", NULL, NULL);

		_kb = std::make_shared<Keyboard>(hInstance);
		if (!_kb)
			MessageBoxA(NULL, "Failed to create Keyboard obj!", NULL, NULL);

		_mouse = std::make_shared<Mouse>(hInstance);
		if (!_mouse)
			MessageBoxA(NULL, "Failed to create Mouse obj!", NULL, NULL);
	}

public:
	std::shared_ptr<DirectX9> GetDirectX9Ptr() { return _dx9; }
	std::shared_ptr<Graphics> GetGraphicsPtr() { return _gfx; }
	std::shared_ptr<XAudio2> GetAudioPtr() { return _audio; }
	std::shared_ptr<Keyboard> GetKeyboardPtr() { return _kb; }
	std::shared_ptr<Mouse> GetMousePtr() { return _mouse; }
};