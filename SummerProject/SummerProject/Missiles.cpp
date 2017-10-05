#include "Missiles.h"

Missiles::Missiles(int _explosionRadius, int _explosionPower, int _explosiveYield)
{
	maxHorizontalV = maxVerticalV = 750.0f;
	speed = rand() % 20 + 10;
	horizontalV = rand() % 10 - 5;
	verticalV = rand() % 10 - 5;
	explosionRadius = _explosionRadius;
	explosionPower = _explosionPower;
	explosionYield = _explosiveYield;
}

Missiles::~Missiles()
{

}

void Missiles::shootMissile(SDL_Point shipLoc, int _missileDamage, float _range)
{
	//starting location is the ship location
	missileLoc = shipLoc;

	//range
	range = _range;

	//fire missile
	fired = true;

	//make the missile rect
	missileRect.w = 8;
	missileRect.h = 16;
	missileRect.x = mX = missileLoc.x;
	missileRect.y = mY = missileLoc.y;

	//set missile damage
	missileDmg = _missileDamage;
}

void Missiles::updateMissile(float fps, SDL_Renderer * renderer, SDL_Texture * missileTexture, std::string missileSrc, std::vector<Bullet>& bulletList, std::vector<Bullet>& enemyBulletList, SDL_Point shipLoc, std::vector<Enemy>& enemyList, SDL_Point mousePos)
{
	//find the nearest enemy
	float targetX = config.getWinWidth(), targetY = config.getWinHeight();

	//prevents crashing if there are no enemies on screen
	if (enemyList.size() <= 0)
	{
		targetX = mousePos.x;
		targetY = mousePos.y;
	}
	else
	{
		for (int curEnemy = 0; curEnemy < enemyList.size(); curEnemy++)
		{
			//get location of the scanned enemy
			SDL_Point enemyLoc = enemyList[curEnemy].getEnemyLoc();

			//if we find a closer enemy, it becomes our new target
			if (enemyLoc.x <= targetX && enemyLoc.y <= targetY)
			{
				targetX = enemyLoc.x;
				targetY = enemyLoc.y;
			}
		}

		//find the direction of the enemy
		float dX = targetX - missileLoc.x;
		float dY = targetY - missileLoc.y;

		//find angle
		float radian = atan2f(dY, dX);

		//set dir to degree value
		dir = (radian * 180 / M_PI);

		//calculating horizontal and vertical velocities
		horizontalV += (cos(dir * M_PI / 180));
		verticalV += (sin(dir * M_PI / 180));

		//make sure the missile doesnt go sanic
		if (horizontalV * speed / fps >= maxHorizontalV)
		{ horizontalV = maxHorizontalV; }
		if (verticalV * speed / fps >= maxHorizontalV)
		{ verticalV = maxVerticalV; }

		//move the missile
		missileRect.x = missileLoc.x = mX += horizontalV * speed / fps;
		missileRect.y = missileLoc.y = mY += verticalV * speed / fps;
	}
	
	//centering graphics
	missileRect.x = missileLoc.x = mX - (missileRect.w / 2);
	missileRect.y = missileLoc.y = mY - (missileRect.h / 2);

	//calculate distance travelled
	float aSqr = pow((horizontalV * speed / fps), 2),
		bSqr = pow((verticalV * speed / fps), 2);
	float hypDist = sqrt(aSqr + bSqr);

	range -= hypDist;

	//bullet colour
	if (missileSrc == "enemy")
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		if (range <= 0)
		{
			setInactive(enemyBulletList);
		}
		//random enemy missile texture
		int randTexture = rand() % 4;

		if (randTexture == 0)
		{
			missileTextureRect = enemyMissile1TextureRect;
		}
		else if (randTexture == 1)
		{
			missileTextureRect = enemyMissile2TextureRect;
		}
		else if (randTexture == 2)
		{
			missileTextureRect = enemyMissile3TextureRect;
		}
		else
		{
			missileTextureRect = enemyMissile4TextureRect;
		}
	}
	else if (missileSrc == "player")
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		if (range <= 0)
		{
			setInactive(bulletList);
		}
		//random missile texture
		int randTexture = rand() % 4;

		if (randTexture == 0)
		{
			missileTextureRect = missile1TextureRect;
		}
		else if (randTexture == 1)
		{
			missileTextureRect = missile2TextureRect;
		}
		else if (randTexture == 2)
		{
			missileTextureRect = missile3TextureRect;
		}
		else
		{
			missileTextureRect = missile4TextureRect;
		}
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	//render missile
	SDL_RenderCopyEx(renderer, missileTexture, &missileTextureRect, &missileRect, dir - 270, NULL, SDL_FLIP_NONE);
}

SDL_Point Missiles::getMissileLoc()
{
	return missileLoc;
}

float Missiles::getMissileDmg()
{
	return missileDmg;
}

bool Missiles::getFired()
{
	return fired;
}

void Missiles::setInactive(std::vector<Bullet>& bulletList)
{
	fired = false;

	//explosion
	for (float angle = 0; angle < 360; angle += explosionYield)
	{
		Bullet b;
		b.shoot(missileLoc, angle, explosionPower, explosionRadius);
		bulletList.push_back(b);
	}
	//secondary explosion
	for (float angle = 0; angle < 360; angle += explosionYield * 2)
	{
		Bullet b;
		b.shoot(missileLoc, angle, explosionPower, explosionRadius / 2);
		bulletList.push_back(b);
	}
}
