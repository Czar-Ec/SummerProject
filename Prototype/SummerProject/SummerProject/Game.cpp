#include "Game.h"

Game::Game()
{	
	//initialise
	init();
	//set to run loop
	config.setRun(true);
	//main menu loop
	mainLoop();
	//close the app
	close();
}


Game::~Game()
{
	close();
}

void Game::init()
{	
	//initialise window
	window = nullptr;

	//initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		//print error
		std::cout << "SDL Initiation Error: " << SDL_GetError() << std::endl;
	}
	//else create window
	else
	{
		//initialise fonts
		if (TTF_Init() < 0)
		{
			//error output if text not initialised
			std::cout << "TTF Error: " << TTF_GetError() << std::endl;
		}

		//setting up a fullscreen window
		if (config.getFullscreen())
		{
			window = SDL_CreateWindow(
				"Summer Project",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				config.getWinWidth(),
				config.getWinHeight(),
				SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
			);
		}
		//setting up a window
		else
		{
			window = SDL_CreateWindow(
				"Summer Project",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				config.getWinWidth(),
				config.getWinHeight(),
				SDL_WINDOW_SHOWN
			);
		}

		//check if window was initialised properly
		if (window == NULL)
		{
			std::cout << "SDL Window error: " << SDL_GetError() << std::endl;
		}
		//if initialised, show window
		else
		{
			//create a renderer
			windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			//allow opaqueness
			SDL_SetRenderDrawBlendMode(windowRenderer, SDL_BLENDMODE_BLEND);

			//game font
			gameFont = TTF_OpenFont("res\\font\\gameFont.ttf", 48);
			if (gameFont == NULL)
			{
				std::cout << TTF_GetError();
			}


			//game music
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
			{
				std::cout << Mix_GetError() << std::endl;
			}

			//background music
			backgroundMusic = Mix_LoadMUS("res\\music\\proxy.mp3");

			if (!Mix_PlayingMusic() && config.getMusicPlay())
			{
				Mix_PlayMusic(backgroundMusic, -1);
			}
			else if (Mix_PausedMusic())
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}

			//sfx
			//Mix_Chunk *shootBullet = Mix_LoadMUS();

			//make game texture
			SDL_Surface *gameTexture = IMG_Load("res\\img\\shipTextures.png");
			gameShips = SDL_CreateTextureFromSurface(windowRenderer, gameTexture);
			SDL_FreeSurface(gameTexture);

			curShipType = shipStarter;
			setPlayerStats();

			//setup enemies
			enemyNum = 2;
			enemyHP = 10;
			enemyShield = 10;
			enemySpeed = 50;

			#pragma region HUDItems;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			//all bkg are the same width
			hpBarBkg.w = shieldBarBkg.w = energyBarBkg.w = config.getWinWidth() / 2;
			//all bars are 25px
			hpBarBkg.h = shieldBarBkg.h = energyBarBkg.h = hpBar.h = shieldBar.h = energyBar.h = 
				hpTxt.h = shieldTxt.h = energyTxt.h = fpsTxt.h =
				survTimeHour.h = survTimeMin.h = survTimeSec.h 
				= 20;
			//the x location of the bars
			hpBar.x = shieldBar.x = energyBar.x = hpBarBkg.x = shieldBarBkg.x = energyBarBkg.x =
				(config.getWinWidth() / 2) - config.getWinWidth() / 4;

			//y locations
			hpBarBkg.y = hpBar.y = hpTxt.y = survTimeHour.y = config.getWinHeight() - 90;
			shieldBarBkg.y = shieldBar.y = shieldTxt.y = survTimeMin.y = config.getWinHeight() - 65;
			energyBarBkg.y = energyBar.y = energyTxt.y = survTimeSec.y = config.getWinHeight() - 40;
			////////////////////////////////////////////////////////////////////////////////////////////////////

			#pragma endregion HUDItems;

			//set menu buttons
			//main menu
			playMain = Button(config.getWinWidth(), config.getWinHeight(), "PLAY", 3);
			upgradeMain = Button(config.getWinWidth(), config.getWinHeight(), "UPGRADE", 4);
			optionMain = Button(config.getWinWidth(), config.getWinHeight(), "OPTION", 5);
			quitMain = Button(config.getWinWidth(), config.getWinHeight(), "QUIT", 6);

			#pragma region UpgradeMenu 
			////////////////////////////////////////////////////////////////////////////////////////////////////

			//upgrade menu
			int xPadding = config.getWinWidth() / 40;

			shipyard = Button(config.getWinWidth(), config.getWinHeight(), xPadding, "SHIPYARD", 2);

			//inner upgrade menu
			xPadding += (config.getWinWidth() / 10) + (config.getWinWidth() / 40);

			shipStarterBkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "FIGHTER 1", 1);
			ship1Bkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "FIGHTER 2", 2);
			ship2Bkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "FIGHTER 3", 3);
			ship3Bkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "FIGHTER 4", 4);

			shipFrigateBkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "FRIGATE", 5);
			shipCruiserBkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "CRUISER", 6);
			shipBattleCruiserBkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "BATTLECRUISER", 7);

			shipDreadnoughtBkg = Button(config.getWinWidth() * 1.8, (config.getWinHeight() * 1.4), xPadding, "DREADNOUGHT", 8);

			//set unlock variables
			config.shipGlassCannonUnlocked = config.shipTankUnlocked = config.shipSpeedyUnlocked =
				config.shipFrigateUnlocked = config.shipCruiserUnlocked = config.shipBattleCruiserUnlocked =
				config.shipDreadnoughtUnlocked = false;

			//calculate the size of the frame
			int dispFrameX = xPadding + ((config.getWinWidth() * 1.8 / 10) + 20) + 20,
				dispFrameY = 50,
				dispFrameW = (config.getWinWidth() - 20) - dispFrameX,
			dispFrameH = (config.getWinHeight()) - dispFrameY * 2;

			shipDisplayFrame = {dispFrameX, dispFrameY, dispFrameW, dispFrameH};
			
			shipImage.w = shipImage.h = shipDisplayFrame.h / 4;
			shipImage.x = shipDisplayFrame.x + ((shipDisplayFrame.w / 2) - (shipImage.w / 2));
			shipImage.y = shipDisplayFrame.y + config.getWinHeight() / 10;

			//buy or equip the ship
			int buyEquipWidth = config.getWinWidth() / 12,
				buyEquipHeight = config.getWinHeight() / 15;
			buyShip = Button(buyEquipWidth, buyEquipHeight, shipDisplayFrame.x - (buyEquipWidth / 2) + (shipDisplayFrame.w / 2), shipDisplayFrame.y + shipDisplayFrame.h - buyEquipHeight, " BUY ");
			equipShip = Button(buyEquipWidth, buyEquipHeight, shipDisplayFrame.x - (buyEquipWidth / 2) + (shipDisplayFrame.w / 2), shipDisplayFrame.y + shipDisplayFrame.h - buyEquipHeight, "EQUIP");

			////////////////////////////////////////////////////////////////////////////////////////////////////
			#pragma endregion UpgradeMenu

			//option menu
			//fullscreen is either on or off
			if(config.getFullscreen())
			{
				fullscreen = Button(config.getWinWidth(), config.getWinHeight(), "FULLSCREEN [OFF]", 1);
			}
			else
			{
				fullscreen = Button(config.getWinWidth(), config.getWinHeight(), "FULLSCREEN [ON]", 1);
			}

			//music option
			if (config.getMusicPlay())
			{
				musicOpt = Button(config.getWinWidth(), config.getWinHeight(), "MUSIC [ON]", 2);
			}
			else
			{
				musicOpt = Button(config.getWinWidth(), config.getWinHeight(), "MUSIC [OFF]", 2);
			}

			//window sizes
			//calculate the button sizes
			int bWidth = config.getWinWidth() / 8;
			int bHeight = config.getWinHeight() / 15;
			int centreX = config.getWinWidth() / 2;

			back = Button(bWidth / 2, bHeight, 10, config.getWinHeight() - (bHeight + 10), "<<");

			//buttons do not exist if fullscreen
			if (!config.getFullscreen())
			{
				size1152x648 = Button(bWidth, bHeight, centreX - (2 * bWidth + bWidth / 2), bHeight, "1152 x 648");
				size1280x720 = Button(bWidth, bHeight, centreX - (bWidth + bWidth/2), bHeight, "1280 x 720");
				size1366x768 = Button(bWidth, bHeight, centreX - (bWidth/2), bHeight, "1366 x 768");
				size1600x900 = Button(bWidth, bHeight, centreX + (bWidth - bWidth/2), bHeight, "1600 x 900");
				size_max = Button(bWidth, bHeight, centreX + (bWidth + bWidth/2), bHeight, "1920 × 1080");
			}
			else
			{
				//temporarily kills the buttons
				//my sister is the perfect test engineer
				//i.e. she breaks everything
				size1152x648 = size1280x720 =
					size1366x768 = size1600x900 = size_max = Button();
			}
		}

		//clear previous background
		starList.clear();

		//game background
		menuRect = { 0, 0, config.getWinWidth(), config.getWinHeight() };
		createStars(menuRect);

		//load game vars
		config.loadGame();
	}

	

	//std::cout << screenRect.x << "\n" << screenRect.y << "\n" << screenRect.w << "\n" << screenRect.h << "\n";
}

void Game::createStars(SDL_Rect starSpaceRect)
{
	starNum = (starSpaceRect.w * starSpaceRect.h) / 250;
		
	//make all these stars
	while (starNum > 0)
	{
		BackgroundStar star = BackgroundStar(starSpaceRect);
		starList.push_back(star);

		starNum--;
	}
}

void Game::close()
{
	//close SDL
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	//std::cout << "shut down" << std::endl;
}


void Game::setPlayerStats()
{
	switch (curShipType)
	{
	case 0:
		//set normal stats
		config.setPlayerHP(starterHP);
		config.setPlayerShield(starterShield);
		config.setFireRate(starterFirerate);
		config.setDmg(starterDmg);
		config.setArmor(starterArmor);
		config.setSpeed(starterSpd);
		config.setEnergy(starterEnergy);

		curShipTexture = shipStarterTextureRect;
		break;

	case 1:
		//set glass cannon stats
		config.setPlayerHP(gcHP);
		config.setPlayerShield(gcShield);
		config.setFireRate(gcFirerate);
		config.setDmg(gcDmg);
		config.setArmor(gcArmor);
		config.setSpeed(gcSpd);
		config.setEnergy(gcEnergy);

		curShipTexture = shipGlassCannonTextureRect;
		break;

	case 2:
		//set tank stats
		config.setPlayerHP(tankHP);
		config.setPlayerShield(tankShield);
		config.setFireRate(tankFirerate);
		config.setDmg(tankDmg);
		config.setArmor(tankArmor);
		config.setSpeed(tankSpd);
		config.setEnergy(tankEnergy);

		curShipTexture = shipTankTextureRect;
		break;

	case 3:
		//set speedy stats
		config.setPlayerHP(speedyHP);
		config.setPlayerShield(speedyShield);
		config.setFireRate(speedyFirerate);
		config.setDmg(speedyDmg);
		config.setArmor(speedyArmor);
		config.setSpeed(speedySpd);
		config.setEnergy(speedyEnergy);

		curShipTexture = shipSpeedyTextureRect;
		break;

	case 4:
		//set frigate stats
		config.setPlayerHP(frigHP);
		config.setPlayerShield(frigShield);
		config.setFireRate(frigFirerate);
		config.setDmg(frigDmg);
		config.setArmor(frigArmor);
		config.setSpeed(frigSpd);
		config.setEnergy(frigEnergy);

		curShipTexture = shipFrigateTextureRect;
		break;

	case 5:
		//set cruiser stats
		config.setPlayerHP(cHP);
		config.setPlayerShield(cShield);
		config.setFireRate(cFirerate);
		config.setDmg(cDmg);
		config.setArmor(cArmor);
		config.setSpeed(cSpd);
		config.setEnergy(cEnergy);

		curShipTexture = shipCruiserTextureRect;
		break;

	case 6:
		//set battlecruiser stats
		config.setPlayerHP(bcHP);
		config.setPlayerShield(bcShield);
		config.setFireRate(bcFirerate);
		config.setDmg(bcDmg);
		config.setArmor(bcArmor);
		config.setSpeed(bcSpd);
		config.setEnergy(bcEnergy);

		curShipTexture = shipBattleCruiserTextureRect;
		break;

	case 7:
		//set dreadnought stats
		config.setPlayerHP(dreadHP);
		config.setPlayerShield(dreadShield);
		config.setFireRate(dreadFirerate);
		config.setDmg(dreadDmg);
		config.setArmor(dreadArmor);
		config.setSpeed(dreadSpd);
		config.setEnergy(dreadEnergy);

		curShipTexture = shipDreadnoughtTextureRect;
		break;

	}
}

void Game::updateMainMenu()
{
	//clear renderer
	SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
	SDL_RenderClear(windowRenderer);

	//draw the stars
	for (int i = 0; i < starList.size(); i++)
	{
		starList[i].update(windowRenderer, menuRect);
	}

	//draw menu buttons
	playMain.draw(windowRenderer, mousePos, gameFont);
	upgradeMain.draw(windowRenderer, mousePos, gameFont);
	optionMain.draw(windowRenderer, mousePos, gameFont);
	quitMain.draw(windowRenderer, mousePos, gameFont);

	//present the changes to the window
	SDL_RenderPresent(windowRenderer);

	//delay by the maximum fps
	SDL_Delay((1000 / config.getFPSCap()));
}

void Game::updateOptionMenu()
{
	//clear renderer
	SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
	SDL_RenderClear(windowRenderer);

	//draw the stars
	for (int i = 0; i < starList.size(); i++)
	{
		starList[i].update(windowRenderer, menuRect);
	}
	
	//option buttons
	fullscreen.draw(windowRenderer, mousePos, gameFont);
	musicOpt.draw(windowRenderer, mousePos, gameFont);
	back.draw(windowRenderer, mousePos, gameFont);

	//only draw if not fullscreen
	if (!config.getFullscreen())
	{
		size1152x648.draw(windowRenderer, mousePos, gameFont);
		size1280x720.draw(windowRenderer, mousePos, gameFont);
		size1366x768.draw(windowRenderer, mousePos, gameFont);
		size1600x900.draw(windowRenderer, mousePos, gameFont);
		size_max.draw(windowRenderer, mousePos, gameFont);
	}

	//present the changes to the window
	SDL_RenderPresent(windowRenderer);

	//delay by the maximum fps
	SDL_Delay((1000 / config.getFPSCap()));
}

void Game::updateUpgradeMenu(int innerMenu, int shipChoice)
{
	//clear renderer
	SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
	SDL_RenderClear(windowRenderer);

	//draw the stars
	for (int i = 0; i < starList.size(); i++)
	{
		starList[i].update(windowRenderer, menuRect);
	}

	//draw the back buton
	back.draw(windowRenderer, mousePos, gameFont);

	//display the scrap
	std::string scrapTxt = "Scrap: " + std::to_string(config.getScrap());
	scrapDisplay = Text(((config.getWinWidth() / 80) * scrapTxt.length()), ((config.getWinHeight() / 40)), config.getWinWidth() / 40, config.getWinHeight() / 40, scrapTxt);
	scrapDisplay.draw(windowRenderer, 255, 255, 255, 255, gameFont);

	shipyard.draw(windowRenderer, mousePos, gameFont);

	switch (innerMenu)
	{
		//shipyard buttons
		case 0:
		{
			shipStarterBkg.draw(windowRenderer, mousePos, gameFont);
			ship1Bkg.draw(windowRenderer, mousePos, gameFont);
			ship2Bkg.draw(windowRenderer, mousePos, gameFont);
			ship3Bkg.draw(windowRenderer, mousePos, gameFont);
			shipFrigateBkg.draw(windowRenderer, mousePos, gameFont);
			shipCruiserBkg.draw(windowRenderer, mousePos, gameFont);
			shipBattleCruiserBkg.draw(windowRenderer, mousePos, gameFont);
			shipDreadnoughtBkg.draw(windowRenderer, mousePos, gameFont);

			SDL_Rect tmpFrame = shipDisplayFrame;

			//draw display frame 25% of the rectthick
			for (int i = 0; i < (tmpFrame.w / 10) - 5; i++)
			{
				SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 100 - i);

				tmpFrame.x = shipDisplayFrame.x + i;
				tmpFrame.y = shipDisplayFrame.y + i;
				tmpFrame.w = shipDisplayFrame.w - (2 * i);
				tmpFrame.h = shipDisplayFrame.h - (2 * i);

				//draw the rect
				SDL_RenderDrawRect(windowRenderer, &tmpFrame);
			}
			SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
			//SDL_RenderDrawRect(windowRenderer, &shipImage);
			SDL_RenderDrawRect(windowRenderer, &shipDisplayFrame);

			SDL_Rect properRectSize = shipImage;
			properRectSize.w = properRectSize.h /= 2;

			//text to display the ship stats
			//calculate height based on the rect size
			SDL_Rect textBox;
			textBox.h = shipDisplayFrame.h / 20;


			//width to multiply to length of text
			int textWidthMultiplier = shipDisplayFrame.w / 10;


			//draw items relevant to the user input
			switch (shipChoice)
			{
			case 0:
				properRectSize.x += (properRectSize.w / 2);
				properRectSize.y += (properRectSize.h / 2);
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipStarterTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);
				equipShip.draw(windowRenderer, mousePos, gameFont);
				break;

			case 1:
				properRectSize.x += (properRectSize.w / 2);
				properRectSize.y += (properRectSize.h / 2);
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipGlassCannonTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipGlassCannonUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;

			case 2:
				properRectSize.x += (properRectSize.w / 2);
				properRectSize.y += (properRectSize.h / 2);
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipTankTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipTankUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;

			case 3:
				properRectSize.x += (properRectSize.w / 2);
				properRectSize.y += (properRectSize.h / 2);
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipSpeedyTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipSpeedyUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;

			case 4:
				properRectSize.h *= 2;
				properRectSize.x += (properRectSize.w / 2);
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipFrigateTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipFrigateUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;

			case 5:
				properRectSize.h *= 2;
				properRectSize.x += (properRectSize.w / 2);
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipCruiserTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipCruiserUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;

			case 6:
				properRectSize.h = properRectSize.w *= 2;
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipBattleCruiserTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipBattleCruiserUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;

			case 7:
				properRectSize.h = properRectSize.w *= 2;
				SDL_RenderCopyEx(windowRenderer, gameShips, &shipDreadnoughtTextureRect, &properRectSize, 0, NULL, SDL_FLIP_NONE);

				if (config.shipDreadnoughtUnlocked)
				{
					equipShip.draw(windowRenderer, mousePos, gameFont);
				}
				else
				{
					buyShip.draw(windowRenderer, mousePos, gameFont);
				}
				break;
			}
			break;
		}
			

		default:
			break;
	}

	//present the changes to the window
	SDL_RenderPresent(windowRenderer);

	//delay by the maximum fps
	SDL_Delay((1000 / config.getFPSCap()));
}

void Game::mainLoop()
{
	while (config.getRun())
	{		
		while (SDL_PollEvent(&mainMenu) != 0)
		{
			//update mouse position
			SDL_GetMouseState(&mousePos.x, &mousePos.y);
			
			switch (mainMenu.type)
			{
				//if window is closed
				case SDL_QUIT:
					//allows the loop to exit and close game
					config.setRun(false);
					SDL_Delay(100);
					close();
					break;

				//mouse clicked
				case SDL_MOUSEBUTTONUP:
					//left click
					if (mainMenu.button.button == SDL_BUTTON_LEFT)
					{
						if (playMain.pointInRect(mousePos))
						{
							//std::cout << "PLAY" << std::endl;
							//set up player ship
							player = Ship(config.getWinWidth(), config.getWinHeight(), config.getSpeed(), curShipType, curShipTexture);

							//delay before begin
							SDL_Delay(100);
							mousePos = { 0, 0 };

							//start the actual game
							gameLoop();

							mousePos = { 0, 0 };
						}

						if (upgradeMain.pointInRect(mousePos))
						{
							//upgrade menu
							//std::cout << "UPGRADE" << std::endl;

							//delay before upgrade menu
							SDL_Delay(100);
							mousePos = { 0, 0 };

							upgradeLoop();

							mousePos = { 0, 0 };
						}

						if (optionMain.pointInRect(mousePos))
						{
							//option menu
							//std::cout << "OPTION" << std::endl;

							//delay before option menu
							SDL_Delay(100);
							mousePos = { 0, 0 };

							optionLoop();

							mousePos = { 0, 0 };
						}

						if (quitMain.pointInRect(mousePos))
						{
							//quit the game
							//std::cout << "QUIT" << std::endl;
							config.setRun(false);
							SDL_Delay(100);
						}
					}
					break;

				default:
					break;
			}
		}
		//redraw the menu
		updateMainMenu();
	}

}

void Game::upgradeLoop()
{
	//boolean to keep the user in the upgrade menu until they want to exit
	bool inUpgradeMenu = true;

	//inner menu
	int innerMenu = 0;

	//current ship being looked at
	int shipChoice = 0;

	//upgrade menu loop
	while (inUpgradeMenu)
	{
		while (SDL_PollEvent(&upgradeMenu))
		{
			//update mouse position
			SDL_GetMouseState(&mousePos.x, &mousePos.y);

			//back to main menu
			if (upgradeMenu.key.keysym.sym == SDLK_ESCAPE)
			{
				inUpgradeMenu = false;
				SDL_Delay(100);
			}

			switch (upgradeMenu.type)
			{
				//if window is closed
				case SDL_QUIT:
					//allows the loop to exit and close game
					config.setRun(false);
					inUpgradeMenu = false;
					SDL_Delay(100);
					break;

				//mouse clicked
				case SDL_MOUSEBUTTONUP:
					//left click
					if (upgradeMenu.button.button == SDL_BUTTON_LEFT)
					{
						//shipyard
						if (shipyard.pointInRect(mousePos))
						{
							innerMenu = 0;

							//std::cout << innerMenu << "\n";
						}

						if (back.pointInRect(mousePos))
						{
							inUpgradeMenu = false;
							SDL_Delay(100);
						}

						//determine the inputs depending on which inner menu is chosen
						switch (innerMenu)
						{
							//shipyard inputs
							case 0:
								//choosing the ship to view
								if (shipStarterBkg.pointInRect(mousePos))
								{ shipChoice = 0; }
								if (ship1Bkg.pointInRect(mousePos))
								{ shipChoice = 1; }
								if (ship2Bkg.pointInRect(mousePos))
								{ shipChoice = 2; }
								if (ship3Bkg.pointInRect(mousePos))
								{ shipChoice = 3; }
								if (shipFrigateBkg.pointInRect(mousePos))
								{ shipChoice = 4; }
								if (shipCruiserBkg.pointInRect(mousePos))
								{ shipChoice = 5; }
								if (shipBattleCruiserBkg.pointInRect(mousePos))
								{ shipChoice = 6; }
								if (shipDreadnoughtBkg.pointInRect(mousePos))
								{ shipChoice = 7; }

								//buying or equipping ships
								switch (shipChoice)
								{
								case 0:
									if (equipShip.pointInRect(mousePos))
									{
										curShipType = shipChoice;
									}
									break;

									//glass cannon
								case 1:
									//if the ship is unlocked
									if (config.shipGlassCannonUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= glassCannonCost)
										{
											//set ship to unlock
											config.shipGlassCannonUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - glassCannonCost);
										}
									}
									break;

									//tank
								case 2:
									//if the ship is unlocked
									if (config.shipTankUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= tankCost)
										{
											//set ship to unlock
											config.shipTankUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - tankCost);
										}
									}
									break;

									//speedy
								case 3:
									//if the ship is unlocked
									if (config.shipSpeedyUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= speedyCost)
										{
											//set ship to unlock
											config.shipSpeedyUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - speedyCost);
										}
									}
									break;

									//frigate
								case 4:
									//if the ship is unlocked
									if (config.shipFrigateUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= frigateCost)
										{
											//set ship to unlock
											config.shipFrigateUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - frigateCost);
										}
									}
									break;

									//cruiser
								case 5:
									//if the ship is unlocked
									if (config.shipCruiserUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= cruiserCost)
										{
											//set ship to unlock
											config.shipCruiserUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - cruiserCost);
										}
									}
									break;

									//battlecruiser
								case 6:
									//if the ship is unlocked
									if (config.shipBattleCruiserUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= battleCruiserCost)
										{
											//set ship to unlock
											config.shipBattleCruiserUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - battleCruiserCost);
										}
									}
									break;

									//dreadnought
								case 7:
									//if the ship is unlocked
									if (config.shipDreadnoughtUnlocked)
									{
										//set the current ship to the chosen ship
										if (equipShip.pointInRect(mousePos))
										{
											curShipType = shipChoice;
										}
									}
									//if not, unlock the ship
									else
									{
										//unlock if the player has enough money
										if (buyShip.pointInRect(mousePos) && config.getScrap() >= dreadnoughtCost)
										{
											//set ship to unlock
											config.shipDreadnoughtUnlocked = true;
											curShipType = shipChoice;

											//reduce the player's money
											config.setScrap(config.getScrap() - dreadnoughtCost);
										}
									}
									break;

								}

								//set the player stats so the correct stats are applied
								setPlayerStats();
								break;

							//offense inputs
							case 1:

								break;

							//defense inputs
							case 2:

								break;

							//auxiliary inputs
							case 3:

								break;

							//special inputs
							case 4:

								break;


							default:
								break;
						}
					}
					break;
			}
		}

		//redraw upgrade menu
		updateUpgradeMenu(innerMenu, shipChoice);
	}

	//set the player stats
	setPlayerStats();

	//autosave
	config.saveGame();
}

void Game::optionLoop()
{
	//boolean to keep the option menu looping
	bool inOptionMenu = true;

	//option menu loop
	while (inOptionMenu)
	{
		while (SDL_PollEvent(&optionMenu) != 0)
		{
			//update mouse position
			SDL_GetMouseState(&mousePos.x, &mousePos.y);

			//back to main menu
			if (optionMenu.key.keysym.sym == SDLK_ESCAPE)
			{
				inOptionMenu = false;
				SDL_Delay(100);
			}

			switch (optionMenu.type)
			{
				//if window is closed
				case SDL_QUIT:
					//allows the loop to exit and close game
					config.setRun(false);
					inOptionMenu = false;
					SDL_Delay(100);
					break;

					//mouse clicked
				case SDL_MOUSEBUTTONUP:
					//left click
					if (mainMenu.button.button == SDL_BUTTON_LEFT)
					{
						//changing fullscreen
						if (fullscreen.pointInRect(mousePos))
						{							
							//if already fullscreen, set to not fullscreen
							if (config.getFullscreen())
							{
								//set to not fullscreen
								config.setFullscreen(false);
								resetWindow(window);
							}
							else
							{
								//set to fullscreen
								config.setFullscreen(true);
								resetWindow(window);
							}
						}

						//music option
						if (musicOpt.pointInRect(mousePos))
						{
							if (config.getMusicPlay())
							{
								config.setMusicPlay(false);
								musicOpt = Button(config.getWinWidth(), config.getWinHeight(), "MUSIC [OFF]", 2);
							}
							else
							{
								config.setMusicPlay(true);
								musicOpt = Button(config.getWinWidth(), config.getWinHeight(), "MUSIC [ON]", 2);
							}
							resetWindow(window);
						}
					
						//changing window sizes
						//1152 x 648
						if (size1152x648.pointInRect(mousePos))
						{
							config.setWinWidth(1152);
							config.setWinHeight(648);
							resetWindow(window);
						}
						//1280 x 720
						if (size1280x720.pointInRect(mousePos))
						{
							config.setWinWidth(1280);
							config.setWinHeight(720);
							resetWindow(window);
						}
						//1366 x 768
						if (size1366x768.pointInRect(mousePos))
						{
							config.setWinWidth(1366);
							config.setWinHeight(768);
							resetWindow(window);
						}
						//1600 x 900
						if (size1600x900.pointInRect(mousePos))
						{
							config.setWinWidth(1600);
							config.setWinHeight(900);
							resetWindow(window);
						}
						if (size_max.pointInRect(mousePos))
						{
							config.setWinWidth(1920);
							config.setWinHeight(1080);
							resetWindow(window);
						}
						if (back.pointInRect(mousePos))
						{
							inOptionMenu = false;
							SDL_Delay(100);
						}
					}
					break;

				default:
					break;
				}
		}

		//redraw option menu
		updateOptionMenu();
	}	
}

void Game::resetWindow(SDL_Window *window)
{
	//close window
	SDL_DestroyWindow(window);
	init();
}
