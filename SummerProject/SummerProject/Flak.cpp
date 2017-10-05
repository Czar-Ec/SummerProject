#include "Flak.h"



Flak::Flak(int _explosionRadius, int _explosionPower, int _explosionYield, SDL_Rect _flakTextureRect)
{
	speed = 500.0f;
	explosionRadius = _explosionRadius;
	explosionPower = _explosionPower;
	explosionYield = _explosionYield;

	flakTextureRect = _flakTextureRect;
}


Flak::~Flak()
{
}

void Flak::shootFlak(SDL_Point shipLoc, float _dir, int inFlakDamage, SDL_Point mousePos)
{
	//set starting bullet location to the ship location
	flakLoc = shipLoc;

	//should pop where the mouse was
	float xDist = mousePos.x - shipLoc.x,
		yDist = mousePos.y - shipLoc.y;

	range = sqrt(pow(xDist, 2) + pow(yDist, 2));

	dir = _dir;

	//setting bullet velocities
	horizontalV = (float)cos(dir * M_PI / 180);
	verticalV = (float)sin(dir * M_PI / 180);

	//fire the flak
	fired = true;

	//make flak rect
	flakRect.w = 10;
	flakRect.h = 10;
	flakRect.x = fX = flakLoc.x;
	flakRect.y = fY = flakLoc.y;

	//set bullet damage
	flakDamage = inFlakDamage;
}

void Flak::updateFlak(float fps, SDL_Renderer * renderer, SDL_Texture *flakTexture, std::string flakSource, std::vector<Bullet> &bulletList, std::vector<Bullet> &enemyBulletList)
{
	//move the bullet
	flakRect.x = flakLoc.x = fX += horizontalV * speed / fps;
	flakRect.y = flakLoc.y = fY += verticalV * speed / fps;

	//proper graphics centering
	flakRect.x = flakLoc.x = fX - (flakRect.w / 2);
	flakRect.y = flakLoc.y = fY - (flakRect.h / 2);

	//calculate distance travelled
	float aSqr = pow((horizontalV * speed / fps), 2),
		bSqr = pow((verticalV * speed / fps), 2);
	float hypDist = sqrt(aSqr + bSqr);

	range -= hypDist;

	

	//bullet colour
	if (flakSource == "enemy")
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		if (range <= 0)
		{
			setInactive(enemyBulletList);
		}
	}
	else if (flakSource == "player")
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		if (range <= 0)
		{
			setInactive(bulletList);
		}
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}
	//draw the flak
	//SDL_RenderFillRect(renderer, &flakRect);

	SDL_RenderCopyEx(renderer, flakTexture, &flakTextureRect, &flakRect, dir + 90, NULL, SDL_FLIP_NONE);
}

SDL_Point Flak::getFlakLoc()
{
	return flakLoc;
}

float Flak::getFlakDamage()
{
	return flakDamage;
}

bool Flak::getFired()
{
	return fired;
}

void Flak::setInactive(std::vector<Bullet>& bulletList)
{
	fired = false;

	//explosion
	for (float angle = 0; angle < 360; angle += explosionYield)
	{
		Bullet b;
		b.shoot(flakLoc, angle, explosionPower, explosionRadius);
		bulletList.push_back(b);
	}
}
