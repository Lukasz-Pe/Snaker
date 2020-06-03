/*
 * Snake.h
 *
 *  Created on: 03.07.2018
 *      Author: Łukasz Pękalski
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include "src/main.h"
#include "Win.h"
#include "LTexture.h"
#include "Dot.h"

class Snake {
public:
	Snake();
	void addLength();
	void eventHandler(SDL_Event event, Win *vWin=NULL, SDL_Rect *vCam=NULL);
	void setStartPos(int sPosX, int sPosY);
	void move(SDL_Rect &gLevelBorders, LTexture &vTexTail, double *timeStep=NULL);
	void render(Win &vWin, LTexture &vTexHead, LTexture &vTexTail, SDL_Rect &gCam, SDL_Rect *vClip=NULL);
	void setCamera(const int &vLWidth, const int &vLHeight, SDL_Rect &vCam);
	bool collectFruit(Dot &vFruit);
	double getAngle();
	SDL_Rect getHeadBox();
	SDL_Rect getTailBox(int &num);
	void collectDistanceMultiplier(int dist);
	void printParams();
	void resetLength();
	void setAngle(double newAngle);
	int getLength();
	int getSnakeFruitDistance(SDL_Rect &vFruit);
	double getHeadToFruitAngle(SDL_Rect &vFruit);
	void updateTail(LTexture &vTex);
	bool hasActivePowerup[5]={false,false,false,false,false};
	int powerupActivationTimestamp[5]={0,0,0,0,0};
	void changeSpeed(int &speed);
	SDL_Point mNewFruitPos;
	SDL_Rect mHeadBox;
	void setCollectAngle(int angle);
private:
	unsigned int mTailLength, mSpeed, mTemp, mVeryFast;
	double mHeadAngle, mPrevHeadAngle, mHeadX, mHeadY, mPrevHeadX, mPrevHeadY, mTargetX, mTargetY, mCollectDist, mCollectAngle,xHeadToFruit,yHeadToFruit, mAngleHeadToFruit, mMinCollectAngle,mMaxCollectAngle, xFruitToHead, yFruitToHead, mAngleFruitToHead, mCollectDistanceMultiplier;
	const double mAngleDelta=5.0;
	std::vector<double> mTailX;
	std::vector<double> mTailY;
	std::vector<double> mTailAngle;
	bool hasMouseFocus;
	std::vector<SDL_Rect> mBox;



};

#endif /* SNAKE_H_ */
