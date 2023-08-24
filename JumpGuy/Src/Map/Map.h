#pragma once
#include <memory>
#include <Windows.h>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "../Player/Player.h"
#include "../Engine/Engine.h"
#include "../Level/Level.h"
#include "../Line/Line.h"

class Map
{
private:
	std::shared_ptr<Engine> _engine;
	std::shared_ptr<Level> _curLevel;
	std::vector<std::shared_ptr<Level>> _levels;
	int _curLvlIndex;

public:
	Map(std::shared_ptr<Engine> engine)
	{
		if (!std::filesystem::exists("Assets"))
			MessageBoxA(NULL, "No Assets dir found!", NULL, NULL);
		_curLvlIndex = 0;
		_engine = engine;
		InitLevels();
		_curLevel = _levels[_curLvlIndex];
	}

public:
	void Update(std::shared_ptr<Player> player, float dt);
	void Render(std::shared_ptr<Player> player, float dt);

	bool InitLevels();

	std::shared_ptr<Level> GetCurLevel(std::shared_ptr<Player> player);
	std::shared_ptr<Level> GetCurLevel() { return _curLevel; }
};