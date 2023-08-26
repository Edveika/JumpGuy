#pragma once
#include <memory>

#include "../Engine/Engine.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Timer/Timer.h"
#include "../Graphics/Graphics.h"

class Game
{
private:
	std::shared_ptr<Engine> m_engine;
	std::shared_ptr<Player> m_localPlayer;
	std::shared_ptr<Map> m_map;
	std::shared_ptr<Timer> m_timer;
	std::shared_ptr<Graphics> m_graphics;
	std::shared_ptr<CollisionDetection> _collisionDetection;

private:
	void display_loading_screen();
	void update(float dt);
	void render(float dt);

public:
	Game(HINSTANCE hInstance);

	void run();
};