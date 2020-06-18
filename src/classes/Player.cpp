//
// Created by lukasz on 18.06.2020.
//

#include "Player.h"

Player::Player(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail,
               const SDL_Point &start_position, const std::shared_ptr<Win> &window,
               const std::shared_ptr<Settings> &settings,
               const std::shared_ptr<SDL_Rect> &level_size,
               const std::shared_ptr<Timer> &timer):Snake(start_position,window
                   ,settings,level_size,timer), _head(head), _tail(tail){
    _speed=_game_settings->settingsFromFile()[4];
}

void Player::move(){

}

void Player::render(){

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
