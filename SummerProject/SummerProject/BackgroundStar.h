#pragma once

#include <cstdlib>
#include <SDL\SDL.h>

class BackgroundStar
{
	public:
		BackgroundStar(SDL_Rect gameWindow);
		~BackgroundStar();

		//get star location
		float getX(), getY();

		//update the star
		void update(SDL_Renderer * renderer, SDL_Rect gameWindowRect);

		//draw star
		void draw(SDL_Renderer * renderer);

		//star visibility
		bool getVis();

	private:
		//star position
		float x, y;

		//allows the star to twinkle
		bool visible;
};

