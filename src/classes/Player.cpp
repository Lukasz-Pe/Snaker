//
// Created by lukasz on 18.06.2020.
//

#include "Player.h"

Player::Player(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
               const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings,
               const SDL_Rect &level_size, const std::shared_ptr<Timer> &timer,
               SDL_Rect &camera):Snake(start_position, window
                   , settings, level_size, timer), _head(head), _tail(tail), _camera(&camera){
    _speed=_game_settings->settingsFromFile()[4];
    _body.emplace_back(SnakeBody::Coordinates{static_cast<double>(_camera->w/2),static_cast<double>(_camera->h/2),0.0});
}

void Player::move(){
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
    _body[0]._x+=_speed*sin(_body[0]._angle * (M_PI / 180.0))*_timer->getSeconds<double>();
    _body[0]._y-=_speed*cos(_body[0]._angle * (M_PI / 180.0))*_timer->getSeconds<double>();
//    if (mTailLength > 0) {
//        updateTail(vTexTail);
//    }
    if ((_body[0]._y + _head->getHeight()) > _level_size.h) {
        _body[0]._y = _level_size.h - _head->getHeight();
    }
    if ( _body[0]._y < 0) {
        _body[0]._y = 0;
    }
    if (( _body[0]._x + _head->getWidth()) > _level_size.w) {
        _body[0]._x = _level_size.w - _head->getWidth();
    }
    if (_body[0]._x < 0) {
        _body[0]._x = 0;
    }
}

void Player::render(){
    move();
    _camera->x = _body[0]._x - _camera->w/2;
    _camera->y = _body[0]._y - _camera->h/2;
//    std::cerr<<"player:"<<_camera->x<<"x"<<_camera->y<<"\n";
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
    _head->render(static_cast<int>(_body[0]._x) - _camera->x, static_cast<int>(_body[0]._y) - _camera->y, _window, NULL, NULL, _body[0]._angle);
//    mHeadBox= {(int)_body[0]._x,_body[0]._y, _head->getWidth(),_head->getHeight()};
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

void Player::updateSnake(){

}

Player::~Player(){
    _camera.release();
}

