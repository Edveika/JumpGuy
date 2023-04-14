#include "../../Includes.h"

void Sprite::Render(D3DXVECTOR2 spritePos, float dt)
{
	_animationTimer += dt;

	if (_animationTimer - _lastUpdate > _update)
	{
		if (_curSpriteFrame + 1 <= _numSpriteFrames)
		{
			++_curSpriteFrame;
		}
		else if (_curSpriteFrame + 1 > _numSpriteFrames)
		{
			_curSpriteFrame = 0;
		}
		_lastUpdate = _animationTimer;
	}

	_curSpriteImageRECT.left = _curSpriteFrame * _spriteWidth;
	_curSpriteImageRECT.right = _curSpriteImageRECT.left + _spriteWidth;

	_gfx->RenderTexture(_spriteSheetTexture, _sprite, spritePos, _curSpriteImageRECT, 0XFFFFFFFF);
}