/*
 * main.cpp
 *
 *  Created on: 03.07.2018
 *      Author: lpekalski
 */

#include "main.h"
#include "classes/Win.h"
#include "classes/LTexture.h"
#include "classes/Snake.h"
#include "classes/Dot.h"
#include "classes/Tile.h"
#include "classes/Menu.h"
#include "classes/Timer.h"
int const MULTIPLIER = 5, TOTAL_FRUITS = 600, ENEMY_COUNT = 10, TOTAL_POWERUPS = 5, POWERUPS_COUNT = 24, MAX_POWERUP_TIME[TOTAL_POWERUPS] = { 60, 30, 20, 60, 30 };
const double POWERUP_SCALE = 2;
bool initSDL(Win *window = NULL);
void close(Win *window = NULL);
void activatePowerup(int &fruitSpriteNum, Snake &vSnake);
void handleEvents();
void print();
//----------------------------------------------------------------Deklaracje zmiennych
bool gContinue = true, gCollision = false, gReset = false;
int const *pTOTAL_TILES = NULL, TOTAL_NUMBER_OF_BUTTONS = 5, TEXT_SIZE = 50, TITLE_TEXT_SIZE = 150, MAIN_MENU_OPTS = TOTAL_NUMBER_OF_BUTTONS - 2;
double gAngle[ENEMY_COUNT], timeStep;
int gScreenWidth = 1024, gScreenHeight = 768, gLvlWidth = MULTIPLIER * gScreenWidth, gLvlHeight = MULTIPLIER * gScreenHeight, gCurrentScore = 0, gOption = -1, gGameState = 1, gCurrentTime[ENEMY_COUNT], gTimeElapsed[ENEMY_COUNT], gSpriteNum[TOTAL_FRUITS], gEnemySprite[ENEMY_COUNT];
const int TOTAL_SPRITES = 25, TOTAL_FRUIT_SPRITES = 30, SPRITE_DIMS = 20, POWERUP_ICON_DIMS = 50;
SDL_Point gEnemyStartPos[ENEMY_COUNT], gEnemy1Pos, gEnemy2Pos, gSnakePos;
stringstream gScore, gTimeLeft;
string gMenuItems[TOTAL_NUMBER_OF_BUTTONS] = { "Start :D (s)", "Reset game (r)", "Quit :( (q/ESC)", "I have to :( (y)", "Maybe not :) (n/ESC)" };
SDL_Renderer *gRenderer = NULL;
SDL_Event event;
Win gWindow;
LTexture gLTFruit, gLTSnakeHead, gLTSnakeTail, gLTLevelTexture, gLTScoreText, gLTTitleText, gLTExitQuestion, gLTMenuBackground, gLTPause, gLTEnemyHead, gLTEnemyTail, gLTGameOver, gLTPressToReset, gLTPowerupIcons, gLTPowerupsTimeText[TOTAL_POWERUPS];
Snake gSnake, gEnemy[ENEMY_COUNT];
Dot gFruit[TOTAL_FRUITS];
SDL_Rect gCamera = { 0, 0, gScreenWidth, gScreenHeight }, gLevelBorders = { 0, 0, gLvlWidth, gLvlHeight }, gBoxExitQuestion = { 0, 0, 0, 0 }, gBoxPause = { 0, 0, 0, 0 }, gMenuCamera = { 0, 0, gScreenWidth, gScreenHeight }, gMouse = { 0, 0, 1, 1 }, gSpriteClips[TOTAL_SPRITES], gFruitSpriteClips[TOTAL_FRUIT_SPRITES], gGameOverBox = { 0, 0, 0, 0 }, gPowerupClip[TOTAL_POWERUPS];
TTF_Font *gFont = NULL, *gTitleFont = NULL;
SDL_Color gTextColor = { 255, 0, 0 }, gTextPauseColor = { 255, 255, 0 }, gTextNormalColor = { 0, 255, 0 };
Button gButtons[TOTAL_NUMBER_OF_BUTTONS];
Timer stepTimer;
int x[TOTAL_FRUITS], y[TOTAL_FRUITS], Button::mButtonNum = 0, gSpritePosX = 0, gSpritePosY = 0, gFruitSpritePosX = 0, gFruitSpritePosY = 0, gEnePosX[ENEMY_COUNT], gEnePosY[ENEMY_COUNT], powerupTime[TOTAL_POWERUPS];
// POWERUPS PARAMS
void powerupCheck(Snake &vSnake, bool render = false);
void gameReset(bool &reset);
int main(int argc, char* args[]) {
	gContinue = initSDL(&gWindow);

	gContinue = gLTFruit.loadFromFile("./assets/images/fruits.png", gWindow);
	gContinue = gLTSnakeHead.loadFromFile("./assets/images/face.png", gWindow);
	gContinue = gLTSnakeTail.loadFromFile("./assets/images/dot.png", gWindow);
	gContinue = gLTLevelTexture.loadFromFile("./assets/images/bo_play_pattern.png", gWindow);
	gContinue = gLTMenuBackground.loadFromFile("./assets/images/escheresque.png", gWindow);
	gContinue = gLTEnemyHead.loadFromFile("./assets/images/faces.png", gWindow);
	gContinue = gLTEnemyTail.loadFromFile("./assets/images/tails.png", gWindow);
	gContinue = gLTPowerupIcons.loadFromFile("./assets/images/powerups.png", gWindow);
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
	gLTExitQuestion.loadFromText("End of game?", gTextColor, gTitleFont, gWindow);
	gLTExitQuestion.setWidth(1.25 * gLTTitleText.getWidth());
	gLTExitQuestion.setHeight(TEXT_SIZE);
	gLTPause.loadFromText("Game paused", gTextPauseColor, gFont, gWindow);
	gLTPause.setWidth(5 * gLTPause.getWidth());
	gLTGameOver.loadFromText("Game over :(", gTextColor, gTitleFont, gWindow);
	gLTGameOver.setHeight(TITLE_TEXT_SIZE);
	gLTGameOver.setWidth(5 * gLTGameOver.getWidth());
	gLTPressToReset.loadFromText("Press R to restart or Q/ESC to exit", gTextNormalColor, gTitleFont, gWindow);
	gLTPressToReset.setHeight(TEXT_SIZE);
	gLTPressToReset.setWidth(5 * gLTPressToReset.getWidth());

	for (int i = 0; i < TOTAL_NUMBER_OF_BUTTONS; i++) {
		gButtons[i].setButtonText(gMenuItems[i], gWindow, gFont, TEXT_SIZE);
	}

	int gTotalTiles = 15 * (gLvlWidth / gLTLevelTexture.getWidth()) * (gLvlHeight / gLTLevelTexture.getHeight());
	int gTotalMenuTiles = 15 * ((gScreenWidth / gLTMenuBackground.getWidth()) * (gScreenHeight / gLTMenuBackground.getHeight()));
	Tile *tileSet[gTotalTiles], *gMenuBackground[gTotalMenuTiles];
	gSnake.setStartPos(gLvlWidth * (((double) rand() / RAND_MAX))/*LEVEL_WIDTH / 2*/, gLvlHeight * (((double) rand() / RAND_MAX))/*LEVEL_HEIGHT / 2*/);
//	gSnake.resetLength();
	for (int i = 0; i < ENEMY_COUNT; i++) {
		gEnemyStartPos[i].x = 100 + 0.5 * gLvlWidth * ((double) rand() / RAND_MAX);
		gEnemyStartPos[i].y = 100 + 0.5 * gLvlHeight * ((double) rand() / RAND_MAX);
		gEnemySprite[i] = (int) (TOTAL_SPRITES * ((float) rand() / RAND_MAX));
		gEnemy[i].setStartPos(gEnemyStartPos[i].x, gEnemyStartPos[i].y);
//		gEnemy[i].resetLength();
	}
	gWindow.setTitle("Snaker");
	gRenderer = gWindow.getRenderer();
	for (int i = 0; i < TOTAL_FRUITS; i++) {
		x[i] = (gLvlWidth - 20) * ((float) rand() / RAND_MAX);
		y[i] = (gLvlHeight - 20) * ((float) rand() / RAND_MAX);
		if (i % (TOTAL_FRUITS / POWERUPS_COUNT) == 0) {
			gSpriteNum[i] = (int) (30 - (5 * ((float) rand() / RAND_MAX)));
		} else {
			gSpriteNum[i] = (int) ((TOTAL_FRUIT_SPRITES - 5) * ((float) rand() / RAND_MAX));
		}
	}
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
	while (gContinue) {
//		HERE RESIDES GAME MENU +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		handleEvents();
//		ACTIONS TAKEN WHEN WINDOW DIMS ARE CHANGED >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		if (gScreenWidth != gWindow.getWidth() || gScreenHeight != gWindow.getHeight()) {
			gScreenWidth = gWindow.getWidth();
			gScreenHeight = gWindow.getHeight();
//			cout<<gScreenWidth<<"x"<<gScreenHeight<<endl;
			gLvlWidth = MULTIPLIER * gScreenWidth;
			gLvlHeight = MULTIPLIER * gScreenHeight;
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
			for (int i = 0; i < TOTAL_FRUITS; i++) {
				x[i] = (gLvlWidth - 20) * ((float) rand() / RAND_MAX);
				y[i] = (gLvlHeight - 20) * ((float) rand() / RAND_MAX);
			}
		}
//		HERE IS GAME MECHANICS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (gGameState >= 1) {
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
					SDL_RenderFillRect(gRenderer, &gBoxPause);
					gLTPause.render(gBoxPause.x, gBoxPause.y, gWindow);
					break;
//					GAME OVER DIALOGUE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 4:
					SDL_RenderFillRect(gRenderer, &gGameOverBox);
					gLTGameOver.render(gGameOverBox.x, gGameOverBox.y, gWindow);
					gLTPressToReset.render(gGameOverBox.x + gGameOverBox.w - gLTPressToReset.getWidth(), gGameOverBox.y + gGameOverBox.h - gLTPressToReset.getHeight(), gWindow);
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
//		HERE IS GAME MECHANICS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (gGameState == 0) {
//			RESETTING AFTER CHOOSING OPTION IN MENU OR INGAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			gameReset(gReset);
//			PREPARING FOR RENDER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			gWindow.prepareRenderer(0, 0, 0);
			gSnake.setCamera(gLvlWidth, gLvlHeight, gCamera);
//			RENDERING OF BACKGROUND >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 0; i < gTotalTiles; i++) {
				tileSet[i]->render(gCamera, gWindow, gLTLevelTexture);
			}
//			SETTING STARTING POS OF FRUITS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 0; i < TOTAL_FRUITS; i++) {
				if (gSpriteNum[i] < 25) {
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]]);
				} else {
//					cout<<gSpriteNum[i]<<endl;
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]], &POWERUP_SCALE);
				}
			}
//			COLLISIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//			COLLISIONS WITH FRUITS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			fruitCollisions();
//			COLLISIONS WITH LEVEL EDGES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			for (int i = 0; i < ENEMY_COUNT; i++) {
				if (gEnemy[i].getHeadBox().x <= 1 || gEnemy[i].getHeadBox().y <= 1 || (gEnemy[i].getHeadBox().w + gEnemy[i].getHeadBox().x) >= (gLvlWidth) || (gEnemy[i].getHeadBox().h + gEnemy[i].getHeadBox().y) >= (gLvlHeight)) {
					gAngle[i] = 360 * ((double) rand() / RAND_MAX);
				}
			}
			gCollision = false;
//			COLLISIONS BETWEEN BOTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			betweenBotsCollisions();
//			COLLISIONS WITH PLAYER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			playerAndBotsCollisions();
//			GETTING POINT/LENGTH ON SCREEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			if (gCurrentScore != gSnake.getLength() || gCurrentScore >= 0) {
				gScore.str("");
				gScore << " Length: " << gSnake.getLength();
				gLTScoreText.loadFromText(gScore.str().c_str(), gTextColor, gFont, gWindow);
				gCurrentScore = gSnake.getLength();
				gLTScoreText.setWidth(3 * gLTScoreText.getWidth());
				gLTScoreText.setHeight(TEXT_SIZE);
			}
//			MOVEMENT AND POSITION CALC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			gSnake.move(gLevelBorders, gLTSnakeTail);
			for (int i = 0; i < ENEMY_COUNT; i++) {
				gEnemy[i].setAngle(gAngle[i]);
				gEnemy[i].move(gLevelBorders, gLTSnakeTail);
			}
//			POWEUPS MANAGEMENT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			powerupCheck(gSnake, true);
			for (int i = 0; i < ENEMY_COUNT; i++) {
				powerupCheck(gEnemy[i]);
			}
//			RENDERING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			for (int i = 0; i < ENEMY_COUNT; i++) {
				gEnemy[i].render(gWindow, gLTEnemyHead, gLTEnemyTail, gCamera, &gSpriteClips[gEnemySprite[i]]);
			}
			gSnake.render(gWindow, gLTSnakeHead, gLTSnakeTail, gCamera);
			gLTScoreText.render(0, 0, gWindow);
			gWindow.render();
		}
	}

	for (int i = 0; i < gTotalTiles; i++) {
		delete tileSet[i];
	}
	for (int i = 0; i < gTotalMenuTiles; i++) {
		delete gMenuBackground[i];
	}
	close(&gWindow);
	return 0;
}
//HERE ARE OTHER FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void gameReset(bool &reset) {
	if (reset) {
		for (int p = 0; p < TOTAL_POWERUPS; p++) {
			gSnake.hasActivePowerup[p] = false;
			gSnake.powerupActivationTimestamp[p] = 0;
		}
		gSnake.resetLength();
		gSnake.setStartPos(0.85 * gLvlWidth * (((double) rand() / RAND_MAX)), 0.85 * gLvlHeight * (((double) rand() / RAND_MAX)));
		for (int i = 0; i < ENEMY_COUNT; i++) {
			gEnemy[i].setStartPos(gEnemyStartPos[i].x, gEnemyStartPos[i].y);
			for (int p = 0; p < TOTAL_POWERUPS; p++) {
				gEnemy[i].hasActivePowerup[p] = false;
				gEnemy[i].powerupActivationTimestamp[p] = 0;
			}
			gEnemy[i].resetLength();
		}
		gReset = false;
	}
}

void playerAndBotsCollisions() {
	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (gEnemy[i].hasActivePowerup[1]) { //GHOST ON ****************
			break;
		}
		gCollision = checkCollision(gSnake.getHeadBox(), gEnemy[i].getHeadBox());
		if (gCollision && (gSnake.getLength() != 0 || gEnemy[i].getLength() != 0)) {
			if (gSnake.hasActivePowerup[0]) {
				for (int et = 0; et < gEnemy[i].getLength(); et++) {
					gSnake.addLength();
					gSnake.updateTail(gLTSnakeTail);
				}
				gEnemy[i].resetLength();
				break;
			}
			if (gEnemy[i].hasActivePowerup[0]) {
				for (int et = 0; et < gSnake.getLength(); et++) {
					gEnemy[i].addLength();
					gEnemy[i].updateTail(gLTEnemyTail);
				}
				gSnake.resetLength();
				break;
			}
			if (gEnemy[i].hasActivePowerup[3]) { //MAGNET ON ****************
				break;
			}
			gGameState = 4;
			//					gSnake.resetLength();
			//					gSnake.setStartPos(0.5 * gLvlWidth * (((double) rand() / RAND_MAX)), 0.5 * gLvlHeight * (((double) rand() / RAND_MAX)));
			gAngle[i] = 360 * ((double) rand() / RAND_MAX);
			gEnemy[i].resetLength();
			//					gEnemy[i].setStartPos(gEnemy[i].getHeadBox().x+gWindow.getWidth()+10, gEnemy[i].getHeadBox().y+gWindow.getHeight()+10);
		}
		if (gSnake.getLength() != 0) {
			for (int t = 0; t < gSnake.getLength(); t++) {
				gCollision = checkCollision(gSnake.getTailBox(t), gEnemy[i].getHeadBox());
				if (gCollision) {
					if (gEnemy[i].hasActivePowerup[0]) {
						for (int et = 0; et < gSnake.getLength(); et++) {
							gEnemy[i].addLength();
							gEnemy[i].updateTail(gLTEnemyTail);
						}
						gSnake.resetLength();
						break;
					}
					gAngle[i] = 360 * ((double) rand() / RAND_MAX);
					if (gEnemy[i].hasActivePowerup[3]) { //MAGNET ON ****************
						break;
					}
					gEnemy[i].resetLength();
					//							gEnemy[i].setStartPos(gEnemy[i].getHeadBox().x+gWindow.getWidth()+10, gEnemy[i].getHeadBox().y+gWindow.getHeight()+10);
				}
			}
		}
		if (gEnemy[i].getLength() != 0) {
			for (int t = 0; t < gEnemy[i].getLength(); t++) {
				gCollision = checkCollision(gEnemy[i].getTailBox(t), gSnake.getHeadBox());
				if (gCollision) {
					if (gSnake.hasActivePowerup[0]) {
						for (int et = 0; et < gEnemy[i].getLength(); et++) {
							gSnake.addLength();
							gSnake.updateTail(gLTSnakeTail);
						}
						gEnemy[i].resetLength();
						break;
					}
					gAngle[i] = 360 * ((double) rand() / RAND_MAX);
					if (gSnake.hasActivePowerup[3]) { //MAGNET ON ****************
						break;
					}
					//							gSnake.resetLength();
					gGameState = 4;
					//							gSnake.setStartPos(gEnemy[i].getHeadBox().x+gWindow.getWidth()+10, gEnemy[i].getHeadBox().y+gWindow.getHeight()+10);
				}
			}
		}

	}
}

void betweenBotsCollisions() {
	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (gEnemy[i].hasActivePowerup[1]) { //GHOST ON ****************
			break;
		}
		for (int j = 0; j < ENEMY_COUNT; j++) {
			gCollision = checkCollision(gEnemy[i].getHeadBox(), gEnemy[j].getHeadBox());
			//					cout<<"gCollision "<<gCollision<<endl;
			//					 cout<<"iEnemy "<<i<<"\tHx:"<<gEnemy[i].getHeadBox().x<<"\tHy:"<<gEnemy[i].getHeadBox().y<<"\tHw:"<<gEnemy[i].getHeadBox().w<<"\tHh:"<<gEnemy[i].getHeadBox().h<<endl;
			//					 cout<<"jEnemy "<<j<<"\tHx:"<<gEnemy[j].getHeadBox().x<<"\tHy:"<<gEnemy[j].getHeadBox().y<<"\tHw:"<<gEnemy[j].getHeadBox().w<<"\tHh:"<<gEnemy[j].getHeadBox().h<<endl;
			if (gCollision && i != j) {
				if (gEnemy[i].hasActivePowerup[0]) {
					for (int et = 0; et < gEnemy[j].getLength(); et++) {
						gEnemy[i].addLength();
						gEnemy[i].updateTail(gLTEnemyTail);
					}
					gEnemy[j].resetLength();
					break;
				}
				if (gEnemy[j].hasActivePowerup[0]) {
					for (int et = 0; et < gEnemy[i].getLength(); et++) {
						gEnemy[j].addLength();
						gEnemy[j].updateTail(gLTEnemyTail);
					}
					gEnemy[i].resetLength();
					break;
				}
				gAngle[i] = 360 * ((double) rand() / RAND_MAX);
				if (gEnemy[i].hasActivePowerup[3]) { //MAGNET ON ****************
					break;
				}
				gEnemy[i].resetLength();
				//						gEnemy[i].setStartPos(gEnemy[i].getHeadBox().x+gWindow.getWidth()+10, gEnemy[i].getHeadBox().y+gWindow.getHeight()+10);
			}
			if (gEnemy[i].getLength() != 0 && i != j) {
				for (int t = 0; t < gEnemy[i].getLength(); t++) {
					gCollision = checkCollision(gEnemy[i].getTailBox(t), gEnemy[j].getHeadBox());
					if (gCollision) {
						if (gEnemy[j].hasActivePowerup[0]) {
							for (int et = 0; et < gEnemy[i].getLength(); et++) {
								gEnemy[j].addLength();
								gEnemy[j].updateTail(gLTEnemyTail);
							}
							gEnemy[i].resetLength();
							break;
						}
						gAngle[j] = 360 * ((double) rand() / RAND_MAX);
						if (gEnemy[i].hasActivePowerup[3]) { //MAGNET ON ****************
							break;
						}
						gEnemy[j].resetLength();
						//								gEnemy[i].setStartPos(gEnemy[i].getHeadBox().x+gWindow.getWidth()+10, gEnemy[i].getHeadBox().y+gWindow.getHeight()+10);
					}
				}
			}
		}
	}
}

void fruitCollisions() {
	for (int i = 0; i < TOTAL_FRUITS; i++) {
		if (gSnake.collectFruit(gFruit[i])) {
			//					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
			//					SDL_RenderDrawLine(gRenderer, gSnake.getHeadBox().x + 10, gSnake.getHeadBox().y + 10, x[i] + 10, y[i] + 10);
			//					SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
			x[i] = gSnake.mNewFruitPos.x;
			y[i] = gSnake.mNewFruitPos.y;
		}
		for (int e = 0; e < ENEMY_COUNT; e++) {
			if (gEnemy[e].collectFruit(gFruit[i])) {
				x[i] = gEnemy[e].mNewFruitPos.x;
				y[i] = gEnemy[e].mNewFruitPos.y;
			}
		}
		gCollision = checkCollision(gFruit[i].getRect(), gSnake.getHeadBox());
		if (gCollision) {
			x[i] = (gLvlWidth - gFruit[i].getRect().w) * ((float) rand() / RAND_MAX);
			y[i] = (gLvlHeight - gFruit[i].getRect().h) * ((float) rand() / RAND_MAX);
			if (gSpriteNum[i] < 25) {
				gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]]);
			} else {
//				cout<<gSpriteNum[i]<<endl;
				gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]], &POWERUP_SCALE);
			}
			activatePowerup(gSpriteNum[i], gSnake);
			if (gSpriteNum[i] == 29) {
				SDL_RenderSetScale(gRenderer, 0.85, 0.85);
			}
			if (gSpriteNum[i] < 25) {
				gSnake.addLength();
			}
		}

		for (int j = 0; j < ENEMY_COUNT; j++) {
			gCollision = checkCollision(gFruit[i].getRect(), gEnemy[j].getHeadBox());
			if (gCollision) {
				x[i] = (gLvlWidth - gFruit[i].getRect().w) * ((float) rand() / RAND_MAX);
				y[i] = (gLvlHeight - gFruit[i].getRect().h) * ((float) rand() / RAND_MAX);
				if (gSpriteNum[i] < 25) {
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]]);
				} else {
//					cout<<gSpriteNum[i]<<endl;
					gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]], &POWERUP_SCALE);
				}
				gAngle[j] = 360 * ((double) rand() / RAND_MAX);
				activatePowerup(gSpriteNum[i], gEnemy[j]);
				if (gSpriteNum[i] < 25) {
					gEnemy[j].addLength();
				}
			}
		}
	}
}

void powerupCheck(Snake &vSnake, bool render) {
	for (int i = 0; i < TOTAL_POWERUPS; i++) {
		if (gSnake.powerupActivationTimestamp[i] < stepTimer.getSeconds()) {
			gSnake.hasActivePowerup[i] = false;
		}
		if (gSnake.hasActivePowerup[i]) {
			if (!render) {
				break;
			}
			switch (i) {
				case 0:
					gTimeLeft.str("");
					if (((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) < 10) && ((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds();
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(gWindow.getWidth() - 50, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(gWindow.getWidth() - 50 - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 1:
					gTimeLeft.str("");
					if (((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) < 10) && ((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds();
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(gWindow.getWidth() - 50, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(gWindow.getWidth() - 50 - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 2:
					gTimeLeft.str("");
					if (((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) < 10) && ((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds();
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(gWindow.getWidth() - 50, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(gWindow.getWidth() - 50 - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 3:
					gTimeLeft.str("");
					if (((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) < 10) && ((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds();
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(gWindow.getWidth() - 50, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(gWindow.getWidth() - 50 - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
					break;
				case 4:
					gTimeLeft.str("");
					if (((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) < 10) && ((gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds()) >= 0)) {
						gTimeLeft << "0";
					}
					gTimeLeft << gSnake.powerupActivationTimestamp[i] - stepTimer.getSeconds();
					gLTPowerupsTimeText[i].loadFromText(gTimeLeft.str().c_str(), gTextColor, gFont, gWindow);
					gLTPowerupsTimeText[i].setWidth(0.25 * gLTScoreText.getWidth());
					gLTPowerupsTimeText[i].setHeight(TEXT_SIZE);
					gLTPowerupIcons.render(gWindow.getWidth() - 50, i * 50, gWindow, &gPowerupClip[i]);
					gLTPowerupsTimeText[i].render(gWindow.getWidth() - 50 - gLTPowerupsTimeText[i].getWidth(), i * gLTPowerupsTimeText[i].getHeight(), gWindow);
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
					break;
				case 3:
					break;
				case 4:
					if (render) {
						SDL_RenderSetScale(gRenderer, 1, 1);
					}
					break;
			}
		}
	}
}

void activatePowerup(int &fruitSpriteNum, Snake &vSnake) {
	switch (fruitSpriteNum) {
		case 25:
			vSnake.powerupActivationTimestamp[fruitSpriteNum - 25] = stepTimer.getSeconds() + MAX_POWERUP_TIME[fruitSpriteNum - 25];
			vSnake.hasActivePowerup[fruitSpriteNum - 25] = true;
			break;
		case 26:
			vSnake.powerupActivationTimestamp[fruitSpriteNum - 25] = stepTimer.getSeconds() + MAX_POWERUP_TIME[fruitSpriteNum - 25];
			vSnake.hasActivePowerup[fruitSpriteNum - 25] = true;
			break;
		case 27:
			vSnake.powerupActivationTimestamp[fruitSpriteNum - 25] = stepTimer.getSeconds() + MAX_POWERUP_TIME[fruitSpriteNum - 25];
			vSnake.hasActivePowerup[fruitSpriteNum - 25] = true;
			vSnake.collectDistanceMultiplier(12);
			break;
		case 28:
			vSnake.powerupActivationTimestamp[fruitSpriteNum - 25] = stepTimer.getSeconds() + MAX_POWERUP_TIME[fruitSpriteNum - 25];
			vSnake.hasActivePowerup[fruitSpriteNum - 25] = true;
			break;
		case 29:
			vSnake.powerupActivationTimestamp[fruitSpriteNum - 25] = stepTimer.getSeconds() + MAX_POWERUP_TIME[fruitSpriteNum - 25];
			vSnake.hasActivePowerup[fruitSpriteNum - 25] = true;
			break;
	}
}
double distanceCalc(SDL_Point &a, SDL_Point &b) {
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void print() {
	cout << "Print drukuje: \n";
	cout << "gSHx: " << gSnake.getHeadBox().x << "\tgSHBy: " << gSnake.getHeadBox().y; //<<endl;
	cout << "\tgSHBw: " << gSnake.getHeadBox().w << "\tgSHBh: " << gSnake.getHeadBox().h << endl;
	for (int i = 0; i < ENEMY_COUNT; i++) {
		cout << "gEHx: " << gEnemy[i].getHeadBox().x << "\tgEHBy: " << gEnemy[i].getHeadBox().y; // << endl;
		cout << "\tgEHw: " << gEnemy[i].getHeadBox().w << "\tgEHh: " << gEnemy[i].getHeadBox().h << endl;
	}
	cout << "Koniec Printa\n";
	gContinue = false;
}

void handleEvents() {
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
			}
		}
		if (event.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&gMouse.x, &gMouse.y);
		}
		gWindow.eventHandler(event);
		gSnake.eventHandler(event, &gWindow, &gCamera);
		for (int i = 0; i < TOTAL_NUMBER_OF_BUTTONS; i++) {
			switch (gGameState) {
				case 1:
					if (i >= 0 && i <= 2) {
						gButtons[i].eventHandler(event);
						if (checkCollision(gButtons[i].getButtonDims(), gMouse)) {
							if (event.type == SDL_MOUSEBUTTONUP) {
								gOption = gButtons[i].getID();
							}
						}
					}
					break;
				case 2:
					if (i >= 3 && i <= 4) {
						gButtons[i].eventHandler(event);
						if (checkCollision(gButtons[i].getButtonDims(), gMouse)) {
							if (event.type == SDL_MOUSEBUTTONUP) {
								gOption = gButtons[i].getID();
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
			case 1:
				gGameState = 0;
				break;
			case 2:
				gGameState = 0;
				gReset = true;
				break;
			case 3:
				gGameState = 2;
				break;
			case 4:
				gContinue = false;
				;
				break;
			case 5:
				gGameState = 1;
				break;
		}
	}
}

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
	window->setWidth(gScreenWidth);
	window->setHeight(gScreenHeight);
	if (!window->init()) {
		printf("Failed to properly initialize Window object.");
		return false;
	}

	return true;
}

void close(Win *window) {
	/*for (int i = 0; i < TOTAL_FRUITS; i++) {
	 delete gFruit[i];
	 }*/
	gLTFruit.free();
	gLTSnakeHead.free();
	gLTSnakeTail.free();
	gLTLevelTexture.free();
	gLTPause.free();
	if (gLTScoreText.getWidth() != 0) {
		gLTScoreText.free();
	}
	gLTExitQuestion.free();
	gLTMenuBackground.free();
	gLTTitleText.free();

	if (window != NULL) {
		window->close();
	}

}

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

