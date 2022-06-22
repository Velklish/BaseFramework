#include "Framework/pch.h"
#include "SolarGame/SolarGame.h"
#include "PongGame/PongGame.h"
#include <iostream>
#include "LightGame/LightGame.h"
using namespace Framework;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try
	{
		//auto game = new PongGame::PongGame();
		//auto game = new SolarGame::SolarGame();
		auto game = new LightGame::LightGame();
		game->Initialize(800, 600);
	}
	catch(::DX::com_exception& e)
	{
		auto test = e.what();
	}

	return 0;
}