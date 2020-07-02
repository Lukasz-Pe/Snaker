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
//TODO Make Snake super class for Bots.
namespace SnakeBody{
    struct Coordinates{
        double _x,_y;
        double _angle;
    };
}
class Snake {
public:
    Snake()= default;
    Snake(const SDL_Point &start_position, const std::shared_ptr<Win> &window,
          const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size, const std::shared_ptr<Timer> &timer);
    void StartPosition(SDL_Point &position);
    virtual void updateSnake()=0;
    void resetLength();
    unsigned long Length();
    virtual void render()=0;
    std::vector<unsigned int> PowerUpsDeactivationTimeStamp();
    void activatePowerUp(const unsigned int &powerup);
    SnakeBody::Coordinates headCoordinates();
    double collectionDistance();
    double collectionAngle();
    virtual SDL_Rect headAndBodyRects(const int &body_part)=0;
    virtual void addLength()=0;
protected:
    virtual void move()=0;
    std::vector<unsigned int> _powerup_deactivation_timestamp;
    SDL_Point _position;
    int _speed,_old_length;
    double _angle,_collection_distance,_collection_angle;
    static const double _max_collection_angle, _min_collection_angle,
                        _max_collection_distance, _min_collection_distance;
    std::vector<SnakeBody::Coordinates> _body;
    std::shared_ptr<Win> _window;
    SDL_Rect _level_size;
    std::shared_ptr<Timer> _timer;
    std::shared_ptr<Settings> _game_settings;
    static const unsigned int _powerup_duration;
};

#endif /* SNAKE_H_ */
