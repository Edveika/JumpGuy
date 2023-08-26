#include "Sprite.h"

Sprite::Sprite(std::shared_ptr<Graphics> gfx, LPCWSTR fileName, int frameWidth, int frameHeight, float animationUpdate)
{
	m_animation.m_update = animationUpdate;
	m_animation.m_cur_sprite_frame = 0;
	m_animation.m_last_update = 0;
	m_animation.m_time_passed = 0.0f;
	m_animation.m_animation_timer = 0;

	m_gfx = gfx;
	if (!m_gfx)
	{
		MessageBoxA(NULL, "[SPRITE]: Failed to create Graphics obj", NULL, NULL);
		throw std::invalid_argument("[SPRITE]: Graphics obj not created");
	}
	m_sprite_width = frameWidth;
	m_sprite_height = frameHeight;
	m_cur_sprite_rect.top = 0;
	m_cur_sprite_rect.left = 0;
	m_cur_sprite_rect.right = m_cur_sprite_rect.left + frameWidth;
	m_cur_sprite_rect.bottom = frameHeight;

	gfx->get_image_info(fileName, m_sprite_sheet_info);

	if (FAILED(D3DXCreateTextureFromFileEx(m_gfx->m_d3d_device, fileName, m_sprite_sheet_info.Width, m_sprite_sheet_info.Height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_sprite_sheet)))
	{
		MessageBoxA(NULL, "[SPRITE]: Failed to create texture from file!", NULL, NULL);
		throw std::invalid_argument("[SPRITE]: Texture from a file was not created");
	}

	m_animation.m_num_sprite_frames = (m_sprite_sheet_info.Width / frameWidth) - 1;

	if (FAILED(D3DXCreateSprite(m_gfx->m_d3d_device, &m_cur_sprite)))
	{
		MessageBoxA(NULL, "[SPRITE]: Failed to create a sprite!", NULL, NULL);
		throw std::invalid_argument("[SPRITE]: Sprite was not created");
	}
}
Sprite::~Sprite()
{
	m_sprite_sheet->Release();
	m_cur_sprite->Release();
}

void Sprite::render(D3DXVECTOR2 spritePos, float dt)
{
	m_animation.m_animation_timer += dt;

	if (m_animation.m_animation_timer - m_animation.m_last_update > m_animation.m_update)
	{
		if (m_animation.m_cur_sprite_frame + 1 <= m_animation.m_num_sprite_frames)
		{
			++m_animation.m_cur_sprite_frame;
		}
		else if (m_animation.m_cur_sprite_frame + 1 > m_animation.m_num_sprite_frames)
		{
			m_animation.m_cur_sprite_frame = 0;
		}
		m_animation.m_last_update = m_animation.m_animation_timer;
	}

	m_cur_sprite_rect.left = m_animation.m_cur_sprite_frame * m_sprite_width;
	m_cur_sprite_rect.right = m_cur_sprite_rect.left + m_sprite_width;

	m_gfx->render_texture(m_sprite_sheet, m_cur_sprite, spritePos, m_cur_sprite_rect, 0XFFFFFFFF);
}