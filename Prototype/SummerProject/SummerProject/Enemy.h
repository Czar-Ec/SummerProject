#pragma once
#include <SDL\SDL.h>

//for passing the bullet list
#include <vector>
#include "Bullet.h"
#include "Settings.h"

class Enemy
{
public:
	//default constructpr
	//Enemy();

	Enemy(int enemyType, int inHP, int inShield, float inSpeed, int damageToPlayer, SDL_Point spawnPoint);
	~Enemy();

	//check if enemy is alive
	bool isAlive();

	//get enemy location
	SDL_Point getEnemyLoc();

	//check if bullet has hit the enemy
	bool pointInEnemy(SDL_Point bulletLoc, int dmg);

	//damage the enemy
	void dmgEnemy(int dmg);

	//setter for where the enemy is facing
	void setDir(float locX, float locY);
	float getDir();

	//enemy movement
	void moveAggressive(float locX, float locY, int fps, SDL_Rect gameWindowRect);
	void moveCautious(float locX, float locY, int fps, SDL_Rect gameWindowRect);
	void moveDefensive(float locX, float locY, int fps, SDL_Rect gameWindowRect);

	void teleport(SDL_Rect gameWindowRect);

	//draw the enemy
	void draw(SDL_Renderer *renderer, SDL_Texture *texture);

	//draw the enemy shield
	void drawShield(SDL_Renderer *renderer, int alpha);
	
	//draw hp bar
	void drawBars(SDL_Renderer *renderer);

	//allows the enemy to do actions
	void update(int frames, float locX, float locY, int fps, SDL_Rect gameWindowRect, std::vector<Bullet> &enemyBulletList);

	//allows enemy to attack
	void attack(int healthPerc, int frames, std::vector<Bullet> &enemyBullet);
	
private:
	//type of ship the enemy has
	int shipType;

	//object location
	SDL_Point enemyLoc;
	//object hitbox
	SDL_Rect enemyHitBox;
	//ship texture rect
	SDL_Rect textureRect;

	float objX, objY;

	//enemy HP and enemy shield
	int curHP, curShield, enemyHP, enemyShield, curEnergy, tpEnergy, randFirerate;
	//enemy speed
	float speed;

	//enemy damage
	float damage;

	//where the enemy is looking at; useful for shooting
	float dir;
};

