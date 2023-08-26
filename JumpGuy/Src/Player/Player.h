#pragma once
#include <memory>
#include <Windows.h>
#include <XAudio2.h>

#include "../Engine/Engine.h"
#include "../Sprite/Sprite.h"

#define SPRITE_ARRAY_SIZE 12

class Player
{
private:
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

	std::shared_ptr<Sprite> m_sprites[SPRITE_ARRAY_SIZE];
	std::shared_ptr<Sprite> m_cur_sprite;
	std::shared_ptr<Engine> m_engine;
	IXAudio2SourceVoice* m_audio;
	int m_cur_state;
	int m_old_state;
	D3DXVECTOR2 m_cur_pos;
	D3DXVECTOR2 m_cur_vel;
	D3DXVECTOR2 m_cur_gravity;
	RECT m_hitbox;
	bool m_bumped_left;
	bool m_bumped_right;
	bool m_charging_jump;
	float m_min_jump_force;
	float m_max_jump_force;
	float m_cur_jump_force;
	float m_max_pos_up;
	float m_max_pos_down;
	float m_max_pos_left;
	float m_max_pos_right;
	bool m_on_ground;
	bool m_enable_fly_mode;

private:
	bool init_graphics();
	void get_sprite_for_current_state();
	void move();
	void jump(float dt);
	void apply_velocity(float dt);
	void gravity(float dt);
	void get_jump_force(float dt);
	void fly();

	bool pressed_left() { if (m_engine->get_keyboard_ptr()->is_pressed(DIK_A))return true; return false; }
	bool pressed_right() { if (m_engine->get_keyboard_ptr()->is_pressed(DIK_D))return true; return false; }
	bool pressed_jump() { if (m_engine->get_keyboard_ptr()->is_pressed(DIK_SPACE))return true; return false; }
	bool pressed_up() { if (m_engine->get_keyboard_ptr()->is_pressed(DIK_W))return true; return false; }
	bool pressed_down() { if (m_engine->get_keyboard_ptr()->is_pressed(DIK_S))return true; return false; }

public:
	Player(std::shared_ptr<Engine> engine);

	void update(float dt);
	void render(float dt);

	// Getters
	std::shared_ptr<Sprite> get_cur_sprite() { return m_cur_sprite; }
	D3DXVECTOR2 get_cur_position() { return m_cur_pos; }
	D3DXVECTOR2 get_cur_velocity() { return m_cur_vel; }
	bool get_on_ground() { if (m_on_ground)return true; return false; }
	bool get_in_air() { if (!m_on_ground)return true; return false; }
	bool get_going_up() { if (m_cur_vel.y < 0)return true; return false; }
	bool get_going_down() { if (m_cur_vel.y > 0)return true; return false; }
	bool get_going_left() { if (m_cur_vel.x < 0)return true; return false; }
	bool get_going_right() { if (m_cur_vel.x > 0)return true; return false; }
	bool get_bumped_left() { if (m_bumped_left)return true; return false; }
	bool get_bumped_right() { if (m_bumped_right)return true; return false; }
	LONG get_hitbox_left() { return m_hitbox.left; }
	LONG get_hitbox_right() { return m_hitbox.right; }
	LONG get_hitbox_top() { return m_hitbox.top; }
	LONG get_hitbox_bottom() { return m_hitbox.bottom; }
	RECT get_hitbox() { return m_hitbox; }
	float get_max_pos_down() { return m_max_pos_down; }

	// Setters
	void set_position(D3DXVECTOR2 newPos) { m_cur_pos = newPos; }
	void set_position_x(float newPosX) { m_cur_pos.x = newPosX; }
	void set_position_y(float newPosY) { m_cur_pos.y = newPosY; }
	void set_velocity(D3DXVECTOR2 newVel) { m_cur_vel = newVel; }
	void set_velocity_x(float newVelX) { m_cur_vel.x = newVelX; }
	void set_velocity_y(float newVelY) { m_cur_vel.y = newVelY; }
	void invert_velocity() { m_cur_vel.x *= -1.0f; m_cur_vel.y *= -1.0f; }
	void invert_velocity_x() { m_cur_vel.x *= -1.0f; }
	void invert_velocity_y() { m_cur_vel.y *= -1.0f; }
	void set_on_ground() { m_on_ground = true; }
	void set_in_air() { m_on_ground = false; }
	void set_bumped_left() { m_bumped_left = true; m_bumped_right = false; }
	void set_bumped_right() { m_bumped_right = true; m_bumped_left = false; }
	void set_max_pos_left(float value) { m_max_pos_left = value; } // replace with RECT?
	void set_max_pos_right(float value) { m_max_pos_right = value; }
	void set_max_pos_up(float value) { m_max_pos_up = value; }
	void set_max_pos_down(float value) { m_max_pos_down = value; }

	void play_jump_sound() { m_engine->get_audio_ptr()->play_audio(m_engine->get_audio_ptr()->load_audio_data(L"Assets\\Sounds\\jump.wav")); }
	void play_land_sound() { m_engine->get_audio_ptr()->play_audio(m_engine->get_audio_ptr()->load_audio_data(L"Assets\\Sounds\\land.wav")); }
	void play_bump_sound() { m_engine->get_audio_ptr()->play_audio(m_engine->get_audio_ptr()->load_audio_data(L"Assets\\Sounds\\bump.wav")); }
	void play_fall_sound() { m_engine->get_audio_ptr()->play_audio(m_engine->get_audio_ptr()->load_audio_data(L"Assets\\Sounds\\fall.wav")); }
};