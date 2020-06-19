/*
 * Snake.cpp
 *
 *  Created on: 03.07.2018
 *      Author: Łukasz Pękalski
 */

#include "Snake.h"

const int Snake::_powerup_duration=20;
const double Snake::_max_collection_angle=60.0;
const double Snake::_min_collection_angle=30.0;
const double Snake::_max_collection_distance=30.0;
const double Snake::_min_collection_distance=15.0;

Snake::Snake(const SDL_Point &start_position, const std::shared_ptr<Win> &window,
             const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
             const std::shared_ptr<Timer> &timer): _position(start_position), _window(window), _game_settings(settings),
             _level_size(level_size), _timer(timer){
    _speed=_game_settings->settingsFromFile()[4];
    _powerup_deactivation_timestamp={0, 0, 0, 0, 0};
    _angle=0.0;
}

void Snake::StartPosition(SDL_Point &position){
    _position=position;
}

void Snake::resetLength(){
    _body.resize(1);
}

unsigned long Snake::Length(){
    return _body.size()-1;
}

void Snake::ActivatePowerUpVision(){
    _powerup_deactivation_timestamp[0]=_timer->getSeconds<int>()+_powerup_duration;
}

void Snake::ActivatePowerUpSnakeEater(){
    _powerup_deactivation_timestamp[1]=_timer->getSeconds<int>()+_powerup_duration;
}

void Snake::ActivatePowerUpGhostMode(){
    _powerup_deactivation_timestamp[2]=_timer->getSeconds<int>()+_powerup_duration;
}

void Snake::ActivatePowerUpEatingDistance(){
    _powerup_deactivation_timestamp[3]=_timer->getSeconds<int>()+_powerup_duration;
}

void Snake::ActivatePowerUpShield(){
    _powerup_deactivation_timestamp[4]=_timer->getSeconds<int>()+_powerup_duration;
}

std::vector<unsigned int> Snake::PowerUpsActivationTimeStamp(){
    return _powerup_deactivation_timestamp;
}
