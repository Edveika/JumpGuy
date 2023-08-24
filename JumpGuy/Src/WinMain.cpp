#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Timer/Timer.h"

// exceptions in constructors

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	srand(time(NULL));

	std::shared_ptr<Engine> engine =  std::make_shared<Engine>(hInstance);
	// After engine instance is created, draw a loading screen, because reading collision data from file will take some time
	engine->GetGraphicsPtr()->ClearScreen(0, 0, 0);
	engine->GetDirectX9Ptr()->GetDevice()->BeginScene();
	LPDIRECT3DSURFACE9 backBuffer;
	LPDIRECT3DSURFACE9 loadScreenImage = engine->GetGraphicsPtr()->GetSurfaceFromBitmap("Assets\\LoadScreen.bmp");
	engine->GetDirectX9Ptr()->GetDevice()->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	engine->GetDirectX9Ptr()->GetDevice()->StretchRect(loadScreenImage, NULL, backBuffer, NULL, D3DTEXF_NONE);
	engine->GetDirectX9Ptr()->GetDevice()->EndScene();
	engine->GetDirectX9Ptr()->GetDevice()->Present(NULL, NULL, NULL, NULL);

	std::shared_ptr<Timer> timer = std::make_shared<Timer>();
	std::shared_ptr<Game> game = std::make_shared<Game>(engine);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer->Start();
			engine->GetGraphicsPtr()->ClearScreen(0, 0, 0);
			engine->GetDirectX9Ptr()->GetDevice()->BeginScene();

			float dt = timer->GetTimePassed();
			game->Update(dt);
			game->Render(dt);

			engine->GetDirectX9Ptr()->GetDevice()->EndScene();
			engine->GetDirectX9Ptr()->GetDevice()->Present(NULL, NULL, NULL, NULL);
			timer->End();
		}
	}

#if _DEBUG
	fclose(fDummy);
	delete fDummy;
#endif

	return 0;
}