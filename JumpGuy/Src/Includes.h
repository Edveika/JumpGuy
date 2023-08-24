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
#include <stdexcept>

#include "DirectX/DirectX9.h"
#include "Graphics/Graphics.h"
#include "XAudio2/XAudio2.h"
#include "Input/Keyboard/Keyboard.h"
#include "Input/Mouse/Mouse.h"
#include "Sprite/Sprite.h"
#include "Engine/Engine.h"
#include "Timer/Timer.h"
#include "Line/Line.h"
#include "Player/Player.h"
#include "CollisionDetection/CollisionDetection.h"
#include "Level/Level.h"
#include "Map/Map.h"
#include "Game/Game.h"