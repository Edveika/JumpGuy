#include "../../Includes.h"

Line GetLineFromStr(std::string str)
{
	std::string p1;
	std::string p2;

	int curLinePoint = 0;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == ' ')
		{
			++curLinePoint;
		}

		if (curLinePoint == 0 && str[i] != ' ')
		{
			p1 += str[i];
		}
		else if (curLinePoint == 1 && str[i] != ' ')
		{
			p2 += str[i];
		}
	}

	std::string x1;
	std::string y1;
	int curP1Coord = 0;
	for (size_t i = 0; i < p1.size(); ++i)
	{
		if (p1[i] == ';')
		{
			++curP1Coord;
		}
		else if (curP1Coord == 0)
		{
			x1 += p1[i];
		}
		else if (curP1Coord == 1)
		{
			y1 += p1[i];
		}
	}

	std::string x2;
	std::string y2;
	int curP2Coord = 0;
	for (size_t i = 0; i < p2.size(); ++i)
	{
		if (p2[i] == ';')
		{
			++curP2Coord;
		}
		else if (curP2Coord == 0)
		{
			x2 += p2[i];
		}
		else if (curP2Coord == 1)
		{
			y2 += p2[i];
		}
	}

	float f_x1 = std::stof(x1);
	float f_y1 = std::stof(y1);
	float f_x2 = std::stof(x2);
	float f_y2 = std::stof(y2);

	Line line({ f_x1,f_y1 }, { f_x2,f_y2 });

	return line;
}

bool Map::InitLevels()
{
	int curLevel = 0;
	std::ifstream fw;
	std::string tmp;
	fw.open("Assets\\CollisionData.dat");

	while (std::getline(fw, tmp))
	{
		if (tmp.substr(tmp.find_last_of(".") + 1) == "png")
		{
			Level* level = new Level(_engine, "Assets\\Levels\\" + tmp);
			_levels.push_back(level);
		}
		else
		{
			if (!tmp.empty())
			{
				Line line = GetLineFromStr(tmp);
				_levels[curLevel]->AddLine(line);
			}
		}
		if (tmp == "")
		{
			++curLevel;
		}
	}

	return true;
}

Level* Map::GetCurLevel(Player* player)
{
	Level* lvl = _levels[_curLvlIndex];

#ifdef _DEBUG
	if (GetAsyncKeyState('P') & 1 && _curLvlIndex + 1 < _levels.size())
	{
		++_curLvlIndex;
	}
#endif

	if (player->GetCurPosition().y + player->GetHitboxTop() < 0 && player->GetIsGoingUp())
	{
		if (_curLvlIndex + 1 < _levels.size() - 1)
		{
			++_curLvlIndex;
			lvl = _levels[_curLvlIndex];
			player->SetPositionY(WINDOW_HEIGHT);
		}
	}
	else if (player->GetCurPosition().y + player->GetHitboxBottom() > WINDOW_HEIGHT && player->GetIsGoingDown())
	{
		if (_curLvlIndex - 1 >= 0)
		{
			--_curLvlIndex;
			lvl = _levels[_curLvlIndex];
			player->SetPositionY(0);
		}
	}

	return lvl;
}

void Map::Update(Player* player, float dt)
{
	_curLevel = GetCurLevel(player);
	_curLevel->Update(player, dt);
}

void Map::Render(Player* player, float dt)
{
	_curLevel->Render(player, dt);
}