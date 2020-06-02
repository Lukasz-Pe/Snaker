//
// Created by lukasz on 02.06.2020.
//

#include "initsdl.h"

bool initSDL(Win *window, const int& gScreenWidth, const int& gScreenHeight) {
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