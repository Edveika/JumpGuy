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
	void Update(std::shared_ptr<Player> player, std::vector<Line> lines, float dt);
	void PosCheck(std::shared_ptr<Player> player, std::vector<Line> lines, float dt);

private:
	Line GetPriorityCollision(std::shared_ptr<Player> player, std::vector<Line> lines);
	void ApplyCollisionLogic(std::shared_ptr<Player> player, Line col, float dt);
	void GroundCheck(std::shared_ptr<Player> player, std::vector<Line> lines);
	void GetLineCollisionData(std::shared_ptr<Player> player, Line& line);
};