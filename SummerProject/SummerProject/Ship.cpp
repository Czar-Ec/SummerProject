#include "Ship.h"



Ship::Ship()
{

}

Ship::Ship(int width, int height, float inSpeed, int inshipType, SDL_Rect shipTexture)
{
	//ship location
	shipX = width / 2;
	shipY = height / 2;

	shipType = inshipType;
	shipTextureSrc = shipTexture;

	hitbox.x = shipX;
	hitbox.y = shipY;

	switch (shipType)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			hitbox.w = hitbox.h = 16;
			break;

		case 4:
		case 5:
			hitbox.w = 16, hitbox.h = 32;
			break;

		case 6:
			hitbox.w = hitbox.h = 32;
			break;

		case 7:
			hitbox.w = hitbox.h = 64;
			break;
	}	
	
	//ship speed
	speed = inSpeed;
}


Ship::~Ship()
{

}

void Ship::draw(SDL_Renderer *renderer, SDL_Texture *texture)
{
	//set colour to red
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);

	//adjust for correct centering
	hitbox.x = shipX - (hitbox.w / 2);
	hitbox.y = shipY - (hitbox.h / 2);

	//temporary point
	SDL_Point shipLoc = { hitbox.x, hitbox.y };

	SDL_RenderCopyEx(renderer, texture, &shipTextureSrc, &hitbox, dir + 90, NULL, SDL_FLIP_NONE);


	
	//draw the rectangle
	//SDL_RenderFillRect(renderer, &hitbox);
}

void Ship::drawShield(SDL_Renderer * renderer, int alpha)
{
	//set colour to red with alpha linked to shield percentage
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, alpha / 8);

	//adjust for correct centering
	hitbox.x = shipX - (hitbox.w / 2);
	hitbox.y = shipY - (hitbox.h / 2);

	//separate rect for shield
	SDL_Rect shieldRect = hitbox;
	shieldRect.h /= 2, shieldRect.w /= 2;

	for (int i = 0; i < 4; i++)
	{
		shieldRect.x = (shipX - shieldRect.w / 2) + i;
		shieldRect.y = (shipY - shieldRect.w / 2) + i;
		shieldRect.w -= i, shieldRect.h -= i;

		//draw the rectangle
		SDL_RenderDrawRect(renderer, &shieldRect);
	}

	//draw the rectangle
	SDL_RenderDrawRect(renderer, &hitbox);
}

void Ship::moveUp(float dTime)
{
	shipY -= speed * dTime;
}

void Ship::moveDown(float dTime)
{
	shipY += speed * dTime;
}

void Ship::moveLeft(float dTime)
{
	shipX -= speed * dTime;
}

void Ship::moveRight(float dTime)
{
	shipX += speed * dTime;
}

void Ship::teleport(int x, int y)
{
	shipX = x;
	shipY = y;
}

float Ship::getLocX()
{	
	return shipX;
}

float Ship::getLocY()
{
	return shipY;
}

void Ship::setDir(float direction)
{
	dir = direction;
}

float Ship::getDir()
{
	return dir;
}

float Ship::getSpeed()
{
	return speed;
}

bool Ship::playerHit(SDL_Point bulletLoc)
{
	return SDL_PointInRect(&bulletLoc, &hitbox);
}


