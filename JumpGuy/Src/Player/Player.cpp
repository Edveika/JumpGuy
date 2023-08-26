#include "Player.h"

Player::Player(std::shared_ptr<Engine> engine)
{
	m_engine = engine;
	if (!m_engine)
	{
		MessageBoxA(NULL, "[Player]: failed to create engine obj", NULL, NULL);
		throw std::invalid_argument("[Player]: engine not created");
	}
	m_cur_pos = { 535.f,737.f };
	m_cur_vel = { 0.0f, 0.0f };
	m_cur_gravity = { 0.0f, 1500.f };
	m_cur_state = IDLE_RIGHT;
	init_graphics();
	m_cur_sprite = m_sprites[m_cur_state];
	if (!m_cur_sprite)
	{
		MessageBoxA(NULL, "[Player]: cursprite was null", NULL, NULL);
		throw std::invalid_argument("[Player]: cursprite null");
	}
	m_hitbox.left = 20;
	m_hitbox.right = m_cur_sprite->get_width() - 20;
	m_hitbox.top = 30;
	m_hitbox.bottom = m_cur_sprite->get_height() - 10;
	m_min_jump_force = -0.5f;
	m_max_jump_force = -6.5f;
	m_on_ground = false;
	m_old_state = MOVE_RIGHT;
	m_cur_state = FALL_RIGHT;
	m_max_pos_up = 0;
	m_max_pos_down = 0;
	m_max_pos_left = 0;
	m_max_pos_right = 0;
	m_enable_fly_mode = false;
}

void Player::update(float dt)
{
	m_engine->get_keyboard_ptr()->get_input();

#ifdef _DEBUG
	if (GetAsyncKeyState('L') & 1)
		_bEnableFlyMode = !_bEnableFlyMode;

	if (!_bEnableFlyMode)
	{
#endif
		move();
		jump(dt);
		apply_velocity(dt);
		gravity(dt);
		get_sprite_for_current_state();
#ifdef _DEBUG
	}
	else
		Fly();
#endif
}

// Needs to be rewritten in a simple way
void Player::get_sprite_for_current_state()
{
	if (get_going_left() && m_on_ground)
	{
		m_old_state = MOVE_LEFT;
		m_cur_sprite = m_sprites[MOVE_LEFT];
	}
	else if (get_going_right() && m_on_ground)
	{
		m_old_state = MOVE_RIGHT;
		m_cur_sprite = m_sprites[MOVE_RIGHT];
	}
	else if (get_going_up() && !m_bumped_left && !m_bumped_right)
	{
		if (get_going_left())
		{
			m_cur_sprite = m_sprites[JUMP_LEFT];
		}
		else if (get_going_right())
		{
			m_cur_sprite = m_sprites[JUMP_RIGHT];
		}
		else
		{
			if (m_old_state == MOVE_LEFT || m_old_state == BUMP_LEFT)
			{
				m_cur_sprite = m_sprites[JUMP_LEFT];
			}
			else if (m_old_state == MOVE_RIGHT || m_old_state == BUMP_RIGHT)
			{
				m_cur_sprite = m_sprites[JUMP_RIGHT];
			}
		}
	}
	else if (get_going_down() && !m_bumped_left && !m_bumped_right)
	{
		if (get_going_left())
		{
			m_cur_sprite = m_sprites[FALL_LEFT];
		}
		else if (get_going_right())
		{
			m_cur_sprite = m_sprites[FALL_RIGHT];
		}
		else
		{
			if (m_old_state == MOVE_LEFT || m_old_state == BUMP_LEFT)
			{
				m_cur_sprite = m_sprites[FALL_LEFT];
			}
			else if (m_old_state == MOVE_RIGHT || m_old_state == BUMP_RIGHT)
			{
				m_cur_sprite = m_sprites[FALL_RIGHT];
			}
		}
	}
	else if (m_charging_jump)
	{
		m_cur_sprite = m_sprites[CHARGING_JUMP];
	}
	else if (m_bumped_left && !m_on_ground)
	{
		m_old_state = BUMP_LEFT;
		m_cur_sprite = m_sprites[BUMP_LEFT];
	}
	else if (m_bumped_right && !m_on_ground)
	{
		m_old_state = BUMP_RIGHT;
		m_cur_sprite = m_sprites[BUMP_RIGHT];
	}
	else
	{
		if (m_old_state == MOVE_LEFT || m_old_state == BUMP_LEFT || m_old_state == JUMP_LEFT)
		{
			m_cur_sprite = m_sprites[IDLE_LEFT];
		}
		else if (m_old_state == MOVE_RIGHT || m_old_state == BUMP_RIGHT || m_old_state == JUMP_RIGHT)
		{
			m_cur_sprite = m_sprites[IDLE_RIGHT];
		}
	}
	if (m_on_ground)
	{
		m_bumped_left = false;
		m_bumped_right = false;
	}
}

void Player::move()
{
	if (!m_charging_jump)
	{
		if (m_on_ground)
		{
			if (pressed_left())
			{
				m_cur_vel.x = -300;
			}
			else if (pressed_right())
			{
				m_cur_vel.x = 300;
			}
			else
			{
				m_cur_vel = { 0,0 };
			}
		}
	}
	else
	{
		if (m_on_ground)
		{
			m_cur_vel = { 0,0 };
		}
	}
}

void Player::jump(float dt)
{
	if (m_on_ground)
	{
		if (pressed_jump())
		{
			get_jump_force(dt);
		}
		else if (m_cur_jump_force < m_min_jump_force)
		{
			play_jump_sound();

			if (pressed_left())
			{
				m_cur_vel.x = -400;
			}
			else if (pressed_right())
			{
				m_cur_vel.x = 400;
			}

			m_cur_vel.y = 200 * m_cur_jump_force;
			m_on_ground = false;
			m_charging_jump = false;
			m_cur_jump_force = 0;
		}
		else
		{
			m_charging_jump = false;
			m_cur_jump_force = 0;
		}
	}
}

void Player::get_jump_force(float dt)
{
	m_charging_jump = true;

	if (m_cur_jump_force == 0)
	{
		m_cur_jump_force = m_min_jump_force;
	}
	else if (m_cur_jump_force - dt * 10 >= m_max_jump_force)
	{
		m_cur_jump_force -= dt * 10;
	}
}

void Player::apply_velocity(float dt)
{
	if (get_going_left() && m_max_pos_left != 0 && m_cur_pos.x + get_hitbox_left() + m_cur_vel.x * dt < m_max_pos_left)
	{
		m_cur_pos.x = m_max_pos_left - get_hitbox_left();
	}
	else if (get_going_right() && m_max_pos_right != 0 && m_cur_pos.x + get_hitbox_right() + m_cur_vel.x * dt > m_max_pos_right)
	{
		m_cur_pos.x = m_max_pos_right - get_hitbox_right();
	}
	else
	{
		m_cur_pos.x = m_cur_pos.x + m_cur_vel.x * dt;
	}

	if (get_going_up() && m_max_pos_up != 0 && (m_cur_pos.y + get_hitbox_top()) + m_cur_vel.y * dt < m_max_pos_up)
	{
		m_cur_pos.y = m_max_pos_up - get_hitbox_top();
	}
	else if (get_going_down() && m_max_pos_down != 0 && m_cur_pos.y + get_hitbox_bottom() + m_cur_vel.y * dt > m_max_pos_down)
	{
		m_cur_pos.y = m_max_pos_down - get_hitbox_bottom();
	}
	else
	{
		m_cur_pos.y = m_cur_pos.y + m_cur_vel.y * dt;
	}
}

void Player::fly()
{
	m_cur_vel = { 0, 0 };
	if (pressed_left())
		m_cur_pos.x -= 10;
	else if (pressed_right())
		m_cur_pos.x += 10;
	else if (pressed_up())
		m_cur_pos.y -= 10;
	else if (pressed_down())
		m_cur_pos.y += 10;
}

// Maybe needs to be moved elsewhere
void Player::gravity(float dt)
{
	if (!m_on_ground)
	{
		m_cur_vel = m_cur_vel + m_cur_gravity * dt;
	}
}

// Needs to be moved elsewhere
bool Player::init_graphics()
{
	float s_AnimationUpdateSpeed = 0.1f;

	if ((m_sprites[MOVE_LEFT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\runLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[MOVE_RIGHT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\run.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[JUMP_LEFT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\jumpLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[JUMP_RIGHT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\jump.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[IDLE_LEFT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\idleLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[IDLE_RIGHT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\idle.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[FALL_LEFT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\fallLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[FALL_RIGHT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\fall.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[BUMP_LEFT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\oofLeft.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[BUMP_RIGHT] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\oof.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;
	if ((m_sprites[CHARGING_JUMP] = std::make_shared<Sprite>(m_engine->get_graphics_ptr(), L"Assets\\Sprites\\squat.png", 93, 103, s_AnimationUpdateSpeed)) == NULL) return false;

	return true;
}

void Player::render(float dt)
{
	m_cur_sprite->render(m_cur_pos, dt);
}