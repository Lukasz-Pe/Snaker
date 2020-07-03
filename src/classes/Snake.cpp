/*
 * Snake.cpp
 *
 *  Created on: 03.07.2018
 *      Author: Łukasz Pękalski
 */

#include "Snake.h"

const unsigned int Snake::_powerup_duration=20;
const double Snake::_max_collection_angle=60.0;
const double Snake::_min_collection_angle=15.0;
const double Snake::_max_collection_distance=300.0;
const double Snake::_min_collection_distance=150.0;

Snake::Snake(const SDL_Point &start_position, const std::shared_ptr<Win> &window,
             const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size, const std::shared_ptr<Timer> &timer)
    :_position(start_position), _window(window), _game_settings(settings),
     _level_size(level_size), _timer(timer){
    _speed=_game_settings->settingsFromFile()[4];
    _powerup_deactivation_timestamp={0, 0, 0, 0, 0};
    _angle=0.0;
    _collection_angle=_max_collection_angle;
    _collection_distance=_min_collection_distance;
    _speed=_game_settings->settingsFromFile()[4];
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

std::vector<unsigned int> Snake::PowerUpsDeactivationTimeStamp(){
    return _powerup_deactivation_timestamp;
}

SnakeBody::Coordinates Snake::headCoordinates(){
    return _body[0];
}

double Snake::collectionDistance(){
    return _collection_distance;
}

double Snake::collectionAngle(){
    return _collection_angle;
}

void Snake::activatePowerUp(const unsigned int &powerup){
    _powerup_deactivation_timestamp[powerup]=_timer->getSeconds<unsigned int>()+_powerup_duration;
}

void Snake::checkPowerUps(){
    for(int i=0;i<_powerup_deactivation_timestamp.size()-2;i++){
        switch(i){
            case SnakeEater:{
                //ToDo at collision with snake transfer his points to hitter
                break;
            }
            case GhostMode:{
                //ToDo at collision allow go through
                break;
            }
            case EatingDistance:{
                if(_powerup_deactivation_timestamp[EatingDistance]>=_timer->getSeconds<unsigned  int>()){
                    _collection_distance=_max_collection_distance;
                    _collection_angle=_min_collection_angle;
                }else{
                    _collection_distance=_min_collection_distance;
                    _collection_angle=_max_collection_angle;
                }
                break;
            }
            case Shield:{
                //ToDo when another player hits, he shall loose his points.
                break;
            }
        }
    }
}
