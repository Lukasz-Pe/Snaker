//
// Created by lukasz on 14.06.2020.
//

#ifndef SNAKER_POWERUP_H
#define SNAKER_POWERUP_H
#include "Fruit.h"

class PowerUp:public Fruit{
public:
    PowerUp(Win *window, const int& level_width, const int& level_height);
    bool Is_PowerUp();
    void Is_PowerUp(const int& value);
    int Lifespan();
    void Lifespan(const int& value);
private:
    bool is_powerup;
    int lifespan;
    
};


#endif //SNAKER_POWERUP_H
