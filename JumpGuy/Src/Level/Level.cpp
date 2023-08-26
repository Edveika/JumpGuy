#include "Level.h"

Level::Level(std::shared_ptr<Engine> eng, std::string mapFileName)
{
	m_engine = eng;
	if (!m_engine)
	{
		MessageBoxA(NULL, "[LEVEL]: Engine was not created", NULL, NULL);
		throw std::invalid_argument("[LEVEL]: Engine was not created");
	}
	m_level_image = m_engine->get_graphics_ptr()->get_surface_from_bitmap(mapFileName);
}

void Level::render(float dt)
{
	m_engine->get_directx9_ptr()->get_device()->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &m_backbuffer);
	m_engine->get_directx9_ptr()->get_device()->StretchRect(m_level_image, NULL, m_backbuffer, NULL, D3DTEXF_NONE);
}