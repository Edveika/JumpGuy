#include "../Includes.h"

void Game::Update(float dt)
{
	_map->Update(_localPlayer, dt);
}

void Game::Render(float dt)
{
	_map->Render(_localPlayer, dt);
}