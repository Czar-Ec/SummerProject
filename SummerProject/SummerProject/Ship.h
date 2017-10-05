#pragma once
//SDL
#include <SDL\SDL.h>
//includes
#include <iostream>
class Ship
{
public:
	//constructors and destructors
	Ship();
	Ship(int width, int height, float inSpeed, int inshipType, SDL_Rect shipTexture);
	~Ship();

	//draw ship
	void draw(SDL_Renderer *renderer, SDL_Texture *texture);
	void drawShield(SDL_Renderer *renderer, int alpha);

	//move ship
	void moveUp(float fps);
	void moveDown(float fps);
	void moveLeft(float fps);
	void moveRight(float fps);
	void teleport(int x, int y);

	//get ship location
	float getLocX();
	float getLocY();

	//set and get directions
	void setDir(float direction);
	float getDir();

	//get speed
	float getSpeed();

	//player hitbox check
	bool playerHit(SDL_Point bulletLoc);

private:
	//the ship type
	int shipType;

	//stores the ship's location
	float shipX, shipY;

	//ship hitbox
	SDL_Rect hitbox;

	//chooses the correct texture
	SDL_Rect shipTextureSrc;

	//ship speed
	float speed;

	//ship direction
	float dir;
};

