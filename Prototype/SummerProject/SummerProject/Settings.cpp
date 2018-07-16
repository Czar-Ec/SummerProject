#include "Settings.h"

Settings::Settings()
{
	//preset to windowed
	fullscreen = false;
	//preset to play music
	playMusic = true;
	//preset to this window size
	winWidth = 1152, winHeight = 648;
	//preset to capped 60 fps
	limitFrame = true;
	fpsCap = framePerSecond = 120;

	//paused set to false initially
	paused = false;
}

Settings::~Settings()
{

}

void Settings::setRun(bool run)
{
	runApplication = run;
}

bool Settings::getRun()
{
	return runApplication;
}

void Settings::setFullscreen(bool setting)
{
	fullscreen = setting;
	if (setting)
	{
		//save current window sizes
		saveWidth = winWidth;
		saveHeight = winHeight;

		//change so it fits the screen
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);
		winWidth = DM.w;
		winHeight = DM.h;
	}
	else
	{
		winWidth = saveWidth;
		winHeight = saveHeight;
	}
}

bool Settings::getFullscreen()
{
	return fullscreen;
}

void Settings::setMusicPlay(bool musOpt)
{
	playMusic = musOpt;
}

bool Settings::getMusicPlay()
{
	return playMusic;
}

int Settings::getWinWidth()
{
	return winWidth;
}

int Settings::getWinHeight()
{
	return winHeight;
}

void Settings::setWinWidth(int width)
{
	winWidth = width;
}

void Settings::setWinHeight(int height)
{
	winHeight = height;
}

void Settings::setLimitFPS(bool setting)
{
	limitFrame = setting;
}

bool Settings::getLimitFPS()
{
	return limitFrame;
}

void Settings::setFPS(int fps)
{
	framePerSecond = fps;
}

int Settings::getFPS()
{
	return framePerSecond;
}

void Settings::setFPSCap(int fps)
{
	fpsCap = fps;
}

int Settings::getFPSCap()
{
	return fpsCap;
}

void Settings::setPause(bool set)
{
	paused = set;
}

bool Settings::getPause()
{
	return paused;
}

void Settings::setPlayerHP(int inPlayerHP)
{
	playerHP = inPlayerHP;
}

void Settings::setPlayerShield(int inPlayerShield)
{
	playerShield = inPlayerShield;
}

void Settings::setFireRate(int inFireRate)
{
	fireRate = inFireRate;
}

void Settings::setDmg(int inDMG)
{
	dmg = inDMG;
}

void Settings::setArmor(int inArmor)
{
	armor = inArmor;
}

void Settings::setSpeed(float inSpeed)
{
	speed = inSpeed;
}

void Settings::setEnergy(int inEnergy)
{
	energy = inEnergy;
}

int Settings::getPlayerHP()
{
	return playerHP;
}

int Settings::getPlayerShield()
{
	return playerShield;
}

int Settings::getFireRate()
{
	return fireRate;
}

int Settings::getDMG()
{
	return dmg;
}

int Settings::getArmor()
{
	return armor;
}

float Settings::getSpeed()
{
	return speed;
}

void Settings::setKills(int kills)
{
	playerKills = kills;
}

int Settings::getKills()
{
	return playerKills;
}

void Settings::setHighScore(int newHighScore)
{
	highscore = newHighScore;
}

int Settings::getHighScore()
{
	return highscore;
}

void Settings::setTopHr(int hour)
{
	topHour = hour;
}

void Settings::setTopMin(int min)
{
	topMin = min;
}

void Settings::setTopSec(float sec)
{
	topSec = sec;
}

int Settings::getTopHr()
{
	return topHour;
}

int Settings::getTopMin()
{
	return topMin;
}

float Settings::getTopSec()
{
	return topSec;
}

void Settings::setScrap(int inScrap)
{
	scrap = inScrap;
}

int Settings::getScrap()
{
	return scrap;
}

int Settings::getEnergy()
{
	return energy;
}




