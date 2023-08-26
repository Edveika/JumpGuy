#include "DirectX9.h"

DirectX9::DirectX9(HINSTANCE hInstance)
{
	// Initialize the window
	if (!init_window(hInstance))
	{
		MessageBoxA(NULL, "[DIRECTX9]: Failed to create a window!", NULL, NULL);
		throw std::invalid_argument("[DIRECTX9]: Window not created");
	}

	// Initialize direct3d
	if (!init_direct_3d())
	{
		MessageBoxA(NULL, "[DIRECTX9]: Failed to init Direct3D!", NULL, NULL);
		throw std::invalid_argument("[DIRECTX9]: Direct3D not created");
	}
}

DirectX9::~DirectX9()
{
	m_d3d_device->Release();
	m_d3d->Release();
}

bool DirectX9::init_window(HINSTANCE hInstance)
{
	// WNDCLASSEX struct describes how thw windows will look
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); // size of the struct
	wcex.style = CS_HREDRAW | CS_VREDRAW; // class style
	wcex.lpfnWndProc = (WNDPROC)wnd_proc; //window procedure callback
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
	m_wnd_handle = CreateWindowW
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
	if (!m_wnd_handle)
		return false;

	// Render the window on the screen
	ShowWindow(m_wnd_handle, SW_SHOW);
	UpdateWindow(m_wnd_handle);

	// Return true, everything went ok
	return true;
}

bool DirectX9::init_direct_3d()
{
	m_d3d = NULL;
	m_d3d_device = NULL;

	// Create the DirectX object
	if ((m_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Gets adapter details
	D3DADAPTER_IDENTIFIER9 ident;
	m_d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &ident);

	// Collects the mode count this adapter has
	UINT num_modes = m_d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);

	// Use the collected mode count to gather info about each one
	for (size_t i = 0; i < num_modes; ++i)
	{
		D3DDISPLAYMODE mode; // D3DDISPLAYMODE struct definition
		//char modeStr[255]; // Temp char array

		// Get the displaymode struct for this adapter mode
		m_d3d->EnumAdapterModes
		(
			D3DADAPTER_DEFAULT,
			D3DFMT_X8R8G8B8,
			i,
			&mode
		);

		SupportedScreenSettings* iSettings = new SupportedScreenSettings();
		iSettings->m_supportedScreenDimensions = { (float)mode.Width, (float)mode.Height };
		iSettings->m_supportedScreenRefreshRate = mode.RefreshRate;
		
		m_supported_screen_settings.push_back(iSettings);
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
	d3dpp.hDeviceWindow = m_wnd_handle;

	if (FAILED(m_d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT, // use the primary video adapter
		D3DDEVTYPE_REF, // create a device that uses a default Direct3D implementation
		m_wnd_handle,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, // ensures that your app runs on most hardware
		&d3dpp,
		&m_d3d_device // stores the created device in pd3dDevice
	))) return false;
	
	return true;
}

LRESULT CALLBACK DirectX9::wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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