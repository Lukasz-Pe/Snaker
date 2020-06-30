//
// Created by lukasz on 29.06.2020.
//

#ifndef SNAKER_MOVEFRUIT_H
#define SNAKER_MOVEFRUIT_H
#include "../classes/Game.h"

template<typename S,typename F> double calculateDistance(S &snake, F &fruit);
template<typename S,typename F> double calculateAngle(S &snake, F &fruit);
template<typename S,typename F> bool fruitIsInRange(S &snake, F &fruit);
template<typename S,typename F> bool fruitIsInCollectionAngle(S &snake, F &fruit);
template<typename S,typename F> void changeFruitPosition(S &snake, F &fruit);
template<typename S,typename F> void moveFruit(S &snake, F &fruit);


#endif //SNAKER_MOVEFRUIT_H
