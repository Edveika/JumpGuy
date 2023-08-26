#include <memory>

#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Timer/Timer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));
#if _DEBUG
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	std::shared_ptr<Game> game = std::make_shared<Game>(hInstance);

	game->run();

#if _DEBUG
	fclose(fDummy);
	delete fDummy;
#endif

	return 0;
}