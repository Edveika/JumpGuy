#pragma once

enum LineTypes
{
	LINE_TYPE_VERTICAL,
	LINE_TYPE_HORIZONTAL,
	LINE_TYPE_DIAGONAL,
};

class Line
{
private:
	D3DXVECTOR2 _p1;
	D3DXVECTOR2 _p2;
	int _lineType;
	bool _collidedLeft;
	bool _collidedRight;
	bool _collidedTop;
	bool _collidedBottom;
	float _collisionLength;

public:
	Line() {}
	Line(D3DXVECTOR2 p1, D3DXVECTOR2 p2)
	{
		_p1 = p1;
		_p2 = p2;
		_collidedLeft = false;
		_collidedRight = false;
		_collidedTop = false;
		_collidedBottom = false;
		_collisionLength = 0.0f;
		_lineType = GetLineType();

		// Ensures lines are in order. Top to bottom, left to right
		if (_lineType == LINE_TYPE_VERTICAL || _lineType == LINE_TYPE_HORIZONTAL || _lineType == LINE_TYPE_DIAGONAL)
		{
			if (p1.x > p2.x || p2.y < p1.y)
			{
				D3DXVECTOR2 tmp = p2;
				_p2 = p1;
				_p1 = tmp;
			}
		}
	}
	~Line()
	{

	}

public:
	void ResetCollisionData();
	// Getters
	int GetLineType();
	D3DXVECTOR2 GetP1() { return _p1; }
	D3DXVECTOR2 GetP2() { return _p2; }
	float GetCollisionLength() { return _collisionLength; }
	bool GetCollidedLeft() { return _collidedLeft; }
	bool GetCollidedRight() { return _collidedRight; }
	bool GetCollidedTop() { return _collidedTop; }
	bool GetCollidedBottom() { return _collidedBottom; }
	bool GetCollided() { if (_collidedLeft || _collidedRight || _collidedTop || _collidedBottom)return true; return false; }
	float GetMaxX() { if (_p1.x > _p2.x)return _p1.x; else return _p2.x; }
	float GetMinX() { if (_p1.x > _p2.x)return _p2.x; else return _p1.x; }
	float GetMaxY() { if (_p1.y > _p2.y)return _p1.y; else return _p2.y; }
	float GetMinY() { if (_p1.y > _p2.y)return _p2.y; else return _p1.y; }

	// Setters
	void SetCollisionLength(float len) { _collisionLength = len; }
	void SetCollidedLeft(bool value) { _collidedLeft = value; }
	void SetCollidedRight(bool value) { _collidedRight = value; }
	void SetCollidedTop(bool value) { _collidedTop = value; }
	void SetCollidedBottom(bool value) { _collidedBottom = value; }
};