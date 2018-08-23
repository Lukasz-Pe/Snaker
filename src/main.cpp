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
vector<int> gCurrentTime, gTimeElapsed, gEnemySprite, gEnePosX, gEnePosY;
vector<vector<int> > gEnemyToTargetDistance(2, vector<int>());
vector<SDL_Point> gEnemyStartPos;
vector<Snake> gEnemy;
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
