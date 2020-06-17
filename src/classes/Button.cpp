/*
 * Menu.cpp
 *
 *  Created on: 5 lip 2018
 *      Author: Łukasz Pękalski
 */

#include "Button.h"
#include "LTexture.h"
int Button::mButtonNum{0};
Button::Button() {
	mButtonNum++;
	mButtonID = mButtonNum;
	mBoxButton= {0,0,0,0};
	mButtonSelected = false;
	mButtonColor= {0,255,0};
	mSelectedButtonColor= {255,0,0};
}

int Button::getID() {
	return mButtonID;
}

void Button::eventHandler(SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		mButtonSelected = false;
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (x > mBoxButton.x && x < (mBoxButton.x + mBoxButton.w) && y > mBoxButton.y && y < (mBoxButton.y + mBoxButton.h)) {
			mButtonSelected = true;
		}
	}
}

SDL_Rect Button::getButtonDims() {
	return mBoxButton;
}

void Button::setButtonText(const std::string& text, TTF_Font *vFont, const int &TEXT_SIZE, bool widen) {
    mButtonTexture.loadFromText(text, mButtonColor, vFont, _window);
	if (widen) {
		mButtonTexture.setWidth(4 * mButtonTexture.getWidth());
	}
	mButtonTexture.setHeight(TEXT_SIZE);
	TTF_SetFontStyle(vFont, TTF_STYLE_UNDERLINE);
	mSelectedButtonTexture.loadFromText(text, mSelectedButtonColor, vFont, _window);
	if (widen) {
		mSelectedButtonTexture.setWidth(4 * mSelectedButtonTexture.getWidth());
	}
	mSelectedButtonTexture.setHeight(TEXT_SIZE);
	TTF_SetFontStyle(vFont, TTF_STYLE_NORMAL);
	mBoxButton.w = mButtonTexture.getWidth();
	mBoxButton.h = mButtonTexture.getHeight();
}

void Button::setPosition(int x, int y) {
	mBoxButton.x = x;
	mBoxButton.y = y;
}

void Button::render(int x, int y){
    mBoxButton.x=x;
    mBoxButton.y=y;
    if(mButtonSelected){
        mSelectedButtonTexture.render(x, y, _window);
    }else{
        mButtonTexture.render(x, y, _window);
    }
}

Button::Button(const std::string& txtID, std::string& text, std::shared_ptr<Win> window, TTF_Font *vFont, const int &TEXT_SIZE, const bool& wide):
mButtonTxtID(txtID),_window(std::move(window)){
    mButtonNum++;
    mButtonID = mButtonNum;
    mBoxButton= {0,0,0,0};
    mButtonSelected = false;
    mButtonColor= {0,255,0};
    mSelectedButtonColor= {255,0,0};
    setButtonText(text,vFont,TEXT_SIZE,wide);
}

std::string Button::getButtonTextID(){
    return mButtonTxtID;
}
