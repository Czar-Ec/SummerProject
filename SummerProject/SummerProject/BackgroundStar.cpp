#include "BackgroundStar.h"

BackgroundStar::BackgroundStar(SDL_Rect gameWindow)
{
	//set random location
	x = rand() % gameWindow.w + gameWindow.x;
	y = rand() % gameWindow.h + gameWindow.y;

	//initially visible
	visible = true;
}

BackgroundStar::~BackgroundStar()
{
}

float BackgroundStar::getX()
{
	return x;
}

float BackgroundStar::getY()
{
	return y;
}

void BackgroundStar::update(SDL_Renderer *renderer, SDL_Rect gameWindowRect)
{
	//chance to twinkle
	int twinkleChance = rand() % 10;
	if (twinkleChance == 0)
	{
		visible = false;
	}

	//move to the right of the screen
	x += 1;
	
	//relocate the star if it goes off screen
	if (x >= gameWindowRect.w + gameWindowRect.x) 
	{
		x = gameWindowRect.x + rand() % 16;

		y = rand() % gameWindowRect.h + gameWindowRect.y;
	}

	//draw the star
	draw(renderer);
}

void BackgroundStar::draw(SDL_Renderer *renderer)
{
	if (visible)
	{
		//set colour to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);

		SDL_Rect star = {x, y , 1, 1};
		SDL_RenderFillRect(renderer, &star);
	}
	else
	{
		visible = true;
	}
}

bool BackgroundStar::getVis()
{
	return visible;
}
