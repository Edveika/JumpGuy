#include "Line.h"

Line::Line() {}

Line::Line(D3DXVECTOR2 p1, D3DXVECTOR2 p2)
{
	m_p1 = p1;
	m_p2 = p2;
	m_collided_left = false;
	m_collided_right = false;
	m_collided_top = false;
	m_collided_bottom = false;
	m_collision_length = 0.0f;
	m_line_type = get_line_type();

	// Ensures lines are in order. Top to bottom, left to right
	if (m_line_type == LINE_TYPE_VERTICAL || m_line_type == LINE_TYPE_HORIZONTAL || m_line_type == LINE_TYPE_DIAGONAL)
	{
		if (p1.x > p2.x || p2.y < p1.y)
		{
			D3DXVECTOR2 tmp = p2;
			m_p2 = p1;
			m_p1 = tmp;
		}
	}
}

int Line::get_line_type()
{
	if (m_p1.x == m_p2.x)
		return LINE_TYPE_VERTICAL;
	else if (m_p1.y == m_p2.y)
		return LINE_TYPE_HORIZONTAL;
	else
		return LINE_TYPE_DIAGONAL;
}