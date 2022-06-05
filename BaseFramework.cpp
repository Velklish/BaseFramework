#include "SolarGame.h"
#include "PongGame.h"
#include <iostream>

#include "pch.h"
using namespace BaseFramework;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try
	{
		//auto game = new PongGame();
		auto game = new SolarGame();
		game->Initialize(600, 800);
	}
	catch(::DX::com_exception& e)
	{
		auto test = e.what();
	}

	return 0;
}