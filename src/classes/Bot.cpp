//
// Created by lukasz on 03.07.2020.
//

#include "Bot.h"

Bot::Bot(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
         const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
         SDL_Rect &clip, const std::shared_ptr<Timer> &timer, const std::vector<Fruit> &fruits,
         const std::vector<PowerUp> &powerups, const double &frame_time, const SDL_Rect &camera):
         Snake(start_position, window, settings, level_size, timer),
         _head(head), _tail(tail), _clip(clip), _fruits(fruits), _powerups(powerups),
         _frame_time(frame_time), _camera(camera)
         {
             _body.emplace_back(SnakeBody::Coordinates{static_cast<double>(start_position.x),
                                                       static_cast<double>(start_position.y),0.0});
}

Bot::Bot(const Bot &source):Snake(source), _fruits(source._fruits), _powerups(source._powerups),
                            _frame_time(source._frame_time), _camera(source._camera){
    _head=source._head;
    _tail=source._tail;
    _clip=source._clip;
}

Bot &Bot::operator=(const Bot &source){
    if(this==&source){
        return *this;
    }
    Snake::operator=(source);
    _head=source._head;
    _tail=source._tail;
    _clip=source._clip;
    return *this;
}

Bot::Bot(Bot &&source):Snake(source), _fruits(source._fruits), _powerups(source._powerups),
                       _frame_time(source._frame_time), _camera(source._camera){
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
                            _head->getWidth(),
                            _head->getHeight()};
        }
        if(body_part>0){
            return SDL_Rect{static_cast<int>(_body[body_part]._x),
                            static_cast<int>(_body[body_part]._y),
                            _tail->getWidth(),
                            _tail->getHeight()};
        }
    }
    return SDL_Rect{0,0,0,0};
}

void Bot::addLength(){
    _body.emplace_back(_body[_body.size()-1]);
}

void Bot::move(){
//    TargetPosition target_fruit{calculateNearestTargetPosition(_fruits)},
//    target_powerup{calculateNearestTargetPosition(_powerups)},
//    target(_level_size.w/2,_level_size.h/2);
//    target=target_fruit;
//    if(sqrt(pow((this->headCoordinates()._x - target_fruit._x), 2) + pow((this->headCoordinates()._y - target_fruit._y), 2))
//        <sqrt(pow((this->headCoordinates()._x - target_powerup._x), 2) + pow((this->headCoordinates()._y - target_powerup._y), 2))
//        ){
//        target=target_fruit;
//    }else{
//        target=target_powerup;
//    }
//    if (target._x >= 0 && target._y >= 0) {
//        _body[0]._angle = (180 / M_PI) * std::atan(target._x/target._y);
//    } else if (target._x > 0 && target._y < 0) {
//        _body[0]._angle = 180 + (180 / M_PI) * std::atan(target._x/target._y);
//    } else if (target._x < 0 && target._y >= 0) {
//        _body[0]._angle = (180 / M_PI) * std::atan(target._x/target._y);
//    } else if (target._x < 0 && target._y < 0) {
//        _body[0]._angle = 180 + (180 / M_PI) * std::atan(target._x/target._y);
//    }
    _previous_position._x=_body[0]._x;
    _previous_position._y=_body[0]._y;
    _previous_position._angle=_body[0]._angle;
    updateSnake();
    _body[0]._x+=_speed*sin(_body[0]._angle * (M_PI / 180.0))*_frame_time;
    _body[0]._y-=_speed*cos(_body[0]._angle * (M_PI / 180.0))*_frame_time;
    if ((_body[0]._y + _head->getHeight()) > _level_size.h) {
        _body[0]._y = _level_size.h - _head->getHeight();
        if(_body[0]._angle>=90&&_body[0]._angle<180){
            _body[0]._angle=180-_body[0]._angle;
        }
        if(_body[0]._angle<270&&_body[0]._angle>=180){
            _body[0]._angle=540-_body[0]._angle;
        }
    }
    if ( _body[0]._y < 1) {
        _body[0]._y = 1;
        if(_body[0]._angle>=0&&_body[0]._angle<90){
            _body[0]._angle=180-_body[0]._angle;
        }
        if(_body[0]._angle<360&&_body[0]._angle>=270){
            _body[0]._angle=540-_body[0]._angle;
        }
    }
    if (( _body[0]._x + _head->getWidth()) > _level_size.w) {
        _body[0]._x = _level_size.w - _head->getWidth();
        if(_body[0]._angle>=90&&_body[0]._angle<180){
            _body[0]._angle=360.0-_body[0]._angle;
        }
        if(_body[0]._angle>=0&&_body[0]._angle<90){
            _body[0]._angle=360-_body[0]._angle;
        }
    }
    if (_body[0]._x < 1) {
        _body[0]._x = 1;
        if(_body[0]._angle>=180&&_body[0]._angle<270){
            _body[0]._angle=360-_body[0]._angle;
        }
        if(_body[0]._angle>=270&&_body[0]._angle<360){
            _body[0]._angle=360-_body[0]._angle;
        }
    }
}

void Bot::updateSnake(){
    if(_body.size()>1){
        _body[1]=_previous_position;
        for(int i=_body.size()-1;i>1;i--){
            _body[i]=_body[i-1];
        }
    }
}

template<typename T>
TargetPosition Bot::calculateNearestTargetPosition(std::vector<T> fruit){
    unsigned int min_index{0};
    std::vector<double> distance;
    for(unsigned int i=0;i<fruit.size();i++){
        distance.emplace_back(
            sqrt(pow((this->headCoordinates()._x - fruit[i].getPosX()), 2) + pow((this->headCoordinates()._y - fruit[i].getPosY()), 2))
        );
    }
    for(int i=0;i<distance.size();i++){
        for(int j=0;j<distance.size();j++){
            if(distance[i]<distance[j]){
                min_index=i;
            }
        }
    }
    return TargetPosition(fruit[min_index].getPosX(),fruit[min_index].getPosY());
}
