#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class Graphics
{
public:
	LPDIRECT3DDEVICE9 _pd3dDevice;

public:
	Graphics(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		_pd3dDevice = pd3dDevice;
	}
	~Graphics()
	{
		_pd3dDevice->Release();
	}

public:
	void ClearScreen(int r, int g, int b);
	void RenderTexture(LPDIRECT3DTEXTURE9 texture, LPD3DXSPRITE sprite, D3DXVECTOR2 pos, RECT src, D3DCOLOR color);
	IDirect3DSurface9* GetSurfaceFromBitmap(std::string filename);
	void GetImageInfo(LPCWSTR fileName, D3DXIMAGE_INFO& info);
};