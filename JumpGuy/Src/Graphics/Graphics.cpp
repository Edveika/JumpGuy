#include "../Includes.h"

void Graphics::ClearScreen(int r, int g, int b)
{
	if (FAILED(_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r, g, b), 1.0f, 0)))
	{
		MessageBoxA(NULL, "Failed to clear the screen!", NULL, NULL);
	} 
}

void Graphics::RenderTexture(LPDIRECT3DTEXTURE9 texture, LPD3DXSPRITE sprite, D3DXVECTOR2 pos, RECT src, D3DCOLOR color) //  draws a sprite not texture
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	
	D3DXVECTOR3 spritePos = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	if (FAILED(sprite->Draw(texture, &src, NULL, &spritePos, color)))
	{
		MessageBoxA(NULL, "Failed to render a sprite!", NULL, NULL);
	}

	sprite->End();
}

IDirect3DSurface9* Graphics::GetSurfaceFromBitmap(std::string filename)
{
	D3DXIMAGE_INFO imageInfo;
	IDirect3DSurface9* surface = NULL;

	// Get image info
	if (FAILED(D3DXGetImageInfoFromFileA(filename.c_str(), &imageInfo))) return NULL;

	// Create surface
	if (FAILED(_pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL))) return NULL;

	// Load bitmap to file
	if (FAILED(D3DXLoadSurfaceFromFileA(surface, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, NULL))) return NULL;

	// Return the created surface
	return surface;
}

void Graphics::GetImageInfo(LPCWSTR fileName, D3DXIMAGE_INFO& info)
{
	if (FAILED(D3DXGetImageInfoFromFile(fileName, &info)))
	{
		MessageBoxA(NULL, "Failed to get image data!", NULL, NULL);
	}
}