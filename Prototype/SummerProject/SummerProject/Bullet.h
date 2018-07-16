#pragma once
#include <SDL\SDL.h>
#include "Settings.h"
class Bullet
{
public:
	//constructor destructor
	Bullet();
	~Bullet();

	//shoot out bullet
	void shoot(SDL_Point shipLoc, float dir, int inBulletDamage, float bulletRange);

	//update bullet
	void update(float fps, SDL_Renderer *renderer, std::string bulletSource);

	//get bullet location
	SDL_Point getBulletLoc();

	//get bullet damage
	float getBulletDamage();

	//function to return if the bullet was fired
	bool getFired();

	//destroys the bullet
	void setInactive();

private:
	//settings
	Settings config;
	
	//bullet location
	SDL_Point bulletLoc;
	float bX, bY;
	SDL_Rect bulletRect;

	//bullet speed
	float speed,
		horizontalV, verticalV;

	//if the bullet is fired
	bool fired;

	float bulletDamage;

	//bullet range
	float range;
};

