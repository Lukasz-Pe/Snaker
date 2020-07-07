//
// Created by lukasz on 18.06.2020.
//

#include "Player.h"

#include <memory>

Player::Player(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
               const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings,
               const SDL_Rect &level_size,
               SDL_Rect &camera, const std::shared_ptr<Timer> &timer):
               Snake(start_position, window, settings, level_size, timer), _head(head), _tail(tail), _camera(&camera){
    _body.emplace_back(SnakeBody::Coordinates{static_cast<double>(_camera->w/2),static_cast<double>(_camera->h/2),0.0});
    _old_length=_body.size();
    _frame_time=0.0;
}

void Player::move(){
    eatingDistance();
    double target_x{static_cast<double>((_mouse_position.x+_camera->x)-_body[0]._x)},
    target_y{static_cast<double>(_body[0]._y-(_mouse_position.y+_camera->y))};
    if (target_x >= 0 && target_y >= 0) {
        _body[0]._angle = (180 / M_PI) * std::atan(target_x/target_y);
    } else if (target_x > 0 && target_y < 0) {
        _body[0]._angle = 180 + (180 / M_PI) * std::atan(target_x/target_y);
    } else if (target_x < 0 && target_y >= 0) {
        _body[0]._angle = (180 / M_PI) * std::atan(target_x/target_y);
    } else if (target_x < 0 && target_y < 0) {
        _body[0]._angle = 180 + (180 / M_PI) * std::atan(target_x/target_y);
    }
    _previous_position._x=_body[0]._x;
    _previous_position._y=_body[0]._y;
    _previous_position._angle=_body[0]._angle;
    updateSnake();
    _body[0]._x+=_speed*sin(_body[0]._angle * (M_PI / 180.0))*_frame_time;
    _body[0]._y-=_speed*cos(_body[0]._angle * (M_PI / 180.0))*_frame_time;
    if ((_body[0]._y + _head->getHeight()) > _level_size.h) {
        _body[0]._y = _level_size.h - _head->getHeight();
    }
    if ( _body[0]._y < 1) {
        _body[0]._y = 1;
    }
    if (( _body[0]._x + _head->getWidth()) > _level_size.w) {
        _body[0]._x = _level_size.w - _head->getWidth();
    }
    if (_body[0]._x < 1) {
        _body[0]._x = 1;
    }
}

void Player::render(){
    _camera->x = _body[0]._x - _camera->w/2;
    _camera->y = _body[0]._y - _camera->h/2;
    if (_camera->x < 0) {
        _camera->x = 0;
    }
    if (_camera->y < 0) {
        _camera->y = 0;
    }
    if (_camera->x > _level_size.w - _camera->w) {
        _camera->x = _level_size.w - _camera->w;
    }
    if (_camera->y > _level_size.h - _camera->h) {
        _camera->y = _level_size.h - _camera->h;
    }
    if(_body.size()>1){
        for(unsigned long i=_body.size()-1;i>1;i--){
            _tail->render(static_cast<int>(_body[i]._x) - _camera->x, static_cast<int>(_body[i]._y) - _camera->y, _window, NULL, NULL, _body[i]._angle);
        }
    }
    _head->render(static_cast<int>(_body[0]._x) - _camera->x, static_cast<int>(_body[0]._y) - _camera->y, _window, NULL, NULL, _body[0]._angle);
}

void Player::eventHandler(SDL_Event &event){
    if(_window->hasMouseFocus()){
        if(event.type==SDL_MOUSEMOTION){
            SDL_GetMouseState(&_mouse_position.x,&_mouse_position.y);
        }
        if(event.type==SDL_MOUSEBUTTONDOWN){
            _speed=2*_game_settings->settingsFromFile()[4];
        }
        if(event.type==SDL_MOUSEBUTTONUP){
            _speed=_game_settings->settingsFromFile()[4];
        }
    }
}

Player::~Player(){
    _camera.release();
}

Player::Player(const Player &source):Snake(source){
    _head=source._head;
    _tail=source._tail;
    _camera=std::make_unique<SDL_Rect>(*source._camera);
}

Player &Player::operator=(const Player &source){
    if(this==&source){
        return *this;
    }
    Snake::operator=(source);
    _head=source._head;
    _tail=source._tail;
    _camera=std::make_unique<SDL_Rect>(*source._camera);
    return *this;
}

Player::Player(Player &&source):Snake(source){
    _head=std::move(source._head);
    _tail=std::move(source._tail);
    _camera=std::move(source._camera);
    if(source._camera!=nullptr){
        source._camera.release();
    }
}

Player &Player::operator=(Player &&source){
    if(this==&source){
        return *this;
    }
    Snake::operator=(std::move(source));
    _head=std::move(source._head);
    _tail=std::move(source._tail);
    _camera=std::move(source._camera);
    if(source._camera!=nullptr){
        source._camera.release();
    }
    return *this;
}

SDL_Rect Player::headAndBodyRects(const int &body_part){
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

void Player::frameTime(const double &frame_time){
    _frame_time=frame_time;
}
