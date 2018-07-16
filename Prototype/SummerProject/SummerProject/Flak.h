#pragma once

#include <SDL\SDL.h>
#include <vector>
#include "Settings.h"
#include "Bullet.h"

class Flak
{
public:
	Flak(int _explosionRadius, int _explosionPower, int _explosionYield, SDL_Rect _flakTextureRect);
	~Flak();

	//shoot out the flak
	void shootFlak(SDL_Point shipLoc, float _dir, int inFlakDamage, SDL_Point mousePos);

	//update the flak
	void updateFlak(float fps, SDL_Renderer *renderer, SDL_Texture *flakTexture, std::string flakSource, std::vector<Bullet> &bulletList, std::vector<Bullet> &enemyBulletList);

	//get flak location
	SDL_Point getFlakLoc();

	//get flak damage
	float getFlakDamage();

	//function to return if the flak was fired
	bool getFired();

	//destroys the flak
	void setInactive(std::vector<Bullet> &bulletList);

private:
	//settings
	Settings config;

	//flak location
	SDL_Point flakLoc;
	float fX, fY;
	SDL_Rect flakRect, flakTextureRect;

	//flak speed
	float speed,
		horizontalV, verticalV;

	float dir;

	//if the flak is fired
	bool fired;

	float flakDamage;

	//explosion stats
	int explosionPower, explosionYield, explosionRadius;

	//flak range
	float range;
};

