/*
 * T39-LTexture.cpp
 *
 *  Created on: 25.06.2018
 *      Author: lpekalski
 */

#include "LTexture.h"

LTexture::LTexture() {
	mTexture = NULL;
	mTextureHeight = 0;
	mTextureWidht = 0;
	mPixels = NULL;
	mPitch = 0;
}

bool LTexture::loadFromFile(string path, Win &vWin) {
	free();
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		cout << "Failed to load " << path.c_str() << " to sufrace. SDL Error: " << SDL_GetError() << endl;
		return false;
	}
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 128, 128, 128));
	mTexture = SDL_CreateTextureFromSurface(vWin.getRenderer(), loadedSurface);
	if (mTexture == NULL) {
		cout << "Failed to transform " << path.c_str() << " to texture. SDL Error: " << SDL_GetError() << endl;
	}
	mTextureWidht = loadedSurface->w;
	mTextureHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
	return mTexture != NULL;
}

bool LTexture::loadFromFileForString(string path, Win &vWin) {
	free();
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	SDL_Surface *formattedSurface = NULL;
	SDL_Texture *newTexture = NULL;

	if (loadedSurface == NULL) {
		cout << "Failed to load " << path.c_str() << " to sufrace. SDL Error: " << SDL_GetError() << endl;
		return false;
	}
	formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
	if (formattedSurface == NULL) {
		printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	newTexture = SDL_CreateTexture(vWin.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
	if (newTexture == NULL) {
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
	SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);
	memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

	mTextureWidht = formattedSurface->w;
	mTextureHeight = formattedSurface->h;
	Uint32 *pixels = (Uint32*) mPixels;
	int pixelCount = (mPitch / 4) * mTextureHeight;
	Uint32 colorKey = SDL_MapRGB(formattedSurface->format, 255, 255, 255);
	Uint32 transparent = SDL_MapRGBA(formattedSurface->format, 0, 255, 255, 0);
	for (int i = 0; i < pixelCount; i++) {
		if (pixels[i] == colorKey) {
			pixels[i] = transparent;
		}
	}
	SDL_UnlockTexture(newTexture);
	mPixels = NULL;
	SDL_FreeSurface(formattedSurface);
	SDL_FreeSurface(loadedSurface);
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromFileAndChange(string path, Win &vWin) {
	free();
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	SDL_Surface *formattedSurface = NULL;
	SDL_Texture *newTexture = NULL;

	if (loadedSurface == NULL) {
		cout << "Failed to load " << path.c_str() << " to sufrace. SDL Error: " << SDL_GetError() << endl;
		return false;
	}
	formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(vWin.getWindow()), 0);
	if (formattedSurface == NULL) {
		printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	newTexture = SDL_CreateTexture(vWin.getRenderer(), SDL_GetWindowPixelFormat(vWin.getWindow()), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
	if (newTexture == NULL) {
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);
	memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
	SDL_UnlockTexture(newTexture);
	mPixels = NULL;
	mTextureWidht = formattedSurface->w;
	mTextureHeight = formattedSurface->h;
	SDL_FreeSurface(formattedSurface);
	SDL_FreeSurface(loadedSurface);
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromText(string textureText, SDL_Color textColor, TTF_Font *mUsedFont, Win &vWin) {
	free();
	SDL_Surface *textSufrace = TTF_RenderUTF8_Blended(mUsedFont, textureText.c_str(), textColor);
	//TTF_RenderText_Solid(mUsedFont, textureText.c_str(), textColor);
	if (textSufrace == NULL) {
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
		return false;
	}
	mTexture = SDL_CreateTextureFromSurface(vWin.getRenderer(), textSufrace);
	if (mTexture == NULL) {
		cout << "Unable to process fonts surface to texture! SDL Error: " << SDL_GetError() << endl;
		return false;
	}
	mTextureHeight = textSufrace->w;
	mTextureWidht = textSufrace->h;
	SDL_FreeSurface(textSufrace);
	return mTexture != NULL;
}

void LTexture::free() {
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mTextureHeight = 0;
		mTextureWidht = 0;
	}
}

void LTexture::render(int posX, int posY, Win &vWin, SDL_Rect *clip, const double *scaleFactor, double angle, SDL_Point *center, SDL_RendererFlip flip) {
	SDL_Rect rect = { posX, posY, mTextureWidht, mTextureHeight };
	if (clip != NULL) {
		if (scaleFactor != NULL) {
			rect.w = *scaleFactor * clip->w;
			rect.h = *scaleFactor * clip->h;
		} else {
			rect.w = clip->w;
			rect.h = clip->h;
		}
	}

	SDL_RenderCopyEx(vWin.getRenderer(), mTexture, clip, &rect, angle, center, flip);
}

int LTexture::getHeight() {
	return mTextureHeight;
}
int LTexture::getWidth() {
	return mTextureWidht;
}

void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

bool LTexture::lockTexture() {
	if (mPixels != NULL) {
		printf("Texture is already locked!\n");
		return false;
	}
	if (SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0) {
		printf("Unable to lock texture! %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool LTexture::unlockTexture() {
	if (mPixels == NULL) {
		printf("Texture is not locked!\n");
		return false;
	}
	SDL_UnlockTexture(mTexture);
	mPixels = NULL;
	mPitch = 0;
	return true;
}

void *LTexture::getPixels() {
	return mPixels;
}

int LTexture::getPitch() {
	return mPitch;
}

Uint32 LTexture::getPixel32(unsigned int x, unsigned int y) {
	Uint32 *pixels = (Uint32*) mPixels;
	return pixels[(y * (mPitch / 4)) + x];
}

LTexture::~LTexture() {
	free();
}

bool LTexture::createBlank(int width, int height, Win &vWin, SDL_TextureAccess access) {
	mTexture = SDL_CreateTexture(vWin.getRenderer(), SDL_PIXELFORMAT_RGBA8888, access, width, height);
	if (mTexture == NULL) {
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	mTextureWidht = width;
	mTextureHeight = height;
	return mTexture != NULL;
}

void LTexture::copyPixels(void *pixels) {
	if (mPixels != NULL) {
		memcpy(mPixels, pixels, mPitch * mTextureHeight);
	}
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setAsRenderTarget(Win &vWin, bool state) {
	if (state) {
		SDL_SetRenderTarget(vWin.getRenderer(), mTexture);
	} else {
		SDL_SetRenderTarget(vWin.getRenderer(), NULL);
	}
}

SDL_Texture *LTexture::getTexture() {
	return mTexture;
}

void LTexture::setWidth(int w) {
	mTextureWidht = w;
}

void LTexture::setHeight(int h) {
	mTextureHeight = h;
}
