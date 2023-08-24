#pragma once
#include <memory>

#include "../Engine/Engine.h"
#include "../Player/Player.h"
#include "../Map/Map.h"

class Game
{
private:
	std::shared_ptr<Engine> _engine;
	std::shared_ptr<Player> _localPlayer;
	std::shared_ptr<Map> _map;

public:
	Game(std::shared_ptr<Engine> engine)
	{
		_map = std::make_shared<Map>(engine);
		_engine = std::make_shared<Engine>();
		_localPlayer = std::make_shared<Player>(engine, IDLE_RIGHT);
	}

public:
	void Update(float dt);
	void Render(float dt);
};