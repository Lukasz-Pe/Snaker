//
// Created by lukasz on 02.06.2020.
//

#include "initsdl.h"

bool initSDL(Win *window, const int& gScreenWidth, const int& gScreenHeight) {
    if (window == NULL) {
        std::cout << "Window not set!\n";
        return false;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout <<"SDL could not initialize! SDL Error: "<< SDL_GetError()<<"\n";
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cout <<"Warning: Linear texture filtering not enabled!\n";
        return false;
    }
    window->setWidth(gScreenWidth);
    window->setHeight(gScreenHeight);
    if (!window->init()) {
        std::cout <<"Failed to properly initialize Window object.\n";
        return false;
    }
    
    return true;
}