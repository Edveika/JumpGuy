#pragma once

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
	LPDIRECT3DDEVICE9 _pd3dDevice;
	LPDIRECT3D9 _pD3D;
	HINSTANCE _hInst;
	HWND _wndHandle;
	std::vector<SupportedScreenSettings*> _supportedScreenSettings;

public:
	DirectX9(HINSTANCE hInstance)
	{
		// Initialize the window
		if (!InitWindow(hInstance))
			MessageBoxA(NULL, "Failed to create a window!", NULL, NULL);

		// Initialize direct3d
		if (!InitDirect3D())
			MessageBoxA(NULL, "Failed to init Direct3D!", NULL, NULL);
	}
	~DirectX9()
	{
		_pd3dDevice->Release();
		_pD3D->Release();
		for (size_t i = 0; i < _supportedScreenSettings.size(); ++i)
			delete _supportedScreenSettings[i];
	}

public:
	// Getters
	int GetWindowWidth() { return WINDOW_WIDTH; }
	int GetWindowHeight() { return WINDOW_HEIGHT; }
	LPDIRECT3DDEVICE9 GetDevice() { return _pd3dDevice; }

private:
	bool InitWindow(HINSTANCE hInstance);
	bool InitDirect3D();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};