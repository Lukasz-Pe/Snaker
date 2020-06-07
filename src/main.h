/*
 * main.h
 *
 *  Created on: 03.07.2018
 *      Author: Łukasz Pękalski
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
#include <map>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <memory>
//--------------------------------SDL include
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_thread.h>
//--------------------------------other functions
#include "functions/collisions.h"
double distanceCalc(SDL_Point &a, SDL_Point &b);

//void betweenBotsCollisions();
//void playerAndBotsCollisions();
void gameReset(bool &reset);
//-------------------------------------needed structs
//Struct for unique_ptr destructor
struct SDLTTFDestroyer
{
    void operator()(TTF_Font* font) const
    {
        TTF_CloseFont(font);
    }
};
struct SDLWindowDestroyer
{
    void operator()(SDL_Window* w) const
    {
        SDL_DestroyWindow(w);
    }
};
#endif /* MAIN_H_ */
