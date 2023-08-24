#pragma once
#pragma once

class Level
{
public:
	std::vector<Line> _lines;

private:
	std::shared_ptr<Engine> _engine;
	std::shared_ptr<CollisionDetection> _collisionDetection;
	LPDIRECT3DSURFACE9 _levelImage;
	LPDIRECT3DSURFACE9 _backbuffer;
	std::fstream _lineData;
	std::fstream _debugData;

public:
	Level() {}
	Level(std::shared_ptr<Engine> eng, std::string mapFileName)
	{
		_engine = eng;
		_levelImage = _engine->GetGraphicsPtr()->GetSurfaceFromBitmap(mapFileName);
		_collisionDetection = std::make_shared<CollisionDetection>();
	}

public:
	void Update(std::shared_ptr<Player> player, float dt);
	void Render(std::shared_ptr<Player> player, float dt);
	void AddLine(Line line) { _lines.push_back(line); }
};