#pragma once
#pragma comment (lib,"dxguid.lib") 
#pragma comment (lib,"dinput8.lib")
#define DIRECTINPUT_VERSION 0x0800

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <tchar.h>
#include <filesystem>
#include <thread>
#include <d3d9.h>
#include <d3dx9.h>
#include <XAudio2.h>
#include <dinput.h>

#include "Engine/DirectX/DirectX9.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/XAudio2/XAudio2.h"
#include "Engine/Input/Keyboard/Keyboard.h"
#include "Engine/Input/Mouse/Mouse.h"
#include "Engine/Sprite/Sprite.h"
#include "Engine/Math/Math.h"
#include "Engine/Engine.h"
#include "Engine/Timer/Timer.h"
#include "Engine/Math/Line/Line.h"
#include "Game/Player/Player.h"
#include "Game/Map/Level/CollisionDetection/CollisionDetection.h"
#include "Game/Map/Level/Level.h"
#include "Game/Map/Map.h"
#include "Game/Game.h"