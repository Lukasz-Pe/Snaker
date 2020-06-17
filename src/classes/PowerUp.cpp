//
// Created by lukasz on 14.06.2020.
//

#include "PowerUp.h"

#include <utility>

PowerUp::PowerUp(std::shared_ptr<Win> window, const int& level_width, const int& level_height):Fruit(std::move(window), level_width, level_height),is_powerup(true), lifespan(20){
}

bool PowerUp::Is_PowerUp(){
    return is_powerup;
}

int PowerUp::Lifespan(){
    return lifespan;
}

void PowerUp::Is_PowerUp(const int &value){
    is_powerup=value;
}

void PowerUp::Lifespan(const int &value){
    lifespan=value;
}
