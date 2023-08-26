#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <stdexcept>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

struct SupportedScreenSettings
{
	D3DXVECTOR2 m_supportedScreenDimensions;
	int m_supportedScreenRefreshRate;
};

class DirectX9
{
private:
	LPDIRECT3DDEVICE9 m_d3d_device;
	LPDIRECT3D9 m_d3d;
	HINSTANCE m_instance;
	HWND m_wnd_handle;
	std::vector<SupportedScreenSettings*> m_supported_screen_settings;

private:
	bool init_window(HINSTANCE hInstance);
	bool init_direct_3d();
	static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	DirectX9(HINSTANCE hInstance);
	~DirectX9();

	// Getters
	int get_window_width() { return WINDOW_WIDTH; }
	int get_window_height() { return WINDOW_HEIGHT; }
	LPDIRECT3DDEVICE9 get_device() { return m_d3d_device; }
};