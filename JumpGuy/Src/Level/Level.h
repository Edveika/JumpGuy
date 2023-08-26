#pragma once
#pragma once
#include <memory>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <fstream>

#include "../CollisionDetection/CollisionDetection.h"
#include "../Engine/Engine.h"
#include "../Line/Line.h"
#include "../Player/Player.h"

class Level
{
private:
	std::vector<Line> m_lines;
	std::shared_ptr<Engine> m_engine;
	LPDIRECT3DSURFACE9 m_level_image;
	LPDIRECT3DSURFACE9 m_backbuffer;

public:
	Level() {}
	Level(std::shared_ptr<Engine> eng, std::string mapFileName);

	void render(float dt);
	void add_line(Line line) { m_lines.push_back(line); }

	// Getters
	std::vector<Line> get_lines() { return m_lines; }
};