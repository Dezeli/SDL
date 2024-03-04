#include "Game.h"

Game::Game() :
	mWindow(nullptr),
	mIsRunning(true)
{

}


bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("SDL initaialize Failed : %s", SDL_GetError());
		return false;
	}

	wchar_t wKtitle[256] = L"게임 개발 연습 1";
	char mKtitle[256];
	WideCharToMultiByte(CP_UTF8, 0, wKtitle, -1, mKtitle, 256, NULL, NULL);

	mWindow = SDL_CreateWindow(
		mKtitle,
		50,
		50,
		1200,
		700,
		0
	);

	if (!mWindow)
	{
		SDL_Log("Window Create Failed: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() 
{

}

void Game::UpdateGame() 
{

}

void Game::GenerateOutput() 
{

}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}