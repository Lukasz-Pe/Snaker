/*
 * T39-Fruit.cpp
 *
 *  Created on: 25 cze 2018
 *      Author: Łukasz Pękalski
 */

#include "Fruit.h"

#include <utility>

Fruit::Fruit(std::shared_ptr<Win> window, const int& level_width, const int& level_height)
            :mWindow(std::move(window)), mLevelWidth(level_width), mLevelHeight(level_height) {
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;
	mBox= {0,0,mDotRadius, mDotRadius};
	mRad = 2 * mDotRadius;
	mAngle = 0.0;
	mCollectDist = 5;
	x = 0;
	y = 0;
}

void Fruit::eventHandler(SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
			case SDLK_UP:
				mVelY -= mDotSpeed;
				break;
			case SDLK_DOWN:
				mVelY += mDotSpeed;
				break;
			case SDLK_LEFT:
				mVelX -= mDotSpeed;
				break;
			case SDLK_RIGHT:
				mVelX += mDotSpeed;
				break;
		}
	}
	if (event.type == SDL_KEYUP && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
			case SDLK_UP:
				mVelY += mDotSpeed;
				break;
			case SDLK_DOWN:
				mVelY -= mDotSpeed;
				break;
			case SDLK_LEFT:
				mVelX += mDotSpeed;
				break;
			case SDLK_RIGHT:
				mVelX -= mDotSpeed;
				break;
		}
	}
}

void Fruit::move() {
	mPosX += mVelX;
	mPosY += mVelY;
	if ((mPosX < 0) || ((mPosX + mDotRadius) > mWindow->getWidth())) {
		mPosX -= mVelX;
	}
	if ((mPosY < 0) || ((mPosY + mDotRadius) > mWindow->getHeight())) {
		mPosY -= mVelY;
	}
}

void Fruit::moveBox(const int &LevelWidht, const int &LevelHeight, Tile *vTiles[], double timeStep) {
	mBox.x += mVelX*timeStep;
	mBox.y += mVelY*timeStep;
	if ((mBox.x < 0) || ((mBox.x + mDotRadius) > LevelWidht)
	/*|| touchesWall(mBox, vTiles)*/) {
		mBox.x -= mVelX;
	}
	if ((mBox.y < 0) || ((mBox.y + mDotRadius) > LevelHeight)
	/*|| touchesWall(mBox, vTiles)*/) {
		mBox.y -= mVelY;
	}
}

void Fruit::moveFramerateIndependent(Win &vWin, float &timeStep) {
	mPosX += mVelX * timeStep;
	mPosY += mVelY * timeStep;
	if ((mPosX < 0)) {
		mPosX = 0;
	} else if (mPosX > (vWin.getWidth() - mDotRadius)) {
		mPosX = vWin.getWidth() - mDotRadius;
	}
	if ((mPosY < 0)) {
		mPosY = 0;
	} else if (mPosY > (vWin.getHeight() - mDotRadius)) {
		mPosY = vWin.getHeight() - mDotRadius;
	}

}

void Fruit::renderBox(LTexture &vDotTexture, SDL_Rect *vCamera) {
	vDotTexture.render(mBox.x - vCamera->x, mBox.y - vCamera->y, mWindow);
}

void Fruit::renderDot(LTexture &vDotTexture, int x, int y, SDL_Rect *vCamera, SDL_Rect *vClip, const double *scaleFactor) {
	int tmpPosX = 0, tmpPosY = 0;
	if (vCamera != NULL) {
		tmpPosX = vCamera->x;
		tmpPosY = vCamera->y;
	}
	if (x != -1 && y != -1) {
		mPosX = x;
		mPosY = y;
	}
	if (scaleFactor != NULL) {
		mBox= {(int)mPosX,(int)mPosY,(int)(*scaleFactor*vClip->w), (int)(*scaleFactor*vClip->h)};
	} else {
		setCollisionBox(vDotTexture, vClip);
	}
	if (vClip != NULL) {
		if (scaleFactor != NULL) {
			setCollisionBox(vDotTexture, vClip);
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, mWindow, vClip, scaleFactor);
		} else {
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, mWindow, vClip);
		}
	} else {
		if (scaleFactor != NULL) {
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, mWindow, vClip, scaleFactor);
		} else {
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, mWindow, vClip);
		}
	}
}

void Fruit::setCamera(SDL_Rect &vCamera, const int &LevelWidht, const int &LevelHeight){
	vCamera.x = (mBox.x + mDotRadius / 2) - mWindow->getWidth() / 2;
	vCamera.y = (mBox.y + mDotRadius / 2) - mWindow->getHeight() / 2;
	if (vCamera.x < 0) {
		vCamera.x = 0;
	}
	if (vCamera.y < 0) {
		vCamera.y = 0;
	}
	if (vCamera.x > LevelWidht - vCamera.w) {
		vCamera.x = LevelWidht - vCamera.w;
	}
	if (vCamera.y > LevelHeight - vCamera.h) {
		vCamera.y = LevelHeight - vCamera.h;
	}
}

int Fruit::getPosX() {
	return mPosX;
}
int Fruit::getPosY() {
	return mPosY;
}

void Fruit::setCollisionBox(LTexture &vTexture, SDL_Rect *vClip) {
	if (vClip == NULL) {
		mBox= {(int)mPosX,(int)mPosY,vTexture.getWidth(), vTexture.getHeight()};
	} else {
		mBox= {(int)mPosX,(int)mPosY,vClip->w, vClip->h};
	}
}

SDL_Rect Fruit::getRect() {
	return mBox;
}

void Fruit::reposition(){
    srand(time(NULL));
    mPosX=rand()%static_cast<int>(0.9*mLevelWidth);
    mPosY=rand()%static_cast<int>(0.9*mLevelHeight);
}

void Fruit::setPosition(const int &posx, const int &posy){
    mPosX=static_cast<double>(posx);
    mPosY=static_cast<double>(posy);
}

