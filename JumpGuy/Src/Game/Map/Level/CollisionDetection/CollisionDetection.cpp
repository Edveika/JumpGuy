#include "../../../../Includes.h"

bool CheckForLineToLineCollision(Line line, float x3, float y3, float x4, float y4)
{
	float x1 = line.GetP1().x;
	float y1 = line.GetP1().y;
	float x2 = line.GetP2().x;
	float y2 = line.GetP2().y;

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

void GetLineRectData(Line& line, float rx, float ry, RECT hitbox)
{
	bool bLeftSideCollided = CheckForLineToLineCollision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.left, ry + hitbox.bottom);
	bool bRightSideCollided = CheckForLineToLineCollision(line, rx + hitbox.right, ry + hitbox.top, rx + hitbox.right, ry + hitbox.bottom);
	bool bTopCollided = CheckForLineToLineCollision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.right, ry + hitbox.top);
	bool bBottomCollided = CheckForLineToLineCollision(line, rx + hitbox.left, ry + hitbox.bottom, rx + hitbox.right, ry + hitbox.bottom);

	if (bLeftSideCollided) line.SetCollidedLeft(true);
	if (bRightSideCollided) line.SetCollidedRight(true);
	if (bTopCollided) line.SetCollidedTop(true);
	if (bBottomCollided) line.SetCollidedBottom(true);
}

bool GetLineRect(Line line, float rx, float ry, RECT hitbox)
{
	bool bLeftSideCollided = CheckForLineToLineCollision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.left, ry + hitbox.bottom);
	bool bRightSideCollided = CheckForLineToLineCollision(line, rx + hitbox.right, ry + hitbox.top, rx + hitbox.right, ry + hitbox.bottom);
	bool bTopCollided = CheckForLineToLineCollision(line, rx + hitbox.left, ry + hitbox.top, rx + hitbox.right, ry + hitbox.top);
	bool bBottomCollided = CheckForLineToLineCollision(line, rx + hitbox.left, ry + hitbox.bottom, rx + hitbox.right, ry + hitbox.bottom);

	if (bLeftSideCollided || bRightSideCollided || bTopCollided || bBottomCollided) return true;

	return false;
}

void CollisionDetection::GroundCheck(Player* player, std::vector<Line> lines)
{
	player->SetIsInAir();
	float playerLeft = player->GetCurPosition().x + player->GetHitboxLeft();
	float playerRight = player->GetCurPosition().x + player->GetHitboxRight();
	float playerBottom = player->GetCurPosition().y + player->GetHitboxBottom();

	int diagonalCollisionCount = 0;

	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].GetLineType() == LINE_TYPE_HORIZONTAL)
		{
			float lineLeft = lines[i].GetP1().x;
			float lineRight = lines[i].GetP2().x;

			if (playerLeft > lineLeft && playerLeft < lineRight || playerRight > lineLeft && playerRight < lineRight || playerLeft < lineLeft && playerRight > lineRight || playerLeft < lineRight && playerRight > lineLeft)
			{
				if (lines[i].GetP1().y == playerBottom)
				{
					player->SetVelocityY(0);
					player->SetIsOnGround();
				}
			}
		}
		else if (lines[i].GetLineType() == LINE_TYPE_DIAGONAL)
		{
			if (lines[i].GetCollidedBottom())
			{
				++diagonalCollisionCount;
				if (diagonalCollisionCount == 2)
				{
					player->SetVelocityY(0);
					player->SetIsOnGround();
				}
			}
		}
	}
}

void CollisionDetection::GetLineCollisionData(Player* player, Line& line)
{
	float playerLeft = player->GetCurPosition().x + player->GetHitboxLeft();
	float playerRight = player->GetCurPosition().x + player->GetHitboxRight();
	float playerTop = player->GetCurPosition().y + player->GetHitboxTop();
	float playerBottom = player->GetCurPosition().y + player->GetHitboxBottom();
	line.SetCollidedLeft(false);
	line.SetCollidedRight(false);
	line.SetCollidedTop(false);
	line.SetCollidedBottom(false);
	line.SetCollisionLength(0.0f);

	if (line.GetLineType() == LINE_TYPE_VERTICAL)
	{
		float lineTop = line.GetP1().y;
		float lineBottom = line.GetP2().y;

		if (playerBottom > line.GetP1().y && playerTop < line.GetP2().y)
		{
			if (playerLeft < line.GetP1().x && playerRight > line.GetP1().x)
			{
				if (line.GetP1().x - playerLeft < playerRight - line.GetP1().x && playerLeft < line.GetP1().x)
				{
					line.SetCollidedLeft(true);
					line.SetCollisionLength(line.GetP1().x - playerLeft);
				}
				else if (line.GetP1().x - playerLeft > playerRight - line.GetP1().x)
				{
					line.SetCollidedRight(true);
					line.SetCollisionLength(playerRight - line.GetP1().x);
				}
			}
		}
	}
	else if (line.GetLineType() == LINE_TYPE_HORIZONTAL)
	{
		float lineStart = line.GetP1().x;
		float lineEnd = line.GetP2().x;

		if (playerLeft > line.GetP1().x && playerLeft < line.GetP2().x || playerRight > line.GetP1().x && playerRight < line.GetP2().x)
		{
			if (line.GetP1().y - playerTop < playerBottom - line.GetP1().y)
			{
				if (line.GetP1().y - playerTop > 0)
				{
					line.SetCollidedBottom(true);
					line.SetCollisionLength(line.GetP1().y - playerTop);
				}
			}
			else if (line.GetP1().y - playerTop > playerBottom - line.GetP1().y)
			{
				if (playerBottom - line.GetP1().y > 0)
				{
					line.SetCollidedTop(true);
					line.SetCollisionLength(playerBottom - line.GetP1().y);
				}
			}
		}
	}
	else if (line.GetLineType() == LINE_TYPE_DIAGONAL)
	{
		GetLineRectData(line, player->GetCurPosition().x, player->GetCurPosition().y, player->GetHitbox());
	}
}

void CollisionDetection::PosCheck(Player* player, std::vector<Line> lines, float dt)
{
	player->SetMaxPosLeft(0);
	player->SetMaxPosRight(0);
	player->SetMaxPosUp(0);
	player->SetMaxPosDown(0);

	float playerLeft = player->GetCurPosition().x + player->GetHitboxLeft();
	float playerRight = player->GetCurPosition().x + player->GetHitboxRight();
	float playerTop = player->GetCurPosition().y + player->GetHitboxTop();
	float playerBottom = player->GetCurPosition().y + player->GetHitboxBottom();

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
		if (lines[i].GetLineType() == LINE_TYPE_VERTICAL)
		{
			float lineTop = lines[i].GetP1().y;
			float lineBottom = lines[i].GetP2().y;

			if (playerTop > lineTop && playerTop < lineBottom // linijos Y intervale
				//|| playerBottom > lineTop 
				|| playerTop < lineTop && playerBottom > lineBottom
				|| playerTop < lineBottom && playerBottom > lineTop)
			{
				if (lines[i].GetP1().x <= playerLeft)
				{
					if (playerLeft - lines[i].GetP1().x <= valueLeft)
					{
						valueLeft = playerLeft - lines[i].GetP1().x;
						closestLeftLine = lines[i];
					}
				}
				if (lines[i].GetP1().x >= playerRight)
				{
					if (lines[i].GetP1().x - playerRight <= valueRight)
					{
						valueRight = lines[i].GetP1().x - playerRight;
						closestRightLine = lines[i];
					}
				}
			}
		}
		else if (lines[i].GetLineType() == LINE_TYPE_HORIZONTAL)
		{
			float lineLeft = lines[i].GetP1().x;
			float lineRight = lines[i].GetP2().x;

			if (playerLeft > lineLeft && playerLeft < lineRight || playerRight > lineLeft && playerRight < lineRight || playerLeft < lineLeft && playerRight > lineRight || playerLeft < lineRight && playerRight > lineLeft)
			{
				if (lines[i].GetP1().y < playerTop)
				{
					if (playerTop - lines[i].GetP1().y < valueTop)
					{
						valueTop = playerTop - lines[i].GetP1().y;
						closestTopLine = lines[i];
					}
				}
				if (lines[i].GetP1().y > playerTop)
				{
					if (lines[i].GetP1().y - playerTop < valueBottom)
					{
						valueBottom = lines[i].GetP1().y - playerTop;
						closestBottomLine = lines[i];
					}
				}
			}
		}
	}
	if (valueLeft < FLT_MAX)
	{
		player->SetMaxPosLeft(closestLeftLine.GetP1().x - 0.1f);
	}
	if (valueRight < FLT_MAX)
	{
		player->SetMaxPosRight(closestRightLine.GetP1().x + 0.1f);
	}
	if (valueTop < FLT_MAX)
	{
		player->SetMaxPosUp(closestTopLine.GetP1().y - 0.1f);
	}
	if (valueBottom < FLT_MAX)
	{
		player->SetMaxPosDown(closestBottomLine.GetP1().y + 0.1f);
	}
}

Line CollisionDetection::GetPriorityCollision(Player* player, std::vector<Line> lines)
{
	Line col;
	float value = FLT_MAX;
	int diagonalLeftCollisionCount = 0;
	int diagonalRightCollisionCount = 0;
	int diagonalBottomCollisionCount = 0;

	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].GetLineType() == LINE_TYPE_DIAGONAL)
		{
			// Picks colliding lines for sliding logic
			if (lines[i].GetCollidedBottom() && lines[i].GetCollidedLeft() || // bottom left
				lines[i].GetCollidedBottom() && lines[i].GetCollidedRight() || // bottom right
				lines[i].GetCollidedTop() && player->GetIsGoingUp()) // top
			{
				col = lines[i];
				break; // :O
			}
			//
			// Picks edges of 2 or more lines
			//
			if (lines[i].GetCollidedLeft())
			{
				++diagonalLeftCollisionCount;
				if (diagonalLeftCollisionCount >= 2)
				{
					col = lines[i];
					break;
				}
			}
			if (lines[i].GetCollidedRight())
			{
				++diagonalRightCollisionCount;
				if (diagonalRightCollisionCount >= 2)
				{
					col = lines[i];
					break;
				}
			}
			if (lines[i].GetCollidedBottom()) // basically if we are standing on a point of 2 or more lines, its a floor
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
			if (player->GetIsGoingDown() && lines[i].GetCollidedBottom()) continue;
			else if (player->GetIsGoingUp() && lines[i].GetCollidedTop()) continue;
			if (lines[i].GetCollisionLength() > 0.0f)
			{
				if (lines[i].GetCollisionLength() < value)
				{
					col = lines[i];
					value = lines[i].GetCollisionLength();
				}
			}
		}
	}
	return col;
}

void CollisionDetection::ApplyCollisionLogic(Player* player, Line col, float dt)
{
	if (col.GetCollisionLength() > 0.0f)
	{
		if (col.GetLineType() == LINE_TYPE_VERTICAL)
		{
			if (col.GetCollidedLeft())
			{
				if (player->GetIsOnGround())
				{
					player->SetVelocityX(0);
					player->SetPositionX(col.GetP1().x - player->GetHitboxLeft());
				}
				else
				{
					player->SetBumpedLeft();
					player->PlayBumpSound();
					float oldVel = player->GetCurVelocity().x;
					player->SetPositionX(col.GetP1().x - player->GetHitboxLeft());
					player->SetVelocityX(oldVel * -1);
				}
			}
			else if (col.GetCollidedRight())
			{
				if (player->GetIsOnGround())
				{
					player->SetVelocityX(0);
					player->SetPositionX(col.GetP1().x - player->GetHitboxRight());
				}
				else
				{
					player->SetBumpedRight();
					player->PlayBumpSound();
					float oldVel = player->GetCurVelocity().x;
					player->SetPositionX(col.GetP1().x - player->GetHitboxRight());
					player->SetVelocityX(oldVel * -1);
				}
			}
		}
		else if (col.GetLineType() == LINE_TYPE_HORIZONTAL)
		{
			if (col.GetCollidedTop())
			{
				player->PlayLandSound();
				player->SetPositionY(col.GetP1().y - player->GetHitboxBottom());
			}
			else if (col.GetCollidedBottom())
			{
				if (player->GetIsInAir())
				{
					player->PlayBumpSound();
					float oldVel = player->GetCurVelocity().y;
					player->SetPositionY(col.GetP1().y - player->GetHitboxTop());
					player->SetVelocityY(oldVel * -1);
				}
			}
		}
	}
	else if (col.GetLineType() == LINE_TYPE_DIAGONAL)
	{
		float playerLeft = player->GetCurPosition().x + player->GetHitboxLeft();
		float playerTop = player->GetCurPosition().y + player->GetHitboxTop();
		bool bCollideLeft = col.GetCollidedLeft();
		bool bCollideRight = col.GetCollidedRight();
		bool bCollideTop = col.GetCollidedTop();
		bool bCollideBottom = col.GetCollidedBottom();
		bool bIsSlidingLeft = bCollideBottom && bCollideRight;
		bool bIsSlidingRight = bCollideBottom && bCollideLeft;

		if (bIsSlidingRight)
		{
			player->SetVelocityX(0);
			player->SetVelocityY(0);
			player->SetPositionX(player->GetCurPosition().x + 150 * dt);
			float posY = player->GetCurPosition().y;
			while (!GetLineRect(col, player->GetCurPosition().x, posY, player->GetHitbox()))
			{
				float posTopY = posY + player->GetHitboxTop();
				if (posTopY + player->GetHitboxBottom() + 1 > col.GetMaxY())
				{
					player->SetVelocityX(350);
					break;
				}
				posY += 1;
			}
			if (posY + player->GetHitboxBottom() > player->GetMaxPosDown() && player->GetMaxPosDown() != 0) posY = player->GetMaxPosDown() - player->GetHitboxBottom();
			player->SetPositionY(posY);
		}
		else if (bIsSlidingLeft)
		{
			player->SetVelocityX(0);
			player->SetVelocityY(0);
			player->SetPositionX(player->GetCurPosition().x - 150 * dt);
			float posY = player->GetCurPosition().y;
			while (!GetLineRect(col, player->GetCurPosition().x, posY, player->GetHitbox()))
			{
				float posTopY = posY + player->GetHitboxTop();
				if (posTopY + player->GetHitboxBottom() + 1 > col.GetMaxY())
				{
					player->SetVelocityX(-350);
					break;
				}
				posY += 1;
			}
			if (posY + player->GetHitboxBottom() > player->GetMaxPosDown() && player->GetMaxPosDown() != 0) posY = player->GetMaxPosDown() - player->GetHitboxBottom();
			player->SetPositionY(posY);
		}
		else if (bCollideTop && player->GetIsGoingUp())
		{
			player->SetVelocityX(0);
			player->SetVelocityY(0);
			float posY = player->GetCurPosition().y + (float)player->GetHitboxTop();
			while (GetLineRect(col, playerLeft, posY, player->GetHitbox()))
			{
				posY += 1;
			}
			player->SetPositionY(posY);
		}
		else if (bCollideLeft)
		{
			player->SetPositionX(col.GetMaxX() - (float)player->GetHitboxLeft());
		}
		else if (bCollideRight)
		{
			player->SetPositionX(col.GetMaxX() - (float)player->GetHitboxRight());
		}
		else if (bCollideBottom)
		{
			player->SetPositionY(col.GetMinY() - (float)player->GetHitboxBottom());
		}
	}
}

void CollisionDetection::Update(Player* player, std::vector<Line> lines, float dt)
{
	for (size_t i = 0; i < lines.size(); ++i)
	{
		GetLineCollisionData(player, lines[i]);
	}
	Line col = GetPriorityCollision(player, lines);
	ApplyCollisionLogic(player, col, dt);
	GroundCheck(player, lines);
}