/*
 * T39-Tile.h
 *
 *  Created on: 26.06.2018
 *      Author: Łukasz Pękalski
 */

#ifndef SRC_T49_TILE_H_
#define SRC_T49_TILE_H_

#include "LTexture.h"
#include "src/main.h"

class Tile {
public:
	Tile(int x, int y, int gTileDimX, int gTileDimY, int tileType=-1);
	void render(SDL_Rect &gCamera, std::shared_ptr<Win> window, LTexture &vTexture, SDL_Rect *vClip=NULL);
	int getType();
	void setWindowCoords(int x, int y);
	void setNewPos(int &x, int &y);
	SDL_Rect getBox();
private:
	SDL_Rect mBox;
	int mType;
	friend bool checkCollision(SDL_Rect a, SDL_Rect b);
};

#endif /* SRC_T49_TILE_H_ */
