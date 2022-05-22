#include "CommanFunc.h"
#include "BaseObject.h"
#include "CarObject.h"
#include "ObstaclesObject.h"
#include "Functions.h"
#include "TextObject.h"
#include "Geomatric.h"
#include "Menu.h"
#include "TimeObject.h"
#include "HightScore.h"
#include "HowToPlay.h"
#include "ExitMenu.h"
#include "ExplosionObject.h"

BaseObject gBackGround;
TextObject gText;
TextObject CountdownText;
SDL_Color Color = { 255, 0, 255 };
static int timeAppearObstacles ;
static int timeMaxAppearObstacles ;
static int currentObstacles = 0;

bool Init();
bool loadmedia(CarObject& gRedCar,ExplosionObject& gExp);
void runCowndown(Uint32& timeCountdown, Uint32& val, std::stringstream& valTextcoundown, std::string& Textcoundown);
void runGamePlay(SDL_Renderer* gRenderer, BaseObject& gBackGround, CarObject& gRedCar, TTF_Font* gFont, std::vector<Obstacles>& gObstacles, TextObject& gText, Uint32& time, Uint32& val, std::stringstream&
	valText, std::string& TextScore, int& Score);
void setExplosion(int posX, int posY,ExplosionObject& gExp, BaseObject& background_,CarObject& car_, TextObject& text_, std::vector<Obstacles>gObstacles,SDL_Renderer* screen);
void closeWhenFinish(BaseObject& gBackGround, CarObject& gRedCar,
	ExplosionObject& gExp, std::vector<Obstacles>& gObstacles,
	TTF_Font* gFont, Mix_Music* gMusic, Mix_Chunk* gCountdown, Mix_Chunk* gEffectCrash);
void close();

int main(int argc, char* args[])
{
	srand(time(0));

	if (!Init())
	{
		std::cout << "Failed to intialize!" << std::endl;
	}
	else
	{
		bool Game = false;
		int menu = M_MENU;
		bool gameOver = false;
		CountdownFont = TTF_OpenFont("font//VNMUSTI.ttf", 80);
		MenuFont = TTF_OpenFont("font//VNMUSTI.ttf", 60);
		HowToPlayFont = TTF_OpenFont("font//VNMUSTI.ttf", 50);
		std::vector<int>HightScore;
		while (!Game) {
			while (menu == M_MENU) {
				int start = ShowMenu::showMenuStart(gRenderer, MenuFont);
				if (start == M_EXIT)
				{
					Game = true;
					menu = M_EXIT;
				}
				else if (start == M_HIGHT_SCORE) {
					int hightscore = ShowHightScore::showHightScore(gRenderer, MenuFont, HightScore);
					if (hightscore == M_EXIT) {
						Game = true;
						menu = M_EXIT;
					}
				}
				else if (start == M_HOW_TO_PLAY)
				{
					int Help = ShowHowToPlay::showHowToPlay(gRenderer,HowToPlayFont);
					if (Help == M_EXIT)
					{
						Game = true;
						menu = M_EXIT;
					}
				}
				else if (start == M_PLAY_GAME)
				{
					menu = M_PLAY_GAME;	
				}
			}
			if(menu == M_PLAY_GAME)
			{
			
			CarObject gRedCar;
			std::vector<Obstacles>gObstacles;
			ExplosionObject gExp;
			currentObstacles = 0;
			timeAppearObstacles = 30;
			timeMaxAppearObstacles = 30;
			TextScore = "Score: 0";
			int increase = 1;
			if (!loadmedia(gRedCar, gExp))
			{
				std::cout << "Failed to load image" << std::endl;
			}
			else
			{
				CountdownFont = TTF_OpenFont("font//VNMUSTI.ttf", 80);
				bool quit = false;
				std::stringstream valText;
				std::stringstream valTextcoundown;
				Mix_PlayChannel(-1, gCountdown, 0);
				Uint32 time;
				// set countdown from 3 to 1
				Uint32 timeCountdown = 4;
				Uint32 val = 4 + (SDL_GetTicks()) / 1000;
				while (timeCountdown > 0)
				{
					runCowndown(timeCountdown, val, valTextcoundown, Textcoundown);
					if (timeCountdown == 0)
					{
						break;
					}
					SetCountdown(gRenderer, CountdownFont, gCountdown, gBackGround, gRedCar, gText, CountdownText);
				}
				SDL_Delay(500);
				// set countdown word "GO"
				Textcoundown = "GO!";
				CountdownText.setText(Textcoundown);
				SetCountdown(gRenderer, CountdownFont, gCountdown, gBackGround, gRedCar, gText, CountdownText);
				SDL_Delay(1000);
				Mix_PlayMusic(gMusic, -1);//play music When playgame
				while (!quit)
				{
					while (SDL_PollEvent(&gEvent) != 0)
					{
						if (gEvent.type == SDL_QUIT)
						{
							Mix_HaltMusic();
							quit = true;
							Game = true;
						}
						gRedCar.handleInputAction(gEvent, gRenderer);
					}
					runGamePlay(gRenderer, gBackGround, gRedCar, gFont, gObstacles, gText, time, val, valText, TextScore, Score);
					for (int cls = 0; cls < gObstacles.size(); cls++)
					{
						if (checkCollision(gRedCar.getCarPixels(), gObstacles[cls].getObsPixels()))
						{
							Mix_HaltMusic();
							Mix_PlayChannel(-1, gEffectCrash, 0);
							setExplosion(gRedCar.GetPosXCar(), gRedCar.GetPosYCar(), gExp, gBackGround, gRedCar, gText, gObstacles, gRenderer);
							gRedCar.free();
							quit = true;
							gameOver = true;
							HightScore.push_back(Score);
							sort(HightScore);
							break;
						}
					}
					int frameticks = frameTime.getTicks();
					if (frameticks < 1000 / FRAME_PER_SECOND)
					{
						SDL_Delay(1000 / FRAME_PER_SECOND - frameticks);
					}
					if (Score >= 10 * increase){
						setVelObstacles(gObstacles, 1);
						increase++;
					}
					if (Score >= 10) {
						timeMaxAppearObstacles = 50;
					}
					if (quit){
						closeWhenFinish(gBackGround, gRedCar, gExp, gObstacles, gFont, gMusic, gCountdown, gEffectCrash);
					}
					
				}

			}
				
		}
		if (gameOver) {
			int  finish = ShowExitMenu::showMenuExit(gRenderer, MenuFont, Score,HightScore);
			if (finish == E_PLAY_AGAIN)
			{
				menu = M_PLAY_GAME;
				gameOver = false;
				Score = 0;
			}

			else if (finish == E_RETURN_MENU) {
				menu = M_MENU;
				gameOver = false;
			}
			else if (finish == E_EXIT)
			{
				Game = true;
				menu = M_EXIT;
				gameOver = false;
			}
		}
		}
	}
	close();
	return 0;
}

bool Init()
{
	success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
		}
		gWindow = SDL_CreateWindow("Race", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Could not create window" << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				std::cout << "Could not create renderer:" << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, COLOR_R, COLOR_G, COLOR_B, COLOR_A);
				SDL_RenderClear(gRenderer);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
					success = false;
				}
				else {
					if (TTF_Init() == -1)
					{
						std::cout << "TTF could not initilize! SDL_ttf Error: " << TTF_GetError() << std::endl;
						success = false;
					}
					else {
						if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
						{
							std::cout << "SDL_mixer could not initialize! SDL_image Error: " << Mix_GetError() << std::endl;
							success = false;
						}
					}
				}

			}
		}

	}
	return success;
}

bool loadmedia(CarObject& gRedCar, ExplosionObject& gExp)
{
	success = true;
	if (!gExp.loadImageExp("image//explosion.png", gRenderer)) {
		success = false;
	}
	gExp.setClip();
	gFont = TTF_OpenFont("font//ARCADE.ttf", 40);
	if (gFont == NULL)
	{
		std::cout << "Failed to load lazy font! SDL_ttf Error:" << TTF_GetError() << std::endl;
		success = false;
	}
	else {
		// set scoretext and set coundown text
		gText.setColor(TextObject::WHITE_TEXT);
		gText.setText(TextScore);
		CountdownText.setColor(TextObject::RED_TEXT);
		CountdownText.setText(Textcoundown);
		if (!gText.loadFromRenderedText(gFont, gRenderer))
		{
			success = false;
		}
	}
	// set music for game
	gMusic = Mix_LoadMUS("sound//Music_play_game.mp3");
	if (gMusic == NULL)
	{
		std::cout << "Failed to load music ! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}
	gEffectCrash = Mix_LoadWAV("sound//Crash.wav");
	if (gEffectCrash == NULL)
	{
		std::cout << "Failed to load music ! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}
	gCountdown = Mix_LoadWAV("sound//Countdown.wav");
	if (gCountdown == NULL)
	{
		std::cout << "Failed to load music ! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}
	//set background for game
	if (!gBackGround.loadImage("image//background_standard.png", gRenderer))
	{
		success = false;
	}
	if (!gRedCar.loadImageCar("image//car.png", gRenderer))
	{
		success = false;
		std::cout << "not load redcar" << std::endl;
	}
	return success;
}



void close()
{
	gBackGround.free();
	// destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	//destroy renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	// destroy font
	gText.free();
	CountdownText.free();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
void setExplosion(int posX, int posY, ExplosionObject& gExp, BaseObject& background_, CarObject& car_,TextObject& text_, std::vector<Obstacles>gObstacles, SDL_Renderer* screen) {
	posX -= gExp.getFrameWidth() * 0.5 - WIDTHCAR / 2;
	posY -= gExp.getFrameHeight() * 0.5 - HEIGHTCAR / 2;
	
	for (int k = 0; k < MAX_FRAME; k++) {
		RunBackground(background_, screen, true);
		for (int obs = 0; obs < gObstacles.size(); obs++) {
			gObstacles[obs].renderObstacles(screen);
		}
		car_.CarRender(screen);
		RenderRectangle(screen, FillColor, FillRect);
		RenderOutLine(screen, OutlineColor, OutlineRect);
		text_.renderText((SCREEN_WIDTH - text_.getWidthText()) / 2, 1, screen);
		SDL_Rect rect = gExp.getFrameClip(k);
		gExp.renderCar(posX, posY, screen,&rect) ;
		SDL_RenderPresent(screen);
		SDL_Delay(70);
	}
	gExp.free();	
		RunBackground(background_, screen, true);
		for (int obs = 0; obs < gObstacles.size(); obs++) {
			gObstacles[obs].renderObstacles(screen);
		}
		RenderRectangle(screen, FillColor, FillRect);
		RenderOutLine(screen, OutlineColor, OutlineRect);
		text_.renderText((SCREEN_WIDTH - text_.getWidthText()) / 2, 1, screen);
		SDL_RenderPresent(screen);
}
void runCowndown(Uint32& timeCountdown, Uint32& val, std::stringstream& valTextcoundown, std::string& Textcoundown)
{
	timeCountdown = val - (SDL_GetTicks() + 600) / 1000;
	if (timeCountdown == 4)
	{
		timeCountdown = 3;
	}
	valTextcoundown.str("");
	valTextcoundown << timeCountdown;
	Textcoundown = valTextcoundown.str().c_str();
	CountdownText.setText(Textcoundown);
	
}

void runGamePlay(SDL_Renderer* gRenderer, BaseObject& gBackGround, CarObject& gRedCar, TTF_Font* gFont, std::vector<Obstacles>& gObstacles, TextObject& gText, Uint32& time, Uint32& val, std::stringstream&
	valText, std::string& TextScore, int& Score) {
	time = (SDL_GetTicks() - 1200) / 1000 - val;
	valText.str("");
	valText << "Score: " << time;
	TextScore = valText.str().c_str();
	Score = time;
	gText.setText(TextScore);
	RunBackground(gBackGround, gRenderer, false);
	gRedCar.move();
	setObstacles(gObstacles, gRenderer, currentObstacles, timeAppearObstacles, timeMaxAppearObstacles);
	gText.loadFromRenderedText(gFont, gRenderer);
	renderObs(gObstacles, gRenderer);
	gRedCar.CarRender(gRenderer);
	RenderRectangle(gRenderer, FillColor, FillRect);
	RenderOutLine(gRenderer, OutlineColor, OutlineRect);
	gText.renderText((SCREEN_WIDTH - gText.getWidthText()) / 2, 1, gRenderer);
	SDL_RenderPresent(gRenderer);
}
void closeWhenFinish(BaseObject& gBackGround, CarObject& gRedCar,
	ExplosionObject& gExp, std::vector<Obstacles>& gObstacles,
	TTF_Font* gFont, Mix_Music* gMusic, Mix_Chunk* gCountdown, Mix_Chunk* gEffectCrash) {
	SDL_Delay(1500);
	gRedCar.free();
	for (int k = 0; k < gObstacles.size(); k++) {
		gObstacles[k].free();
	}
	gExp.free();
	gText.free();
	TTF_CloseFont(gFont);
	Mix_FreeChunk(gEffectCrash);
	Mix_FreeChunk(gCountdown);
	Mix_FreeMusic(gMusic);
	gBackGround.free();
}