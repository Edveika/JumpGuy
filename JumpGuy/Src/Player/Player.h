#pragma once
#include <memory>
#include <Windows.h>
#include <XAudio2.h>

#include "../Engine/Engine.h"
#include "../Sprite/Sprite.h"

#define SPRITE_ARRAY_SIZE 12

enum PlayerState
{
	IDLE_RIGHT,
	IDLE_LEFT,
	MOVE_RIGHT,
	MOVE_LEFT,
	JUMP_UP,
	JUMP_RIGHT,
	JUMP_LEFT,
	FALL_LEFT,
	FALL_RIGHT,
	BUMP_LEFT,
	BUMP_RIGHT,
	CHARGING_JUMP,
};

class Player
{
private:
	std::shared_ptr<Sprite> _sprites[SPRITE_ARRAY_SIZE];
	std::shared_ptr<Sprite> _curSprite;
	std::shared_ptr<Engine> _engine;
	IXAudio2SourceVoice* _audio;
	int _curState;
	int _oldState;
	D3DXVECTOR2 _curPos;
	D3DXVECTOR2 _curVel;
	D3DXVECTOR2 _curGravity;
	RECT _hitbox;
	bool _bBumpedLeft;
	bool _bBumpedRight;
	bool _bIsChargingJump;
	float _minJumpForce;
	float _maxJumpForce;
	float _curJumpForce;
	float _maxPosUp;
	float _maxPosDown;
	float _maxPosLeft;
	float _maxPosRight;
	bool _bIsOnGround;
	bool _bEnableFlyMode;

public:
	Player(std::shared_ptr<Engine> engine, int startingState)
	{
		_curPos = { 535.f,737.f };
		_curVel = { 0.0f, 0.0f };
		_curGravity = { 0.0f, 1500.f };
		_curState = startingState;
		_engine = engine;
		InitGraphics();
		_curSprite = _sprites[_curState];
		_hitbox.left = 20;
		_hitbox.right = _curSprite->GetWidth() - 20;
		_hitbox.top = 30;
		_hitbox.bottom = _curSprite->GetHeight() - 10;
		_minJumpForce = -0.5f;
		_maxJumpForce = -6.5f;
		_bIsOnGround = false;
		_oldState = MOVE_RIGHT;
		_curState = FALL_RIGHT;
		_maxPosUp = 0;
		_maxPosDown = 0;
		_maxPosLeft = 0;
		_maxPosRight = 0;
		_bEnableFlyMode = false;
	}

public:
	void Update(float dt);
	void Render(float dt);

	// Getters
	std::shared_ptr<Sprite> GetCurSprite() { return _curSprite; }
	D3DXVECTOR2 GetCurPosition() { return _curPos; }
	D3DXVECTOR2 GetCurVelocity() { return _curVel; }
	bool GetIsOnGround() { if (_bIsOnGround)return true; return false; }
	bool GetIsInAir() { if (!_bIsOnGround)return true; return false; }
	bool GetIsGoingUp() { if (_curVel.y < 0)return true; return false; }
	bool GetIsGoingDown() { if (_curVel.y > 0)return true; return false; }
	bool GetIsGoingLeft() { if (_curVel.x < 0)return true; return false; }
	bool GetIsGoingRight() { if (_curVel.x > 0)return true; return false; }
	bool GetBumpedLeft() { if (_bBumpedLeft)return true; return false; }
	bool GetBumpedRight() { if (_bBumpedRight)return true; return false; }
	LONG GetHitboxLeft() { return _hitbox.left; }
	LONG GetHitboxRight() { return _hitbox.right; }
	LONG GetHitboxTop() { return _hitbox.top; }
	LONG GetHitboxBottom() { return _hitbox.bottom; }
	RECT GetHitbox() { return _hitbox; }
	float GetMaxPosDown() { return _maxPosDown; }

	// Setters
	void SetPosition(D3DXVECTOR2 newPos) { _curPos = newPos; }
	void SetPositionX(float newPosX) { _curPos.x = newPosX; }
	void SetPositionY(float newPosY) { _curPos.y = newPosY; }
	void SetVelocity(D3DXVECTOR2 newVel) { _curVel = newVel; }
	void SetVelocityX(float newVelX) { _curVel.x = newVelX; }
	void SetVelocityY(float newVelY) { _curVel.y = newVelY; }
	void InvertVelocity() { _curVel.x *= -1.0f; _curVel.y *= -1.0f; }
	void InvertVelocityX() { _curVel.x *= -1.0f; }
	void InvertVelocityY() { _curVel.y *= -1.0f; }
	void SetIsOnGround() { _bIsOnGround = true; }
	void SetIsInAir() { _bIsOnGround = false; }
	void SetBumpedLeft() { _bBumpedLeft = true; _bBumpedRight = false; }
	void SetBumpedRight() { _bBumpedRight = true; _bBumpedLeft = false; }
	void SetMaxPosLeft(float value) { _maxPosLeft = value; } // replace with RECT?
	void SetMaxPosRight(float value) { _maxPosRight = value; }
	void SetMaxPosUp(float value) { _maxPosUp = value; }
	void SetMaxPosDown(float value) { _maxPosDown = value; }

private:
	bool InitGraphics();
	void GetSpriteForCurrentState();
	void Move();
	void Jump(float dt);
	void Movement(float dt);
	void ApplyVelocity(float dt);
	void Gravity(float dt);
	void GetJumpForce(float dt);
	void Fly();

	//
	// Shouldnt be here stuff
	//
	bool PressedLeft() { if (_engine->GetKeyboardPtr()->IsPressed(DIK_A))return true; return false; }
	bool PressedRight() { if (_engine->GetKeyboardPtr()->IsPressed(DIK_D))return true; return false; }
	bool PressedJump() { if (_engine->GetKeyboardPtr()->IsPressed(DIK_SPACE))return true; return false; }
	bool PressedUp() { if (_engine->GetKeyboardPtr()->IsPressed(DIK_W))return true; return false; }
	bool PressedDown() { if (_engine->GetKeyboardPtr()->IsPressed(DIK_S))return true; return false; }

public:
	void PlayJumpSound() { _engine->GetAudioPtr()->StartAudio(_engine->GetAudioPtr()->LoadAudioData(L"Assets\\Sounds\\jump.wav")); }
	void PlayLandSound() { _engine->GetAudioPtr()->StartAudio(_engine->GetAudioPtr()->LoadAudioData(L"Assets\\Sounds\\land.wav")); }
	void PlayBumpSound() { _engine->GetAudioPtr()->StartAudio(_engine->GetAudioPtr()->LoadAudioData(L"Assets\\Sounds\\bump.wav")); }
	void PlayFallSound() { _engine->GetAudioPtr()->StartAudio(_engine->GetAudioPtr()->LoadAudioData(L"Assets\\Sounds\\fall.wav")); }
};