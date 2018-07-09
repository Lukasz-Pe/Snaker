/*
 * Snake.cpp
 *
 *  Created on: 03.07.2018
 *      Author: lpekalski
 */

#include "Snake.h"

Snake::Snake() {
	mTailLength = 0;
	mHeadX = 0;
	mHeadY = 0;
	mHeadAngle = 0.0;
	mSpeed = 3;
	mPrevHeadX = 0;
	mPrevHeadY = 0;
	mPrevHeadAngle = 0;
	mTargetX = 0;
	mTargetY = 0;
	hasMouseFocus = false;
	mTemp = mSpeed;
	mVeryFast = 3 * mSpeed;
	mHeadBox= {0,0,20,20};
	mCollectDist = 5;
	mCollectAngle = 45;
	x = 0;
	y = 0;
	mCalcAngle = 0;
	mMinCollectAngle = 0;
	mMaxCollectAngle = 0;
}

bool Snake::collectFruit(Dot &vFruit) {
//	cout<<"\tDist: "<<sqrt(pow((mHeadX - vFruit.getPosX()), 2) + pow((mHeadY - vFruit.getPosY()), 2))<<"\tHx: "<<mHeadX<<"\tHy: "<<mHeadY<<"\tFx: "<<vFruit.getPosX()<<"\tFy: "<<vFruit.getPosY()<<endl;

	if (sqrt(pow((mHeadX - vFruit.getPosX()), 2) + pow((mHeadY - vFruit.getPosY()), 2)) < 2*mHeadBox.w) {
		x = (mHeadX + 10) - (vFruit.getPosX() + 10);
		y = (mHeadY + 10) - (vFruit.getPosY() + 10);
		if (x > 0 && y >= 0) {
			mCalcAngle = (180 / M_PI) * atan(x / y);
		} else if (x > 0 && y < 0) {
			mCalcAngle = 180 + (180 / M_PI) * atan(x / y);
		} else if (x < 0 && y >= 0) {
			mCalcAngle = (180 / M_PI) * atan(x / y);
		} else if (x < 0 && y < 0) {
			mCalcAngle = 180 + (180 / M_PI) * atan(x / y);
		}
		mMinCollectAngle = mCalcAngle - mCollectAngle / 2;
		mMaxCollectAngle = mCalcAngle + mCollectAngle / 2;
		if (mMinCollectAngle < 0) {
			mMinCollectAngle = 360 + mMinCollectAngle;
		}
		if (mMaxCollectAngle > 360) {
			mMaxCollectAngle = mMaxCollectAngle - 360;
		}
		if (mHeadAngle >= mMinCollectAngle || mHeadAngle <= mMinCollectAngle) {
			mNewFruitPos.x = vFruit.getPosX() + mCollectDist * sin(mCalcAngle * (M_PI / 180.0));
			mNewFruitPos.y = vFruit.getPosY() + mCollectDist * cos(mCalcAngle * (M_PI / 180.0));
			return true;
		}
	}
	return false;
}

void Snake::setStartPos(int sPosX, int sPosY) {
	mHeadX = sPosX;
	mHeadY = sPosY;
	mHeadBox.x = sPosX;
	mHeadBox.y = sPosY;
}

void Snake::addLength() {
	mTailLength++;
}

void Snake::eventHandler(SDL_Event event, Win *vWin, SDL_Rect *vCam) {
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				mSpeed = mVeryFast;
				break;
		}
	}
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_LEFT:
				mHeadAngle -= mAngleDelta;
				if (mHeadAngle > 360) {
					mHeadAngle = mHeadAngle - ((int) (mHeadAngle / 360)) * 360;
				}
				if (mHeadAngle < 0) {
					mHeadAngle = 360 - mHeadAngle;
				}
				break;
			case SDLK_DOWN:
			case SDLK_RIGHT:
				mHeadAngle += mAngleDelta;
				if (mHeadAngle < 0.0 || mHeadAngle > 360.0) {
					mHeadAngle = 0;
				}
				break;
		}
	}
	if (event.type == SDL_KEYUP && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				mSpeed = mTemp;
				break;
		}
	}
	if (event.type == SDL_MOUSEBUTTONDOWN && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))) {

		mSpeed = 10;
	}
	if (event.type == SDL_MOUSEBUTTONUP) {
		mSpeed = mTemp;
	}

	if (vWin != NULL) {
		hasMouseFocus = vWin->hasMouseFocus();
	}
	if (event.type == SDL_MOUSEMOTION && hasMouseFocus) {
		int x, y, camWinDiffX = 0, camWinDiffY = 0;
		if (vCam != NULL) {
			camWinDiffX = vCam->x;
			camWinDiffY = vCam->y;
		}
		SDL_GetMouseState(&x, &y);
		mTargetX = (double) ((x + camWinDiffX) - mHeadX);
		mTargetY = (double) (mHeadY - (y + camWinDiffY));
		if (mTargetX > 0 && mTargetY >= 0) {
			mHeadAngle = (180 / M_PI) * atan(mTargetX / mTargetY);
		} else if (mTargetX > 0 && mTargetY < 0) {
			mHeadAngle = 180 + (180 / M_PI) * atan(mTargetX / mTargetY);
		} else if (mTargetX < 0 && mTargetY >= 0) {
			mHeadAngle = (180 / M_PI) * atan(mTargetX / mTargetY);
		} else if (mTargetX < 0 && mTargetY < 0) {
			mHeadAngle = 180 + (180 / M_PI) * atan(mTargetX / mTargetY);
		}
	}
}

void Snake::move(SDL_Rect &gLevelBorders, LTexture &vTexTail, double timeStep) {
	mPrevHeadX = mHeadX;
	mPrevHeadY = mHeadY;
	mPrevHeadAngle = mHeadAngle;
	mHeadY -= mSpeed * cos(mHeadAngle * (M_PI / 180.0)) * timeStep;
	mHeadX += mSpeed * sin(mHeadAngle * (M_PI / 180.0)) * timeStep;
	if (mTailLength > 0) {
		updateTail(vTexTail);
	}
	if (((int) mHeadY + mHeadBox.h) > gLevelBorders.h) {
		//mHeadY = 0;
		mHeadY = gLevelBorders.h - mHeadBox.h;
	} else if ((int) mHeadY < 0) {
		//mHeadY = gLevelBorders.h;
		mHeadY = 0;
	}
	if (((int) mHeadX + mHeadBox.w) > gLevelBorders.w) {
		//mHeadX = 0;
		mHeadX = gLevelBorders.w - mHeadBox.w;
	}
	if ((int) mHeadX < 0) {
		//mHeadX = gLevelBorders.w;
		mHeadX = 0;
	}
}

void Snake::updateTail(LTexture &vTex) {
	if ((mTailLength != mTailX.size())) {
		mTailX.push_back(mPrevHeadX);
		mTailY.push_back(mPrevHeadY);
		mTailAngle.push_back(mPrevHeadAngle);
		mBox.push_back( { (int) mPrevHeadX, (int) mPrevHeadY, mHeadBox.w, mHeadBox.h });
	}
	for (unsigned int i = 1; i <= mTailLength; i++) {
		mTailX[i - 1] = mTailX[i];
		mTailY[i - 1] = mTailY[i];
		mTailAngle[i - 1] = mTailAngle[i];
		mBox[i - 1] = mBox[i];
	}
	mTailX[mTailLength - 1] = mPrevHeadX;
	mTailY[mTailLength - 1] = mPrevHeadY;
	mTailAngle[mTailLength - 1] = mPrevHeadAngle;
	mBox[mTailLength-1]= {(int)mPrevHeadX, (int)mPrevHeadY, mHeadBox.w, mHeadBox.h};
}

void Snake::render(Win &vWin, LTexture &vTexHead, LTexture &vTexTail, SDL_Rect &gCam, SDL_Rect *vClip) {
	for (unsigned int i = 0; i < mTailLength; i++) {
		vTexTail.render(mTailX[i] - gCam.x, mTailY[i] - gCam.y, vWin, vClip, mTailAngle[i]);
//		SDL_Rect tbox={mBox[i].x - gCam.x,mBox[i].y - gCam.y,mBox[i].w,mBox[i].h};
//		SDL_RenderDrawRect(vWin.getRenderer(), &tbox);
	}
	vTexHead.render(mHeadX - gCam.x, mHeadY - gCam.y, vWin, vClip, mHeadAngle);
	if (vClip == NULL) {
		mHeadBox= {(int)mHeadX,(int)mHeadY, vTexHead.getWidth(),vTexHead.getHeight()};
	} else {
		mHeadBox= {(int)mHeadX ,(int)mHeadY, vClip->w,vClip->h};
	}
//	SDL_RenderDrawRect(vWin.getRenderer(), &mHeadBox);
}

double Snake::getAngle() {
	return mHeadAngle;
}

void Snake::printParams() {
	cout << "mHeadX=" << mHeadX << "\tmHeadY=" << mHeadY << "\tmHeadAngle=" << mHeadAngle << "\tSpeed*cos(x)=" << (mSpeed * cos(mHeadAngle * (M_PI / 180.0))) << "\tSpeed*sin(x)" << (mSpeed * sin(mHeadAngle * (M_PI / 180.0))) << endl;
}

void Snake::setCamera(const int &vLWidth, const int &vLHeight, SDL_Rect &vCam) {
	vCam.x = mHeadX - 0.5 * vCam.w;
	vCam.y = mHeadY - 0.5 * vCam.h;
	if (vCam.x < 0) {
		vCam.x = 0;
	}
	if (vCam.y < 0) {
		vCam.y = 0;
	}
	if (vCam.x > vLWidth - vCam.w) {
		vCam.x = vLWidth - vCam.w;
	}
	if (vCam.y > vLHeight - vCam.h) {
		vCam.y = vLHeight - vCam.h;
	}
}

SDL_Rect Snake::getHeadBox() {
	return mHeadBox;
}

int Snake::getLength() {
	return mTailLength;
}

void Snake::resetLength() {
	if (mTailLength > 0) {
		mTailLength = 0;
		mTailX.clear();
		mTailY.clear();
		mTailAngle.clear();
		mBox.clear();
	}

}

void Snake::setAngle(double newAngle) {
	mHeadAngle = newAngle;
}

SDL_Rect Snake::getTailBox(int &num) {
	SDL_Rect tmp = { 0, 0, 0, 0 };
	if (mBox.size() != 0) {
		tmp = mBox[num];
	}
	return tmp;
}

Snake::~Snake() {
// TODO Auto-generated destructor stub
}

