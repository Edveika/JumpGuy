#pragma once

class Map
{
private:
	Engine* _engine;
	Level* _curLevel;
	std::vector<Level*> _levels;
	int _curLvlIndex;

public:
	Map(Engine* engine)
	{
		if (!std::filesystem::exists("Assets"))
			MessageBoxA(NULL, "No Assets dir found!", NULL, NULL);
		_curLvlIndex = 0;
		_engine = engine;
		InitLevels();
		_curLevel = _levels[_curLvlIndex];
	}
	~Map()
	{
		delete _curLevel;
		delete _engine;
		for (size_t i = 0; i < _levels.size(); ++i)
			delete _levels[i];
	}

public:
	void Update(Player* player, float dt);
	void Render(Player* player, float dt);

	bool InitLevels();
	Level* GetCurLevel(Player* player);

	// Getters
	Level* GetCurLevel() { return _curLevel; }
};