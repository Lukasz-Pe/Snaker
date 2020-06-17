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
namespace SnakeBody{
    struct Coordinates{
        SDL_Point _position;
        double _angle;
    };
}
class Snake {
public:
    Snake();
    void StartPosition(SDL_Point &position);
    void changeSpeed();
    void updateSnake();
    void resetLength();
    int Length();
    virtual void render()=0;
    void ActivatePowerUpVision();
    void ActivatePowerUpSnakeEater();
    void ActivatePowerUpGhostMode();
    void ActivatePowerUpEatingDistance();
    void ActivatePowerUpGodMode();
    std::vector<bool> ActivePowerUps();
    std::vector<int> PowerUpsActivationTimeStamp();
    
private:
    std::vector<bool> _active_powerup;
    std::vector<unsigned int> _powerup_activation_timestamp;
    SDL_Point _position;
    int _speed;
    bool _faster;
    double _angle, _max_collection_angle, _min_collection_angle, _max_collection_distance, _min_collection_distance;
    std::vector<SnakeBody::Coordinates> _body;
    Win *_window;
    LTexture *_head, *_tail;
    SDL_Rect *_camera, *_level_size;
    double *_time_step;
};

#endif /* SNAKE_H_ */
