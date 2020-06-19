//
// Created by lukasz on 12.06.2020.
//

#ifndef SNAKER_GAME_H
#define SNAKER_GAME_H
#include "../main.h"
#include "Player.h"
#include "PowerUp.h"


class Game{
public:
    Game(const std::shared_ptr<Win> &window, const std::shared_ptr<LTexture> &background_texture,
         const std::shared_ptr<LTexture> &player_head, const std::shared_ptr<LTexture> &player_tail,
         const std::shared_ptr<LTexture> &bot_head, const std::shared_ptr<LTexture> &bot_tail,
         const std::shared_ptr<LTexture> &fruit, const std::shared_ptr<LTexture> &poweup_textures,
         const std::shared_ptr<Timer> &timer);
    bool setLevelSize(const int& width=13660, const int& height=7200);
    void render();
    void eventHandler(SDL_Event &event);
private:
    void centerCameraOnMouse();
    void renderHUD();
    void renderLevelBackground();
    void generateBackground();
    std::shared_ptr<Win> _window;
    std::shared_ptr<LTexture> _player_head, _player_tail, _background_texture,
        _bot_head, _bot_tail, _fruit, _powerup_textures;
    std::shared_ptr<Timer> _timer;
    int _level_width, _level_height;
    std::vector<std::unique_ptr<Tile>> _background;
    SDL_Rect _camera;
    SDL_Point  _mouse;
};


#endif //SNAKER_GAME_H
