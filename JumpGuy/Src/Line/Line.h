#pragma once
#include <d3d9.h>
#include <d3dx9.h>

enum LineTypes
{
	LINE_TYPE_VERTICAL,
	LINE_TYPE_HORIZONTAL,
	LINE_TYPE_DIAGONAL,
};

class Line
{
private:
	D3DXVECTOR2 m_p1;
	D3DXVECTOR2 m_p2;
	int m_line_type;
	bool m_collided_left;
	bool m_collided_right;
	bool m_collided_top;
	bool m_collided_bottom;
	float m_collision_length;

public:
	Line();
	Line(D3DXVECTOR2 p1, D3DXVECTOR2 p2);

	// Getters
	int get_line_type();
	D3DXVECTOR2 get_p1() { return m_p1; }
	D3DXVECTOR2 get_p2() { return m_p2; }
	float get_collision_length() { return m_collision_length; }
	bool get_collided_left() { return m_collided_left; }
	bool get_collided_right() { return m_collided_right; }
	bool get_collided_top() { return m_collided_top; }
	bool get_collided_bottom() { return m_collided_bottom; }
	bool get_collided() { if (m_collided_left || m_collided_right || m_collided_top || m_collided_bottom)return true; return false; }
	float get_max_x() { if (m_p1.x > m_p2.x)return m_p1.x; else return m_p2.x; }
	float get_min_x() { if (m_p1.x > m_p2.x)return m_p2.x; else return m_p1.x; }
	float get_max_y() { if (m_p1.y > m_p2.y)return m_p1.y; else return m_p2.y; }
	float get_min_y() { if (m_p1.y > m_p2.y)return m_p2.y; else return m_p1.y; }

	// Setters
	void set_collision_length(float len) { m_collision_length = len; }
	void set_collided_left(bool value) { m_collided_left = value; }
	void set_collided_right(bool value) { m_collided_right = value; }
	void set_collided_top(bool value) { m_collided_top = value; }
	void set_collided_bottom(bool value) { m_collided_bottom = value; }
};