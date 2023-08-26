#pragma once
#include <memory>
#include <vector>

#include "../Line/Line.h"
#include "../Player/Player.h"

class CollisionDetection
{
private:
	Line get_priority_collision(std::shared_ptr<Player> player, std::vector<Line> lines);
	void apply_collision_logic(std::shared_ptr<Player> player, Line col, float dt);
	void ground_check(std::shared_ptr<Player> player, std::vector<Line> lines);
	void get_line_collision_data(std::shared_ptr<Player> player, Line& line);

public:
	void update(std::shared_ptr<Player> player, std::vector<Line> lines, float dt);
	void pos_check(std::shared_ptr<Player> player, std::vector<Line> lines, float dt);
};