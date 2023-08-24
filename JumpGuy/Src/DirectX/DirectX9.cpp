#include "DirectX9.h"

bool DirectX9::InitWindow(HINSTANCE hInstance)
{
	// WNDCLASSEX struct describes how thw windows will look
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); // size of the struct
	wcex.style = CS_HREDRAW | CS_VREDRAW; // class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; //window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this 
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = 0; //icon for the application
	wcex.hIconSm = 0; // handle to the small icon
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // background color
	wcex.lpszMenuName = L"Jump Guy";    // name of menu resource 
	wcex.lpszClassName = L"Jump Guy";  // name of window class 
	RegisterClassEx(&wcex);
	
	// Create window
	_wndHandle = CreateWindowW
	(
		L"Jump Guy", // window class to use
		L"Jump Guy", // window title
		WS_EX_APPWINDOW | WS_VISIBLE | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		CW_USEDEFAULT, // starting x coordinate
		CW_USEDEFAULT, // starting y coordinate
		WINDOW_WIDTH, // window width(pixels)
		WINDOW_HEIGHT, // window height(pixels)
		NULL, // parrent window; NULL for desktop
		NULL, // menu for the application
		hInstance, // handle to application instance
		NULL // no values passed to the window
	);

	// Make sure that the window handle created is valid
	if (!_wndHandle)
	{
		return false;
	}

	// Render the window on the screen
	ShowWindow(_wndHandle, SW_SHOW);
	UpdateWindow(_wndHandle);

	// Return true, everything went ok
	return true;
}

bool DirectX9::InitDirect3D()
{
	_pD3D = NULL;
	_pd3dDevice = NULL;

	// Create the DirectX object
	if ((_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}

	// Gets adapter details
	D3DADAPTER_IDENTIFIER9 ident;
	_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &ident);

	// Collects the mode count this adapter has
	UINT numModes = _pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);

	// Use the collected mode count to gather info about each one
	for (size_t i = 0; i < numModes; ++i)
	{
		D3DDISPLAYMODE mode; // D3DDISPLAYMODE struct definition
		//char modeStr[255]; // Temp char array

		// Get the displaymode struct for this adapter mode
		_pD3D->EnumAdapterModes
		(
			D3DADAPTER_DEFAULT,
			D3DFMT_X8R8G8B8,
			i,
			&mode
		);

		SupportedScreenSettings* iSettings = new SupportedScreenSettings();
		iSettings->m_supportedScreenDimensions = { (float)mode.Width, (float)mode.Height };
		iSettings->m_supportedScreenRefreshRate = mode.RefreshRate;
		
		_supportedScreenSettings.push_back(iSettings);
	}

	// Fill the Present parameters struct
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.hDeviceWindow = _wndHandle;

	if (FAILED(_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT, // use the primary video adapter
		D3DDEVTYPE_REF, // create a device that uses a default Direct3D implementation
		_wndHandle,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, // ensures that your app runs on most hardware
		&d3dpp,
		&_pd3dDevice // stores the created device in pd3dDevice
	))) return false;
	
	return true;
}

LRESULT CALLBACK DirectX9::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messaged from the queue
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	// Always return the message to the default window
	// Procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}