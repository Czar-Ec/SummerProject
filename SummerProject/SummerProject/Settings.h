#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <iostream>
#include <SDL\SDL_mixer.h>
static class Settings
{
public:
	Settings();
	~Settings();

	//setter and getter for app running
	void setRun(bool run);
	bool getRun();

	//if fullscreen
	void setFullscreen(bool setting);
	bool getFullscreen();

	//music option
	void setMusicPlay(bool musOpt);
	bool getMusicPlay();
	void stopMusic();
	void continueMusic();

	//get window width and height
	int getWinWidth();
	int getWinHeight();

	//set window width and height
	void setWinWidth(int width);
	void setWinHeight(int height);

	//frame per second limiting
	void setLimitFPS(bool setting);
	bool getLimitFPS();

	//setting fps
	void setFPS(int fps);
	int getFPS();

	//setting fps limit
	void setFPSCap(int fps);
	int getFPSCap();

	//get and set pause
	void setPause(bool set);
	bool getPause();

	//getter and setters for player stats
	void setPlayerHP(int inPlayerHP),
		setPlayerShield(int inPlayerShield),

		setFireRate(int inFireRate),


		setDmg(int inDMG),
		setArmor(int inArmor),
		setSpeed(float inSpeed),
		setEnergy(int inEnergy);

	int getPlayerHP(),
		getPlayerShield(),

		getFireRate(),


		getDMG(),
		getArmor(),
		getEnergy();
		
	float getSpeed();

	//kills
	void setKills(int kills);
	int getKills();

	//highscore
	void setHighScore(int newHighScore);
	int getHighScore();

	//highscore
	void setTopHr(int hour);
	void setTopMin(int min);
	void setTopSec(float sec);

	int getTopHr();
	int getTopMin();
	float getTopSec();

	//game currency
	void setScrap(int inScrap);
	int getScrap();



	//public variables
	bool shipGlassCannonUnlocked, shipTankUnlocked, shipSpeedyUnlocked,
		shipFrigateUnlocked, shipCruiserUnlocked, shipBattleCruiserUnlocked, shipDreadnoughtUnlocked;

private:
	//bool if the game is set to fullscreen
	bool fullscreen;

	//bool if music should play
	Mix_Music *backgroundMusic;
	bool playMusic;
	
	//if the app should stil be run (global var)
	bool runApplication;
	
	//window global vars
	int winWidth, winHeight;
	int saveWidth, saveHeight; //temporary for switching from fullscreen to window

	//limiting frames
	bool limitFrame;
	int framePerSecond, fpsCap;

	//pause
	bool paused;

	////////////////////player stats////////////////////

	//plyaer health
	int playerHP, playerShield,

		//player shooting speed
		fireRate,

		//player damage
		dmg,

		//player defense
		armor,

		//player energy
		energy;

	//player move speed
	float speed;

	//enemy kills the player has done
	int playerKills;

	//highscore
	int highscore;

	//longest survival time
	int topHour, topMin;
	float topSec;

	//game currency
	int scrap;
};

