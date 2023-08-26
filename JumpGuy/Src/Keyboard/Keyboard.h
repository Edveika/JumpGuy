#pragma once
#pragma comment (lib,"dxguid.lib") 
#pragma comment (lib,"dinput8.lib")
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <stdexcept>

class Keyboard
{
private:
	LPDIRECTINPUT8 m_direct_input;
	LPDIRECTINPUTDEVICE8 m_direct_input_dev;
	char m_buffer[256];

private:
	bool create_direct_input_object(HINSTANCE hInstance);
	bool create_direct_input_device();
	bool set_data_format();
	bool acquire_access();

public:
	Keyboard(HINSTANCE hInstance);
	~Keyboard();

	void get_input();
	bool is_pressed(int key);
};