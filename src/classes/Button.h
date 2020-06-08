/*
 * Menu.h
 *
 *  Created on: 5 lip 2018
 *      Author: Łukasz Pękalski
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include "src/main.h"
#include "LTexture.h"

class Button {
public:
	Button();
	Button(const std::string& txtID, std::string& text, Win &vWin, TTF_Font *vFont, const int &TEXT_SIZE);
	int getID();
	void eventHandler(SDL_Event &event);
	SDL_Rect getButtonDims();
	void setButtonText(std::string text, Win &vWin, TTF_Font *vFont, const int &TEXT_SIZE, bool widen=1);
	void setPosition(int x, int y);
	void render(int x, int y, Win &vWin);
	std::string getButtonTextID();
private:
	static int mButtonNum;
	int mButtonID;
	SDL_Rect mBoxButton;
	LTexture mButtonTexture, mSelectedButtonTexture;
    std::string mButtonText;
    const std::string mButtonTxtID;
	SDL_Color mButtonColor, mSelectedButtonColor;
	bool mButtonSelected;

};

#endif /* SRC_MENU_H_ */
