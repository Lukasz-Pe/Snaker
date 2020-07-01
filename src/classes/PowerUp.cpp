//
// Created by lukasz on 14.06.2020.
//

#include "PowerUp.h"

#include <utility>

PowerUp::PowerUp(const char &powerup_type, std::shared_ptr<Win> window, const int &level_width, const int &level_height)
    :Fruit(std::move(window), level_width, level_height), _type(static_cast<PowerUpTypes>(powerup_type)){}

int PowerUp::powerUpType(){
    return _type;
}

