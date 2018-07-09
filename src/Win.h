/*
 * T39-Win.h
 *
 *  Created on: 25.06.2018
 *      Author: lpekalski
 */

#ifndef SRC_T49_WIN_H_
#define SRC_T49_WIN_H_

#include "main.h"

class Win {
public:
	Win();
	void eventHandler(SDL_Event &event);
	void free();
	void focus();
	void render();
	void prepareRenderer(int red=255, int green=255, int blue=255);
	void close();
	//--------------------------------------getters
	int getWidth();
	int getHeight();
	SDL_Renderer* getRenderer();
	SDL_Window *getWindow();
	//--------------------------------------setters
	void setWidth(int widht);
	void setHeight(int height);
	void setTitle(string caption);
	//--------------------------------------checkers
	bool init();
	bool isMinimized();
	bool hasMouseFocus();

	virtual ~Win();

private:
	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	int mHeight, mWidth;
	bool mMinimized, mMouseFocus, mFullscreen;
	//--------------------------------------Friends



};

#endif /* SRC_T49_WIN_H_ */
