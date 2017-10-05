#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <iostream>
class Button
{
public:
	//constructor and destructor
	Button();
	Button(int winWidth, int winHeight, std::string buttonText, int buttonPosition);
	Button(int winWidth, int winHeight, int xPadding, std::string buttonText, int buttonPosition);
	Button(int bWidth, int bHeight, int buttonX, int buttonY, std::string buttonText);
	~Button();

	//checks if passed point is within the button
	bool pointInRect(SDL_Point point);

	//draw the button
	//if highlight = 1, draw in a different colour
	void draw(SDL_Renderer *renderer, SDL_Point point, TTF_Font *font);

private:
	//button area
	SDL_Rect bRect;
	SDL_Rect textRect;

	//button text
	std::string bText;
};

