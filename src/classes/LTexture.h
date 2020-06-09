/*
 * T39-LTexture.h
 *
 *  Created on: 25.06.2018
 *      Author: Łukasz Pękalski
 */

#ifndef SRC_T49_LTEXTURE_H_
#define SRC_T49_LTEXTURE_H_
#include "src/main.h"
#include "Win.h"

class LTexture {
public:
	LTexture();
	//------------------------------loading
	bool loadFromFile(std::string path, Win &vWin);
	bool loadFromFileAndChange(std::string path, Win &vWin);
	bool loadFromFileForString(std::string path, Win &vWin);
	bool loadFromText(std::string textureText, const SDL_Color &textColor, TTF_Font *mUsedFont, Win &vWin);
	bool createBlank(int width, int height, Win &vWin, SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING);
	//------------------------------other state mutators
	void render(int posX, int posY, Win &vWin, SDL_Rect *clip = NULL, const double *scaleFactor = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void setAlpha(Uint8 alpha);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void copyPixels(void *pixels);
	void free();
	bool lockTexture();
	bool unlockTexture();
	//------------------------------setters
	void setAsRenderTarget(Win &vWin, bool state = true);
	void setWidth(int w);
	void setHeight(int h);
	//------------------------------getters
	int getPitch();
	int getWidth();
	int getHeight();
	int getPosX();
	int getPosY();
	Uint32 getPixel32(unsigned int x, unsigned int y);
	void *getPixels();
	SDL_Texture *getTexture();

private:
	SDL_Texture *mTexture;
	void *mPixels;
	int mPitch;
	int mTextureWidht, mTextureHeight, mPosX, mPosY;
	SDL_Rect mBox;
};

#endif /* SRC_T49_LTEXTURE_H_ */
