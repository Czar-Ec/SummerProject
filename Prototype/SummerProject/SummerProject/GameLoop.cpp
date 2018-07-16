#include "Game.h"

void Game::updateGame()
{	
	//clear renderer
	SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
	SDL_RenderClear(windowRenderer);

	//draw the stars
	for (int i = 0; i < starList.size(); i++)
	{
		starList[i].update(windowRenderer, gameWindowRect);
	}

	//set colour to red
	SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 150);
	
	//draw world border 10 thick
	for (int i = 0; i < 10; i++)
	{
		gameWindowRect.x = gameWindowRect.y = i;
		gameWindowRect.w = config.getWinWidth() - 2*i;
		gameWindowRect.h = config.getWinHeight() - 200 - 2*i;

		//draw the rect
		SDL_RenderDrawRect(windowRenderer, &gameWindowRect);
	}

	//rect is smaller than shown
	gameWindowRect.x = gameWindowRect.y = 16;
	gameWindowRect.w = config.getWinWidth() - 32;
	gameWindowRect.h = config.getWinHeight() - 232;

	//colour in the HUD
	SDL_SetRenderDrawColor(windowRenderer, 24, 24, 24, 255);
	SDL_RenderFillRect(windowRenderer, &gameHUDRect);

	
	////////////////////////////////////////////////////////////////////////////////////////
	
	//missile
	for (int curEnemy = 0; curEnemy < enemyList.size(); curEnemy++)
	{
		for (int curMissile = 0; curMissile < missileList.size(); curMissile++)
		{
			if (enemyList[curEnemy].pointInEnemy(missileList[curMissile].getMissileLoc(), missileList[curMissile].getMissileDmg()))
			{
				//add damage done to score
				score += missileList[curMissile].getMissileDmg();

				//remove the missile
				missileList[curMissile].setInactive(bulletList);
			}
		}
	}

	//draw missile
	for (int curMissile = 0; curMissile < missileList.size(); curMissile++)
	{
		//update missile based on current frame
		missileList[curMissile].updateMissile(config.getFPS(), windowRenderer, gameShips, "player", bulletList, enemyBulletList, shipLoc, enemyList, mousePos);

		//destroy missile if outside world border
		if (!SDL_PointInRect(&missileList[curMissile].getMissileLoc(), &gameWindowRect) || !missileList[curMissile].getFired())
		{
			missileList.erase(missileList.begin() + curMissile);
		}
	}

	//flak
	for (int curEnemy = 0; curEnemy < enemyList.size(); curEnemy++)
	{
		for (int curFlak = 0; curFlak < flakList.size(); curFlak++)
		{
			if (enemyList[curEnemy].pointInEnemy(flakList[curFlak].getFlakLoc(), flakList[curFlak].getFlakDamage()))
			{
				//add damage done to enemies to score
				score += flakList[curFlak].getFlakDamage();

				//remove the flak
				flakList[curFlak].setInactive(bulletList);
			}
		}
	}

	//draw flak
	for (int curFlak = 0; curFlak < flakList.size(); curFlak++)
	{
		//update the flak
		flakList[curFlak].updateFlak(config.getFPS(), windowRenderer, gameShips, "player", bulletList, enemyBulletList);

		//if flak is outside the level, destroy it
		if (!SDL_PointInRect(&flakList[curFlak].getFlakLoc(), &gameWindowRect) || !flakList[curFlak].getFired())
		{
			flakList.erase(flakList.begin() + curFlak);
		}
	}
	
	//check through bullets
	//this checks which need to be removed
	//check if bullet has hit any enemies
	for (int curEnemy = 0; curEnemy < enemyList.size(); curEnemy++)
	{
		for (int curBullet = 0; curBullet < bulletList.size(); curBullet++)
		{
			if (enemyList[curEnemy].pointInEnemy(bulletList[curBullet].getBulletLoc(), bulletList[curBullet].getBulletDamage()))
			{
				//debug
				//std::cout << "player dmg: " << config.getDMG() << std::endl;
				//std::cout << "enemy HP: " << enemyHP << std::endl;

				//add damage done to enemies to score
				score += bulletList[curBullet].getBulletDamage();

				//remove the bullet
				bulletList.erase(bulletList.begin() + curBullet);
			}
		}
	}
	
	//this one is for drawing the bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		//update the bullet
		bulletList[i].update(config.getFPS(), windowRenderer, "player");
		
		//if bullet is outside the level, destroy it
		if (!SDL_PointInRect(&bulletList[i].getBulletLoc(), &gameWindowRect) || !bulletList[i].getFired())
		{
			bulletList.erase(bulletList.begin() + i);
		}	
	}

	////////////////////////////////////////////////////////////////////////////////////////
	
	//difficulty adjustment

	//30 second adjustment
	if (survivalTimeSec > 30 && Adj1 == false)
	{
		//increase damage
		damageToPlayer++;

		//increase enemy hp and shield
		enemyHP += 10;
		enemyShield += 5;

		//increase enemy speed
		enemySpeed ++;

		//adjustment 30 second done
		Adj1 = true;
	}

	//1 minute adjustment
	if (survivalTimeSec > 0 && Adj2 == false)
	{
		//increase damage by 2
		damageToPlayer += 2;

		//increase amount of enemies to spawn
		enemyNum ++;

		//increase enemy hp and shield
		enemyHP += 15;
		enemyShield += 10;

		//increase enemy speed
		enemySpeed ++;

		//adjustment 1 minute done
		Adj2 = true;
	}

	//adjustment at 1 hour, happens only once
	if (survivalTimeHour > 0 && adjHr == false)
	{
		//time to git gud
		damageToPlayer += config.getPlayerHP() / 2;

		//enemies get hardcore
		enemyNum += 100;
		enemyHP += 1000;
		enemyShield += 10000;
		enemySpeed += 100;

		adjHr = true;
	}

	//reset so it can keep adjusting
	if (survivalTimeSec > 59 && Adj1 == true && Adj2 == true && adjReset == false)
	{
		Adj1 = Adj2 = false;
		adjReset = true;
	}

	//launch frigates after 1 minute
	if (survivalTimeMin >= 1)
	{
		launchFrigates = true;
	}

	//launch cruisers after 3 minutes
	if (survivalTimeMin >= 3)
	{
		launchCruisers = true;
	}

	//launch battleships after 5 minutes
	if (survivalTimeMin >= 5)
	{
		launchBattleShips = true;
	}

	//launch dreadnoughts after 8 minutes
	if (survivalTimeMin >= 8)
	{
		launchDreadnoughts = true;
	}
	
	//call spawn function
	for (int spawnedEnemies = 0; spawnedEnemies < enemyNum; spawnedEnemies++)
	{		
		spawnSetup(enemyNum, enemyHP, enemyShield, enemySpeed);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	
	//enemy bullet list
	//check if bullet hits the player
	for (int enemyBullet = 0; enemyBullet < enemyBulletList.size(); enemyBullet++)
	{
		if (player.playerHit(enemyBulletList[enemyBullet].getBulletLoc()))
		{
			//get the bullet's damage amount
			float bulletDamage = enemyBulletList[enemyBullet].getBulletDamage();
			
			//damage shields first
			if (curPlayerShield > bulletDamage)
			{
				curPlayerShield -= bulletDamage;
			}
			else if (curPlayerShield <= bulletDamage && !curPlayerShield == 0)
			{
				curPlayerShield = 0;
				if (bulletDamage > config.getArmor() + curPlayerShield)
				{
					curPlayerHP -= (bulletDamage - (config.getArmor() + curPlayerShield));
				}
			}
			else
			{
				if (bulletDamage > config.getArmor())
				{
					curPlayerHP -= (bulletDamage - config.getArmor());
				}
				else
				{
					//no damage done
				}
			}

			//remove the bullet
			enemyBulletList.erase(enemyBulletList.begin() + enemyBullet);
		}
	}

	//check if within the world
	for (int enemyBullet = 0; enemyBullet < enemyBulletList.size(); enemyBullet++)
	{
		//update the bullet
		enemyBulletList[enemyBullet].update(config.getFPS(), windowRenderer, "enemy");

		//if bullet is outside the level, destroy it
		if (!SDL_PointInRect(&enemyBulletList[enemyBullet].getBulletLoc(), &gameWindowRect) || !enemyBulletList[enemyBullet].getFired())
		{
			enemyBulletList.erase(enemyBulletList.begin() + enemyBullet);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////

	//draw and update enemies
	for (int i = 0; i < enemyList.size(); i++)
	{
		//check if the enemy is alive first
		if (enemyList[i].isAlive())
		{
			enemyList[i].draw(windowRenderer, gameShips);

			enemyList[i].drawBars(windowRenderer);
			SDL_Point playerLoc = { player.getLocX(), player.getLocY() };
			enemyList[i].update(frames, player.getLocX(), player.getLocY(), fps, gameWindowRect, enemyBulletList);
		}
		else
		{
			//dead, remove from enemy list
			enemyList.erase(enemyList.begin() + i);
			kills++;
		}
	}

	//draw the player
	player.draw(windowRenderer, gameShips);

	//if shield is over 0
	if (curPlayerShield > 0)
	{
		//alpha calculations
		float shieldPerc =  ((float)curPlayerShield / (float)config.getPlayerShield()) * 255;

		//draw shields with the calculated alpha
		player.drawShield(windowRenderer, shieldPerc);
	}

	//draw the game HUD
	drawHUD();

	//present the changes to the window
	SDL_RenderPresent(windowRenderer);
}

void Game::drawHUD()
{
	//calculating the widths of the rects for current stats
	double	hpPerc = (double) curPlayerHP / config.getPlayerHP(),
			shieldPerc = (double) curPlayerShield / config.getPlayerShield(),
			energyPerc = (double) curPlayerEnergy / config.getEnergy();

	hpBar.w = (config.getWinWidth() / 2) * hpPerc;
	shieldBar.w = (config.getWinWidth() / 2) * shieldPerc;
	energyBar.w = (config.getWinWidth() / 2) * energyPerc;

	//draw the background bars first

	//hpbkg
	SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 100);
	SDL_RenderFillRect(windowRenderer, &hpBarBkg);

	//shieldbkg
	//SDL_SetRenderDrawColor(windowRenderer, 0, 20, 150, 100);
	SDL_RenderFillRect(windowRenderer, &shieldBarBkg);

	//energybkg
	//SDL_SetRenderDrawColor(windowRenderer, 0, 100, 0, 100);
	SDL_RenderFillRect(windowRenderer, &energyBarBkg);

	//draw the current stats on top of the backgrounds
	//hpbar
	//SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(windowRenderer, &hpBar);

	//shieldbar
	//SDL_SetRenderDrawColor(windowRenderer, 0, 20, 150, 255);
	SDL_RenderFillRect(windowRenderer, &shieldBar);

	//energybar
	//SDL_SetRenderDrawColor(windowRenderer, 0, 100, 0, 255);
	SDL_RenderFillRect(windowRenderer, &energyBar);

	//draw text
	//text colour
	SDL_Color textColour = { 255, 255, 255, 255 };

	//set up labels

	hpTxt.x = shieldTxt.x = energyTxt.x = ((config.getWinWidth() / 2) + config.getWinWidth() / 4) + 10;

	//hptxt
	hpTxt.w = 40;

	SDL_Surface *textSurface = TTF_RenderText_Solid(gameFont, "HP", textColour);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &hpTxt);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	
	//shieldtxt
	shieldTxt.w = 100;

	textSurface = TTF_RenderText_Solid(gameFont, "SHIELD", textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &shieldTxt);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	
	//energytxt
	energyTxt.w = 100;

	textSurface = TTF_RenderText_Solid(gameFont, "ENERGY", textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &energyTxt);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//set up num labels
	hpTxt.w = std::to_string(curPlayerHP).length() * 20;
	shieldTxt.w = std::to_string(curPlayerShield).length() * 20;
	energyTxt.w = std::to_string(curPlayerEnergy).length() * 20;


	hpTxt.x = (config.getWinWidth() / 2) - hpTxt.w / 2;
	shieldTxt.x = (config.getWinWidth() / 2) - shieldTxt.w / 2;
	energyTxt.x = (config.getWinWidth() / 2) - energyTxt.w / 2;

	//hp num
	textSurface = TTF_RenderText_Solid(gameFont, std::to_string(curPlayerHP).c_str(), textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &hpTxt);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//shield num
	textSurface = TTF_RenderText_Solid(gameFont, std::to_string(curPlayerShield).c_str(), textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &shieldTxt);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//energy num
	textSurface = TTF_RenderText_Solid(gameFont, std::to_string(curPlayerEnergy).c_str(), textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &energyTxt);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	
	//display survival time
	if (survivalTimeMin >= 60)
	{
		survivalTimeMin = 0;
		survivalTimeHour++;
	}
	if (survivalTimeSec >= 60)
	{
		survivalTimeMin++;
		survivalTimeSec = 0;

		//reset flags
		resetFlag = false;
		adjReset = false;
	}

	//convert second to 2 decimal precision
	std::stringstream ss;
	if (survivalTimeSec >= 10)
	{
		ss << std::setprecision(4) << survivalTimeSec;
	}
	else if(survivalTimeSec < 1)
	{
		ss << std::setprecision(2) << survivalTimeSec;
	}
	else
	{
		ss << std::setprecision(3) << survivalTimeSec;
	}
	std::string secStr = ss.str().c_str();

	//std::cout << secStr << "\n";

	//draw time labels
	survTimeHour.w = 80;
	survTimeMin.w = 60;
	survTimeSec.w = 60;

	survTimeHour.x = survTimeMin.x = survTimeSec.x = 10;

	//hour
	textSurface = TTF_RenderText_Solid(gameFont, "HOUR", textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &survTimeHour);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//min
	textSurface = TTF_RenderText_Solid(gameFont, "MIN", textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &survTimeMin);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//sec
	textSurface = TTF_RenderText_Solid(gameFont, "SEC", textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &survTimeSec);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//draw survival times

	survTimeHour.w = std::to_string(survivalTimeHour).length() * 20;
	survTimeMin.w = std::to_string(survivalTimeMin).length() * 20;
	survTimeSec.w = secStr.length() * 20;

	survTimeHour.x = survTimeMin.x = survTimeSec.x = 100;

	//hour
	textSurface = TTF_RenderText_Solid(gameFont, std::to_string(survivalTimeHour).c_str(), textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &survTimeHour);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//min
	textSurface = TTF_RenderText_Solid(gameFont, std::to_string(survivalTimeMin).c_str(), textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &survTimeMin);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	//sec
	textSurface = TTF_RenderText_Solid(gameFont, secStr.c_str(), textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &survTimeSec);
	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	
	/*std::cout << "h: " << survivalTimeHour << "\n"
		<< "m: " << survivalTimeMin << "\n"
		<< "s: " << survivalTimeSec << "\n";*/
		

	//show if autofire is on or not
	
	SDL_Rect autofireTxt = { config.getWinWidth() - 105, config.getWinHeight() - 95, 100, 25 };

	if (autofire)
	{
		SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 100);
		SDL_RenderFillRect(windowRenderer, &autofireTxt);
	}

	autofireTxt.w /= 2;
	autofireTxt.h /= 2;
	autofireTxt.x += autofireTxt.w / 2;
	autofireTxt.y += autofireTxt.h / 2;
	textSurface = TTF_RenderText_Solid(gameFont, "AUTOFIRE", textColour);
	textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
	SDL_RenderCopy(windowRenderer, textTexture, NULL, &autofireTxt);

	//free up the surfaces
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void Game::spawnSetup(int enemyNum, int enemyHP, int enemyShield, int enemySpd)
{	
	//if no enemies on screen, spawn more
	if (enemyList.size() <= 0)
	{
		if (enemyNum <= 0)
		{
			spawnEnemies(1);
		}
		else
		{
			spawnEnemies(enemyNum);
		}
	}
	
	//first spawn at 0 seconds
	if (survivalTimeSec > 0 && spawn1 == false && resetFlag == false)
	{
		spawnEnemies(enemyNum);
		spawn1 = true;
	}

	//second spawn at 15 seconds
	if (survivalTimeSec > 15 && spawn2 == false && resetFlag == false)
	{
		spawnEnemies(enemyNum);
		spawn2 = true;
	}

	//third spawn at 30 seconds
	if (survivalTimeSec > 30 && spawn3 == false && resetFlag == false)
	{
		spawnEnemies(enemyNum);
		spawn3 = true;
	}

	//last spawn at 45 seconds
	if (survivalTimeSec > 45 && spawn4 == false && resetFlag == false)
	{
		spawnEnemies(enemyNum);
		spawn4 = true;
	}

	//reset all flags
	if (survivalTimeSec > 59 && spawn1 == true && spawn2 == true && spawn3 == true && spawn4 == true && resetFlag == false)
	{
		spawn1 = spawn2 = spawn3 = spawn4 = false;
		resetFlag = true;
	}
}

int Game::enemyShipType(bool specialShip)
{
	int enemyType = 0;
	
	if (specialShip)
	{
		//loop until something is spawnable
		bool choose = true;
		
		while (choose)
		{
			int randomSpecial = rand() % 100;
			
			//figure out which special ships can be spawned
			//55% chance to spawn
			if (launchFrigates && randomSpecial >= 50)
			{
				enemyType = 4;
				choose = false;
			}
			//25% chance to spawn
			else if (launchCruisers && randomSpecial >= 25)
			{
				enemyType = 5;
				choose = false;
			}
			//15% chance to spawn
			else if (launchBattleShips && randomSpecial >= 10)
			{
				enemyType = 6;
				choose = false;
			}
			//5% chance to spawn
			else if(launchDreadnoughts && randomSpecial >= 5)
			{
				enemyType = 7;
				choose = false;
			}
			else
			{
				enemyType = 4;
				choose = false;
			}
		}
	}
	else
	{
		enemyType = rand() % 4;
	}

	return enemyType;
}

void Game::spawnEnemies(int enemyNum)
{	
	//loop to spawn the correct amount of enemies
	int enemyCount = 0;

	while (enemyCount < enemyNum)
	{
		//randomises where the enemy spawn
		int spawnRand = rand() % 4;

		SDL_Point topLeft = { (rand() % (config.getWinWidth() / 8)) + 40, (rand() % (config.getWinHeight() / 8)) + 40 },
			topRight = { config.getWinWidth() - (40 + rand() % (config.getWinWidth() / 8)), (rand() % (config.getWinHeight() / 8)) + 40 },
			botLeft = { (rand() % (config.getWinWidth() / 8)) + 40, config.getWinHeight() - (240 + rand() % (config.getWinHeight() / 8)) },
			botRight = { config.getWinWidth() - (40 + rand() % (config.getWinWidth() / 8)), config.getWinHeight() - (240 + rand() % (config.getWinHeight() / 8)) };

		//10% chance to randomly choose whether to spawn a special ship or not
		bool specialShip = false;
		int spawnSpecialShipChance = rand() % 100;
		if (spawnSpecialShipChance <= 20 && launchFrigates)
		{
			specialShip = true;
		}

		//determine spawn spot
		if (spawnRand == 0)
		{			
			Enemy enemy = Enemy(enemyShipType(specialShip), enemyHP, enemyShield, enemySpeed, damageToPlayer, topLeft);
			enemyList.push_back(enemy);
		}
		if (spawnRand == 1)
		{
			Enemy enemy = Enemy(enemyShipType(specialShip), enemyHP, enemyShield, enemySpeed, damageToPlayer, topRight);
			enemyList.push_back(enemy);
		}
		if (spawnRand == 2)
		{
			Enemy enemy = Enemy(enemyShipType(specialShip), enemyHP, enemyShield, enemySpeed, damageToPlayer, botLeft);
			enemyList.push_back(enemy);
		}
		if (spawnRand == 3)
		{
			Enemy enemy = Enemy(enemyShipType(specialShip), enemyHP, enemyShield, enemySpeed, damageToPlayer, botRight);
			enemyList.push_back(enemy);
		}

		enemyCount++;
	}
}

void Game::gameLoop()
{
	//boolean to run or stop the game
	bool runGame = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//reset things

	//fps calculators
	float frameTime = 0, prevTime = 0, curTime = 0;
	float deltaTime = 0.0f;
	frames = 0;
	fps = 0;

	//reset survival time
	survivalTimeSec = 0;
	survivalTimeMin = 0;
	survivalTimeHour = 0;

	//autofire and fire
	fire = autofire = false;

	gameOver = false;

	//unpause if paused from previous
	config.setPause(false);

	//setup for game hud
	gameHUDRect = { 0, config.getWinHeight() - 200, config.getWinWidth(), 200 };

	//set values for current player hp, shields and energy
	curPlayerHP = config.getPlayerHP(),
		curPlayerShield = config.getPlayerShield(),
		curPlayerEnergy = config.getEnergy();

	kills = 0;

	//reset bullet lists
	bulletList.clear();
	flakList.clear();
	missileList.clear();

	//reset enemylist and enemy bullets
	enemyBulletList.clear();
	enemyFlakList.clear();
	enemyMissileList.clear();
	enemyList.clear();

	//reset enemy values
	damageToPlayer = 10;

	//setup enemies
	enemyNum = 2;
	enemyHP = 10;
	enemyShield = 10;
	enemySpeed = 25;

	//enemy difficulty adjustment
	Adj1 = Adj2 = adjHr = adjReset = false;

	//reset spawn flags
	spawn1 = spawn2 = spawn3 = spawn4 = resetFlag = false;
	launchFrigates = launchCruisers = launchBattleShips = launchDreadnoughts = false;

	//game window rect is smaller than shown
	gameWindowRect.x = gameWindowRect.y = 16;
	gameWindowRect.w = config.getWinWidth() - 32;
	gameWindowRect.h = config.getWinHeight() - 232;

	//clear previous stars
	starList.clear();

	//make all these stars
	createStars(gameWindowRect);

	//create background stars


	////////////////////////////////////////////////////////////////////////////////////////////////////

	//spawnSetup(enemyNum, enemyHP, enemyShield, enemySpeed, enemyFirerate);
	Uint32 startTime, elapsedTime;
	
	startTime = SDL_GetTicks();

	//main game loop
	while (runGame && !gameOver)
	{
		//increment frame
		frames++;

		//universal game time
		elapsedTime = SDL_GetTicks() - startTime;

		//previous time set to current for this iteration
		prevTime = curTime;
		//get current time
		curTime = SDL_GetTicks();

		//mouse
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		//player location
		shipLoc.x = player.getLocX();
		shipLoc.y = player.getLocY();

		//regenerate energy if less than max energy
		if (curPlayerEnergy < config.getEnergy() && (frames % 8 == 0))
		{
			curPlayerEnergy++;
		}
		//regenerate shields
		if (curPlayerShield < config.getPlayerShield() //if less than max shield
			&& (frames % 10 == 0) //shield recharge rate
			)
		{
			curPlayerShield++;
		}
		
		//bullet firing
		if ((autofire || fire) // if either firing or on autofire
			&& (frames % config.getFireRate() == 0) //this is to limit the firing rate
			)
		{
			playerAttack();
		}

		//input checking//////////////////////////////////////
		while (SDL_PollEvent(&gameInput) != 0)
		{
			//closing the window
			if (gameInput.type == SDL_QUIT)
			{
				//end run game loop
				runGame = false;
				//end the main menu loop
				config.setRun(false);
				SDL_Delay(500);
			}

			//pause toggle
			if (gameInput.type == SDL_KEYUP)
			{
				if (gameInput.key.keysym.sym == SDLK_SPACE)
				{
					//if already paused
					if (config.getPause())
					{
						//unpause
						config.setPause(false);
						fire = false;
					}
					//else
					else
					{
						//pause
						config.setPause(true);

						//pause text
						Button pauseText = Button(config.getWinWidth()/10, config.getWinHeight() / 10, 
							((config.getWinWidth()/2) - (config.getWinWidth()/20)), ((config.getWinHeight()/2) - (config.getWinHeight()/20)), 
								"pause");
						pauseText.draw(windowRenderer, mousePos, gameFont);
						SDL_RenderPresent(windowRenderer);
					}
				}
				//return to main menu
				if (gameInput.key.keysym.sym == SDLK_ESCAPE)
				{
					runGame = false;
					gameOver = true;

					starList.clear();
					createStars(menuRect);

					SDL_Delay(100);
				}
			}

			//if not paused
			if (!config.getPause())
			{
				//follows the mouse
				if (gameInput.type == SDL_MOUSEMOTION)
				{
					//std::cout << mousePos.x << "," << mousePos.y << std::endl;
					dir = (float) getDirection(mousePos, shipLoc);
					//std::cout << dir << std::endl;
					//set player ship to direction
					player.setDir(dir);
				}

				//pressing down buttons on the mouse
				if (gameInput.type == SDL_MOUSEBUTTONDOWN)
				{
					//left click
					if (gameInput.button.button == SDL_BUTTON_LEFT)
					{
						fire = true;
					}
					//right click
					if (gameInput.button.button == SDL_BUTTON_RIGHT)
					{
						//if within game window, teleport
						if (SDL_PointInRect(&mousePos, &gameWindowRect))
						{
							//check if player has enough energy
							if (curPlayerEnergy >= 100)
							{
								curPlayerEnergy -= 100;
								//for debug
								//curPlayerHP -= 100;
								//curPlayerShield -= 100;
								player.teleport(mousePos.x, mousePos.y);

								//explosion at teleport
								for (float angle = 0; angle < 360; angle += 60)
								{
									Bullet b;
									b.shoot(mousePos, angle, config.getDMG(), 250);
									bulletList.push_back(b);
								}
							}
						}
						//std::cout << mousePos.x << "," << mousePos.y << std::endl;
					}
				}

				//letting go of a button
				if (gameInput.type == SDL_MOUSEBUTTONUP)
				{
					//set firing to false
					if (gameInput.button.button == SDL_BUTTON_LEFT)
					{
						fire = false;
					}
				}

				//autofire toggle
				if (gameInput.type == SDL_KEYUP)
				{
					if (gameInput.key.keysym.sym == SDLK_e)
					{
						//if already autofiring, stop autofiring
						if (autofire)
						{
							autofire = false;
							//std::cout << "autofire off" << std::endl;
						}
						//else autofire
						else
						{
							autofire = true;
							//std::cout << "autofire on" << std::endl;
						}
					}
				}
			}
		}

		//cant do anything if paused
		if (!config.getPause())
		{			
			//get keyboard inputs
			keyState = SDL_GetKeyboardState(NULL);
			
			//move up
			if (keyState[SDL_SCANCODE_W])
			{
				if(shipLoc.y - (player.getSpeed() * deltaTime) > (gameWindowRect.y))
				{
					player.moveUp(deltaTime);
				}
			}
			//move down
			if (keyState[SDL_SCANCODE_S])
			{
				if (shipLoc.y + (player.getSpeed() * deltaTime) < (gameWindowRect.y + gameWindowRect.h))
				{
					player.moveDown(deltaTime);
				}
			}
			//move left
			if (keyState[SDL_SCANCODE_A])
			{
				if (shipLoc.x - (player.getSpeed() * deltaTime) > (gameWindowRect.x))
				{
					player.moveLeft(deltaTime);
				}
			}
			//move right
			if (keyState[SDL_SCANCODE_D])
			{
				if (shipLoc.x + (player.getSpeed() * deltaTime) < (gameWindowRect.x + gameWindowRect.w))
				{
					player.moveRight(deltaTime);
				}
			}

			//update screen
			updateGame();

			//survival time
			survivalTimeSec += (float)deltaTime;
			//std::cout << survivalTime << "\n";
		}

		///////getting fps
		//skip the first frame
		if (elapsedTime)
		{
			//calculate delta time - div 100 because SDL_GetTicks returns milliseconds
			deltaTime = (curTime - prevTime) / 1000.0f;

			//update frametime
			frameTime += deltaTime;

			//convert to seconds
			double elapsedSeconds = elapsedTime / 1000;
			fps = frames / elapsedSeconds;
			//std::cout << fps << std::endl;
			//if we cap the frame rate
			if (config.getLimitFPS() && (deltaTime < 1000 / config.getFPS()))
			{
				//prevent divide by 0
				if (config.getFPS() != 0)
				{
					//sleep the remaining frame time
					SDL_Delay((1000 / config.getFPS()) - deltaTime);
				}
			}
		}

		//end game if the player has no health left
		if (curPlayerHP <= 0)
		{
			gameOver = true;
			gameOverScreen();
		}

		//makes sure 0 is the least value
		if (curPlayerEnergy < 0)
		{
			curPlayerEnergy = 0;
		}
		if (curPlayerShield <= 0)
		{
			curPlayerShield = 0;
		}
	}
}

void Game::playerAttack()
{
	if (frames % config.getFireRate() == 0 && (curShipType == 0 || (curShipType >= 4 && curShipType < 7)))
	{
		//fire normal bullets
		Bullet b0;
		b0.shoot(shipLoc, dir, config.getDMG(), 500);
		bulletList.push_back(b0);
	}



	//attacks are based on current ship
	switch (curShipType)
	{
		//starter ship
		case 0:
			break;

		//glass cannon
		case 1:
		{
			if (frames % (config.getFireRate()) == 0)
			{
				Flak f = Flak(40, config.getDMG(), 60, flakTextureRect);
				f.shootFlak(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), mousePos);
				flakList.push_back(f);
			}
			
			
		}
		break;	

		//tank
		case 2:
			if (frames % (config.getFireRate()) == 0)
			{
				Bullet b0, b1, b2, b3, b4;

				//fire shotguns
				b0.shoot(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), 250);
				b1.shoot(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), 250);
				b2.shoot(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), 250);
				b3.shoot(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), 250);
				b4.shoot(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), 250);
				bulletList.push_back(b0);
				bulletList.push_back(b1);
				bulletList.push_back(b2);
				bulletList.push_back(b3);
				bulletList.push_back(b4);
			}
			break;

		//speedy
		case 3:
		{
			if (frames % config.getFireRate() == 0)
			{
				Bullet b;
				b.shoot(shipLoc, dir + (rand() % 10 - 5), config.getDMG(), 750);
				bulletList.push_back(b);
			}
		}
		break;

		//frigate
		case 4:
		{
			if (frames % (config.getFireRate() + 15) == 0)
			{
				Flak f = Flak(50, 5, 60, flakTextureRect);
				f.shootFlak(shipLoc, dir, config.getDMG(), mousePos);
				flakList.push_back(f);
			}
			break;
		}
			
		//cruiser
		case 5:
			if (frames % (config.getFireRate() + 45) == 0)
			{
				Missiles m = Missiles(100, (config.getDMG() / 2), 60);
				m.shootMissile(shipLoc, config.getDMG(), 750);
				missileList.push_back(m);

				Missiles m1 = Missiles(100, (config.getDMG() / 2), 60);
				m1.shootMissile(shipLoc, config.getDMG(), 750);
				missileList.push_back(m1);
			}
			break;

		//battle cruiser
		case 6:
		{
			if (frames % (config.getFireRate() + 5) == 0)
			{
				Flak f = Flak(25, 10, 75, flakTextureRect);
				f.shootFlak(shipLoc, dir + 5, config.getDMG(), mousePos);
				flakList.push_back(f);
				f.shootFlak(shipLoc, dir - 5, config.getDMG(), mousePos);
				flakList.push_back(f);
				f.shootFlak(shipLoc, dir, config.getDMG(), mousePos);
				flakList.push_back(f);
			}

			if (frames % (config.getFireRate() + 45) == 0)
			{
				Missiles m = Missiles(100, (config.getDMG() / 2), 60);
				m.shootMissile(shipLoc, config.getDMG(), 750);
				missileList.push_back(m);
			}

			if (frames % config.getFireRate() == 0)
			{
				//fire normal bullets
				Bullet b0;
				b0.shoot(shipLoc, dir + 2, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir - 2, config.getDMG(), 500);
				bulletList.push_back(b0);
			}
			break;
		}

		//dreadnought
		case 7:
			if (frames % (config.getFireRate() + 5) == 0)
			{
				Flak f = Flak(50, 10, 60, flakTextureRect);

				f.shootFlak(shipLoc, dir + 30, config.getDMG(), mousePos);
				flakList.push_back(f);

				f.shootFlak(shipLoc, dir + 15, config.getDMG(), mousePos);
				flakList.push_back(f);

				f.shootFlak(shipLoc, dir, config.getDMG(), mousePos);
				flakList.push_back(f);

				f.shootFlak(shipLoc, dir - 15, config.getDMG(), mousePos);
				flakList.push_back(f);

				f.shootFlak(shipLoc, dir - 30, config.getDMG(), mousePos);
				flakList.push_back(f);
			}

			if (frames % (120) == 0)
			{
				Missiles m = Missiles(75, (config.getDMG() / 2), 60);
				Missiles m1 = Missiles(75, (config.getDMG() / 2), 60);
				Missiles m2 = Missiles(75, (config.getDMG() / 2), 60);
				m.shootMissile(shipLoc, config.getDMG(), 500);
				m1.shootMissile(shipLoc, config.getDMG(), 500);
				m2.shootMissile(shipLoc, config.getDMG(), 500);
				missileList.push_back(m);
				missileList.push_back(m1);
				missileList.push_back(m2);
			}
			if (frames % config.getFireRate() == 0)
			{
				//fire normal bullets
				Bullet b0;
				b0.shoot(shipLoc, dir + 2, config.getDMG(), 500);
				bulletList.push_back(b0);b0.shoot(shipLoc, dir + 2, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir + 1, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir - 1, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir - 2, config.getDMG(), 500);
				bulletList.push_back(b0);


				b0.shoot(shipLoc, dir + 17, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir + 15, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir + 10, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir - 10, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir - 15, config.getDMG(), 500);
				bulletList.push_back(b0);
				b0.shoot(shipLoc, dir - 17, config.getDMG(), 500);
				bulletList.push_back(b0);
			}
			break;
	}
}

void Game::gameOverScreen()
{
	bool gameOverScreenShown = true, newHighScore = false;

	//calculate the score
	//score is calculated by multiplying the kills to how long the player survived
	score += (kills * (sqrt(kills))
		* (
		(survivalTimeHour * 1000000) //hour multiplier
			+ (survivalTimeMin * 1000) //minute multiplier
			+ ((survivalTimeHour * 60) + (survivalTimeMin * 60) + survivalTimeSec) //calculate the overall time the player survived
			));

	if (kills == 0)
	{
		score += (
			(survivalTimeHour * 1000000) //hour multiplier
			+ (survivalTimeMin * 1000) //minute multiplier
			+ ((survivalTimeHour * 60) + (survivalTimeMin * 60) + survivalTimeSec) //calculate the overall time the player survived
		);
	}

	//compare highscore
	if (score > config.getHighScore())
	{
		config.setHighScore(score);
		newHighScore = true;
	}

	//increase scrap for the player by dividing score by 1000
	config.setScrap(config.getScrap() + (score / 1000));

	//autosave after each full game
	config.saveGame();

	//std::cout << score;

	//the text "R.I.P." that appears when the player dies
	//rip header size
	int ripTextHeight = config.getWinHeight() / 10;
	int ripTextX = 50;

	Text ripText = Text(ripTextHeight, config.getWinWidth(), ripTextX, "RIP");

	//stat text
	int statTextHeight = config.getWinHeight() / 30;
	int statTextPadding = 10;
	ripTextX += 30;

	Text killsText = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight), "KILLS");
	Text killsNum = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding)), std::to_string(kills).c_str());
	
	Text scoreText;
	
	if (newHighScore)
	{
		scoreText = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding) * 2), "NEW HIGHSCORE");
	}
	else
	{
		scoreText = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding) * 2), "SCORE");
	}
	
	Text scoreNum = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding) * 3), std::to_string(score).c_str());

	Text survLabel = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding) * 6), "SURVIVED FOR");

	//string concatenation for the survival time
	std::stringstream survScore;
	survScore << survivalTimeHour << " HOUR " << survivalTimeMin << " MIN " << std::setprecision(2) << survivalTimeSec << " SEC";

	Text survivalTimeScore = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding) * 7), survScore.str());

	std::string scrapTxt = "SCRAP GAINED: " + std::to_string(score / 1000);

	Text scrapGained = Text(statTextHeight, config.getWinWidth(), ripTextX + (ripTextHeight + (statTextHeight + statTextPadding) * 9), scrapTxt);

	//black out the screen
	for (int blackout = 255; blackout > 0; blackout--)
	{
		SDL_SetRenderDrawColor(windowRenderer, blackout, blackout, blackout, 0);
		SDL_RenderClear(windowRenderer);

		ripText.draw(windowRenderer, 255, 255, 255, 255, gameFont);
		
		//present the changes to the window
		SDL_RenderPresent(windowRenderer);
		SDL_Delay(1000 / (config.getFPS() / 2));
	}

	//continue button size
	int seenBX = 175, seenBY = 50;

	//button to continue
	Button seenButton = Button(
	//button size
	seenBX, seenBY,
	//button location
	(config.getWinWidth() / 2) - seenBX / 2, config.getWinHeight() - (seenBY + 25), "CONTINUE");
	
	killsText.draw(windowRenderer, 255, 0, 0, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);

	killsNum.draw(windowRenderer, 255, 0, 0, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);

	scoreText.draw(windowRenderer, 255, 0, 0, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);

	scoreNum.draw(windowRenderer, 255, 0, 0, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);

	survLabel.draw(windowRenderer, 255, 0, 0, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);

	survivalTimeScore.draw(windowRenderer, 255, 0, 0, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);

	scrapGained.draw(windowRenderer, 255, 255, 255, 255, gameFont);
	SDL_Delay(100); SDL_RenderPresent(windowRenderer);
	


	//show on screen until the player says to continue
	while(gameOverScreenShown)
	{
		//clear screen
		SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
		SDL_RenderClear(windowRenderer);
		
		//draw text
		ripText.draw(windowRenderer, 255, 255, 255, 255, gameFont);
		killsText.draw(windowRenderer, 255, 0, 0, 255, gameFont);
		killsNum.draw(windowRenderer, 255, 0, 0, 255, gameFont);
		scoreText.draw(windowRenderer, 255, 0, 0, 255, gameFont);
		scoreNum.draw(windowRenderer, 255, 0, 0, 255, gameFont);
		survLabel.draw(windowRenderer, 255, 0, 0, 255, gameFont);
		survivalTimeScore.draw(windowRenderer, 255, 0, 0, 255, gameFont);
		scrapGained.draw(windowRenderer, 255, 255, 255, 255, gameFont);

		//draw the continue button
		seenButton.draw(windowRenderer, mousePos, gameFont);

		//present the changes to the window
		SDL_RenderPresent(windowRenderer);

		//update mousePos
		SDL_GetMouseState(&mousePos.x, &mousePos.y);


		//input check
		while (SDL_PollEvent(&gameInput) != 0)
		{
			//closing the window
			if (gameInput.type == SDL_QUIT)
			{
				//end the main menu loop
				config.setRun(false);
				SDL_Delay(100);
			}
			//pressing down buttons on the mouse
			if (gameInput.type == SDL_MOUSEBUTTONUP)
			{
				//left click
				if (gameInput.button.button == SDL_BUTTON_LEFT)
				{
					if (seenButton.pointInRect(mousePos))
					{
						gameOverScreenShown = false;
						SDL_Delay(100);
					}
				}
			}
		}
	}

	createStars(menuRect);
}

float Game::getDirection(SDL_Point mousePos, SDL_Point shipLoc)
{
	//find delta x and y
	float dx = (float) mousePos.x - player.getLocX();
	float dy = (float) mousePos.y - player.getLocY();

	//find angle
	float radian = atan2f(dy, dx);
	
	//return in degrees
	return (radian * 180 / M_PI);
}

