#include "Button.h"

Button::Button()
{

}

Button::Button(int winWidth, int winHeight, std::string buttonText, int buttonPosition)
{
	//starting from 0
	buttonPosition++;
	
	//centre of the screen
	int centerX = winWidth / 2,
		centerY = winHeight / 2;

	//rectange size of the button
	int rectWidth = winWidth,
		rectHeight = winHeight / 10;

	//set button rectangle hotspot
	bRect.x = (centerX - rectWidth / 2);
	bRect.y = buttonPosition * (rectHeight + 10);
	bRect.w = rectWidth;
	bRect.h = rectHeight;

	//if button text is not null, apply text
	if (!buttonText.empty())
	{
		//save the text that the button should display
		bText = buttonText;

		//set the text rect
		/*assuming each letter is the length of the hotspot width / 20
		if the text will be larger than the button rectangle, set its width equal to the rectangle*/
		textRect.w = (bRect.w / 40) * bText.length();
		if (textRect.w > bRect.w)
		{
			textRect.w = bRect.w;
		}

		//height is fraction of hotspot i.e. 1/x
		int guiScale = 2;
		textRect.h = bRect.h / guiScale;

		//centering the text in the hotspot
		textRect.x = (centerX - textRect.w / 2);
		textRect.y = bRect.y + (bRect.h / (2 * guiScale));
	}
}

Button::Button(int winWidth, int winHeight, int xPadding, std::string buttonText, int buttonPosition)
{
	//rectange size of the button
	int rectWidth = winWidth,
		rectHeight = winHeight / 15;

	//set button rectangle hotspot
	bRect.x = xPadding;
	bRect.y = buttonPosition * (rectHeight + 10);
	bRect.w = winWidth / 10;
	bRect.h = rectHeight;

	bRect.y += 10;

	//if button text is not null, apply text
	if (!buttonText.empty())
	{
		//save the text that the button should display
		bText = buttonText;

		//set the text rect
		/*assuming each letter is the length of the hotspot width / 20
		if the text will be larger than the button rectangle, set its width equal to the rectangle*/
		textRect.w = (bRect.w / 10) * bText.length();

		if (textRect.w >= bRect.w)
		{
			textRect.w = bRect.w;
		}

		//height is fraction of hotspot i.e. 1/x
		int guiScale = 3;
		textRect.h = bRect.h / guiScale;

		//centering the text in the hotspot
		textRect.x = ((bRect.w - textRect.w) / 2) + bRect.x;
		textRect.y = ((bRect.h - textRect.h) / 2) + bRect.y;
	}
}

Button::Button(int bWidth, int bHeight, int buttonX, int buttonY, std::string buttonText)
{
	//setting button rectangle
	bRect.x = buttonX;
	bRect.y = buttonY;
	bRect.w = bWidth;
	bRect.h = bHeight;

	if (!buttonText.empty())
	{
		//set the text to be the same size as the button
		textRect = bRect;
		textRect.w = bRect.w / 2;
		textRect.h = bRect.h / 4;
		textRect.x = buttonX + bRect.w / 4;
		textRect.y = buttonY + bRect.h / 3;
		//save the text
		bText = buttonText;
	}

}

Button::~Button()
{

}

bool Button::pointInRect(SDL_Point point)
{
	//return 1 if point is in the menu rectangle
	return SDL_PointInRect(&point, &bRect);
}

void Button::draw(SDL_Renderer *renderer, SDL_Point point, TTF_Font *font)
{
	//text colour
	SDL_Color textColour;
	
	//if point is in menu rectangle, draw a different colour
	if (pointInRect(point))
	{
		textColour = { 255,255,255,0 };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	}
	else
	{
		textColour = { 255,0,0,0 };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 20);
	}
	//draw the rectangle
	SDL_RenderFillRect(renderer, &bRect);

	//if bText not empty, draw text
	if (!bText.empty())
		{
			//process of printing text is 
			SDL_Surface *textSurface = TTF_RenderText_Solid(font, bText.c_str() , textColour);
			SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

			SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

			//prevents memory leaks
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(textTexture);
		}	
}


