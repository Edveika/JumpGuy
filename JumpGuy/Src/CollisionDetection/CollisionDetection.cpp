#include "CollisionDetection.h"

bool line_to_line_collision(Line line, float x3, float y3, float x4, float y4)
{
	float x1 = line.get_p1().x;
	float y1 = line.get_p1().y;
	float x2 = line.get_p2().x;
	float y2 = line.get_p2().y;

	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) return true;
	else if (y3 == y4)
	{
		if (x4 >= x1 && x4 <= x2)
		{
			if (y3 == y1 || y3 == y2) return true;
		}
	}

	return false;
}

void get_line_rect_data(Line& line, float rx, float ry, RECT hitbox)
{
	bool bLeftSideCollided = line_to_line_collision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.left, ry + hitbox.bottom);
	bool bRightSideCollided = line_to_line_collision(line, rx + hitbox.right, ry + hitbox.top, rx + hitbox.right, ry + hitbox.bottom);
	bool bTopCollided = line_to_line_collision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.right, ry + hitbox.top);
	bool bBottomCollided = line_to_line_collision(line, rx + hitbox.left, ry + hitbox.bottom, rx + hitbox.right, ry + hitbox.bottom);

	if (bLeftSideCollided) line.set_collided_left(true);
	if (bRightSideCollided) line.set_collided_right(true);
	if (bTopCollided) line.set_collided_top(true);
	if (bBottomCollided) line.set_collided_bottom(true);
}

bool get_line_to_rect(Line line, float rx, float ry, RECT hitbox)
{
	bool bLeftSideCollided = line_to_line_collision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.left, ry + hitbox.bottom);
	bool bRightSideCollided = line_to_line_collision(line, rx + hitbox.right, ry + hitbox.top, rx + hitbox.right, ry + hitbox.bottom);
	bool bTopCollided = line_to_line_collision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.right, ry + hitbox.top);
	bool bBottomCollided = line_to_line_collision(line, rx + hitbox.left, ry + hitbox.bottom, rx + hitbox.right, ry + hitbox.bottom);

	if (bLeftSideCollided || bRightSideCollided || bTopCollided || bBottomCollided) return true;

	return false;
}

void CollisionDetection::ground_check(std::shared_ptr<Player> player, std::vector<Line> lines)
{
	player->set_in_air();
	float playerLeft = player->get_cur_position().x + player->get_hitbox_left();
	float playerRight = player->get_cur_position().x + player->get_hitbox_right();
	float playerBottom = player->get_cur_position().y + player->get_hitbox_bottom();

	int diagonalCollisionCount = 0;

	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].get_line_type() == LINE_TYPE_HORIZONTAL)
		{
			float lineLeft = lines[i].get_p1().x;
			float lineRight = lines[i].get_p2().x;

			if (playerLeft > lineLeft && playerLeft < lineRight || playerRight > lineLeft && playerRight < lineRight || playerLeft < lineLeft && playerRight > lineRight || playerLeft < lineRight && playerRight > lineLeft)
			{
				if (lines[i].get_p1().y == playerBottom)
				{
					player->set_velocity_y(0);
					player->set_on_ground();
				}
			}
		}
		else if (lines[i].get_line_type() == LINE_TYPE_DIAGONAL)
		{
			if (lines[i].get_collided_bottom())
			{
				++diagonalCollisionCount;
				if (diagonalCollisionCount == 2)
				{
					player->set_velocity_y(0);
					player->set_on_ground();
				}
			}
		}
	}
}

void CollisionDetection::get_line_collision_data(std::shared_ptr<Player> player, Line& line)
{
	float playerLeft = player->get_cur_position().x + player->get_hitbox_left();
	float playerRight = player->get_cur_position().x + player->get_hitbox_right();
	float playerTop = player->get_cur_position().y + player->get_hitbox_top();
	float playerBottom = player->get_cur_position().y + player->get_hitbox_bottom();
	line.set_collided_left(false);
	line.set_collided_right(false);
	line.set_collided_top(false);
	line.set_collided_bottom(false);
	line.set_collision_length(0.0f);

	if (line.get_line_type() == LINE_TYPE_VERTICAL)
	{
		float lineTop = line.get_p1().y;
		float lineBottom = line.get_p2().y;

		if (playerBottom > line.get_p1().y && playerTop < line.get_p2().y)
		{
			if (playerLeft < line.get_p1().x && playerRight > line.get_p1().x)
			{
				if (line.get_p1().x - playerLeft < playerRight - line.get_p1().x && playerLeft < line.get_p1().x)
				{
					line.set_collided_left(true);
					line.set_collision_length(line.get_p1().x - playerLeft);
				}
				else if (line.get_p1().x - playerLeft > playerRight - line.get_p1().x)
				{
					line.set_collided_right(true);
					line.set_collision_length(playerRight - line.get_p1().x);
				}
			}
		}
	}
	else if (line.get_line_type() == LINE_TYPE_HORIZONTAL)
	{
		float lineStart = line.get_p1().x;
		float lineEnd = line.get_p2().x;

		if (playerLeft > line.get_p1().x && playerLeft < line.get_p2().x || playerRight > line.get_p1().x && playerRight < line.get_p2().x)
		{
			if (line.get_p1().y - playerTop < playerBottom - line.get_p1().y)
			{
				if (line.get_p1().y - playerTop > 0)
				{
					line.set_collided_bottom(true);
					line.set_collision_length(line.get_p1().y - playerTop);
				}
			}
			else if (line.get_p1().y - playerTop > playerBottom - line.get_p1().y)
			{
				if (playerBottom - line.get_p1().y > 0)
				{
					line.set_collided_top(true);
					line.set_collision_length(playerBottom - line.get_p1().y);
				}
			}
		}
	}
	else if (line.get_line_type() == LINE_TYPE_DIAGONAL)
	{
		get_line_rect_data(line, player->get_cur_position().x, player->get_cur_position().y, player->get_hitbox());
	}
}

void CollisionDetection::pos_check(std::shared_ptr<Player> player, std::vector<Line> lines, float dt)
{
	player->set_max_pos_left(0);
	player->set_max_pos_right(0);
	player->set_max_pos_up(0);
	player->set_max_pos_down(0);

	float playerLeft = player->get_cur_position().x + player->get_hitbox_left();
	float playerRight = player->get_cur_position().x + player->get_hitbox_right();
	float playerTop = player->get_cur_position().y + player->get_hitbox_top();
	float playerBottom = player->get_cur_position().y + player->get_hitbox_bottom();

	Line closestLeftLine;
	float valueLeft = FLT_MAX;
	Line closestRightLine;
	float valueRight = FLT_MAX;
	Line closestTopLine;
	float valueTop = FLT_MAX;
	Line closestBottomLine;
	float valueBottom = FLT_MAX;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].get_line_type() == LINE_TYPE_VERTICAL)
		{
			float lineTop = lines[i].get_p1().y;
			float lineBottom = lines[i].get_p2().y;

			if (playerTop > lineTop && playerTop < lineBottom // linijos Y intervale
				//|| playerBottom > lineTop 
				|| playerTop < lineTop && playerBottom > lineBottom
				|| playerTop < lineBottom && playerBottom > lineTop)
			{
				if (lines[i].get_p1().x <= playerLeft)
				{
					if (playerLeft - lines[i].get_p1().x <= valueLeft)
					{
						valueLeft = playerLeft - lines[i].get_p1().x;
						closestLeftLine = lines[i];
					}
				}
				if (lines[i].get_p1().x >= playerRight)
				{
					if (lines[i].get_p1().x - playerRight <= valueRight)
					{
						valueRight = lines[i].get_p1().x - playerRight;
						closestRightLine = lines[i];
					}
				}
			}
		}
		else if (lines[i].get_line_type() == LINE_TYPE_HORIZONTAL)
		{
			float lineLeft = lines[i].get_p1().x;
			float lineRight = lines[i].get_p2().x;

			if (playerLeft > lineLeft && playerLeft < lineRight || playerRight > lineLeft && playerRight < lineRight || playerLeft < lineLeft && playerRight > lineRight || playerLeft < lineRight && playerRight > lineLeft)
			{
				if (lines[i].get_p1().y < playerTop)
				{
					if (playerTop - lines[i].get_p1().y < valueTop)
					{
						valueTop = playerTop - lines[i].get_p1().y;
						closestTopLine = lines[i];
					}
				}
				if (lines[i].get_p1().y > playerTop)
				{
					if (lines[i].get_p1().y - playerTop < valueBottom)
					{
						valueBottom = lines[i].get_p1().y - playerTop;
						closestBottomLine = lines[i];
					}
				}
			}
		}
	}
	if (valueLeft < FLT_MAX)
	{
		player->set_max_pos_left(closestLeftLine.get_p1().x - 0.1f);
	}
	if (valueRight < FLT_MAX)
	{
		player->set_max_pos_right(closestRightLine.get_p1().x + 0.1f);
	}
	if (valueTop < FLT_MAX)
	{
		player->set_max_pos_up(closestTopLine.get_p1().y - 0.1f);
	}
	if (valueBottom < FLT_MAX)
	{
		player->set_max_pos_down(closestBottomLine.get_p1().y + 0.1f);
	}
}

Line CollisionDetection::get_priority_collision(std::shared_ptr<Player> player, std::vector<Line> lines)
{
	Line col;
	float value = FLT_MAX;
	int diagonalLeftCollisionCount = 0;
	int diagonalRightCollisionCount = 0;
	int diagonalBottomCollisionCount = 0;

	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].get_line_type() == LINE_TYPE_DIAGONAL)
		{
			// Picks colliding lines for sliding logic
			if (lines[i].get_collided_bottom() && lines[i].get_collided_left() || // bottom left
				lines[i].get_collided_bottom() && lines[i].get_collided_right() || // bottom right
				lines[i].get_collided_top() && player->get_going_up()) // top
			{
				col = lines[i];
				break; // :O
			}
			//
			// Picks edges of 2 or more lines
			//
			if (lines[i].get_collided_left())
			{
				++diagonalLeftCollisionCount;
				if (diagonalLeftCollisionCount >= 2)
				{
					col = lines[i];
					break;
				}
			}
			if (lines[i].get_collided_right())
			{
				++diagonalRightCollisionCount;
				if (diagonalRightCollisionCount >= 2)
				{
					col = lines[i];
					break;
				}
			}
			if (lines[i].get_collided_bottom()) // basically if we are standing on a point of 2 or more lines, its a floor
			{
				++diagonalBottomCollisionCount;
				if (diagonalBottomCollisionCount >= 2)
				{
					col = lines[i];
					break;
				}
			}
		}
		else
		{
			if (player->get_going_down() && lines[i].get_collided_bottom()) continue;
			else if (player->get_going_up() && lines[i].get_collided_top()) continue;
			if (lines[i].get_collision_length() > 0.0f)
			{
				if (lines[i].get_collision_length() < value)
				{
					col = lines[i];
					value = lines[i].get_collision_length();
				}
			}
		}
	}
	return col;
}

void CollisionDetection::apply_collision_logic(std::shared_ptr<Player> player, Line col, float dt)
{
	if (col.get_collision_length() > 0.0f)
	{
		if (col.get_line_type() == LINE_TYPE_VERTICAL)
		{
			if (col.get_collided_left())
			{
				if (player->get_on_ground())
				{
					player->set_velocity_x(0);
					player->set_position_x(col.get_p1().x - player->get_hitbox_left());
				}
				else
				{
					player->set_bumped_left();
					player->play_bump_sound();
					float oldVel = player->get_cur_velocity().x;
					player->set_position_x(col.get_p1().x - player->get_hitbox_left());
					player->set_velocity_x(oldVel * -1);
				}
			}
			else if (col.get_collided_right())
			{
				if (player->get_on_ground())
				{
					player->set_velocity_x(0);
					player->set_position_x(col.get_p1().x - player->get_hitbox_right());
				}
				else
				{
					player->set_bumped_right();
					player->play_bump_sound();
					float oldVel = player->get_cur_velocity().x;
					player->set_position_x(col.get_p1().x - player->get_hitbox_right());
					player->set_velocity_x(oldVel * -1);
				}
			}
		}
		else if (col.get_line_type() == LINE_TYPE_HORIZONTAL)
		{
			if (col.get_collided_top())
			{
				player->play_land_sound();
				player->set_position_y(col.get_p1().y - player->get_hitbox_bottom());
			}
			else if (col.get_collided_bottom())
			{
				if (player->get_in_air())
				{
					player->play_bump_sound();
					float oldVel = player->get_cur_velocity().y;
					player->set_position_y(col.get_p1().y - player->get_hitbox_top());
					player->set_velocity_y(oldVel * -1);
				}
			}
		}
	}
	else if (col.get_line_type() == LINE_TYPE_DIAGONAL)
	{
		float playerLeft = player->get_cur_position().x + player->get_hitbox_left();
		float playerTop = player->get_cur_position().y + player->get_hitbox_top();
		bool bCollideLeft = col.get_collided_left();
		bool bCollideRight = col.get_collided_right();
		bool bCollideTop = col.get_collided_top();
		bool bCollideBottom = col.get_collided_bottom();
		bool bIsSlidingLeft = bCollideBottom && bCollideRight;
		bool bIsSlidingRight = bCollideBottom && bCollideLeft;

		if (bIsSlidingRight)
		{
			player->set_velocity_x(0);
			player->set_velocity_y(0);
			player->set_position_x(player->get_cur_position().x + 150 * dt);
			float posY = player->get_cur_position().y;
			while (!get_line_to_rect(col, player->get_cur_position().x, posY, player->get_hitbox()))
			{
				float posTopY = posY + player->get_hitbox_top();
				if (posTopY + player->get_hitbox_bottom() + 1 > col.get_max_y())
				{
					player->set_velocity_x(350);
					break;
				}
				posY += 1;
			}
			if (posY + player->get_hitbox_bottom() > player->get_max_pos_down() && player->get_max_pos_down() != 0) posY = player->get_max_pos_down() - player->get_hitbox_bottom();
			player->set_position_y(posY);
		}
		else if (bIsSlidingLeft)
		{
			player->set_velocity_x(0);
			player->set_velocity_y(0);
			player->set_position_x(player->get_cur_position().x - 150 * dt);
			float posY = player->get_cur_position().y;
			while (!get_line_to_rect(col, player->get_cur_position().x, posY, player->get_hitbox()))
			{
				float posTopY = posY + player->get_hitbox_top();
				if (posTopY + player->get_hitbox_bottom() + 1 > col.get_max_y())
				{
					player->set_velocity_x(-350);
					break;
				}
				posY += 1;
			}
			if (posY + player->get_hitbox_bottom() > player->get_max_pos_down() && player->get_max_pos_down() != 0) posY = player->get_max_pos_down() - player->get_hitbox_bottom();
			player->set_position_y(posY);
		}
		else if (bCollideTop && player->get_going_up())
		{
			player->set_velocity_x(0);
			player->set_velocity_y(0);
			float posY = player->get_cur_position().y + (float)player->get_hitbox_top();
			while (get_line_to_rect(col, playerLeft, posY, player->get_hitbox()))
			{
				posY += 1;
			}
			player->set_position_y(posY);
		}
		else if (bCollideLeft)
		{
			player->set_position_x(col.get_max_x() - (float)player->get_hitbox_left());
		}
		else if (bCollideRight)
		{
			player->set_position_x(col.get_max_x() - (float)player->get_hitbox_right());
		}
		else if (bCollideBottom)
		{
			player->set_position_y(col.get_min_y() - (float)player->get_hitbox_bottom());
		}
	}
}

void CollisionDetection::update(std::shared_ptr<Player> player, std::vector<Line> lines, float dt)
{
	for (size_t i = 0; i < lines.size(); ++i)
	{
		get_line_collision_data(player, lines[i]);
	}
	Line col = get_priority_collision(player, lines);
	apply_collision_logic(player, col, dt);
	ground_check(player, lines);
}