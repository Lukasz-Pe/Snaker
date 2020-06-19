//
// Created by lukasz on 12.06.2020.
//

#include "Game.h"

Game::Game(const std::shared_ptr<Win> &window, const std::shared_ptr<LTexture> &background_texture,
           const std::shared_ptr<LTexture> &player_head, const std::shared_ptr<LTexture> &player_tail,
           const std::shared_ptr<LTexture> &bot_head, const std::shared_ptr<LTexture> &bot_tail,
           const std::shared_ptr<LTexture> &fruit, const std::shared_ptr<LTexture> &poweup_textures,
           const std::shared_ptr<Timer> &timer, const std::shared_ptr<Settings> &settings):
    _window(window), _background_texture(background_texture), _player_head(player_head),
    _player_tail(player_tail), _bot_head(bot_head), _bot_tail(bot_tail),
    _fruit(fruit), _powerup_textures(poweup_textures), _timer(timer), _settings(settings){
    _camera={0,0,_window->getWidth(), _window->getHeight()};
    _mouse={0,0};
}

void Game::renderLevelBackground(){
    _camera.x=_mouse.x+_camera.w/2;
    _camera.y=_mouse.y+_camera.h/2;
    for(int i=0;i<_background.size();i++){
        _background[i]->render(_camera,_window,*_background_texture);
    }
}

void Game::generateBackground(){
    int tile_count_width{1*(_level_width/_background_texture->getWidth())},
    tile_count_height{1*(_level_height/_background_texture->getHeight())};
    for(int i=0;i<=tile_count_height;i++){
        for(int j=0;j<=tile_count_width;j++){
            _background.emplace_back(std::make_unique<Tile>(j*_background_texture->getWidth(),
                                    i*_background_texture->getHeight(),_background_texture->getWidth(),
                                    _background_texture->getHeight()));
        }
    }
}

bool Game::setLevelSize(const int &width, const int &height){
    if(width>0&&height>0){
        _level_width=width;
        _level_height=height;
        _level_size={0,0,_level_width,_level_height};
//        _level_size->x=0;
//        _level_size->y=0;
//        _level_size->w=_level_width;
//        _level_size->h=_level_height;
        generateBackground();
        return true;
    }
    std::cerr<<"Level dimensions have to be greater than 0!\n";
    return false;
    
}

void Game::render(){
    renderLevelBackground();
    _player.render();
}

void Game::eventHandler(SDL_Event &event){
    if(event.type==SDL_MOUSEMOTION&&_window->hasMouseFocus()){
        SDL_GetMouseState(&_mouse.x,&_mouse.y);
    }
}

void Game::renderHUD(){

}

void Game::centerCameraOnMouse(){
}

void Game::generatePlayer(){
    _player=Player(_player_head, _player_tail, SDL_Point{_window->getWidth()/2, _window->getHeight()/2},
                   _window, _settings, _level_size, _timer, _camera);
}
