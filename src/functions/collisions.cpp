//
// Created by lukasz on 02.06.2020.
//

#include "collisions.h"

bool checkCollision(SDL_Rect a, SDL_Rect b) {
//The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

//Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

//Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

//If any of the sides from A are outside of B
    if (bottomA <= topB) {
        return false;
    }
    
    if (topA >= bottomB) {
        return false;
    }
    
    if (rightA <= leftB) {
        return false;
    }
    
    if (leftA >= rightB) {
        return false;
    }

//If none of the sides from A are outside B
    return true;
}

/*
void fruitCollisions(const int& gPlayerQuantity, const int& gPlayerToTargetDistance, const int& gLvlWidth, const int& gLvlHeight,
    const int& gFruitsQuantity, vector<Snake>& gPlayer, bool& gCollision, vector<double>& gAngle, const vector<int>& gSpriteNum,
    ) {
    
    for (int i = 0; i <= gPlayerQuantity; i++) {
        //		cout << "BFR gPlayerToTargetDistance[1][" << i << "] " << gPlayerToTargetDistance[1][i] << "\tgPlayerToTargetDistance[0][" << i << "] " << gPlayerToTargetDistance[0][i] << endl;
        gPlayerToTargetDistance[0][i] = gLvlWidth * gLvlHeight;
        gPlayerToTargetDistance[1][i] = -1;
        //		cout<<"AFR gPlayerToTargetDistance[1]["<<i<<"] "<<EnemyToTargetDistance[1][i]<<"\tgPlayerToTargetDistance[0]["<<i<<"] "<<gPlayerToTargetDistance[0][i]<<endl;
    }
    for (int i = 0; i < gFruitsQuantity; i++) {
        for (int e = 0; e <= gPlayerQuantity; e++) {
            if (gPlayer[e].collectFruit(gFruit[i])) {
                x[i] = gPlayer[e].mNewFruitPos.x;
                y[i] = gPlayer[e].mNewFruitPos.y;
            }
        }
        for (int j = 0; j <= gPlayerQuantity; j++) {
            gCollision = checkCollision(gFruit[i].getRect(), gPlayer[j].getHeadBox());
            if (gPlayer[j].hasActivePowerup[0] && (gPlayer[0].hasActivePowerup[0] || gPlayer[0].hasActivePowerup[1] || gPlayer[0].hasActivePowerup[3])) {
                gPlayer[j].hasActivePowerup[0] = false;
            }
            if (gPlayer[j].hasActivePowerup[0] && j != 0) {
                gPlayerToTargetDistance[0][j] = gPlayer[j].getSnakeFruitDistance(gPlayer[0].mHeadBox);
                gPlayerToTargetDistance[1][j] = -1;
                for (int e = 0; e <= gPlayerQuantity; e++) {
                    if ((gPlayerToTargetDistance[0][j] > gPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox)) && j != e) {
                        gPlayerToTargetDistance[1][j] = e;
                    }
                }
                if (gPlayerQuantity > 0) {
                    for (int e = 1; e <= gPlayerQuantity; e++) {
                        //					cout<<"gPlayerToTargetDistance[0][j]"<<gPlayerToTargetDistance[0][j]<<"\tgPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox) "<<gPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox)<<endl;
                        if ((gPlayerToTargetDistance[0][j] < gPlayer[j].getSnakeFruitDistance(gPlayer[e].mHeadBox)) && j != e) {
                            gAngle[j] = gPlayer[j].getHeadToFruitAngle(gPlayer[0].mHeadBox);
                        } else {
                            gAngle[j] = gPlayer[j].getHeadToFruitAngle(gPlayer[e].mHeadBox);
                        }
                        //					cout << "AFC gPlayerToTargetDistance[1][" << e << "] " << gPlayerToTargetDistance[1][e] << "\tgPlayerToTargetDistance[0][" << e << "] " << gPlayerToTargetDistance[0][e] <<"\tgAngle["<<j<<"] "<<gAngle[j]<< endl;
                    }
                } else {
                    gAngle[j] = gPlayer[j].getHeadToFruitAngle(gPlayer[0].mHeadBox);
                }
                
                break;
            }
            if (gCollision) {
                //				cout << "gPlayerToTargetDistance[0][j] " << gPlayerToTargetDistance[0][j] << "\tgPlayerToTargetDistence[1][j] " << gPlayerToTargetDistance[1][j] << endl;
                x[i] = (gLvlWidth - gFruit[i].getRect().w) * ((float) rand() / RAND_MAX);
                y[i] = (gLvlHeight - gFruit[i].getRect().h) * ((float) rand() / RAND_MAX);
                if (gSpriteNum[i] < 25) {
                    gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]]);
                } else {
                    //					cout<<gSpriteNum[i]<<endl;
                    gFruit[i].renderDot(gLTFruit, gWindow, x[i], y[i], &gCamera, &gFruitSpriteClips[gSpriteNum[i]], &POWERUP_SCALE);
                }
                //				gAngle[j] = gPlayer[j].getHeadToFruitAngle(gFruit[gPlayerToTargetDistence[1][j]].mBox);
                //				cout << "j " << j << "\t " << gPlayerToTargetDistence[1][j] << endl;
                activatePowerup(gSpriteNum[i], gPlayer[j]);
                if (gSpriteNum[i] < 25) {
                    gPlayer[j].addLength();
                }
                gAngle[j] = gPlayer[j].getHeadToFruitAngle(gFruit[gPlayerToTargetDistance[1][j]].mBox);
            }
        }
    }
}*/
