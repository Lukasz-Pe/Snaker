/*
 * T39-Win.cpp
 *
 *  Created on: 25.06.2018
 *      Author: lpekalski
 */

#include "Win.h"

Win::Win() {
	mWindow = NULL;
	mRenderer = NULL;
	mMinimized = false;
	mMouseFocus = false;
	mFullscreen = false;
	mWidth = 0;
	mHeight = 0;
}

bool Win::init() {
	mWindow = SDL_CreateWindow("Default window title", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN);
	if (mWindow == NULL) {
		cout << "Window could not be created. SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == NULL) {
		cout << "Renderer could not be created. SDL_Error: " << SDL_GetError() << endl;
		SDL_DestroyWindow(mWindow);
		mWindow = NULL;
		return false;
	}
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image could not initialize! SDL_image Error: %s \n",
		IMG_GetError());
		return false;
	}

	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

void Win::eventHandler(SDL_Event &event) {
//	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f && event.key.repeat == 0) {
//		if (mFullscreen) {
//			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
//			mFullscreen = false;
//		} else {
//			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
//			mFullscreen = true;
//		}
//	}
	if (event.type == SDL_WINDOWEVENT) {
		switch (event.window.event) {
			case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				mMinimized = false;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;
			case SDL_WINDOWEVENT_ENTER:
				mMouseFocus = true;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				mMouseFocus = false;
				break;
		}
	}
}

int Win::getWidth() {
	return mWidth;
}
int Win::getHeight() {
	return mHeight;
}
void Win::setHeight(int height) {
	mHeight = height;
}
void Win::setWidth(int width) {
	mWidth = width;
}

bool Win::isMinimized() {
	return mMinimized;
}

bool Win::hasMouseFocus() {
	return mMouseFocus;
}

SDL_Renderer *Win::getRenderer() {
	return mRenderer;
}

void Win::prepareRenderer(int red, int green, int blue) {
	SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
	SDL_RenderClear(mRenderer);
}

void Win::render() {
	SDL_RenderPresent(mRenderer);
}

void Win::free() {
	if (mWindow != NULL) {
		SDL_DestroyWindow(mWindow);
	}
	mWidth = 0;
	mHeight = 0;
}

SDL_Window *Win::getWindow() {
	return mWindow;
}

void Win::setTitle(string caption) {
	SDL_SetWindowTitle(mWindow, caption.c_str());
}

void Win::close() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mRenderer = NULL;
	mWindow = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

Win::~Win() {
	// TODO Auto-generated destructor stub
}

