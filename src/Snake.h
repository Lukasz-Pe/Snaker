/*
 * Snake.h
 *
 *  Created on: 03.07.2018
 *      Author: lpekalski
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include "main.h"
#include "Win.h"
#include "LTexture.h"

class Snake {
public:
	Snake();
	void addLength();
	void eventHandler(SDL_Event event, Win *vWin=NULL, SDL_Rect *vCam=NULL);
	void setStartPos(int sPosX, int sPosY);
	void move(SDL_Rect &gLevelBorders, LTexture &vTexTail, double timeStep=1.0);
	void render(Win &vWin, LTexture &vTexHead, LTexture &vTexTail, SDL_Rect &gCam, SDL_Rect *vClip=NULL);
	void setCamera(const int &vLWidth, const int &vLHeight, SDL_Rect &vCam);
	double getAngle();
	SDL_Rect getHeadBox();
	SDL_Rect getTailBox(int &num);
	void printParams();
	void resetLength();
	void setAngle(double newAngle);
	int getLength();
	virtual ~Snake();
private:
	void updateTail(LTexture &vTex);
	unsigned int mTailLength, mSpeed, mTemp, mVeryFast;
	double mHeadAngle, mPrevHeadAngle, mHeadX, mHeadY, mPrevHeadX, mPrevHeadY, mTargetX, mTargetY;
	const double mAngleDelta=5.0;
	vector<double> mTailX;
	vector<double> mTailY;
	vector<double> mTailAngle;
	bool hasMouseFocus;
	vector<SDL_Rect> mBox;
	SDL_Rect mHeadBox;

};

#endif /* SNAKE_H_ */
