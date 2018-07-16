#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <iostream>

class Text
{
	public:
		Text();
		Text(int textWidth, int textHeight, int x, int y, std::string text);
		Text(int textHeight, int winWidth, int y, std::string text);
		~Text();

		void draw(SDL_Renderer *renderer, int red, int blue, int green, int alpha, TTF_Font *font);

	private:
		//text to display
		std::string displayText;
		//rectangle where the text will be displayed
		SDL_Rect textRect;
};

