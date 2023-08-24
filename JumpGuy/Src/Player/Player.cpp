#include "../Includes.h"

void Player::Update(float dt)
{
	_engine->GetKeyboardPtr()->GetInput();

#ifdef _DEBUG
	if (GetAsyncKeyState('L') & 1)
		_bEnableFlyMode = !_bEnableFlyMode;

	if (!_bEnableFlyMode)
	{
#endif
		Move();
		Jump(dt);
		ApplyVelocity(dt);
		Gravity(dt);
		GetSpriteForCurrentState();
#ifdef _DEBUG
	}
	else
		Fly();
#endif
}

void Player::Render(float dt)
{
	_curSprite->Render(_curPos, dt);
}

void Player::GetSpriteForCurrentState()
{
	if (GetIsGoingLeft() && _bIsOnGround)
	{
		_oldState = MOVE_LEFT;
		_curSprite = _sprites[MOVE_LEFT];
	}
	else if (GetIsGoingRight() && _bIsOnGround)
	{
		_oldState = MOVE_RIGHT;
		_curSprite = _sprites[MOVE_RIGHT];
	}
	else if (GetIsGoingUp() && !_bBumpedLeft && !_bBumpedRight)
	{
		if (GetIsGoingLeft())
		{
			_curSprite = _sprites[JUMP_LEFT];
		}
		else if (GetIsGoingRight())
		{
			_curSprite = _sprites[JUMP_RIGHT];
		}
		else
		{
			if (_oldState == MOVE_LEFT || _oldState == BUMP_LEFT)
			{
				_curSprite = _sprites[JUMP_LEFT];
			}
			else if (_oldState == MOVE_RIGHT || _oldState == BUMP_RIGHT)
			{
				_curSprite = _sprites[JUMP_RIGHT];
			}
		}
	}
	else if (GetIsGoingDown() && !_bBumpedLeft && !_bBumpedRight)
	{
		if (GetIsGoingLeft())
		{
			_curSprite = _sprites[FALL_LEFT];
		}
		else if (GetIsGoingRight())
		{
			_curSprite = _sprites[FALL_RIGHT];
		}
		else
		{
			if (_oldState == MOVE_LEFT || _oldState == BUMP_LEFT)
			{
				_curSprite = _sprites[FALL_LEFT];
			}
			else if (_oldState == MOVE_RIGHT || _oldState == BUMP_RIGHT)
			{
				_curSprite = _sprites[FALL_RIGHT];
			}
		}
	}
	else if (_bIsChargingJump)
	{
		_curSprite = _sprites[CHARGING_JUMP];
	}
	else if (_bBumpedLeft && !_bIsOnGround)
	{
		_oldState = BUMP_LEFT;
		_curSprite = _sprites[BUMP_LEFT];
	}
	else if (_bBumpedRight && !_bIsOnGround)
	{
		_oldState = BUMP_RIGHT;
		_curSprite = _sprites[BUMP_RIGHT];
	}
	else
	{
		if (_oldState == MOVE_LEFT || _oldState == BUMP_LEFT || _oldState == JUMP_LEFT)
		{
			_curSprite = _sprites[IDLE_LEFT];
		}
		else if (_oldState == MOVE_RIGHT || _oldState == BUMP_RIGHT || _oldState == JUMP_RIGHT)
		{
			_curSprite = _sprites[IDLE_RIGHT];
		}
	}
	if (_bIsOnGround)
	{
		_bBumpedLeft = false;
		_bBumpedRight = false;
	}
}

void Player::Move()
{
	if (!_bIsChargingJump)
	{
		if (_bIsOnGround)
		{
			if (PressedLeft())
			{
				_curVel.x = -300;
			}
			else if (PressedRight())
			{
				_curVel.x = 300;
			}
			else
			{
				_curVel = { 0,0 };
			}
		}
	}
	else
	{
		if (_bIsOnGround)
		{
			_curVel = { 0,0 };
		}
	}
}

void Player::Jump(float dt)
{
	if (_bIsOnGround)
	{
		if (PressedJump())
		{
			GetJumpForce(dt);
		}
		else if (_curJumpForce < _minJumpForce)
		{
			PlayJumpSound();

			if (PressedLeft())
			{
				_curVel.x = -400;
			}
			else if (PressedRight())
			{
				_curVel.x = 400;
			}

			_curVel.y = 200 * _curJumpForce;
			_bIsOnGround = false;
			_bIsChargingJump = false;
			_curJumpForce = 0;
		}
		else
		{
			_bIsChargingJump = false;
			_curJumpForce = 0;
		}
	}
}

void Player::GetJumpForce(float dt)
{
	_bIsChargingJump = true;

	if (_curJumpForce == 0)
	{
		_curJumpForce = _minJumpForce;
	}
	else if (_curJumpForce - dt * 10 >= _maxJumpForce)
	{
		_curJumpForce -= dt * 10;
	}
}

void Player::ApplyVelocity(float dt)
{
	if (GetIsGoingLeft() && _maxPosLeft != 0 && _curPos.x + GetHitboxLeft() + _curVel.x * dt < _maxPosLeft)
	{
		_curPos.x = _maxPosLeft - GetHitboxLeft();
	}
	else if (GetIsGoingRight() && _maxPosRight != 0 && _curPos.x + GetHitboxRight() + _curVel.x * dt > _maxPosRight)
	{
		_curPos.x = _maxPosRight - GetHitboxRight();
	}
	else
	{
		_curPos.x = _curPos.x + _curVel.x * dt;
	}

	if (GetIsGoingUp() && _maxPosUp != 0 && (_curPos.y + GetHitboxTop()) + _curVel.y * dt < _maxPosUp)
	{
		_curPos.y = _maxPosUp - GetHitboxTop();
	}
	else if (GetIsGoingDown() && _maxPosDown != 0 && _curPos.y + GetHitboxBottom() + _curVel.y * dt > _maxPosDown)
	{
		_curPos.y = _maxPosDown - GetHitboxBottom();
	}
	else
	{
		_curPos.y = _curPos.y + _curVel.y * dt;
	}
}

void Player::Fly()
{
	_curVel = { 0, 0 };
	if (PressedLeft())
		_curPos.x -= 10;
	else if (PressedRight())
		_curPos.x += 10;
	else if (PressedUp())
		_curPos.y -= 10;
	else if (PressedDown())
		_curPos.y += 10;
}

void Player::Gravity(float dt)
{
	if (!_bIsOnGround)
	{
		_curVel = _curVel + _curGravity * dt;
	}
}

bool Player::InitGraphics()
{
	float s_AnimationUpdateSpeed = 0.1f;

	if ((_sprites[MOVE_LEFT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\runLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[MOVE_RIGHT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\run.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[JUMP_LEFT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\jumpLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[JUMP_RIGHT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\jump.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[IDLE_LEFT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\idleLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[IDLE_RIGHT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\idle.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[FALL_LEFT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\fallLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[FALL_RIGHT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\fall.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[BUMP_LEFT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\oofLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[BUMP_RIGHT] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\oof.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((_sprites[CHARGING_JUMP] = new Sprite(_engine->GetGraphicsPtr(), L"Assets\\Sprites\\squat.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;

	return true;
}