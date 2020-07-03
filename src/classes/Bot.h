//
// Created by lukasz on 03.07.2020.
//

#ifndef SNAKER_BOT_H
#define SNAKER_BOT_H

#include "Snake.h"

class Bot: public Snake{
public:
    Bot()=default;;
    Bot(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
           const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
           SDL_Rect &clip, const std::shared_ptr<Timer> &timer);
    void render() override;
    SDL_Rect headAndBodyRects(const int &body_part) override;
    void addLength() override;
    Bot(const Bot &source);//copy constructor
    Bot &operator=(const Bot &source);//copy assigment operator
    Bot(Bot &&source);//move constructor
    Bot &operator=(Bot &&source);//move assigment operator
    ~Bot();
    void move() override;
    void frameTime(const double &frame_time);
private:
    void updateSnake() override;
    double _frame_time;
    std::shared_ptr<LTexture> _head,_tail;
    SDL_Rect _clip;
};


#endif //SNAKER_BOT_H
