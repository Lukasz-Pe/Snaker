/*
 * T39-Fruit.h
 *
 *  Created on: 25 cze 2018
 *      Author: Łukasz Pękalski
 */

#ifndef T39_DOT_H_
#define T39_DOT_H_
#include "src/main.h"
#include "Tile.h"
#include "Win.h"
//TODO Derive Powerup class from Fruit
bool touchesWall(SDL_Rect vBox, Tile *vTiles[]);
class Fruit {
	public:
		Fruit(std::shared_ptr<Win> window, const int& level_width, const int& level_height);
		void eventHandler(SDL_Event &event);
		//TODO Write code for fruit repositioning
		void move();
		void reposition();
		void moveBox(const int &LevelWidht, const int &LevelHeight, Tile *vTiles[]=NULL, double timeStep=1.0);
		void moveFramerateIndependent(Win &vWin, float &timeStep);
		void renderBox(LTexture &vDotTexture, SDL_Rect *vCamera=NULL);
		void renderDot(LTexture &vDotTexture, int x=-1, int y=-1, SDL_Rect *vCamera=NULL, SDL_Rect *vClip=NULL, const double *scaleFactor=NULL);
		void setCamera(SDL_Rect &vCamera, const int &LevelWidht, const int &LevelHeight);
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>getters
		int getPosX();
		int getPosY();
		SDL_Rect getRect();
		void setPosition(const int &posx, const int &posy);
		void setCollisionBox(LTexture &vTexture, SDL_Rect *vClip=NULL);
	private:
        SDL_Rect mBox;
		double mPosX, mPosY, mVelX, mVelY, mAngle, mCollectDist,x,y;
		int mRad, mLevelWidth, mLevelHeight;
		const static int mDotRadius=20, mDotSpeed=10;
        std::shared_ptr<Win> mWindow;
		//friend bool touchesWall();
		friend bool checkCollision();
};

#endif /* T39_DOT_H_ */
