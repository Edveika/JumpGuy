#pragma once

class CollisionDetection
{
public:
	CollisionDetection()
	{

	}
	~CollisionDetection()
	{

	}

public:
	void Update(Player* player, std::vector<Line> lines, float dt);
	void PosCheck(Player* player, std::vector<Line> lines, float dt);

private:
	Line GetPriorityCollision(Player* player, std::vector<Line> lines);
	void ApplyCollisionLogic(Player* player, Line col, float dt);
	void GroundCheck(Player* player, std::vector<Line> lines);
	void GetLineCollisionData(Player* player, Line& line);
};