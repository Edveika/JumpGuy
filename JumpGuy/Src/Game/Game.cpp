#include "Game.h"

Game::Game(HINSTANCE hInstance)
	: m_engine{ std::make_shared<Engine>(hInstance) }, m_graphics { std::make_shared<Graphics>(m_engine->get_directx9_ptr()->get_device())}
{
	if (!m_engine)
	{
		MessageBoxA(NULL, "[GAME]: Failed to create engine obj", NULL, NULL);
		throw std::invalid_argument("[GAME]: Engine obj was not created");
	}

	if (!m_graphics)
	{
		MessageBoxA(NULL, "[GAME]: Failed to create graphics obj", NULL, NULL);
		throw std::invalid_argument("[GAME]: Graphics obj was not created");
	}

	_collisionDetection = std::make_shared<CollisionDetection>();

	m_timer = std::make_shared<Timer>();
	if (!m_timer)
	{
		MessageBoxA(NULL, "[GAME]: Failed to create timer obj", NULL, NULL);
		throw std::invalid_argument("[GAME]: Timer obj was not created");
	}

	m_localPlayer = std::make_shared<Player>(m_engine);
	if (!m_localPlayer)
	{
		MessageBoxA(NULL, "[GAME]: Failed to create player obj", NULL, NULL);
		throw std::invalid_argument("[GAME]: Player obj was not created");
	}

	// Reading data from the collision.dat file to setup map will take some time, this will display a nice load screen while that is being done
	display_loading_screen();
	m_map = std::make_shared<Map>(m_engine);
	if (!m_map)
	{
		MessageBoxA(NULL, "[GAME]: Failed to create map obj", NULL, NULL);
		throw std::invalid_argument("[GAME]: Map obj was not created");
	}
}

void Game::update(float dt)
{
	m_map->update(m_localPlayer);
	_collisionDetection->pos_check(m_localPlayer, m_map->get_cur_level()->get_lines(), dt);
	m_localPlayer->update(dt);
	_collisionDetection->update(m_localPlayer, m_map->get_cur_level()->get_lines(), dt);
}

void Game::render(float dt)
{
	m_map->render(dt);
	m_localPlayer->render(dt);
}

void Game::run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer->start();
			m_graphics->clear_screen();

			float dt = m_timer->get_time_passed();
			update(dt);
			render(dt);

			m_graphics->display();
			m_timer->end();
		}
	}
}

void Game::display_loading_screen()
{
	m_graphics->clear_screen();

	LPDIRECT3DSURFACE9 backBuffer;
	LPDIRECT3DSURFACE9 loadScreenImage = m_engine->get_graphics_ptr()->get_surface_from_bitmap("Assets\\LoadScreen.bmp");
	m_engine->get_directx9_ptr()->get_device()->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	m_engine->get_directx9_ptr()->get_device()->StretchRect(loadScreenImage, NULL, backBuffer, NULL, D3DTEXF_NONE);

	m_graphics->display();
}