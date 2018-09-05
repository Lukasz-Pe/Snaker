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

int const TOTAL_POWERUPS = 5, MAX_POWERUP_TIME[TOTAL_POWERUPS] = { 20, 20, 20, 20, 20 }, gLvlWidth = 10000 / 2, gLvlHeight = gLvlWidth;
int gPowerUpsQuantity, gFruitsQuantity, gPlayerQuantity;

const double POWERUP_SCALE = 2, SCREEN_SCALE = 0.5;
bool initSDL(Win *window = NULL);
void close(Win *window = NULL);
void activatePowerup(int &fruitSpriteNum, Snake &vSnake);
void handleEvents();
void print();
bool escapeFromDanger(Snake &vSnake1, Snake &vSnake2, double &vAngle);
void betweenPlayersCollisions();
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
//void gameReset(bool &reset);

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

	gPlayerQuantity = gSettingsFileContent[1];
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
		for (int i = 0; i <= gPlayerQuantity; i++) {
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
	for (unsigned int i = 0; i < gPlayer.size(); i++) {
		gPlayerStartPos[i].x = static_cast<int>(gLvlWidth * (static_cast<double>(rand()) / RAND_MAX));
		gPlayerStartPos[i].y = static_cast<int>(gLvlHeight * (static_cast<double>(rand()) / RAND_MAX));
		gPlayerSprite[i] = static_cast<int>(TOTAL_SPRITES * ((float) rand() / RAND_MAX));
		gPlayer[i].setStartPos(gPlayerStartPos[i].x, gPlayerStartPos[i].y);
	}
	gWindow.setTitle("Snaker");
	gRenderer = gWindow.getRenderer();
//	PLACING FRUITS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for (int i = 0; i < gFruitsQuantity; i++) {
		x[i] = (gLvlWidth - 20) * ((float) rand() / RAND_MAX);
		y[i] = (gLvlHeight - 20) * ((float) rand() / RAND_MAX);
		if (i % (gFruitsQuantity / gPowerUpsQuantity) == 0) {
			gSpriteNum[i] = (int) (30 - (5 * ((float) rand() / RAND_MAX)));
		} else {
			gSpriteNum[i] = (int) ((TOTAL_FRUIT_SPRITES - 5) * ((float) rand() / RAND_MAX));
		}
	}
//	BACKGROUND POSITIONING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int tilePosX = 0, tilePosY = 0;
	for (int i = 0; i < gTotalTiles; i++) {
		tileSet[i] = new Tile(tilePosX, tilePosY, gLTLevelTexture.getWidth(), gLTLevelTexture.getHeight());
		tilePosX += gLTLevelTexture.getWidth();
		if (tilePosX >= 1.5 * gLvlWidth) {
			tilePosX = 0;
			tilePosY += gLTLevelTexture.getHeight();
		}
	}
	tilePosX = 0;
	tilePosY = 0;
	for (int i = 0; i < gTotalMenuTiles; i++) {
		gMenuBackground[i] = new Tile(tilePosX, tilePosY, gLTMenuBackground.getWidth(), gLTMenuBackground.getHeight());
		tilePosX += gLTMenuBackground.getWidth();
		if (tilePosX >= 1.5 * gScreenWidth) {
			tilePosX = 0;
			tilePosY += gLTMenuBackground.getHeight();
		}
	}
//TEXT POSITIONING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	gBoxExitQuestion.x = ((gWindow.getWidth() - gLTExitQuestion.getWidth()) / 2);
	gBoxExitQuestion.y = (gWindow.getHeight() - gBoxExitQuestion.h) / 2;
	gBoxExitQuestion.h = 1.25 * gLTExitQuestion.getHeight() + gButtons[4].getButtonDims().h;
	gBoxExitQuestion.w = gLTExitQuestion.getWidth();
	gBoxPause.x = (gWindow.getWidth() - gLTPause.getWidth()) / 2;
	gBoxPause.y = (gWindow.getHeight() - gLTPause.getHeight()) / 2;
	gBoxPause.w = gLTPause.getWidth();
	gBoxPause.h = gLTPause.getHeight();
	gGameOverBox.x = (gWindow.getWidth() - gLTPressToReset.getWidth()) / 2;
	gGameOverBox.y = (gWindow.getHeight() - gLTGameOver.getHeight()) / 2;
	gGameOverBox.w = gLTPressToReset.getWidth();
	gGameOverBox.h = 1.1 * (gLTGameOver.getHeight() + gLTPressToReset.getHeight());
	stepTimer.start();
	for (int i = 0; i <= gPlayerQuantity; i++) {
		gPlayerToTargetDistance[0][i] = gLvlWidth * gLvlHeight;
		gPlayerToTargetDistance[1][i] = i;
	}
	for (int i = 1; i < 5; i++) {
		gChangeableOptionsPos[i] = to_string(gSettingsFileContent[i]);
	}
	gTmpVal = gSettingsFileContent[4];
	for (int i = 0; i <= gPlayerQuantity; i++) {
		gPlayer[i].changeSpeed(gSettingsFileContent[4]);
	}
// MAIN GAME LOOP >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	while (gContinue) {
//		EVENTS HANDLER +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		handleEvents();
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
		gChangeableOptionsPos[0] = gLangList[gSettingsFileContent[0]];
		for (int i = 1; i < 5; i++) {
			gChangeableOptionsPos[i] = to_string(gSettingsFileContent[i]);
		}
		if (gTmpVal != gSettingsFileContent[4]) {
			gTmpVal = gSettingsFileContent[4];
			for (int i = 0; i <= gPlayerQuantity; i++) {
				gPlayer[i].changeSpeed(gSettingsFileContent[4]);
			}
		}
//		ACTIONS TAKEN WHEN WINDOW DIMS ARE CHANGED >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		if (gScreenWidth != gWindow.getWidth() || gScreenHeight != gWindow.getHeight()) {
			gScreenWidth = gWindow.getWidth();
			gScreenHeight = gWindow.getHeight();
			gCamera.w = gScreenWidth;
			gCamera.h = gScreenHeight;
			gLevelBorders = {0, 0, gLvlWidth, gLvlHeight};
			gBoxPause.x = (gWindow.getWidth() - gLTPause.getWidth()) / 2;
			gBoxPause.y = (gWindow.getHeight() - gLTPause.getHeight()) / 2;
			gBoxExitQuestion.x = ((gWindow.getWidth() - gLTExitQuestion.getWidth()) / 2);
			gBoxExitQuestion.y = (gWindow.getHeight() - gBoxExitQuestion.h) / 2;
			gGameOverBox.x = (gWindow.getWidth() - gLTPressToReset.getWidth()) / 2;
			gGameOverBox.y = (gWindow.getHeight() - gLTGameOver.getHeight()) / 2;
			tilePosX = 0;
			tilePosY = 0;
			for (int i = 0; i < gTotalTiles; i++) {
				tileSet[i]->setNewPos(tilePosX, tilePosY);
				tilePosX += gLTLevelTexture.getWidth();
				if (tilePosX >= gLvlWidth) {
					tilePosX = 0;
					tilePosY += gLTLevelTexture.getHeight();
				}
			}
			tilePosX = 0;
			tilePosY = 0;
			for (int i = 0; i < gTotalMenuTiles; i++) {
				gMenuBackground[i]->setNewPos(tilePosX, tilePosY);
				tilePosX += gLTMenuBackground.getWidth();
				if (tilePosX >= gScreenWidth) {
					tilePosX = 0;
					tilePosY += gLTMenuBackground.getHeight();
				}
			}
			for (int i = 0; i < gFruitsQuantity; i++) {
				x[i] = (gLvlWidth - 20) * ((float) rand() / RAND_MAX);
				y[i] = (gLvlHeight - 20) * ((float) rand() / RAND_MAX);
			}
		}
//		HERE IS GAME MENU ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (gGameState >= 1) {
			if (gGameState != 3) {
				frameTimer.stop();
			}
			if (gRenderScaleX < 1 && gRenderScaleY < 1) {
				tempScale = gRenderScaleX;
				gRenderScaleX = 1.0;
				gRenderScaleY = gRenderScaleX;
				gLevelBorders = {0, 0, gLvlWidth, gLvlHeight};
				SDL_RenderSetScale(gRenderer, gRenderScaleX, gRenderScaleY);
			}
			gWindow.prepareRenderer(0, 0, 0);
//			GETTING MENU BACKGROUND ON SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 0; i < gTotalMenuTiles; i++) {
				gMenuBackground[i]->render(gMenuCamera, gWindow, gLTMenuBackground);
			}
			gLTTitleText.render((gWindow.getWidth() - gLTTitleText.getWidth()) / 2, 0, gWindow);
			switch (gGameState) {
//				QUIT DIALOGUE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 2:
					SDL_RenderFillRect(gRenderer, &gBoxExitQuestion);
					gLTExitQuestion.render(gBoxExitQuestion.x, gBoxExitQuestion.y, gWindow);
					gButtons[MAIN_MENU_OPTS].render(gBoxExitQuestion.x, gBoxExitQuestion.y + 1.25 * gLTExitQuestion.getHeight(), gWindow);
					gButtons[MAIN_MENU_OPTS + 1].render(gBoxExitQuestion.x + gLTExitQuestion.getWidth() - gButtons[3].getButtonDims().w, gBoxExitQuestion.y + 1.25 * gLTExitQuestion.getHeight(), gWindow);
					break;
//					PAUSE DIALOGUE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 3:
					frameTimer.pause();
					SDL_RenderFillRect(gRenderer, &gBoxPause);
					gLTPause.render(gBoxPause.x, gBoxPause.y, gWindow);
					break;
//					GAME OVER DIALOGUE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 4:
					SDL_RenderFillRect(gRenderer, &gGameOverBox);
					gLTGameOver.render(gGameOverBox.x, gGameOverBox.y, gWindow);
					gLTPressToReset.render(gGameOverBox.x + gGameOverBox.w - gLTPressToReset.getWidth(), gGameOverBox.y + gGameOverBox.h - gLTPressToReset.getHeight(), gWindow);
					break;
//					OPTIONS MENU >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 5:
					gLTOptionsText.render((gWindow.getWidth() - gLTOptionsText.getWidth()) / 2, 1.25 * gLTTitleText.getHeight(), gWindow);
					for (int i = 0; i < (POSITIONS_IN_OPTIONS_MENU - 2); i++) {
						gOptionButtons[i].render((gLTOptionsText.getPosX() + 0.5 * gLTOptionsText.getWidth() - mOptsButtonsWidth[i] / 2), gWindow.getHeight() / 3 + i * gOptionButtons[i].getButtonDims().h, gWindow);
						fTextToShow(i, gOptionButtons[i].getButtonDims().x + gOptionButtons[i].getButtonDims().w + 80, gOptionButtons[i].getButtonDims().y, gChangeableOptionsPos[i]);
					}
					gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 2].render((gWindow.getWidth() - (gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 2].getButtonDims().w + 100 + gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 1].getButtonDims().w)) / 2, gWindow.getHeight() / 3 + (POSITIONS_IN_OPTIONS_MENU - 2) * gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 2].getButtonDims().h, gWindow);
					gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 1].render(((gWindow.getWidth() - (gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 2].getButtonDims().w + 100 + gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 1].getButtonDims().w)) / 2) + (gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 2].getButtonDims().w + 100), gWindow.getHeight() / 3 + (POSITIONS_IN_OPTIONS_MENU - 2) * gOptionButtons[POSITIONS_IN_OPTIONS_MENU - 2].getButtonDims().h, gWindow);

					break;
//					MAIN MENU >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				default:
					for (int i = 0; i < MAIN_MENU_OPTS; i++) {
						gButtons[i].render((gWindow.getWidth() - gButtons[i].getButtonDims().w) / 2, gWindow.getHeight() / 3 + i * gButtons[i].getButtonDims().h, gWindow);
					}
					break;
			}
			gWindow.render();
		}
//		HERE IS END OF GAME MENU AND START OF GAME LOGIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (gGameState == 0) {
			gTimer = stepTimer.getSeconds<int>();
			if (tempScale < 1) {
				gRenderScaleX = tempScale;
				gRenderScaleY = tempScale;
				SDL_RenderSetScale(gRenderer, gRenderScaleX, gRenderScaleY);
				//					gLvlWidth = MULTIPLIER * (int) (gScreenWidth / gRenderScaleX);
				//					gLvlHeight = MULTIPLIER * (int) (gScreenHeight / gRenderScaleY);
				//					gLvlWidth += 50;
				//					gLvlHeight += 50;
				gLevelBorders = {0, 0, gLvlWidth, gLvlHeight};
				//					gLvlWidth -= 50;
				//					gLvlHeight -= 50;
			}
//			RESETTING AFTER CHOOSING OPTION IN MENU OR INGAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			gameReset(gReset);
//			PREPARING FOR RENDER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			gWindow.prepareRenderer(0, 0, 0);
			gPlayer[0].setCamera(gLvlWidth, gLvlHeight, gCamera);
//			RENDERING OF BACKGROUND >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 0; i < gTotalTiles; i++) {
				tileSet[i]->render(gCamera, gWindow, gLTLevelTexture);
			}
//			SETTING STARTING POS OF FRUITS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 0; i < gFruitsQuantity; i++) {
				if (gSpriteNum[i] < 25) {
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]]);
				} else {
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]], &POWERUP_SCALE);
				}
			}

//			COLLISIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//			COLLISIONS WITH FRUITS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			fruitCollisions();
//			COLLISIONS WITH LEVEL EDGES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 1; i < gPlayerQuantity; i++) {
				if (gPlayer[i].getHeadBox().x < 1 || gPlayer[i].getHeadBox().y < 1 || (gPlayer[i].getHeadBox().w + gPlayer[i].getHeadBox().x) >= (gLvlWidth) || (gPlayer[i].getHeadBox().h + gPlayer[i].getHeadBox().y) >= (gLvlHeight)) {
					gAngle[i] = 360 * ((double) rand() / RAND_MAX);
				}
			}
			gCollision = false;
//			COLLISIONS BETWEEN PLAYERS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			betweenPlayersCollisions();
//			GETTING POINT/LENGTH ON SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			if (gCurrentScore != gPlayer[0].getLength() || gCurrentScore >= 0) {
				gScore.str("");
				gScore << " Length: " << gPlayer[0].getLength();
				gLTScoreText.loadFromText(gScore.str().c_str(), gTextColor, gFont, gWindow);
				gCurrentScore = gPlayer[0].getLength();
				gLTScoreText.setWidth(3 * gLTScoreText.getWidth());
				gLTScoreText.setHeight(TEXT_SIZE);
			}
//			MOVEMENT AND POSITION CALC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			gFrameTimer = frameTimer.getSeconds<double>();
			gPlayer[0].move(gLevelBorders, gLTSnakeTail, &gFrameTimer);
			for (int i = 1; i <= gPlayerQuantity; i++) {
				gPlayer[i].setAngle(gAngle[i]);
				gPlayer[i].move(gLevelBorders, gLTSnakeTail, &gFrameTimer);
			}
			frameTimer.start();
//			POWEUPS MANAGEMENT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			powerupCheck(gPlayer[0], true);
			for (int i = 1; i <= gPlayerQuantity; i++) {
				powerupCheck(gPlayer[i]);
			}
//			RENDERING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			for (int i = 1; i <= gPlayerQuantity; i++) {
				gPlayer[i].render(gWindow, gLTEnemyHead, gLTEnemyTail, gCamera, &gSpriteClips[gPlayerSprite[i]]);
			}

			gPlayer[0].render(gWindow, gLTSnakeHead, gLTSnakeTail, gCamera);
			gLTScoreText.render(0, 0, gWindow);
			gWindow.render();
//		END OF GAME LOGIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}
// END OF MAIN GAME LOOP >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	}
	return 0;
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
		gPlayer[0].eventHandler(event, &gWindow, &gCamera);
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
				gPlayerQuantity = gSettingsFileContent[1];
				gAngle.clear();
				gCurrentTime.clear();
				gTimeElapsed.clear();
				gPlayerSprite.clear();
				gPlayerStartPos.clear();
				gPlayer.clear();
				gPlayerToTargetDistance[0].clear();
				gPlayerToTargetDistance[1].clear();
				for (int i = 0; i < gPlayerQuantity; i++) {
					gAngle.push_back(0.0);
					gCurrentTime.push_back(0);
					gTimeElapsed.push_back(0);
					gPlayerSprite.push_back(0);
					gPlayerStartPos.push_back( { 0, 0 });
					gPlayer.push_back(Snake());
					gPlayerToTargetDistance[0].push_back(0);
					gPlayerToTargetDistance[1].push_back(0);
				}
				for (unsigned int i = 0; i < gPlayer.size(); i++) {
					gPlayerStartPos[i].x = static_cast<int>(gLvlWidth * (static_cast<double>(rand()) / RAND_MAX));
					gPlayerStartPos[i].y = static_cast<int>(gLvlHeight * (static_cast<double>(rand()) / RAND_MAX));
					gPlayer[i].setStartPos(gPlayerStartPos[i].x, gPlayerStartPos[i].y);
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
// OPTIONS MENU STEERING +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void fTextToShow(int &i, int xpos, int ypos, string &vText) {
	gTextToShow.str("");
	gTextToShow << vText.c_str();
	gLTPosTxt.loadFromText(gTextToShow.str().c_str(), gTextNormalColor, gFont, gWindow);
	gLTPosTxt.setHeight(gButtonNextPrev[0].getButtonDims().h);
	gLTPosTxt.setWidth(1 * gLTPosTxt.getWidth());
	gButtonNextPrev[i].render(xpos, ypos, gWindow);
	gLTPosTxt.render(xpos + (1.25 * gButtonNextPrev[0].getButtonDims().w), ypos, gWindow);
	gButtonNextPrev[i + 5].render(xpos + gButtonNextPrev[i].getButtonDims().w + 10 + gLTPosTxt.getWidth() + 10, ypos, gWindow);
	mOptsButtonsWidth[i] = gOptionButtons[i].getButtonDims().w + 80 + gButtonNextPrev[i].getButtonDims().w + 10 + gLTPosTxt.getWidth() + 10 + gButtonNextPrev[i + 4].getButtonDims().w;
}
// COLLISION CHECKING +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool checkCollision(SDL_Rect a, SDL_Rect b) {
//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

//If any of the sides from A are outside of B
	if (bottomA <= topB) {
		return false;
	}

	if (topA >= bottomB) {
		return false;
	}

	if (rightA <= leftB) {
		return false;
	}

	if (leftA >= rightB) {
		return false;
	}

//If none of the sides from A are outside B
	return true;
}
// SDL INITIALIZZATION +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool initSDL(Win *window) {
	if (window == NULL) {
		cout << "Window not set!\n";
		return false;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning: Linear texture filtering not enabled!");
		return false;
	}
	if (SDLNet_Init() < 0) {
		std::cout << "SDLNet_Init: %s\n" << SDLNet_GetError() << std::endl;
	}
	window->setWidth(gScreenWidth);
	window->setHeight(gScreenHeight);
	if (!window->init()) {
		printf("Failed to properly initialize Window object.");
		return false;
	}

	return true;
}
// COLLISIONS WITH FRUITS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void fruitCollisions() {

	for (int i = 0; i <= gPlayerQuantity; i++) {
		//		cout << "BFR gPlayerToTargetDistance[1][" << i << "] " << gPlayerToTargetDistance[1][i] << "\tgPlayerToTargetDistance[0][" << i << "] " << gPlayerToTargetDistance[0][i] << endl;
		gPlayerToTargetDistance[0][i] = gLvlWidth * gLvlHeight;
		gPlayerToTargetDistance[1][i] = -1;
		//		cout<<"AFR gPlayerToTargetDistance[1]["<<i<<"] "<<EnemyToTargetDistance[1][i]<<"\tgPlayerToTargetDistance[0]["<<i<<"] "<<gPlayerToTargetDistance[0][i]<<endl;
	}
	for (int i = 0; i < gFruitsQuantity; i++) {
		for (int e = 0; e <= gPlayerQuantity; e++) {
			if (gPlayer[e].collectFruit(gFruit[i])) {
				x[i] = gPlayer[e].mNewFruitPos.x;
				y[i] = gPlayer[e].mNewFruitPos.y;
			}
		}
		for (int j = 0; j <= gPlayerQuantity; j++) {
			gCollision = checkCollision(gFruit[i].getRect(), gPlayer[j].getHeadBox());
			if (gPlayer[j].hasActivePowerup[0] && (gPlayer[0].hasActivePowerup[0] || gPlayer[0].hasActivePowerup[1] || gPlayer[0].hasActivePowerup[3])) {
				gPlayer[j].hasActivePowerup[0] = false;
			}
			if (gPlayer[j].hasActivePowerup[0] && j != 0) {
				gPlayerToTargetDistance[0][j] = gPlayer[j].getSnakeFruitDistance(gPlayer[0].mHeadBox);
				gPlayerToTargetDistance[1][j] = -1;
				for (int e = 0; e <= gPlayerQuantity; e++) {
					if ((gPlayerToTargetDistance[0][j] > gPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox)) && j != e) {
						gPlayerToTargetDistance[1][j] = e;
					}
				}
				if (gPlayerQuantity > 0) {
					for (int e = 1; e <= gPlayerQuantity; e++) {
						//					cout<<"gPlayerToTargetDistance[0][j]"<<gPlayerToTargetDistance[0][j]<<"\tgPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox) "<<gPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox)<<endl;
						if ((gPlayerToTargetDistance[0][j] < gPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox)) && j != e) {
							gAngle[j] = gPlayer[j].getHeadToFruitAngle(gPlayer[0].mHeadBox);
						} else {
							gAngle[j] = gPlayer[j].getHeadToFruitAngle(gPlayer[e].mHeadBox);
						}
						//					cout << "AFC gPlayerToTargetDistance[1][" << e << "] " << gPlayerToTargetDistance[1][e] << "\tgPlayerToTargetDistance[0][" << e << "] " << gPlayerToTargetDistance[0][e] <<"\tgAngle["<<j<<"] "<<gAngle[j]<< endl;
					}
				} else {
					gAngle[j] = gPlayer[j].getHeadToFruitAngle(gPlayer[0].mHeadBox);
				}

				break;
			}
			if (gCollision) {
				//				cout << "gPlayerToTargetDistance[0][j] " << gPlayerToTargetDistance[0][j] << "\tgPlayerToTargetDistence[1][j] " << gPlayerToTargetDistance[1][j] << endl;
				x[i] = (gLvlWidth - gFruit[i].getRect().w) * ((float) rand() / RAND_MAX);
				y[i] = (gLvlHeight - gFruit[i].getRect().h) * ((float) rand() / RAND_MAX);
				if (gSpriteNum[i] < 25) {
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]]);
				} else {
					//					cout<<gSpriteNum[i]<<endl;
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]], &POWERUP_SCALE);
				}
				//				gAngle[j] = gPlayer[j].getHeadToFruitAngle(gFruit[gPlayerToTargetDistence[1][j]].mBox);
				//				cout << "j " << j << "\t " << gPlayerToTargetDistence[1][j] << endl;
				activatePowerup(gSpriteNum[i], gPlayer[j]);
				if (gSpriteNum[i] < 25) {
					gPlayer[j].addLength();
				}
				gAngle[j] = gPlayer[j].getHeadToFruitAngle(gFruit[gPlayerToTargetDistance[1][j]].mBox);
			}
		}
	}
}
// POWERUP ACTIVATION +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void activatePowerup(int &fruitSpriteNum, Snake &vSnake) {
	switch (fruitSpriteNum) {
		case 25:
			vSnake.powerupActivationTimestamp[0] = gTimer + MAX_POWERUP_TIME[0];
			vSnake.hasActivePowerup[0] = true;
//			cout << vSnake.hasActivePowerup[0] << "\t" << vSnake.powerupActivationTimestamp[0] << "\tCurrent time: " << gTimer << endl;
			break;
		case 26:
			vSnake.powerupActivationTimestamp[1] = gTimer + MAX_POWERUP_TIME[1];
			vSnake.hasActivePowerup[1] = true;
			break;
		case 27:
			vSnake.powerupActivationTimestamp[2] = gTimer + MAX_POWERUP_TIME[2];
			vSnake.hasActivePowerup[2] = true;
			vSnake.collectDistanceMultiplier(16);
			vSnake.setCollectAngle(60);
			break;
		case 28:
			vSnake.powerupActivationTimestamp[3] = gTimer + MAX_POWERUP_TIME[3];
			vSnake.hasActivePowerup[3] = true;
			break;
		case 29:
			vSnake.powerupActivationTimestamp[4] = gTimer + MAX_POWERUP_TIME[4];
			vSnake.hasActivePowerup[4] = true;
			break;
	}
}
// POWERUP CHECKING +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void powerupCheck(Snake &vSnake, bool render) {

	for (int i = 0; i < TOTAL_POWERUPS; i++) {
//		cout<<i<<" "<<vSnake.powerupActivationTimestamp[i]<<endl;
		if (vSnake.powerupActivationTimestamp[i] < gTimer) {
			vSnake.hasActivePowerup[i] = false;
		}
		if (vSnake.hasActivePowerup[i]) {
			if (!render) {
				break;
			}
			int iconPosX = (int) ((gWindow.getWidth() - 50) / gRenderScaleX);
			switch (i) {
				case 0:
					gTimeLeft.str("");
					if (((vSnake.powerupActivationTimestamp[i] - gTimer) < 10) && ((vSnake.powerupActivationTimestamp[i] - gTimer) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << vSnake.powerupActivationTimestamp[i] - gTimer;
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(iconPosX, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(iconPosX - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 1:
					gTimeLeft.str("");
					if (((vSnake.powerupActivationTimestamp[i] - gTimer) < 10) && ((vSnake.powerupActivationTimestamp[i] - gTimer) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << vSnake.powerupActivationTimestamp[i] - gTimer;
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(iconPosX, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(iconPosX - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 2:
					gTimeLeft.str("");
					if (((vSnake.powerupActivationTimestamp[i] - gTimer) < 10) && ((vSnake.powerupActivationTimestamp[i] - gTimer) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << vSnake.powerupActivationTimestamp[i] - gTimer;
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(iconPosX, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(iconPosX - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 3:
					gTimeLeft.str("");
					if (((vSnake.powerupActivationTimestamp[i] - gTimer) < 10) && ((vSnake.powerupActivationTimestamp[i] - gTimer) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << vSnake.powerupActivationTimestamp[i] - gTimer;
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(iconPosX, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(iconPosX - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 4:
					gTimeLeft.str("");
					if (((vSnake.powerupActivationTimestamp[i] - gTimer) < 10) && ((vSnake.powerupActivationTimestamp[i] - gTimer) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << vSnake.powerupActivationTimestamp[i] - gTimer;
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(iconPosX, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(iconPosX - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
			}
		} else {
			switch (i) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					vSnake.collectDistanceMultiplier(4);
					vSnake.setCollectAngle(120);
					break;
				case 3:
					break;
				case 4:
					if (render) {
						SDL_RenderSetScale(gRenderer, 1, 1);
						SDL_RenderGetScale(gRenderer, &gRenderScaleX, &gRenderScaleY);
					}
					break;
			}
		}
	}
}
// GAME RESETTING +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void gameReset(bool &reset) {
	if (reset) {
		for (int p = 0; p < TOTAL_POWERUPS; p++) {
			for (int i = 0; i <= gPlayerQuantity; i++) {
				gPlayer[i].hasActivePowerup[p] = false;
//			gSnake.powerupActivationTimestamp[p] = gTime;
			}
		}
//		gPlayer[0].resetLength();
//		gPlayer[0].setStartPos(0.85 * gLvlWidth * (((double) rand() / RAND_MAX)), 0.85 * gLvlHeight * (((double) rand() / RAND_MAX)));
		for (int i = 0; i <= gPlayerQuantity; i++) {
			gPlayer[i].setStartPos(gPlayerStartPos[i].x, gPlayerStartPos[i].y);
			gPlayer[i].resetLength();
			for (int p = 0; p < TOTAL_POWERUPS; p++) {
				gPlayer[i].hasActivePowerup[p] = false;
//				gEnemy[i].powerupActivationTimestamp[p] = gTime;
			}
		}
		reset = false;

	}
}
// COLLISIONS BETWEEN PLAYERS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void betweenPlayersCollisions() {
	for (int i = 0; i <= gPlayerQuantity; i++) {
		for (int j = 0; j <= gPlayerQuantity; j++) {
			if (i != j) {

				if (gPlayer[i].hasActivePowerup[1] || gPlayer[j].hasActivePowerup[1]) {
					escapeFromDanger(gPlayer[i], gPlayer[j], gAngle[i]);
				}
				gCollision = checkCollision(gPlayer[i].getHeadBox(), gPlayer[j].getHeadBox());
				if (gPlayer[i].hasActivePowerup[1] || gPlayer[j].hasActivePowerup[1]) { //GHOST ON ****************
					gCollision = false;
				}
				if (gCollision) {
					if (gPlayer[i].hasActivePowerup[0]) {
						for (int et = 0; et < gPlayer[j].getLength(); et++) {
							gPlayer[i].addLength();
							gPlayer[i].updateTail(gLTEnemyTail);
						}
						gPlayer[j].resetLength();
						break;
					}
					if (gPlayer[j].hasActivePowerup[0]) {
						for (int et = 0; et < gPlayer[i].getLength(); et++) {
							gPlayer[j].addLength();
							gPlayer[j].updateTail(gLTEnemyTail);
						}
						gPlayer[i].resetLength();
						break;
					}
					gAngle[i] = 360 * ((double) rand() / RAND_MAX);
					if (gPlayer[i].hasActivePowerup[3]) { //SHIELD ON ****************
						break;
					}
					gPlayer[i].resetLength();
					if (i == 0 && gGameState == 0) {
						gGameState = 4;
					}
					//						gPlayer[i].setStartPos(gPlayer[i].getHeadBox().x+gWindow.getWidth()+10, gPlayer[i].getHeadBox().y+gWindow.getHeight()+10);
				}
				if (gPlayer[i].getLength() != 0 && i != j) {
					for (int t = 0; t < gPlayer[i].getLength(); t++) {
						gCollision = checkCollision(gPlayer[i].getTailBox(t), gPlayer[j].getHeadBox());
						if (gPlayer[j].hasActivePowerup[1]) { //GHOST ON ****************
							gCollision = false;
						}
						if (gCollision) {
							if (gPlayer[j].hasActivePowerup[0]) {
								for (int et = 0; et < gPlayer[i].getLength(); et++) {
									gPlayer[j].addLength();
									gPlayer[j].updateTail(gLTEnemyTail);
								}
								gPlayer[i].resetLength();
								break;
							}
							gAngle[j] = 360 * ((double) rand() / RAND_MAX);
							if (gPlayer[i].hasActivePowerup[3]) { //SHIELD ON ****************
								break;
							}
							gPlayer[j].resetLength();
							//								gPlayer[i].setStartPos(gPlayer[i].getHeadBox().x+gWindow.getWidth()+10, gPlayer[i].getHeadBox().y+gWindow.getHeight()+10);
						}
					}
				}
			}
		}
	}
}
//	ESCAPE FROM DANGER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool escapeFromDanger(Snake &vSnake1, Snake &vSnake2, double &vAngle) {
	if ((vSnake1.getSnakeFruitDistance(vSnake2.mHeadBox) < (vSnake1.mHeadBox.w * 20)) && (vSnake2.hasActivePowerup[0]) && !vSnake1.hasActivePowerup[1] && !vSnake1.hasActivePowerup[3]) {
		if (vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) >= 0 || vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) < 90) {
			vAngle = vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) + 90 + 180 * ((double) rand() / RAND_MAX);
			return true;
		} else if (vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) >= 90 || vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) < 180) {
			vAngle = vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) + 90 + 180 * ((double) rand() / RAND_MAX);
			return true;
		} else if (vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) >= 180 || vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) < 270) {
			vAngle = vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) + 90 + 180 * ((double) rand() / RAND_MAX);
			return true;
		} else {
			vAngle = vSnake1.getHeadToFruitAngle(vSnake2.mHeadBox) + 90 + 180 * ((double) rand() / RAND_MAX);
			return true;
		}
	}
	return false;
}
