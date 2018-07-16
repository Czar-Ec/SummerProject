#pragma once
//SDL
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>

//include
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>
#include <iomanip>

//other classes
#include "Button.h"
#include "Settings.h"
#include "Ship.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Text.h"
#include "BackgroundStar.h"
#include "Flak.h"
#include "Missiles.h"

class Game
{
public:
	//constructor and destructor
	Game();
	~Game();

	//initialising
	void init();
	//create background
	void createStars(SDL_Rect starSpaceRect);

	//closing and cleaning up
	void close();

	////UPDATE FUNCTIONS
	//update screen
	void updateMainMenu();
	void updateOptionMenu();
	void updateUpgradeMenu(int innerMenu, int shipChoice);
	void updateGame();

	////MENU LOOPS
	//main menu
	void mainLoop();
	//upgrade menu
	void upgradeLoop();
	//option menu
	void optionLoop();
	//game loop
	void gameLoop();

	//resetting the window
	void resetWindow(SDL_Window *window);

	//draw gameHud
	void drawHUD();

	//functions
	float getDirection(SDL_Point mousePos, SDL_Point shipLoc);

	//set up player stats
	void setPlayerStats();

	//player attack
	void playerAttack();

	//spawn enemies
	void spawnSetup(int enemyNum, int enemyHP, int enemyShield, int enemySpd);
	int enemyShipType(bool specialShip);
	void spawnEnemies(int enemyNum);

	//gameover screen
	void gameOverScreen();

private:
	//////////////////////////////////////////////
	//variables
	//////////////////////////////////////////////
	//settings
	Settings config;
	Mix_Music *backgroundMusic;

	//mouse position
	SDL_Point mousePos;

	//SDL window and renderer
	SDL_Window *window;
	SDL_Renderer *windowRenderer;

	//game font
	TTF_Font *gameFont;

	//game texture
	SDL_Texture *gameShips;

	//ship texture rects
	SDL_Rect 
		shipStarterTextureRect = { 64, 0, 16, 16 },
		shipGlassCannonTextureRect = { 80, 0, 16, 16 },
		shipTankTextureRect = { 96, 0, 16, 16 },
		shipSpeedyTextureRect = { 112, 0, 16, 16 },
		shipFrigateTextureRect = { 64, 16, 16, 32 },
		shipCruiserTextureRect = { 80, 16, 16, 32 },
		shipBattleCruiserTextureRect = { 96, 16, 32, 32 },
		shipDreadnoughtTextureRect = { 64, 48, 64, 64 };

	//offensive texture rects
	SDL_Rect flakTextureRect = {0, 112, 8, 8};

	SDL_Rect curShipTexture;

	//ship types
	const int shipStarter = 0,
		shipType1 = 1,
		shipType2 = 2,
		shipType3 = 3,
		shipFrigate = 4,
		shipCruiser = 5,
		shipBattleCruiser = 6,
		shipDreadnought = 7;

	//current ship type
	int curShipType;

	//ship stats
	//starter
	int starterHP = 100,
		starterShield = 10,
		starterFirerate = 30,
		starterDmg = 8,
		starterArmor = 0,
		starterSpd = 100,
		starterEnergy = 100;

	//glasscannon
	int gcHP = 50,
		gcShield = 10,
		gcFirerate = 45,
		gcDmg = 12,
		gcArmor = 0,
		gcSpd = 100,
		gcEnergy = 100;

	//tank
	int tankHP = 200,
		tankShield = 50,
		tankFirerate = 45,
		tankDmg = 6,
		tankArmor = 2,
		tankSpd = 85,
		tankEnergy = 200;

	//speedy
	int speedyHP = 100,
		speedyShield = 10,
		speedyFirerate = 10,
		speedyDmg = 2,
		speedyArmor = 0,
		speedySpd = 200,
		speedyEnergy = 250;

	//frigate
	int frigHP = 500,
		frigShield = 75,
		frigFirerate = 15,
		frigDmg = 12,
		frigArmor = 2,
		frigSpd = 75,
		frigEnergy = 300;

	//cruiser
	int cHP = 750,
		cShield = 75,
		cFirerate = 15,
		cDmg = 14,
		cArmor = 5,
		cSpd = 75,
		cEnergy = 450;

	//battlecruiser
	int bcHP = 1000,
		bcShield = 100,
		bcFirerate = 15,
		bcDmg = 20,
		bcArmor = 10,
		bcSpd = 70,
		bcEnergy = 600;

	//dreadnought
	int dreadHP = 1500,
		dreadShield = 200,
		dreadFirerate = 15,
		dreadDmg = 40,
		dreadArmor = 15,
		dreadSpd = 50,
		dreadEnergy = 750;


	//boolean to check if program is running
	bool runProgram;

	//SDL Events
	SDL_Event mainMenu,
		upgradeMenu,
		optionMenu,
		gameInput;
	//keystate
	const Uint8 *keyState;

	//main menu
	Button playMain,
		upgradeMain,
		optionMain,
		quitMain;

	//option menu
	Button fullscreen, musicOpt;

	Button size1152x648,
		size1280x720,
		size1366x768,
		size1600x900,
		size_max,
		back;

	//play sound effects

	//upgrade menu
	//outer upgrade menu
	Button shipyard;

	Text scrapDisplay;

	//shipyard item costs
	const int
		glassCannonCost = 1000,
		tankCost = 1000,
		speedyCost = 1000,
		frigateCost = 15000,
		cruiserCost = 15000,
		battleCruiserCost = 50000,
		dreadnoughtCost = 150000;

	//inner upgrade menu
	//shipyard upgrades
	Button shipStarterBkg, ship1Bkg, ship2Bkg, ship3Bkg, 
		shipFrigateBkg, shipCruiserBkg, 
		shipBattleCruiserBkg, shipDreadnoughtBkg;

	Button buyShip, equipShip;

	//draw ship
	SDL_Rect shipImage;
	SDL_Rect shipDisplayFrame;


	//player ship
	Ship player;
	//ship location
	SDL_Point shipLoc;

	//draw stars
	int starNum;
	std::vector<BackgroundStar> starList;

	//rect for stars in the menu screen
	SDL_Rect menuRect;

	//universal game time
	/*Uint32 startTime;
	Uint32 elapsedTime;*/

	//fps variables
	/*float frameTime = 0;
	int prevTime = 0,
		curTime = 0;
	float deltaTime = 0;*/

	////frame count
	int frames;
	float fps;

	//bullet direction
	float dir;

	//vector to store bullets and attack objects

	//player
	std::vector <Bullet> bulletList;
	std::vector <Flak> flakList;
	std::vector <Missiles> missileList;
	//enemy
	std::vector <Bullet> enemyBulletList;
	std::vector <Flak> enemyFlakList;
	std::vector <Missiles> enemyMissileList;

	//store list of enemies
	std::vector <Enemy> enemyList;

	//autofire toggle
	bool fire = false;
	bool autofire = false;
	
	//current player stats
	//separated from settings because settings holds the max values, these are current stats
	int curPlayerHP,
		curPlayerShield,
		curPlayerEnergy;


	//draw world border
	SDL_Rect gameWindowRect;
	SDL_Rect gameHUDRect;

	//Game HUD objects
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//text
	SDL_Rect hpTxt, shieldTxt, energyTxt, fpsTxt;
	//bar backgrounds
	SDL_Rect hpBarBkg, shieldBarBkg, energyBarBkg;
	//current bar levels
	SDL_Rect hpBar, shieldBar, energyBar;

	//survival time display
	SDL_Rect survTimeHour, survTimeMin, survTimeSec;

	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//Difficulty increasing
	//damage the player
	int damageToPlayer = 1;

	//enemy stats
	int enemyHP, enemyShield, enemyNum;
	float enemySpeed;

	//enemy adjustment flags
	//damage
	bool Adj1, Adj2, adjReset;
	bool adjHr;

	//enemy spawning flags
	//helps determine if enemies spawned for that time
	bool spawn1, spawn2, spawn3, spawn4, resetFlag;

	//launching the harder ships
	bool launchFrigates, launchCruisers, launchBattleShips, launchDreadnoughts;

	////////////////////////////////////////////////////////////////////////////////////////////////////

	//Game scoring
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//gameover
	bool gameOver;


	//survival time
	float survivalTimeSec;
	int survivalTimeMin, survivalTimeHour;

	//enemy kills
	int kills;

	//score
	int score;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
};

