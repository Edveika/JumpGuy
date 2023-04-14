#pragma once

class Game
{
private:
	Engine* _engine;
	Player* _localPlayer;
	Map* _map;

public:
	Game(Engine* engine)
	{
		_map = new Map(engine);
		_engine = engine;
		_localPlayer = new Player(engine, IDLE_RIGHT);
	}
	~Game()
	{
		delete _engine;
		delete _localPlayer;
		delete _map;
	}

public:
	void Update(float dt);
	void Render(float dt);
};