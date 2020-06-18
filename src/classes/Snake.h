/*
 * Snake.h
 *
 *  Created on: 03.07.2018
 *      Author: Łukasz Pękalski
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include "src/main.h"
#include "Timer.h"
#include "Settings.h"
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
    Snake(const SDL_Point& start_position, const std::shared_ptr<Win>& window, const std::shared_ptr<Settings>& settings,
        const std::shared_ptr<SDL_Rect>& level_size, const std::shared_ptr<Timer>& timer, const std::shared_ptr<double>& time_step);
    void StartPosition(SDL_Point &position);
    virtual void updateSnake()=0;
    void resetLength();
    unsigned long Length();
    virtual void render()=0;
    void ActivatePowerUpVision();
    void ActivatePowerUpSnakeEater();
    void ActivatePowerUpGhostMode();
    void ActivatePowerUpEatingDistance();
    void ActivatePowerUpShield();
    std::vector<unsigned int> PowerUpsActivationTimeStamp();
private:
    std::vector<unsigned int> _powerup_deactivation_timestamp;
    SDL_Point _position;
    int _speed;
    double _angle;
    static const double _max_collection_angle, _min_collection_angle,
                        _max_collection_distance, _min_collection_distance;
    std::vector<SnakeBody::Coordinates> _body;
    std::shared_ptr<Win> _window;
    std::shared_ptr<SDL_Rect> _level_size;
    std::shared_ptr<double> _time_step;
    std::shared_ptr<Timer> _timer;
    std::shared_ptr<Settings> _game_settings;
    static const int _powerup_duration;
};

#endif /* SNAKE_H_ */
