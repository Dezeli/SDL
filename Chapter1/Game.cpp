#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

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

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddle1Pos.x = 10.0f;
	mPaddle1Pos.y = 700.0f / 2.0f;
	mPaddle2Pos.x = 1200.0f - 10.0f - thickness;
	mPaddle2Pos.y = 700.0f / 2.0f;

	sBall ball1{ Vector2{1200.0f / 2.0f, 700.0f / 2.0f}, Vector2{-200.0f, 235.0f} };
	sBall ball2{ Vector2{1000.0f / 2.0f, 700.0f / 2.0f}, Vector2{-100.0f, 110.0f} };
	Balls.push_back(ball1);
	Balls.push_back(ball2);
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
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1Dir += 1;
	}

	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame() 
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;


	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	if (mPaddle1Dir != 0)
	{
		mPaddle1Pos.y += mPaddle1Dir * 300.0f * deltaTime;
		if (mPaddle1Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle1Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle1Pos.y > (700.0f - paddleH / 2.0f - thickness))
		{
			mPaddle1Pos.y = 700.0f - paddleH / 2.0f - thickness;
		}
	}
	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;
		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (700.0f - paddleH / 2.0f - thickness))
		{
			mPaddle2Pos.y = 700.0f - paddleH / 2.0f - thickness;
		}
	}
	for (std::vector<sBall>::size_type i = 0; i < Balls.size(); i++) {
		Balls[i].Pos.x += Balls[i].Vel.x * deltaTime;
		Balls[i].Pos.y += Balls[i].Vel.y * deltaTime;

		float p1diff = mPaddle1Pos.y - Balls[i].Pos.y;
		float p2diff = mPaddle2Pos.y - Balls[i].Pos.y;
		p1diff = (p1diff > 0.0f) ? p1diff : -p1diff;
		p2diff = (p2diff > 0.0f) ? p2diff : -p2diff;
		if (
			p1diff <= paddleH / 2.0f &&
			Balls[i].Pos.x <= 25.0f && Balls[i].Pos.x >= 20.0f &&
			Balls[i].Vel.x < 0.0f)
		{
			Balls[i].Vel.x *= -1.0f;
		}
		else if (
			p2diff <= paddleH / 2.0f &&
			Balls[i].Pos.x >= 1200.0f - 25.0f && Balls[i].Pos.x <= 1200.0f - 20.0f &&
			Balls[i].Vel.x > 0.0f
			)
		{
			Balls[i].Vel.x *= -1.0f;
		}
		else if (Balls[i].Pos.x <= 0.0f)
		{
			mIsRunning = false;
		}
		else if (Balls[i].Pos.x >= 1200.0f)
		{
			mIsRunning = false;
		}
		if (Balls[i].Pos.y <= thickness && Balls[i].Vel.y < 0.0f)
		{
			Balls[i].Vel.y *= -1;
		}
		else if (Balls[i].Pos.y >= (700 - thickness) && Balls[i].Vel.y > 0.0f)
		{
			Balls[i].Vel.y *= -1;
		}

	}
}

void Game::GenerateOutput() 
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		255,
		255,
		255
	);
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect wall{
		0,
		0,
		1200,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);

	wall.y = 700 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);



	SDL_Rect paddle{
		static_cast<int>(mPaddle1Pos.x),
		static_cast<int>(mPaddle1Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	paddle.x = static_cast<int>(mPaddle2Pos.x);
	paddle.y = static_cast<int>(mPaddle2Pos.y - paddleH / 2);
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_Rect ball{
			static_cast<int>(1200.0f / 2.0f - thickness / 2),
			static_cast<int>(700.0f / 2.0f - thickness / 2),
			thickness,
			thickness
	};
	for (std::vector<sBall>::size_type i = 0; i < Balls.size(); i++) {
		ball.x = static_cast<int>(Balls[i].Pos.x - static_cast<float>(thickness) / 2);
		ball.y = static_cast<int>(Balls[i].Pos.y - static_cast<float>(thickness) / 2);
		SDL_RenderFillRect(mRenderer, &ball);
	}
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}