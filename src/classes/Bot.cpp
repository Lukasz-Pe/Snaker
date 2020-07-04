//
// Created by lukasz on 03.07.2020.
//

#include <algorithm>
#include "Bot.h"

Bot::Bot(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
         const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
         SDL_Rect &clip, const std::shared_ptr<Timer> &timer, const std::vector<Fruit> &fruits,
         const std::vector<PowerUp> &powerups, const double &frame_time, const SDL_Rect &camera):
         Snake(start_position, window, settings, level_size, timer),
         _head(head), _tail(tail), _clip(clip), _fruits(fruits), _powerups(powerups),
         _frame_time(frame_time), _camera(camera)
         {
    _target={0, 0};
    _body.emplace_back(SnakeBody::Coordinates{static_cast<double>(start_position.x),
                                              static_cast<double>(start_position.y),0.0});
}

Bot::Bot(const Bot &source):Snake(source), _fruits(source._fruits), _powerups(source._powerups),
                            _frame_time(source._frame_time), _camera(source._camera){
    _target=source._target;
    _head=source._head;
    _tail=source._tail;
    _clip=source._clip;
}

Bot &Bot::operator=(const Bot &source){
    if(this==&source){
        return *this;
    }
    Snake::operator=(source);
    _target=source._target;
    _head=source._head;
    _tail=source._tail;
    _clip=source._clip;
    return *this;
}

Bot::Bot(Bot &&source):Snake(source), _fruits(source._fruits), _powerups(source._powerups),
                       _frame_time(source._frame_time), _camera(source._camera){
    _target=source._target;
    _head=std::move(source._head);
    _tail=std::move(source._tail);
    _clip=source._clip;
    source._clip={0,0,0,0};
}

Bot &Bot::operator=(Bot &&source){
    if(this==&source){
        return *this;
    }
    _target=source._target;
    Snake::operator=(std::move(source));
    _head=std::move(source._head);
    _tail=std::move(source._tail);
    _clip=source._clip;
    source._clip={0,0,0,0};
    return *this;
}

void Bot::render(){
    if(_body.size()>1){
        for(unsigned long i=_body.size()-1;i>1;i--){
            _tail->render(static_cast<int>(_body[i]._x) - _camera.x, static_cast<int>(_body[i]._y) - _camera.y, _window, &_clip, NULL, _body[i]._angle);
        }
    }
    _head->render(static_cast<int>(_body[0]._x) - _camera.x, static_cast<int>(_body[0]._y) - _camera.y, _window, &_clip, NULL, _body[0]._angle);
}

SDL_Rect Bot::headAndBodyRects(const int &body_part){
    if(body_part>=0&&body_part<_body.size()){
        if(body_part==0){
            return SDL_Rect{static_cast<int>(_body[body_part]._x),
                            static_cast<int>(_body[body_part]._y),
                            _head->getWidth()/5,
                            _head->getHeight()/5};
        }
        if(body_part>0){
            return SDL_Rect{static_cast<int>(_body[body_part]._x),
                            static_cast<int>(_body[body_part]._y),
                            _tail->getWidth()/5,
                            _tail->getHeight()/5};
        }
    }
    return SDL_Rect{0,0,0,0};
}

void Bot::move(){
    eatingDistance();
    if(_target._x>=0&&_target._y>=0){
        _body[0]._angle=(180/M_PI)*std::atan(_target._x/_target._y);
    }else if(_target._x>0&&_target._y<0){
        _body[0]._angle=180+(180/M_PI)*std::atan(_target._x/_target._y);
    }else if(_target._x<0&&_target._y>0){
        _body[0]._angle=(180/M_PI)*std::atan(_target._x/_target._y);
    }else if(_target._x<0&&_target._y<0){
        _body[0]._angle=180+(180/M_PI)*std::atan(_target._x/_target._y);
    }
    _previous_position._x=_body[0]._x;
    _previous_position._y=_body[0]._y;
    _previous_position._angle=_body[0]._angle;
    updateSnake();
    if ((_body[0]._y + this->headAndBodyRects(0).h) > _level_size.h) {
        _body[0]._y = _level_size.h - this->headAndBodyRects(0).h;
        this->findNewTarget();
    }
    if ( _body[0]._y < 1) {
        _body[0]._y = 1;
        this->findNewTarget();
    }
    if (( _body[0]._x + this->headAndBodyRects(0).w) > _level_size.w) {
        _body[0]._x = _level_size.w - this->headAndBodyRects(0).w;
        this->findNewTarget();
    }
    if (_body[0]._x < 1) {
        _body[0]._x = 1;
        this->findNewTarget();
    }
    _body[0]._x+=_speed*sin(_body[0]._angle * (M_PI / 180.0))*_frame_time;
    _body[0]._y-=_speed*cos(_body[0]._angle * (M_PI / 180.0))*_frame_time;
}



template<typename T>
TargetPosition Bot::calculateNearestTargetPosition(std::vector<T> fruit){
    std::vector<double> distance;
    for(unsigned int i=0;i<fruit.size();i++){
        distance.emplace_back(
            sqrt(pow((this->headCoordinates()._x - fruit[i].getPosX()), 2) + pow((this->headCoordinates()._y - fruit[i].getPosY()), 2))
        );
    }
    int min_index=std::min_element(distance.begin(),distance.end())-distance.begin();
    return TargetPosition(fruit[min_index].getPosX(),fruit[min_index].getPosY());
}

void Bot::findNewTarget(){
    TargetPosition target_fruit{calculateNearestTargetPosition(_fruits)},
        target_powerup{calculateNearestTargetPosition(_powerups)};
    if(sqrt(pow((this->headCoordinates()._x - target_fruit._x), 2) + pow((this->headCoordinates()._y - target_fruit._y), 2))
       <sqrt(pow((this->headCoordinates()._x - target_powerup._x), 2) + pow((this->headCoordinates()._y - target_powerup._y), 2))
        ){
        _target={target_fruit._x-_body[0]._x,_body[0]._y-target_fruit._y};
    }else{
        _target={target_powerup._x-_body[0]._x,_body[0]._y-target_powerup._y};
    }
}
