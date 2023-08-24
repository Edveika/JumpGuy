#include "Line.h"

int Line::GetLineType()
{
	if (_p1.x == _p2.x)
		return LINE_TYPE_VERTICAL;
	else if (_p1.y == _p2.y)
		return LINE_TYPE_HORIZONTAL;
	else
		return LINE_TYPE_DIAGONAL;
}