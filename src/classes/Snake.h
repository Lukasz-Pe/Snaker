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
#include "Fruit.h"
//TODO Make Snake super class for Player and Bots.
class Snake {
public:
	Snake(Win *vWin, LTexture *vTexHead, LTexture *vTexTail, SDL_Rect *gCam, SDL_Rect *vClip=nullptr, double *timeStep=nullptr);
	void addLength();
	void eventHandler(SDL_Event event);
	void setStartPos(int sPosX, int sPosY);
	void move();
	void render();
	void setCamera(const int &vLWidth, const int &vLHeight, SDL_Rect &vCam);
	bool collectFruit(Fruit &vFruit);
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
	void changeSpeed(int &speed);
	void setCollectAngle(int angle);
private:
    SDL_Point mNewFruitPos;
    SDL_Rect mHeadBox;
    std::vector<bool> hasActivePowerup{false,false,false,false,false};
    std::vector<int> powerupActivationTimestamp{0,0,0,0,0};
	unsigned int mTailLength, mSpeed, mTemp, mVeryFast;
	double mHeadAngle, mPrevHeadAngle, mHeadX, mHeadY, mPrevHeadX, mPrevHeadY, mTargetX, mTargetY, mCollectDist,
	    mCollectAngle,xHeadToFruit,yHeadToFruit, mAngleHeadToFruit, mMinCollectAngle,mMaxCollectAngle, xFruitToHead,
	    yFruitToHead, mAngleFruitToHead, mCollectDistanceMultiplier;
	const double mAngleDelta=5.0;
	std::vector<double> mTailX;
	std::vector<double> mTailY;
	std::vector<double> mTailAngle;
	bool hasMouseFocus;
	std::vector<SDL_Rect> mBox;
	Win *vWin;
	LTexture *mHeadTexture, *mTailTexture;
    SDL_Rect *mCamera, *mClip, *mLevelSize;
    double *mTimeStep;


};

#endif /* SNAKE_H_ */
