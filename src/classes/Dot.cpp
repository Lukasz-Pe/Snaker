/*
 * T39-Dot.cpp
 *
 *  Created on: 25 cze 2018
 *      Author: lukasz
 */

#include "Dot.h"

Dot::Dot() {
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

void Dot::eventHandler(SDL_Event &event) {
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

//void Dot::moveToSnake(Snake &vSnake) {
//	double dist = sqrt(pow((vSnake.getHeadBox().x - mPosX), 2) + pow((vSnake.getHeadBox().y - mPosY), 2));
//	if (dist <= mRad) {
//		x = (vSnake.getHeadBox().x+10) - (mPosX+10);
//		y = (vSnake.getHeadBox().y+10) - (mPosY+10);
//		if (x > 0 && y >= 0) {
//			mAngle = (180 / M_PI) * atan(x / y);
//		} else if (x > 0 && y < 0) {
//			mAngle = 180 + (180 / M_PI) * atan(x / y);
//		} else if (x < 0 && y >= 0) {
//			mAngle = (180 / M_PI) * atan(x / y);
//		} else if (x < 0 && y < 0) {
//			mAngle = 180 + (180 / M_PI) * atan(x / y);
//		}
//		mPosX = mPosX + mCollectDist * sin(mAngle * (M_PI / 180.0));
//		mPosY = mPosY + mCollectDist * cos(mAngle * (M_PI / 180.0));
//	}
//}

void Dot::move(Win &vWin) {
	mPosX += mVelX;
	mPosY += mVelY;
	if ((mPosX < 0) || ((mPosX + mDotRadius) > vWin.getWidth())) {
		mPosX -= mVelX;
	}
	if ((mPosY < 0) || ((mPosY + mDotRadius) > vWin.getHeight())) {
		mPosY -= mVelY;
	}
}

void Dot::moveBox(const int &LevelWidht, const int &LevelHeight, Tile *vTiles[]) {
	mBox.x += mVelX;
	mBox.y += mVelY;
	if ((mBox.x < 0) || ((mBox.x + mDotRadius) > LevelWidht)
	/*|| touchesWall(mBox, vTiles)*/) {
		mBox.x -= mVelX;
	}
	if ((mBox.y < 0) || ((mBox.y + mDotRadius) > LevelHeight)
	/*|| touchesWall(mBox, vTiles)*/) {
		mBox.y -= mVelY;
	}
}

void Dot::moveFramerateIndependent(Win &vWin, float &timeStep) {
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

void Dot::renderBox(LTexture &vDotTexture, Win &vWin, SDL_Rect *vCamera) {
	vDotTexture.render(mBox.x - vCamera->x, mBox.y - vCamera->y, vWin);
}

void Dot::renderDot(LTexture &vDotTexture, Win &vWin, int x, int y, SDL_Rect *vCamera, SDL_Rect *vClip, const double *scaleFactor) {
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
//		cout<<*scaleFactor<<endl;
		mBox= {(int)mPosX,(int)mPosY,(int)(*scaleFactor*vClip->w), (int)(*scaleFactor*vClip->h)};
	} else {
		setCollisionBox(vDotTexture, vClip);
	}
	if (vClip != NULL) {
		if (scaleFactor != NULL) {
			setCollisionBox(vDotTexture, vClip);
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, vWin, vClip, scaleFactor);
		} else {
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, vWin, vClip);
		}
	} else {
		if (scaleFactor != NULL) {
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, vWin, vClip, scaleFactor);
		} else {
			vDotTexture.render((int) mPosX - tmpPosX, (int) mPosY - tmpPosY, vWin, vClip);
		}
	}
//	SDL_RenderDrawRect(vWin.getRenderer(), &mBox);
}

void Dot::setCamera(Win &vWin, SDL_Rect &vCamera, const int &LevelWidht, const int &LevelHeight) {
	vCamera.x = (mBox.x + mDotRadius / 2) - vWin.getWidth() / 2;
	vCamera.y = (mBox.y + mDotRadius / 2) - vWin.getHeight() / 2;
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

int Dot::getPosX() {
	return mPosX;
}
int Dot::getPosY() {
	return mPosY;
}

void Dot::setCollisionBox(LTexture &vTexture, SDL_Rect *vClip) {
	if (vClip == NULL) {
		mBox= {(int)mPosX,(int)mPosY,vTexture.getWidth(), vTexture.getHeight()};
	} else {
		mBox= {(int)mPosX,(int)mPosY,vClip->w, vClip->h};
	}
}

SDL_Rect Dot::getRect() {
	return mBox;
}

Dot::~Dot() {

}

