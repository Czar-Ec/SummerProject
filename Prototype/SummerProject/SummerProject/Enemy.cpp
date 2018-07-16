#include "Enemy.h"

Enemy::Enemy(int enemyType, int inHP, int inShield, float inSpeed, int damageToPlayer, SDL_Point spawnPoint)
{
	//enemy's ship type
	shipType = enemyType;

	//randomise the firerate
	randFirerate = rand() % 10 - 5;
	
	////current and max hp
	//enemyHP = curHP = inHP;

	////current and max shield
	//enemyShield = curShield = inShield;

	////speed
	//speed = inSpeed;

	//where the enemy spawns
	enemyLoc = spawnPoint;

	////how often the enemy will fire
	//firerate = inFirerate;

	objX = spawnPoint.x;
	objY = spawnPoint.y;

	enemyHitBox.x = spawnPoint.x;
	enemyHitBox.y = spawnPoint.y;

	damage = damageToPlayer;

	//apply texture
	switch (enemyType)
	{
		case 0:
			//hitbox
			enemyHitBox.w = enemyHitBox.h = 16;
			
			//normal stats
			enemyHP = curHP = inHP;
			enemyShield = curShield = inShield;
			speed = inSpeed;

			//ship texture
			textureRect = {0, 0, 16, 16};
			break;

		case 1:
			//hitbox
			enemyHitBox.w = enemyHitBox.h = 16;

			//tanky stats
			enemyHP = curHP = inHP * 1.2;
			enemyShield = curShield = inShield * 1.2;
			speed = inSpeed * 0.8;

			//ship texture
			textureRect = { 16, 0, 16, 16 };
			break;

		case 2:
			//hitbox
			enemyHitBox.w = enemyHitBox.h = 16;

			//glass cannon stats
			enemyHP = curHP = inHP * 0.8;
			enemyShield = curShield = inShield / 2;
			speed = inSpeed;

			//ship texture
			textureRect = { 32, 0, 16, 16 };
			break;

		case 3:
			//hitbox
			enemyHitBox.w = enemyHitBox.h = 16;

			//fast stats
			enemyHP = curHP = inHP / 4;
			enemyShield = curShield = inShield / 4;
			speed = inSpeed * 2;

			//ship texture
			textureRect = { 48, 0, 16, 16 };
			break;

		case 4:
			//new hitbox
			enemyHitBox.w = 16, enemyHitBox.h = 32;

			//frigate stats
			enemyHP = curHP = inHP * 1.2;
			enemyShield = curShield = inShield * 2;
			speed = inSpeed / 2;

			//ship texture
			textureRect = { 0, 16, 16, 32 };
			break;

		case 5:
			//new hitbox
			enemyHitBox.w = 16, enemyHitBox.h = 32;

			//cruiser stats
			enemyHP = curHP = inHP * 3;
			enemyShield = curShield = inShield;
			speed = inSpeed / 2;

			//ship texture
			textureRect = { 16, 16, 16, 32 };
			break;

		case 6:
			//new hitbox
			enemyHitBox.w = enemyHitBox.h = 32;

			//battlecruiser stats
			enemyHP = curHP = inHP * 4;
			enemyShield = curShield = inShield * 1.2;
			speed = inSpeed / 4;

			//ship texture
			textureRect = { 32, 16, 32, 32 };
			break;

		case 7:
			//new hitbox
			enemyHitBox.w = enemyHitBox.h = 64;

			//dreadnought stats
			enemyHP = curHP = inHP * 5;
			enemyShield = curShield = inShield * 3;
			speed = inSpeed / 6;

			//ship texture
			textureRect = { 0, 48, 64, 64 };
			break;
	}

	//all enemies have the same tpenergy, prevents tp spam
	curEnergy = tpEnergy = 200;

	//larger hitbox so player doesnt need to hit a small ship
	enemyHitBox.w *= 2;
	enemyHitBox.h *= 2;
}

Enemy::~Enemy()
{

}

bool Enemy::isAlive()
{
	return (curHP > 0);
}

SDL_Point Enemy::getEnemyLoc()
{
	return enemyLoc;
}

bool Enemy::pointInEnemy(SDL_Point bulletLoc, int dmg)
{
	//adjust for correct centering
	enemyHitBox.x = objX - enemyHitBox.w / 2;
	enemyHitBox.y = objY - enemyHitBox.h / 2;
	
	if (SDL_PointInRect(&bulletLoc, &enemyHitBox))
	{
		dmgEnemy(dmg);
	
		return true;
	}
	else
	{
		return false;
	}
	
}

void Enemy::dmgEnemy(int dmg)
{
	//makes sure the shield doesn't get broken / glitched
	if (curShield > dmg)
	{
		curShield -= dmg;
	}
	//if the dmg is more than shield
	else if (curShield <= dmg)
	{
		curShield = 0;
		curHP -= (dmg - curShield);
	}
	else
	{
		curHP -= dmg;
	}
}

void Enemy::setDir(float locX, float locY)
{
	//find difference between player and enemy location
	//find delta x and y
	float dx = (float) objX - locX;
	float dy = (float) objY - locY;

	//find angle
	float radian = atan2f(dy, dx);

	//set facing dir
	dir = (radian * 180 / M_PI);

}

float Enemy::getDir()
{
	return dir;
}

void Enemy::moveAggressive(float locX, float locY, int fps, SDL_Rect gameWindowRect)
{
	//agressive enemies will try to get close to the player
	//x movement
	if (SDL_PointInRect(&enemyLoc, &gameWindowRect))
	{
		if (objX < locX - 64)
		{
			enemyHitBox.x = enemyLoc.x = objX += speed / fps;
		}
		else if (objX > locX + 64)
		{
			enemyHitBox.x = enemyLoc.x = objX -= speed / fps;
		}
		else
		{
			if (locX > objX)
			{
				enemyHitBox.x = enemyLoc.x = objX -= speed / fps;
			}
			else if (locX < objX)
			{
				enemyHitBox.x = enemyLoc.x = objX += speed / fps;
			}
			//prevents jittery motion
			else {}
		}

		//y movement
		if (objY < locY - 64)
		{
			enemyHitBox.y = enemyLoc.y = objY += speed / fps;
		}
		else if (objY > locY + 64)
		{
			enemyHitBox.y = enemyLoc.y = objY -= speed / fps;
		}
		else
		{
			if (locY > objY)
			{
				enemyHitBox.y = enemyLoc.y = objY -= speed / fps;
			}
			else if (locY < objY)
			{
				enemyHitBox.y = enemyLoc.y = objY += speed / fps;
			}
			//prevents jittery motion
			else {}
		}
	}
	//if the enemy pops outside the window it will teleport to a place within the game window
	else
	{
		teleport(gameWindowRect);
	}
}

void Enemy::moveCautious(float locX, float locY, int fps, SDL_Rect gameWindowRect)
{
	//cautious enemies will tend to try staying at least 250 pixels away from the player
	//but stay within the window
	if (SDL_PointInRect(&enemyLoc, &gameWindowRect))
	{
		//x movement
		if (objX < locX - 150)
		{
			enemyHitBox.x = enemyLoc.x = objX += speed / fps;
		}
		else if (objX > locX + 150)
		{
			enemyHitBox.x = enemyLoc.x = objX -= speed / fps;
		}
		else
		{
			if (locX > objX)
			{
				enemyHitBox.x = enemyLoc.x = objX -= speed / fps;
			}
			else if (locX < objX)
			{
				enemyHitBox.x = enemyLoc.x = objX += speed / fps;
			}
			//prevents jittery motion
			else {}
		}

		//y movement
		if (objY < locY - 150)
		{
			enemyHitBox.y = enemyLoc.y = objY += speed / fps;
		}
		else if (objY > locY + 150)
		{
			enemyHitBox.y = enemyLoc.y = objY -= speed / fps;
		}
		else
		{
			if (locY > objY)
			{
				enemyHitBox.y = enemyLoc.y = objY -= speed / fps;
			}
			else if (locY < objY)
			{
				enemyHitBox.y = enemyLoc.y = objY += speed / fps;
			}
			//prevents jittery motion
			else {}
		}
	}

	//if the enemy pops outside the window it will teleport to a place within the game window
	else
	{
		teleport(gameWindowRect);
	}
}

void Enemy::moveDefensive(float locX, float locY, int fps, SDL_Rect gameWindowRect)
{
	//face away from player
	dir -= 180;
	
	//enemy will stay as far away as possible
	if (SDL_PointInRect(&enemyLoc, &gameWindowRect))
	{
		//if the player gets too close, the enemy teleports away
		if (objX < locX + 100 && objX > locX - 100 && objY < locY + 100 && objY > locY - 100)
		{
			teleport(gameWindowRect);
		}
		//else just run away
		else
		{
			if (locX > objX)
			{
				enemyHitBox.x = enemyLoc.x = objX -= speed / fps;
			}
			else if (locX < objX)
			{
				enemyHitBox.x = enemyLoc.x = objX += speed / fps;
			}
			//prevents jittery motion
			else {}

			if (locY > objY)
			{
				enemyHitBox.y = enemyLoc.y = objY -= speed / fps;
			}
			else if (locY < objY)
			{
				enemyHitBox.y = enemyLoc.y = objY += speed / fps;
			}
			//prevents jittery motion
			else {}
		}
	}
	//if is out of the gamewindow, teleport somewhere else
	else
	{
		teleport(gameWindowRect);
	}
}

void Enemy::teleport(SDL_Rect gameWindowRect)
{
	//check if enemy has enough energy
	if (curEnergy >= 100)
	{
		int newX = rand() % gameWindowRect.w,
			newY = rand() % gameWindowRect.h;

		enemyHitBox.x = enemyLoc.x = objX = newX;
		enemyHitBox.y = enemyLoc.y = objY = newY;

		curEnergy -= 100;
	}
}

void Enemy::draw(SDL_Renderer * renderer, SDL_Texture *texture)
{
	//set colour to red
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 150);

	//draws the enemy half the size of the actual hitbox
	SDL_Rect enemyDraw = enemyHitBox;

	//sort out the centering
	enemyDraw.w /= 2, enemyDraw.h /= 2;
	enemyDraw.x = objX - (enemyDraw.w/2);
	enemyDraw.y = objY - (enemyDraw.h/2);

	//adjust for correct centering
	enemyHitBox.x = objX - enemyHitBox.w / 2;
	enemyHitBox.y = objY - enemyHitBox.h / 2;

	//temporary point
	SDL_Point shipLoc = { enemyHitBox.x, enemyHitBox.y };

	SDL_RenderCopyEx(renderer, texture, &textureRect, &enemyDraw, dir - 90, NULL, SDL_FLIP_NONE);

	//draw the rectangle
	//SDL_RenderFillRect(renderer, &enemyHitBox);

	//draw shield if active
	if (curShield > 0)
	{
		//alpha calculations
		float shieldPerc = ((float)curShield / (float)enemyShield) * 255;
		
		drawShield(renderer, shieldPerc);
	}
}

void Enemy::drawShield(SDL_Renderer * renderer, int alpha)
{
	//set colour to red
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, alpha / 8);

	//adjust for correct centering
	enemyHitBox.x = objX - (enemyHitBox.w / 2);
	enemyHitBox.y = objY - (enemyHitBox.h / 2);

	//separate rect for shield
	SDL_Rect shieldRect = enemyHitBox;
	shieldRect.h /= 2, shieldRect.w /= 2;

	for (int i = 0; i < 4; i++)
	{
		shieldRect.x = (objX - shieldRect.w / 2) + i;
		shieldRect.y = (objY - shieldRect.w / 2) + i;
		shieldRect.w -= 2 * i, shieldRect.h -= 2 * i;

		//draw the rectangle
		SDL_RenderDrawRect(renderer, &shieldRect);
	}
}

void Enemy::drawBars(SDL_Renderer * renderer)
{
	SDL_Rect hpBar = {
		enemyLoc.x - enemyHitBox.w,
		enemyLoc.y + enemyHitBox.h / 2,
		enemyHitBox.w,
		2
	};
	hpBar.x = enemyLoc.x - hpBar.w / 2;

	SDL_Rect shieldBar = hpBar;

	//draw the hp bar background first
	SDL_SetRenderDrawColor(renderer,255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &hpBar);

	//calculate current hps
	double hpPerc = (double)curHP / enemyHP;
	double shieldPerc = (double)curShield / enemyShield;

	//draw the current hp bar
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	hpBar.w = hpBar.w * hpPerc;
	SDL_RenderFillRect(renderer, &hpBar);

	//draw current shield
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	shieldBar.w = shieldBar.w * shieldPerc;
	SDL_RenderFillRect(renderer, &shieldBar);
}

void Enemy::update(int frames, float locX, float locY, int fps, SDL_Rect gameWindowRect, std::vector<Bullet> &enemyBulletList)
{	
	setDir(locX, locY);

	//choose movement and actions based on health
	int healthPerc = ((float)curHP / (float)enemyHP) * 100;

	//attacks
	attack(healthPerc, frames, enemyBulletList);

	//increment energy if less than max
	if (curEnergy < tpEnergy && frames % 10 == 0)
	{
		curEnergy++;
	}

	//if enemy has enough hp, it can be aggressive
	if (healthPerc >= 80)
	{
		moveAggressive(locX, locY, fps, gameWindowRect);
	}
	//enemy becomes cautious once it has <80% hp
	else if (healthPerc >= 40)
	{
		moveCautious(locX, locY, fps, gameWindowRect);
	}
	//else enemy runs away from the player
	else
	{
		moveDefensive(locX, locY, fps, gameWindowRect);
	}
}

void Enemy::attack(int healthPerc, int frames, std::vector<Bullet>& enemyBulletList)
{
	//all ships fire one bullet, except for the special ships i.e.
	//shotgun ships, tanky ships and the fast firing ships for small vessels
	//large ships will fire missiles and other planned stuff
	//fire bullets
	if (frames % (120 + randFirerate) == 0 && healthPerc > 20 && (shipType == 0 || shipType >= 4))
	{
		Bullet b;
		b.shoot(enemyLoc, dir - 180, damage * 0.4, 300);
		enemyBulletList.push_back(b);
	}
	
	//attacks are based on the ship type
	switch (shipType)
	{
		//shot guns
		case 1:
			//fire shotgun
			if (frames % (120 + randFirerate) == 0 && healthPerc > 20)
			{
				//randomises where the bullets will be
				float bulletDir = dir - 180;
				
				Bullet b, b1, b2, b3, b4;
				b.shoot(enemyLoc, (bulletDir + (rand() % 20) - 10), damage * 0.3, 250);
				b1.shoot(enemyLoc, (bulletDir + (rand() % 20) - 10), damage * 0.3, 250);
				b2.shoot(enemyLoc, (bulletDir + (rand() % 20) - 10), damage * 0.3, 250);
				b3.shoot(enemyLoc, (bulletDir + (rand() % 20) - 10), damage * 0.3, 250);
				b4.shoot(enemyLoc, (bulletDir + (rand() % 20) - 10), damage * 0.3, 250);
				enemyBulletList.push_back(b); 
				enemyBulletList.push_back(b1);
				enemyBulletList.push_back(b2);
				enemyBulletList.push_back(b3);
				enemyBulletList.push_back(b4);
			}
			break;

		//tanky ship fires harder bullets
		case 2:
			//fire heavy bullets
			if (frames % (120 + randFirerate) == 0 && healthPerc > 20)
			{
				Bullet b;
				b.shoot(enemyLoc, dir - 180, damage * 0.6, 1000);
				enemyBulletList.push_back(b);
			}
			break;

		//fast ship has high fire rate but low dmg
		case 3:
			//fire many weak bullets
			if (frames % (20 + randFirerate) == 0 && healthPerc > 20)
			{
				//not completely accurate
				float randomSpread = (rand() % 10) - 5;
				
				Bullet b;
				b.shoot(enemyLoc, (dir - 180) + randomSpread, damage * 0.1, 500);
				enemyBulletList.push_back(b);
			}
			break;

		case 4:
		case 5:
		case 6:
		case 7:
			//fire cannons if frigate or above
			if (frames % (180 + randFirerate) == 0 && healthPerc > 40 && shipType >= 4)
			{
				Bullet b;
				b.shoot(enemyLoc, dir - 180, damage * 0.75, 1500);
				enemyBulletList.push_back(b);
			}
			break;
	}
}
