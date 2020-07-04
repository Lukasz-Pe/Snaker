//
// Created by lukasz on 03.07.2020.
//

#ifndef SNAKER_BOT_H
#define SNAKER_BOT_H

#include "Snake.h"

struct TargetPosition{
    TargetPosition()=default;
    TargetPosition(const double& x, const double& y):_x(x),_y(y){}
    double _x,_y;
};

class Bot: public Snake{
public:
    Bot()=default;;
    Bot(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
        const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
        SDL_Rect &clip, const std::shared_ptr<Timer> &timer, const std::vector<Fruit> &fruits,
        const std::vector<PowerUp> &powerups, const double &frame_time, const SDL_Rect &camera);
    void render() override;
    SDL_Rect headAndBodyRects(const int &body_part) override;
    void findNewTarget();
    Bot(const Bot &source);//copy constructor
    Bot &operator=(const Bot &source);//copy assigment operator
    Bot(Bot &&source);//move constructor
    Bot &operator=(Bot &&source);//move assigment operator
    ~Bot()=default;
    void move() override;
    
private:
    template <typename T>TargetPosition calculateNearestTargetPosition(std::vector<T> fruit);
    const double &_frame_time;
    const std::vector<Fruit> &_fruits;
    const std::vector<PowerUp> &_powerups;
    std::shared_ptr<LTexture> _head,_tail;
    SDL_Rect _clip;
    const SDL_Rect &_camera;
    TargetPosition _target;
};


#endif //SNAKER_BOT_H
