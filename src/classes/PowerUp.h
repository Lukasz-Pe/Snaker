//
// Created by lukasz on 14.06.2020.
//

#ifndef SNAKER_POWERUP_H
#define SNAKER_POWERUP_H
#include "Fruit.h"

enum PowerUpTypes: char{
    SnakeEater, GhostMode, EatingDistance, Shield, Vision
};

class PowerUp:public Fruit{
public:
    PowerUp(const char &powerup_type, std::shared_ptr<Win> window, const int &level_width, const int &level_height);
    int powerUpType();
private:
    PowerUpTypes _type;
    
};


#endif //SNAKER_POWERUP_H
