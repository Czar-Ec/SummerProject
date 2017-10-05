#include "Text.h"



Text::Text()
{
}

Text::Text(int textWidth, int textHeight, int x, int y, std::string text)
{
	textRect.w = textWidth,
		textRect.h = textHeight,
		textRect.x = x,
		textRect.y = y;

	displayText = text;
}

Text::Text(int textHeight,int winWidth, int y, std::string text)
{
	textRect.h = textHeight;
	textRect.w = textHeight * text.length();
	textRect.x = (winWidth / 2) - textRect.w / 2;
	textRect.y = y;

	displayText = text;
}


Text::~Text()
{
}

void Text::draw(SDL_Renderer * renderer, int red, int blue, int green, int alpha, TTF_Font * font)
{
	//text colour
	SDL_Color textColour = {red, blue, green, alpha};

	//printing text
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, displayText.c_str(), textColour);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	//prevents memory leaks
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}
