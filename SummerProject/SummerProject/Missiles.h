#pragma once

#include <SDL\SDL.h>
#include <vector>
#include "Settings.h"
#include "Bullet.h"
#include "Enemy.h"

class Missiles
{
public:
	Missiles(int _explosionRadius, int _explosionPower, int _explosiveYield);
	~Missiles();

	//shoot missile
	void shootMissile(SDL_Point shipLoc, int _missileDamage, float _range);

	//update missile
	void updateMissile(float fps, SDL_Renderer *renderer, SDL_Texture *missileTexture, std::string missileSrc, std::vector<Bullet> &bulletList, std::vector<Bullet> &enemyBulletList, SDL_Point shipLoc, std::vector <Enemy> &enemyList, SDL_Point mousePos);

	//get missile location
	SDL_Point getMissileLoc();

	//missile dmg
	float getMissileDmg();

	//return if missile is active
	bool getFired();

	//destroys the missile
	void setInactive(std::vector<Bullet> &bulletList);

private:
	//settings
	Settings config;

	//missile location
	SDL_Point missileLoc;
	float mX, mY;

	//render rects
	SDL_Rect missileRect, missileTextureRect;

	//missile rects
	SDL_Rect missile1TextureRect = { 120, 112, 8, 16 },
		missile2TextureRect = { 112, 112, 8, 16 },
		missile3TextureRect = { 104, 112, 8, 16 },
		missile4TextureRect = { 96, 112, 8, 16 };

	//enemy missile rects
	SDL_Rect enemyMissile1TextureRect = { 88, 112, 8, 16 },
		enemyMissile2TextureRect = { 80, 112, 8, 16 },
		enemyMissile3TextureRect = { 72, 112, 8, 16 },
		enemyMissile4TextureRect = { 64, 112, 8, 16 };

	//missile speed
	float horizontalV, maxHorizontalV, verticalV, maxVerticalV, speed;

	//direction
	float dir;

	//if the flak is fired
	bool fired;


	//missile damage
	float missileDmg;

	int explosionPower, explosionYield, explosionRadius;

	float range;
};

