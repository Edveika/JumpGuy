#pragma once
#include <Windows.h>
#include <memory>

#include "../DirectX/DirectX9.h"
#include "../Graphics/Graphics.h"
#include "../XAudio2/XAudio2.h"
#include "../Keyboard/Keyboard.h"

class Engine
{
private:
	HINSTANCE m_instance;
	std::shared_ptr<DirectX9> m_dx9;
	std::shared_ptr<Graphics> m_gfx;
	std::shared_ptr<XAudio2> m_audio;
	std::shared_ptr<Keyboard> m_kb;

public:
	Engine() {}
	Engine(HINSTANCE hInstance)
	{
		m_instance = hInstance;

		m_dx9 = std::make_shared<DirectX9>(hInstance);
		if (!m_dx9)
		{
			MessageBoxA(NULL, "[ENGINE]: Failed to create DirectX9 obj!", NULL, NULL);
			throw std::invalid_argument("[ENGINE]: Directx9 obj not created");
		}

		m_gfx = std::make_shared<Graphics>(m_dx9->get_device());
		if (!m_gfx)
		{
			MessageBoxA(NULL, "[ENGINE]: Failed to create Graphics obj!", NULL, NULL);
			throw std::invalid_argument("[ENGINE]: Graphics obj not created");
		}

		m_audio = std::make_shared<XAudio2>();
		if (!m_audio)
		{
			MessageBoxA(NULL, "[ENGINE]: Failed to create XAudio2 obj!", NULL, NULL);
			throw std::invalid_argument("[ENGINE]: XAudio2 obj not created");
		}

		m_kb = std::make_shared<Keyboard>(hInstance);
		if (!m_kb)
		{
			MessageBoxA(NULL, "[ENGINE]: Failed to create Keyboard obj!", NULL, NULL);
			throw std::invalid_argument("[ENGINE]: Keyboard obj not created");
		}
	}
	
	std::shared_ptr<DirectX9> get_directx9_ptr() { return m_dx9; }
	std::shared_ptr<Graphics> get_graphics_ptr() { return m_gfx; }
	std::shared_ptr<XAudio2> get_audio_ptr() { return m_audio; }
	std::shared_ptr<Keyboard> get_keyboard_ptr() { return m_kb; }
};