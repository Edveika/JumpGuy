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
	std::shared_ptr<Engine> m_engine;
	std::shared_ptr<Level> m_curLevel;
	std::vector<std::shared_ptr<Level>> m_levels;
	int m_cur_lvl_index;

private:
	bool init_levels();

public:
	Map(std::shared_ptr<Engine> engine);

	void update(std::shared_ptr<Player> player);
	void render(float dt);

	// Getters
	std::shared_ptr<Level> get_cur_level(std::shared_ptr<Player> player);
	std::shared_ptr<Level> get_cur_level() { return m_curLevel; }
};