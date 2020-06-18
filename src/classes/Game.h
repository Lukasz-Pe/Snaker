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
    Game(std::shared_ptr<Win> window, std::shared_ptr<LTexture> player_head,
         std::shared_ptr<LTexture> player_tail, std::shared_ptr<LTexture> bot_head,
         std::shared_ptr<LTexture> bot_tail, std::shared_ptr<LTexture> fruit,
         std::shared_ptr<LTexture> tile, std::shared_ptr<LTexture> poweup_vision,
         std::shared_ptr<LTexture> powerup_snake_eater,
         std::shared_ptr<LTexture> powerup_ghost_mode,
         std::shared_ptr<LTexture> powerup_eating_distance,
         std::shared_ptr<LTexture> powerup_shield, std::shared_ptr<Timer> timer);
    void render();
    void eventHandler();
private:
    void renderHUD();
    void renderLevelBackground();
    std::shared_ptr<Win> _window;
    std::shared_ptr<LTexture> _player_head, _player_tail,
        _bot_head, _bot_tail, _fruit, _tile, _poweup_vision,
        _powerup_snake_eater, _powerup_ghost_mode,
        _powerup_eating_distance, _powerup_shield;
    std::shared_ptr<Timer> _timer;
};


#endif //SNAKER_GAME_H
