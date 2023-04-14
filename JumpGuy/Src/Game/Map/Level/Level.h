#pragma once
#pragma once

class Level
{
public:
	std::vector<Line> _lines;

private:
	Engine* _engine;
	CollisionDetection* _collisionDetection;
	LPDIRECT3DSURFACE9 _levelImage;
	LPDIRECT3DSURFACE9 _backbuffer;
	std::fstream _lineData;
	std::fstream _debugData;

public:
	Level() {}
	Level(Engine* eng, std::string mapFileName)
	{
		_engine = eng;
		_levelImage = _engine->GetGraphicsPtr()->GetSurfaceFromBitmap(mapFileName);
		_collisionDetection = new CollisionDetection();
	}
	~Level()
	{
		delete _engine;
		delete _collisionDetection;
	}

public:
	void Update(Player* player, float dt);
	void Render(Player* player, float dt);
	void AddLine(Line line) { _lines.push_back(line); }
};