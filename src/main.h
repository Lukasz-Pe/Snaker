/*
 * main.h
 *
 *  Created on: 03.07.2018
 *      Author: lpekalski
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <fstream>
//--------------------------------SDL include
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
//--------------------------------namespace
using namespace std;

bool checkCollision(SDL_Rect a, SDL_Rect b);
double distanceCalc(SDL_Point &a,SDL_Point &b);
void activatePowerup(int &fruitSpriteNum);
#endif /* MAIN_H_ */
