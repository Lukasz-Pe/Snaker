//
// Created by lukasz on 03.07.2020.
//

#include "Bot.h"

Bot::Bot(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
         const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
         SDL_Rect &clip, const std::shared_ptr<Timer> &timer):
         Snake(start_position, window, settings, level_size, timer),
         _head(head), _tail(tail), _clip(clip)
         {
    _frame_time=0;
}

Bot::Bot(const Bot &source):Snake(source){
    _head=source._head;
    _tail=source._tail;
    _clip=source._clip;
    _speed=source._speed;
}

Bot &Bot::operator=(const Bot &source){
    if(this==&source){
        return *this;
    }
    Snake::operator=(source);
    _head=source._head;
    _tail=source._tail;
    _clip=source._clip;
    _speed=source._speed;
    return *this;
}

Bot::Bot(Bot &&source):Snake(source){
    _head=std::move(source._head);
    _tail=std::move(source._tail);
    _clip=source._clip;
    source._clip={0,0,0,0};
}

Bot &Bot::operator=(Bot &&source){
    if(this==&source){
        return *this;
    }
    Snake::operator=(std::move(source));
    _head=std::move(source._head);
    _tail=std::move(source._tail);
    _clip=source._clip;
    source._clip={0,0,0,0};
    return *this;
}

Bot::~Bot(){

}

void Bot::render(){

}

SDL_Rect Bot::headAndBodyRects(const int &body_part){
    return SDL_Rect();
}

void Bot::addLength(){

}

void Bot::move(){

}

void Bot::frameTime(const double &frame_time){

}

void Bot::updateSnake(){

}
