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
	Button(const std::string& txtID, std::string& text, std::shared_ptr<Win> window, TTF_Font *vFont, const int &TEXT_SIZE, const bool& wide=true);
	int getID();
	void eventHandler(SDL_Event &event);
	SDL_Rect getButtonDims();
	void setButtonText(const std::string& text, TTF_Font *vFont, const int &TEXT_SIZE, bool widen=1);
	void setPosition(int x, int y);
	void render(int x, int y);
	std::string getButtonTextID();
private:
    std::shared_ptr<Win> _window;
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
