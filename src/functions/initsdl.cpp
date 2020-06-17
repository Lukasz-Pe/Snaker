//
// Created by lukasz on 02.06.2020.
//

#include "initsdl.h"

bool initSDL(std::shared_ptr<Win> window, const int& gScreenWidth, const int& gScreenHeight) {
    if (window ==nullptr) {
        std::cerr << "Window not set!\n";
        return false;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr <<"SDL could not initialize! SDL Error: "<< SDL_GetError()<<"\n";
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cerr <<"Warning: Linear texture filtering not enabled!\n";
        return false;
    }
    window->setWidth(gScreenWidth);
    window->setHeight(gScreenHeight);
    if (!window->init()) {
        std::cerr <<"Failed to properly initialize Window object.\n";
        return false;
    }
    
    return true;
}