#include "Map.h"

Map::Map(std::shared_ptr<Engine> engine)
{
	if (!std::filesystem::exists("Assets"))
	{
		MessageBoxA(NULL, "[MAP]: No Assets dir found!", NULL, NULL);
		throw std::invalid_argument("[MAP]: Assets dir does not exist");
	}
	m_cur_lvl_index = 0;
	m_engine = engine;
	if (!m_engine)
	{
		MessageBoxA(NULL, "[MAP]: Engine obj was not created", NULL, NULL);
		throw std::invalid_argument("[MAP]: Engine was not created");
	}
	init_levels();
	m_curLevel = m_levels[m_cur_lvl_index];
}

Line GetLineFromStr(std::string str)
{
	std::string p1;
	std::string p2;

	int cur_line_point = 0;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == ' ')
		{
			++cur_line_point;
		}

		if (cur_line_point == 0 && str[i] != ' ')
		{
			p1 += str[i];
		}
		else if (cur_line_point == 1 && str[i] != ' ')
		{
			p2 += str[i];
		}
	}

	std::string x1;
	std::string y1;
	int cur_p1_coord = 0;
	for (size_t i = 0; i < p1.size(); ++i)
	{
		if (p1[i] == ';')
		{
			++cur_p1_coord;
		}
		else if (cur_p1_coord == 0)
		{
			x1 += p1[i];
		}
		else if (cur_p1_coord == 1)
		{
			y1 += p1[i];
		}
	}

	std::string x2;
	std::string y2;
	int cur_p2_coord = 0;
	for (size_t i = 0; i < p2.size(); ++i)
	{
		if (p2[i] == ';')
		{
			++cur_p2_coord;
		}
		else if (cur_p2_coord == 0)
		{
			x2 += p2[i];
		}
		else if (cur_p2_coord == 1)
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

bool Map::init_levels()
{
	int cur_level = 0;
	std::ifstream fw;
	std::string tmp;
	fw.open("Assets\\CollisionData.dat");

	while (std::getline(fw, tmp))
	{
		if (tmp.substr(tmp.find_last_of(".") + 1) == "png")
		{
			std::shared_ptr<Level> level = std::make_shared<Level>(m_engine, "Assets\\Levels\\" + tmp);
			m_levels.push_back(level);
		}
		else
		{
			if (!tmp.empty())
			{
				Line line = GetLineFromStr(tmp);
				m_levels[cur_level]->add_line(line);
			}
		}
		if (tmp == "")
		{
			++cur_level;
		}
	}

	return true;
}

std::shared_ptr<Level> Map::get_cur_level(std::shared_ptr<Player> player)
{
	std::shared_ptr<Level> lvl = m_levels[m_cur_lvl_index];

#ifdef _DEBUG
	if (GetAsyncKeyState('P') & 1 && _curLvlIndex + 1 < _levels.size())
	{
		++_curLvlIndex;
	}
#endif

	if (player->get_cur_position().y + player->get_hitbox_top() < 0 && player->get_going_up())
	{
		if (m_cur_lvl_index + 1 < m_levels.size() - 1)
		{
			++m_cur_lvl_index;
			lvl = m_levels[m_cur_lvl_index];
			player->set_position_y(WINDOW_HEIGHT);
		}
	}
	else if (player->get_cur_position().y + player->get_hitbox_bottom() > WINDOW_HEIGHT && player->get_going_down())
	{
		if (m_cur_lvl_index - 1 >= 0)
		{
			--m_cur_lvl_index;
			lvl = m_levels[m_cur_lvl_index];
			player->set_position_y(0);
		}
	}

	return lvl;
}

void Map::update(std::shared_ptr<Player> player)
{
	m_curLevel = get_cur_level(player);
}

void Map::render(float dt)
{
	m_curLevel->render(dt);
}