//
// Created by lukasz on 12.06.2020.
//

#include "Game.h"

Game::Game(const std::shared_ptr<Win> &window, const std::shared_ptr<LTexture> &background_texture,
           const std::shared_ptr<LTexture> &player_head, const std::shared_ptr<LTexture> &player_tail,
           const std::shared_ptr<LTexture> &bot_head, const std::shared_ptr<LTexture> &bot_tail,
           const std::shared_ptr<LTexture> &fruit, const std::shared_ptr<LTexture> &poweup_textures,
           const std::shared_ptr<Timer> &timer, const std::shared_ptr<Settings> &settings, TTF_Font *font,
           const int &text_size):
    _window(window), _background_texture(background_texture), _player_head(player_head),
    _player_tail(player_tail), _bot_head(bot_head), _bot_tail(bot_tail),
    _fruit(fruit), _powerup_textures(poweup_textures), _timer(timer), _settings(settings),
    _font(font), _text_size(text_size){
    _camera={0,0,_window->getWidth(),_window->getHeight()};
    _powerups_count=_settings->settingsFromFile()[2]*_settings->settingsFromFile()[3]/100;
    
//    _camera->x={0};
//    _camera->y={0};
//    _camera->w={_window->getWidth()};
//    _camera->h={_window->getHeight()};
}

void Game::renderLevelBackground(){
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
        for(int i=0;i<_settings->settingsFromFile()[2]-_powerups_count;i++){
            int x=rand()%static_cast<int>(0.9*_level_width);
            int y=rand()%static_cast<int>(0.9*_level_height);
            _fruits.emplace_back(Fruit(_window,_level_width,_level_height));
            _fruits[i].setPosition(x,y);
        }
        for(int i=0;i<_powerups_count;i++){
            int x=(_level_width-20)*(rand()/RAND_MAX);
            int y=(_level_height-20)*(rand()/RAND_MAX);
            _powerups.emplace_back(PowerUp(_window,_level_width,_level_height));
            _powerups[i].setPosition(x,y);
        }
        TOTAL_FRUIT_SPRITES=5;
        int SPRITE_DIMS=20;
        for (int i = 0; i < TOTAL_FRUIT_SPRITES; i++) {
            for(int j=0;j<TOTAL_FRUIT_SPRITES;j++){
                _clip_fruit.emplace_back(SDL_Rect{i*SPRITE_DIMS, j*SPRITE_DIMS, SPRITE_DIMS, SPRITE_DIMS});
            }
        }
        for (int i = 0; i < TOTAL_FRUIT_SPRITES; i++) {
            _clip_powerup.emplace_back(SDL_Rect{i*SPRITE_DIMS, 5*SPRITE_DIMS, SPRITE_DIMS, SPRITE_DIMS});
        }
        generateBackground();
        return true;
    }
    std::cerr<<"Level dimensions have to be greater than 0!\n";
    return false;
    
}

void Game::render(){
    _timer->start();
    renderLevelBackground();
    for(int i=0;i<_fruits.size();i++){
        _fruits[i].renderDot(*_fruit,-1,-1,&_camera,&_clip_fruit[i&25]);
    }
    for(int i=0;i<_powerups.size();i++){
        _powerups[i].renderDot(*_powerup_textures,-1,-1,&_camera,&_clip_powerup[i&5]);
    }
    _player->render();
    renderHUD();
//    if(_timer->getSeconds<double>()<0.017){
//        std::this_thread::sleep_for(std::chrono::microseconds (15600-static_cast<int>(1000*_timer->getSeconds<double>())));
//    }
//    std::cerr<<1.0/_timer->getSeconds<double>()<<" FPS\n";
}

void Game::eventHandler(SDL_Event &event){
    _player->eventHandler(event);
}

void Game::renderHUD(){
    std::stringstream score, fps;
    score<<"Points: "<<_player->Length();
    _score.loadFromText(score.str(),SDL_Color{255,0,0},_font,_window);
    _score.setWidth(3*_score.getWidth());
    _score.setHeight(_text_size);
    fps<<1.0/_timer->getSeconds<double>()<<" FPS";
    _fps.loadFromText(fps.str(),SDL_Color{255,0,0},_font,_window);
    _fps.setWidth(3*_fps.getWidth());
    _fps.setHeight(_text_size);
    _score.render(0,0,_window);
    _fps.render((_window->getWidth()-_fps.getWidth())/2,0,_window);
}

void Game::generatePlayer(){
    _player=std::make_unique<Player>(_player_head, _player_tail,
                SDL_Point{_window->getWidth()/2, _window->getHeight()/2},
                _window, _settings, _level_size, _timer, _camera);
}
