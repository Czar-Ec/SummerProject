#include "Bullet.h"

#include <cmath>


Bullet::Bullet()
{
	speed = 650.0f;
}


Bullet::~Bullet()
{

}

void Bullet::shoot(SDL_Point shipLoc, float dir, int inBulletDamage, float bulletRange)
{
	//set starting bullet location to the ship location
	bulletLoc = shipLoc;

	range = bulletRange;

	//setting bullet velocities
	horizontalV = (float) cos(dir * M_PI / 180);
	verticalV = (float) sin(dir * M_PI / 180);

	//fire the bullet
	fired = true;

	//make bullet rect
	bulletRect.w = 4;
	bulletRect.h = 4;
	bulletRect.x = bX = bulletLoc.x;
	bulletRect.y = bY = bulletLoc.y;

	//set bullet damage
	bulletDamage = inBulletDamage;
}

void Bullet::update(float fps, SDL_Renderer *renderer, std::string bulletSource)
{
	//move the bullet
	bulletRect.x = bulletLoc.x = bX += horizontalV * speed / fps;
	bulletRect.y = bulletLoc.y = bY += verticalV * speed / fps;

	//proper graphics centering
	bulletRect.x = bulletLoc.x = bX - (bulletRect.w / 2);
	bulletRect.y = bulletLoc.y = bY - (bulletRect.h / 2);

	//calculate distance travelled
	float aSqr = pow((horizontalV * speed / fps), 2),
		bSqr = pow((verticalV * speed / fps), 2);
	float hypDist = sqrt(aSqr + bSqr);

	range -= hypDist;

	if (range <= 0)
	{
		setInactive();
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &bulletRect);

	//bullet colour
	if (bulletSource == "enemy")
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	}
	else if (bulletSource == "player")
	{		
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	//draw the bullet
	SDL_RenderDrawRect(renderer, &bulletRect); 
}

SDL_Point Bullet::getBulletLoc()
{
	return bulletLoc;
}

float Bullet::getBulletDamage()
{
	return bulletDamage;
}

bool Bullet::getFired()
{
	return fired;
}

void Bullet::setInactive()
{
	fired = false;
}
