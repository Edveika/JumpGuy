#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <stdexcept>

class Graphics
{
public:
	LPDIRECT3DDEVICE9 m_d3d_device;

private:
	void clear_screen(int r, int g, int b);

public:
	Graphics(LPDIRECT3DDEVICE9 pd3dDevice);
	~Graphics();

	void display();
	void clear_screen();
	void render_texture(LPDIRECT3DTEXTURE9 texture, LPD3DXSPRITE sprite, D3DXVECTOR2 pos, RECT src, D3DCOLOR color);
	IDirect3DSurface9* get_surface_from_bitmap(std::string filename);
	void get_image_info(LPCWSTR fileName, D3DXIMAGE_INFO& info);
};