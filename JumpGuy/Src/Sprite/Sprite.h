#pragma once
#include <memory>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <stdexcept>

#include "../Graphics/Graphics.h"

class Sprite
{
private:
	struct SpriteAnimation
	{
		float m_time_passed;
		int m_num_sprite_frames;
		int m_cur_sprite_frame;
		float m_update;
		float m_last_update;
		float m_animation_timer;
	};

	std::shared_ptr<Graphics> m_gfx;
	LPD3DXSPRITE m_cur_sprite = NULL;
	RECT m_cur_sprite_rect;
	LPDIRECT3DTEXTURE9 m_sprite_sheet = NULL;
	D3DXIMAGE_INFO m_sprite_sheet_info;
	SpriteAnimation m_animation;
	int m_sprite_width;
	int m_sprite_height;

public:
	Sprite(std::shared_ptr<Graphics> gfx, LPCWSTR fileName, int frameWidth, int frameHeight, float animationUpdate);
	~Sprite();

	void render(D3DXVECTOR2 spritePos, float dt);

	// Getters
	int get_width() { return m_sprite_width; }
	int get_height() { return m_sprite_height; }
};