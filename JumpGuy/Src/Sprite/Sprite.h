#pragma once
#include <memory>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>

#include "../Graphics/Graphics.h"

enum AnimationType
{
	ANIM_NONE,
	ANIM_FORWARD,
	ANIM_BACKWARD,
};

class Sprite
{
private:
	LPDIRECT3DTEXTURE9 _spriteSheetTexture = NULL;
	LPD3DXSPRITE _sprite = NULL;
	D3DXIMAGE_INFO _spriteSheetInfo;
	std::shared_ptr<Graphics> _gfx;
	float _timePassed;
	RECT _curSpriteImageRECT;
	RECT _spriteImageRECT;
	int _spriteWidth;
	int _spriteHeight;
	int _numSpriteFrames;
	int _curSpriteFrame;
	float _update;
	float _lastUpdate;
	float _animationTimer;

public:
	Sprite(std::shared_ptr<Graphics> gfx, LPCWSTR fileName, int frameWidth, int frameHeight, float animationUpdate)
	{
		_update = animationUpdate;
		_gfx = gfx;
		_spriteWidth = frameWidth;
		_spriteHeight = frameHeight;
		_curSpriteFrame = 0;
		_curSpriteImageRECT.top = 0;
		_curSpriteImageRECT.left = 0;
		_curSpriteImageRECT.right = _curSpriteImageRECT.left + frameWidth;
		_curSpriteImageRECT.bottom = frameHeight;
		_timePassed = 0.0f;
		_lastUpdate = 0;
		_animationTimer = 0;

		gfx->GetImageInfo(fileName, _spriteSheetInfo);

		if (FAILED(D3DXCreateTextureFromFileEx(_gfx->_pd3dDevice, fileName, _spriteSheetInfo.Width, _spriteSheetInfo.Height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &_spriteSheetTexture)))
			MessageBoxA(NULL, "Failed to create texture from file!", NULL, NULL);

		_numSpriteFrames = (_spriteSheetInfo.Width / frameWidth) - 1;

		if (FAILED(D3DXCreateSprite(_gfx->_pd3dDevice, &_sprite)))
			MessageBoxA(NULL, "Failed to create a sprite!", NULL, NULL);
	}
	~Sprite()
	{
		_spriteSheetTexture->Release();
		_sprite->Release();
	}

public:
	void Render(D3DXVECTOR2 spritePos, float dt);

	// Getters
	int GetWidth() { return _spriteWidth; }
	int GetHeight() { return _spriteHeight; }
};