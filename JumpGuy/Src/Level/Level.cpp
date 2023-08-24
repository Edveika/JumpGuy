#include "Level.h"

void Level::Update(std::shared_ptr<Player> player, float dt)
{
	_collisionDetection->PosCheck(player, _lines, dt);
	player->Update(dt);
	_collisionDetection->Update(player, _lines, dt);
}

void Level::Render(std::shared_ptr<Player> player, float dt)
{
	_engine->GetDirectX9Ptr()->GetDevice()->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &_backbuffer);
	_engine->GetDirectX9Ptr()->GetDevice()->StretchRect(_levelImage, NULL, _backbuffer, NULL, D3DTEXF_NONE);
	player->Render(dt);
}