/*
 * main.cpp
 *
 *  Created on: 23.08.2018
 *      Author: Łukasz Pękalski
 */

#include "main.h"
#include "classes/Win.h"
#include "classes/LTexture.h"
#include "classes/Snake.h"
#include "classes/Dot.h"
#include "classes/Tile.h"
#include "classes/Menu.h"
#include "classes/Timer.h"

int const TOTAL_POWERUPS = 5, MAX_POWERUP_TIME[TOTAL_POWERUPS] = { 20, 20, 20, 20, 20 }, gLvlWidth = 10000, gLvlHeight = gLvlWidth;
int gPowerUpsQuantity, gFruitsQuantity, gEnemyQuantity;

const double POWERUP_SCALE = 2, SCREEN_SCALE = 0.5;
bool initSDL(Win *window = NULL);
void close(Win *window = NULL);
void activatePowerup(int &fruitSpriteNum, Snake &vSnake);
void handleEvents();
void print();
bool escapeFromDanger(Snake &vSnake1, Snake &vSnake2, double &vAngle);
//----------------------------------------------------------------Deklaracje zmiennych
//Fruits vars
vector<int> gSpriteNum, x, y;
vector<Dot> gFruit;
//Players vars
vector<double> gAngle;
vector<int> gCurrentTime, gTimeElapsed, gPlayerSprite, gPlayerPosX, gPlayerPosY;
vector<vector<int> > gPlayerToTargetDistance(2, vector<int>());
vector<SDL_Point> gPlayerStartPos;
vector<Snake> gPlayer;
//-------Other vars
bool gContinue = true, gCollision = false, gReset = false;
int const *pTOTAL_TILES = NULL, TOTAL_NUMBER_OF_BUTTONS = 7, TEXT_SIZE = 50, TITLE_TEXT_SIZE = 150, MAIN_MENU_OPTS = TOTAL_NUMBER_OF_BUTTONS - 2, POSITIONS_IN_OPTIONS_MENU = 7;
double timeStep;
float gRenderScaleX = 1.0, gRenderScaleY = 1.0, tempScale = 1.0;
int gScreenWidth = 1024, gScreenHeight = 768, gCurrentScore = 0, gOption = -1, gGameState = 1;
const int TOTAL_SPRITES = 25, TOTAL_FRUIT_SPRITES = 30, SPRITE_DIMS = 20, POWERUP_ICON_DIMS = 50;
SDL_Point gEnemy1Pos, gEnemy2Pos;
stringstream gScore, gTimeLeft;
string gMenuItems[TOTAL_NUMBER_OF_BUTTONS] = { "Start :D (s)", "Reset game (r)", "Network game (i)", "Options (o)", "Quit :( (q/ESC)", "I have to :( (y)", "Maybe not :) (n/ESC)" };
string gOptionsItems[POSITIONS_IN_OPTIONS_MENU] = { "Change language", "Bots quantity", "Fruits quantity", "Powerups quantity", "Snake speed", "Save changes", "Back to main menu (no saving)" };
string gScreenInfos[5] = { "End of game?", "Game paused", "Game over :(", "Press R to restart or Q/ESC to exit", "Options" };
SDL_Renderer *gRenderer = NULL;
SDL_Event event;
Win gWindow;
LTexture gLTFruit, gLTSnakeHead, gLTSnakeTail, gLTLevelTexture, gLTScoreText, gLTTitleText, gLTExitQuestion, gLTMenuBackground, gLTPause, gLTEnemyHead, gLTEnemyTail, gLTGameOver, gLTPressToReset, gLTPowerupIcons, gLTPowerupsTimeText[TOTAL_POWERUPS], gLTOptionsText;
LTexture gLTPosTxt;
SDL_Rect gCamera = { 0, 0, (int) (gScreenWidth / gRenderScaleX), (int) (gScreenHeight / gRenderScaleY) }, gLevelBorders = { 0, 0, (int) (gLvlWidth / gRenderScaleX), (int) (gLvlHeight / gRenderScaleY) }, gBoxExitQuestion = { 0, 0, 0, 0 }, gBoxPause = { 0, 0, 0, 0 }, gMenuCamera = { 0, 0, gScreenWidth, gScreenHeight }, gMouse = { 0, 0, 1, 1 }, gSpriteClips[TOTAL_SPRITES], gFruitSpriteClips[TOTAL_FRUIT_SPRITES], gGameOverBox = { 0, 0, 0, 0 }, gPowerupClip[TOTAL_POWERUPS];
TTF_Font *gFont = NULL, *gTitleFont = NULL;
SDL_Color gTextColor = { 255, 0, 0 }, gTextPauseColor = { 255, 255, 0 }, gTextNormalColor = { 0, 255, 0 };
Button gButtons[TOTAL_NUMBER_OF_BUTTONS], gOptionButtons[POSITIONS_IN_OPTIONS_MENU], gButtonNextPrev[10];
Timer stepTimer, frameTimer;
double gFrameTimer = 0;
int gTimer = 0, Button::mButtonNum = 0, gSpritePosX = 0, gSpritePosY = 0, gFruitSpritePosX = 0, gFruitSpritePosY = 0, powerupTime[TOTAL_POWERUPS], gTmpVal = 0;
// fstream vars
fstream gSettingsFile, gLangFile, gFileLangList;
vector<string> gLangList;
string gAllTexts[TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 5], gTmpText, gChangeableOptionsPos[5], fSettingsInstructions[5] = { "/Languages, as they are present in the list", "/Enemies count - original 20", "/Fruits count - original 200", "/Powerups count, cannot be 0 - original 25", "/Snake speed - original 300" };
stringstream gPathToLangFile, gTextToShow;
int gSettingsFileContent[5], mOptsButtonsWidth[10];
// POWERUPS PARAMS
void powerupCheck(Snake &vSnake, bool render = false);
void gameReset(bool &reset);
void fTextToShow(int &i, int xpos, int ypos, string &vText);

int main(int argc, char* args[]) {
//Opening lang list file
	gFileLangList.open("./assets/lang/_LangList.txt");
	if (gFileLangList.is_open()) {
		while (!gFileLangList.eof()) {
			gFileLangList >> gTmpText;
			gLangList.push_back(gTmpText.c_str());
		}
		gFileLangList.close();
	} else {
		gFileLangList.close();
		cout << "Unable to load language list file!\n";
		gContinue = false;
	}
//Loading settings file
	if (gContinue) {
		gSettingsFile.open("./assets/settings.txt", ios::in);
		if (gSettingsFile.is_open()) {
			gPathToLangFile.str("");
			gPathToLangFile << "./assets/lang/";
			{
				int i = 0;
				while (getline(gSettingsFile, gTmpText, '/')) {
					gSettingsFile.ignore(1024, '\n');
					gSettingsFileContent[i] = stoi(gTmpText.c_str());
					i++;
				}
			}
			gPathToLangFile << gLangList[gSettingsFileContent[0]].c_str() << ".txt";
			gLangFile.open(gPathToLangFile.str().c_str(), ios::in);
			if (gLangFile.is_open()) {
				unsigned int i = 0;
				while (i < sizeof(gAllTexts) / sizeof(string)) {
					getline(gLangFile, gAllTexts[i]);
					i++;
				}
				gSettingsFile.close();
				gLangFile.close();
			} else {
				gSettingsFile.close();
				gLangFile.close();
				cout << "Unable to load translations from " << gPathToLangFile.str().c_str() << "!\n";
				gContinue = false;
			}
			gSettingsFile.close();
			gLangFile.close();
		} else {
			gSettingsFile.close();
			gLangFile.close();
			cout << "Unable to load translations from " << gPathToLangFile.str().c_str() << "!\n";
			cout << "Unable to load settings file!\n";
			gContinue = false;
		}
	} else {
		gSettingsFile.close();
		cout << "Unable to load settings file!\n";
		gContinue = false;
		gLangFile.close();
	}
	gSettingsFile.close();
	gPowerUpsQuantity = gSettingsFileContent[3];
	gChangeableOptionsPos[0] = gLangList[gSettingsFileContent[0]].c_str();
	gChangeableOptionsPos[1] = to_string(gSettingsFileContent[1]);
	gChangeableOptionsPos[2] = to_string(gSettingsFileContent[2]);
	gChangeableOptionsPos[3] = to_string(gSettingsFileContent[3]);
	gChangeableOptionsPos[4] = to_string(gSettingsFileContent[4]);

	gEnemyQuantity = gSettingsFileContent[1];
	gFruitsQuantity = gSettingsFileContent[2];
	gPowerUpsQuantity = gSettingsFileContent[3];

	if (gContinue) {
		for (int i = 0; i < gFruitsQuantity; i++) {
			gSpriteNum.push_back(0);
			x.push_back(0);
			y.push_back(0);
			gFruit.push_back(Dot());
		}
		for (int i = 0; i < TOTAL_NUMBER_OF_BUTTONS; i++) {
			gMenuItems[i] = gAllTexts[i];
		}
		for (int i = 0; i < POSITIONS_IN_OPTIONS_MENU; i++) {
			gOptionsItems[i] = gAllTexts[i + TOTAL_NUMBER_OF_BUTTONS];
		}
		for (int i = 0; i < 5; i++) {
			gScreenInfos[i] = gAllTexts[TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + i];
		}
		for (int i = 0; i <= gEnemyQuantity; i++) {
			gAngle.push_back(0.0);
			gCurrentTime.push_back(0);
			gTimeElapsed.push_back(0);
			gPlayerSprite.push_back(0);
			gPlayerStartPos.push_back( { 0, 0 });
			gPlayer.push_back(Snake());
			gPlayerToTargetDistance[0].push_back(0);
			gPlayerToTargetDistance[1].push_back(0);
		}
	}

	if (gContinue) {
		gContinue = initSDL(&gWindow);
	}
	if (gContinue) {
		gContinue = gLTFruit.loadFromFile("./assets/images/fruits.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTSnakeHead.loadFromFile("./assets/images/face.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTSnakeTail.loadFromFile("./assets/images/dot.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTLevelTexture.loadFromFile("./assets/images/bo_play_pattern.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTMenuBackground.loadFromFile("./assets/images/escheresque.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTEnemyHead.loadFromFile("./assets/images/faces.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTEnemyTail.loadFromFile("./assets/images/tails.png", gWindow);
	}
	if (gContinue) {
		gContinue = gLTPowerupIcons.loadFromFile("./assets/images/powerups.png", gWindow);
	}

	for (int i = 0; i < TOTAL_SPRITES; i++) {
		gSpriteClips[i]= {gSpritePosX, gSpritePosY, SPRITE_DIMS, SPRITE_DIMS};
		gSpritePosX+=SPRITE_DIMS;
		if(gSpritePosX>=100) {
			gSpritePosY+=SPRITE_DIMS;
			gSpritePosX=0;
		}
	}
	for (int i = 0; i < TOTAL_FRUIT_SPRITES; i++) {
		gFruitSpriteClips[i]= {gFruitSpritePosX, gFruitSpritePosY, SPRITE_DIMS, SPRITE_DIMS};
		gFruitSpritePosX+=SPRITE_DIMS;
		if(gFruitSpritePosX>=100) {
			gFruitSpritePosY+=SPRITE_DIMS;
			gFruitSpritePosX=0;
		}
	}
	gSpritePosX = 0;
	for (int i = 0; i < TOTAL_POWERUPS; i++) {
		gPowerupClip[i]= {gSpritePosX, 0, POWERUP_ICON_DIMS, POWERUP_ICON_DIMS};
		gSpritePosX+=POWERUP_ICON_DIMS;
	}

	gFont = TTF_OpenFont("./assets/fonts/Horta.ttf", TEXT_SIZE);
	gTitleFont = TTF_OpenFont("./assets/fonts/Horta.ttf", TITLE_TEXT_SIZE);
	TTF_SetFontStyle(gTitleFont, TTF_STYLE_UNDERLINE);
	if (gFont == NULL || gTitleFont == NULL) {
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		gContinue = false;
	}

	gLTTitleText.loadFromText("Snaker", gTextColor, gTitleFont, gWindow);
	gLTTitleText.setWidth(3 * gLTTitleText.getWidth());
	gLTTitleText.setHeight(TITLE_TEXT_SIZE);
//	TEXT FOR EXIT GAME DIALOGUE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	gLTExitQuestion.loadFromText(gScreenInfos[0].c_str(), gTextColor, gTitleFont, gWindow);
	gLTExitQuestion.setWidth(1.25 * gLTTitleText.getWidth());
	gLTExitQuestion.setHeight(TEXT_SIZE);
//	TEXT FOR PAUSE SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	gLTPause.loadFromText(gScreenInfos[1], gTextPauseColor, gFont, gWindow);
	gLTPause.setWidth(5 * gLTPause.getWidth());
//	TEXT FOR GAME OVER SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	gLTGameOver.loadFromText(gScreenInfos[2].c_str(), gTextColor, gTitleFont, gWindow);
	gLTGameOver.setHeight(TITLE_TEXT_SIZE);
	gLTGameOver.setWidth(5 * gLTGameOver.getWidth());
	gLTPressToReset.loadFromText(gScreenInfos[3].c_str(), gTextNormalColor, gTitleFont, gWindow);
	gLTPressToReset.setHeight(TEXT_SIZE);
	gLTPressToReset.setWidth(5 * gLTPressToReset.getWidth());
//	TEXT FOR OPTIONS SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	gLTOptionsText.loadFromText(gScreenInfos[4].c_str(), gTextColor, gTitleFont, gWindow);
	gLTOptionsText.setHeight(TEXT_SIZE);
//	gLTOptionsText.setWidth(5 * gLTOptionsText.getWidth());
//	BUTTONS FOR MAIN MENU >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for (int i = 0; i < TOTAL_NUMBER_OF_BUTTONS; i++) {
		gButtons[i].setButtonText(gMenuItems[i], gWindow, gFont, TEXT_SIZE);
	}
//	BUTTONS FOR OPTIONS MENU >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for (int i = 0; i < POSITIONS_IN_OPTIONS_MENU; i++) {
		gOptionButtons[i].setButtonText(gOptionsItems[i], gWindow, gFont, TEXT_SIZE);
	}
	for (int i = 0; i < 5; i++) {
		gButtonNextPrev[i].setButtonText("<-", gWindow, gFont, TEXT_SIZE, 0);
		gButtonNextPrev[i + 5].setButtonText("+>", gWindow, gFont, TEXT_SIZE, 0);
	}
	int gTotalTiles = 3 * ((gLvlWidth / 0.9) / gLTLevelTexture.getWidth()) * ((gLvlHeight / 0.9) / gLTLevelTexture.getHeight());
	int gTotalMenuTiles = 3 * ((gScreenWidth / gLTMenuBackground.getWidth()) * (gScreenHeight / gLTMenuBackground.getHeight()));
	Tile *tileSet[gTotalTiles], *gMenuBackground[gTotalMenuTiles];

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++EventHandler
void handleEvents() {
	if (gReset) {
		gReset = false;
	}
	while (SDL_PollEvent(&event)) {
		gOption = -1;
		if (event.type == SDL_QUIT) {
			gContinue = false;
		}
//		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) {
//			gSnake.addLength();
//			for (int i = 0; i < ENEMY_COUNT; i++) {
//				gEnemy[i].addLength();
//			}
//		}
//		if (event.type == SDL_KEYUP && event.key.repeat == 0) {
//			switch (event.key.keysym.sym) {
//				case SDLK_a:
//					if (gGameState == 0) {
//						SDL_RenderSetScale(gRenderer, 1, 1);
//					}
//					break;
//			}
//		}
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			switch (event.key.keysym.sym) {
				case SDLK_a:
					if (gGameState == 0) {
//						SDL_RenderSetScale(gRenderer, 0.85, 0.85);
//						distanceMultiplier = 12;
					}
					break;
				case SDLK_ESCAPE:
				case SDLK_q:
					if (gGameState == 1) {
						gGameState = 2;
					} else {
						gGameState = 1;
					}
					break;
				case SDLK_p:
					if (gGameState == 3) {
						stepTimer.unpause();
						gGameState = 0;
					} else {
						stepTimer.pause();
						gGameState = 3;
					}
					break;
				case SDLK_s:
					gGameState = 0;
					break;
				case SDLK_y:
				case SDLK_t:
					if (gGameState == 2) {
						gContinue = false;
					}
					break;
				case SDLK_n:
					if (gGameState == 2) {
						gGameState = 1;
					}
					break;
				case SDLK_r:
					gGameState = 0;
					gReset = true;
					break;
				case SDLK_o:
					gGameState = 5;
					break;
			}
		}
		if (event.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&gMouse.x, &gMouse.y);
		}
		gWindow.eventHandler(event);
		gSnake.eventHandler(event, &gWindow, &gCamera);
		for (int i = 0; i < (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU); i++) {
			switch (gGameState) {
				case 1:
					if (i >= 0 && i <= MAIN_MENU_OPTS - 1) {
						gButtons[i].eventHandler(event);
						if (checkCollision(gButtons[i].getButtonDims(), gMouse)) {
							if (event.type == SDL_MOUSEBUTTONUP) {
								gOption = gButtons[i].getID();
//								cout<<"gOption:"<<gOption<<endl;
							}
						}
					}
					break;
				case 2:
					if (i >= MAIN_MENU_OPTS && i <= MAIN_MENU_OPTS + 1) {
						gButtons[i].eventHandler(event);
						if (checkCollision(gButtons[i].getButtonDims(), gMouse)) {
							if (event.type == SDL_MOUSEBUTTONUP) {
								gOption = gButtons[i].getID();
//								cout<<"gOption:"<<gOption<<endl;
							}
						}
					}
					break;
				case 5:
					if (i >= POSITIONS_IN_OPTIONS_MENU - 2 && i <= POSITIONS_IN_OPTIONS_MENU - 1) {
						gOptionButtons[i].eventHandler(event);
						if (checkCollision(gOptionButtons[i].getButtonDims(), gMouse)) {
							if (event.type == SDL_MOUSEBUTTONUP) {
								gOption = gOptionButtons[i].getID();
//								cout<<"gOption:"<<gOption<<endl;
							}
						}
					}
					break;
			}
		}
		for (int j = 0; j < 10; j++) {
			switch (gGameState) {
				case 5:
					gButtonNextPrev[j].eventHandler(event);
					if (checkCollision(gButtonNextPrev[j].getButtonDims(), gMouse)) {
						if (event.type == SDL_MOUSEBUTTONUP) {
							gOption = gButtonNextPrev[j].getID();
//							cout<<"gOption:"<<gOption<<endl;
						}
						if (event.type == SDL_MOUSEWHEEL) {
							if (event.wheel.y < 0) {
								gOption = gButtonNextPrev[j].getID();
							}
							if (event.wheel.y > 0) {
								gOption = gButtonNextPrev[j].getID();
							}
						}
					}
					break;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))) {
			if (gGameState == 0) {
				stepTimer.pause();
				gGameState = 3;
			} else if (gGameState == 3) {
				stepTimer.unpause();
				gGameState = 0;
			}
		}
		switch (gOption) {
			case 1: //Start game
				gGameState = 0;
				break;
			case 2: //Reset game
				gGameState = 0;
				gReset = true;
				break;
			case 4: //Options
				gGameState = 5;
				break;
			case TOTAL_NUMBER_OF_BUTTONS - 2: //Quit game
				gGameState = 2;
				break;
			case TOTAL_NUMBER_OF_BUTTONS - 1: //Yes - kill game
				gContinue = false;
				break;
			case TOTAL_NUMBER_OF_BUTTONS: //No - get back to main menu
				gGameState = 1;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU - 1):
				if (gSettingsFileContent[0] < 0) {
					gSettingsFileContent[0] = 0;
				}
				if ((unsigned) gSettingsFileContent[0] > gLangList.size() - 1) {
					gSettingsFileContent[0] = gLangList.size() - 1;
				}
				if (gSettingsFileContent[1] <= 0) {
					gSettingsFileContent[1] = 0;
				}
				if (gSettingsFileContent[2] <= 2) {
					gSettingsFileContent[2] = 2;
				}
				if (gSettingsFileContent[3] <= 1) {
					gSettingsFileContent[3] = 1;
				}
				if (gSettingsFileContent[4] <= 1) {
					gSettingsFileContent[4] = 1;
				}
				gSettingsFile.open("./assets/settings.txt", ios::out);
				for (int i = 0; i < 5; i++) {
					gSettingsFile << gSettingsFileContent[i] << "\t" << fSettingsInstructions[i] << endl;
				}
				gSettingsFile.close();
				gPathToLangFile.str("");
				gPathToLangFile << "./assets/lang/";
				gPathToLangFile << gLangList[gSettingsFileContent[0]].c_str() << ".txt";
				gLangFile.open(gPathToLangFile.str().c_str(), ios::in);
				if (gLangFile.is_open()) {
					unsigned int i = 0;
					while (i < sizeof(gAllTexts) / sizeof(string)) {
						getline(gLangFile, gAllTexts[i]);
						i++;
					}
					gSettingsFile.close();
					gLangFile.close();
				} else {
					gSettingsFile.close();
					gLangFile.close();
					cout << "Unable to load translations from " << gPathToLangFile.str().c_str() << "!\n";
					gContinue = false;
				}
				gLangFile.close();
				for (int i = 0; i < TOTAL_NUMBER_OF_BUTTONS; i++) {
					gMenuItems[i] = gAllTexts[i];
				}
				for (int i = 0; i < POSITIONS_IN_OPTIONS_MENU; i++) {
					gOptionsItems[i] = gAllTexts[i + TOTAL_NUMBER_OF_BUTTONS];
				}
				for (int i = 0; i < 5; i++) {
					gScreenInfos[i] = gAllTexts[TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + i];
				}
				gLTExitQuestion.free();
				gLTPause.free();
				gLTGameOver.free();
				gLTPressToReset.free();
				gLTOptionsText.free();

				//	TEXT FOR EXIT GAME DIALOGUE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				gLTExitQuestion.loadFromText(gScreenInfos[0].c_str(), gTextColor, gTitleFont, gWindow);
				gLTExitQuestion.setWidth(1.25 * gLTTitleText.getWidth());
				gLTExitQuestion.setHeight(TEXT_SIZE);
				//	TEXT FOR PAUSE SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				gLTPause.loadFromText(gScreenInfos[1], gTextPauseColor, gFont, gWindow);
				gLTPause.setWidth(5 * gLTPause.getWidth());
				//	TEXT FOR GAME OVER SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				gLTGameOver.loadFromText(gScreenInfos[2].c_str(), gTextColor, gTitleFont, gWindow);
				gLTGameOver.setHeight(TITLE_TEXT_SIZE);
				gLTGameOver.setWidth(5 * gLTGameOver.getWidth());
				gLTPressToReset.loadFromText(gScreenInfos[3].c_str(), gTextNormalColor, gTitleFont, gWindow);
				gLTPressToReset.setHeight(TEXT_SIZE);
				gLTPressToReset.setWidth(5 * gLTPressToReset.getWidth());
				//	TEXT FOR OPTIONS SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				gLTOptionsText.loadFromText(gScreenInfos[4].c_str(), gTextColor, gTitleFont, gWindow);
				gLTOptionsText.setHeight(TEXT_SIZE);
				//	gLTOptionsText.setWidth(5 * gLTOptionsText.getWidth());
				//	BUTTONS FOR MAIN MENU >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				for (int i = 0; i < TOTAL_NUMBER_OF_BUTTONS; i++) {
					gButtons[i].setButtonText(gMenuItems[i], gWindow, gFont, TEXT_SIZE);
				}
				//	BUTTONS FOR OPTIONS MENU >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				for (int i = 0; i < POSITIONS_IN_OPTIONS_MENU; i++) {
					gOptionButtons[i].setButtonText(gOptionsItems[i], gWindow, gFont, TEXT_SIZE);
				}
				gPowerUpsQuantity = gSettingsFileContent[3];
				for (int i = 0; i < gFruitsQuantity; i++) {
					x[i] = (gLvlWidth - 20) * ((float) rand() / RAND_MAX);
					y[i] = (gLvlHeight - 20) * ((float) rand() / RAND_MAX);
					if (i % (gFruitsQuantity / gPowerUpsQuantity) == 0) {
						gSpriteNum[i] = (int) (30 - (5 * ((float) rand() / RAND_MAX)));
					} else {
						gSpriteNum[i] = (int) ((TOTAL_FRUIT_SPRITES - 5) * ((float) rand() / RAND_MAX));
					}
				}
				gFruitsQuantity = gSettingsFileContent[2];
				gSpriteNum.push_back(0);
				x.clear();
				y.clear();
				gFruit.clear();
				for (int i = 0; i < gFruitsQuantity; i++) {
					gSpriteNum.push_back(0);
					x.push_back(0);
					y.push_back(0);
					gFruit.push_back(Dot());
				}
				for (int i = 0; i < gFruitsQuantity; i++) {
					x[i] = (gLvlWidth - 20) * ((float) rand() / RAND_MAX);
					y[i] = (gLvlHeight - 20) * ((float) rand() / RAND_MAX);
					if (i % (gFruitsQuantity / gPowerUpsQuantity) == 0) {
						gSpriteNum[i] = (int) (30 - (5 * ((float) rand() / RAND_MAX)));
					} else {
						gSpriteNum[i] = (int) ((TOTAL_FRUIT_SPRITES - 5) * ((float) rand() / RAND_MAX));
					}
				}
				gEnemyQuantity = gSettingsFileContent[1];
				gAngle.clear();
				gCurrentTime.clear();
				gTimeElapsed.clear();
				gEnemySprite.clear();
				gEnemyStartPos.clear();
				gEnemy.clear();
				gEnemyToTargetDistance[0].clear();
				gEnemyToTargetDistance[1].clear();
				for (int i = 0; i < gEnemyQuantity; i++) {
					gAngle.push_back(0.0);
					gCurrentTime.push_back(0);
					gTimeElapsed.push_back(0);
					gEnemySprite.push_back(0);
					gEnemyStartPos.push_back( { 0, 0 });
					gEnemy.push_back(Snake());
					gEnemyToTargetDistance[0].push_back(0);
					gEnemyToTargetDistance[1].push_back(0);
				}
				for (int i = 0; i < gEnemyQuantity; i++) {
					gEnemyStartPos[i].x = 100 + 0.5 * gLvlWidth * ((double) rand() / RAND_MAX);
					gEnemyStartPos[i].y = 100 + 0.5 * gLvlHeight * ((double) rand() / RAND_MAX);
					gEnemySprite[i] = (int) (TOTAL_SPRITES * ((float) rand() / RAND_MAX));
					gEnemy[i].setStartPos(gEnemyStartPos[i].x, gEnemyStartPos[i].y);
					//		gEnemy[i].resetLength();
				}
				gGameState = 1;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU):
				gSettingsFile.open("./assets/settings.txt", ios::in);
				if (gSettingsFile.is_open()) {
					gPathToLangFile.str("");
					gPathToLangFile << "./assets/lang/";
					{
						int i = 0;
						while (getline(gSettingsFile, gTmpText, '/')) {
							gSettingsFile.ignore(1024, '\n');
							gSettingsFileContent[i] = stoi(gTmpText.c_str());
							i++;
						}
					}
					gPathToLangFile << gLangList[gSettingsFileContent[0]].c_str() << ".txt";
					gLangFile.open(gPathToLangFile.str().c_str(), ios::in);
					if (gLangFile.is_open()) {
						unsigned int i = 0;
						while (i < sizeof(gAllTexts) / sizeof(string)) {
							getline(gLangFile, gAllTexts[i]);
							i++;
						}
						gSettingsFile.close();
						gLangFile.close();
					} else {
						gSettingsFile.close();
						gLangFile.close();
						cout << "Unable to load translations from " << gPathToLangFile.str().c_str() << "!\n";
						gContinue = false;
					}
					gSettingsFile.close();
					gLangFile.close();
				} else {
					gSettingsFile.close();
					gLangFile.close();
					cout << "Unable to load translations from " << gPathToLangFile.str().c_str() << "!\n";
					cout << "Unable to load settings file!\n";
					gContinue = false;
				}
				gGameState = 1;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 1):
				gSettingsFileContent[0]--;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 1 + 5):
				gSettingsFileContent[0]++;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 2):
				gSettingsFileContent[1]--;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 2 + 5):
				gSettingsFileContent[1]++;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 3):
				gSettingsFileContent[2]--;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 3 + 5):
				gSettingsFileContent[2]++;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 4):
				gSettingsFileContent[3]--;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 4 + 5):
				gSettingsFileContent[3]++;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 5):
				gSettingsFileContent[4]--;
				break;
			case (TOTAL_NUMBER_OF_BUTTONS + POSITIONS_IN_OPTIONS_MENU + 5 + 5):
				gSettingsFileContent[4]++;
				break;
		}
	}
}
