//
// Created by lukasz on 12.06.2020.
//

#ifndef SNAKER_GAME_H
#define SNAKER_GAME_H
#include "../main.h"
#include "../functions/moveFruit.cpp"
#include "../functions/collisions.h"
#include "Player.h"
#include "PowerUp.h"
#include "Bot.h"


class Game{
public:
    Game(const std::shared_ptr<Win> &window, const std::shared_ptr<LTexture> &background_texture,
         const std::shared_ptr<LTexture> &player_head, const std::shared_ptr<LTexture> &player_tail,
         const std::shared_ptr<LTexture> &bot_head, const std::shared_ptr<LTexture> &bot_tail,
         const std::shared_ptr<LTexture> &fruit, const std::shared_ptr<LTexture> &powerup_textures,
         const std::shared_ptr<Settings> &settings, TTF_Font *font, const int &text_size,
         const std::shared_ptr<Timer> &timer);
    bool setLevelSize(const int& width=13660, const int& height=7200);
    void recalculateVariables();
    void render();
    void eventHandler(SDL_Event &event);
    void generatePlayers();
    void moveFruitsAndPowerUps();
    void checkCollisionsWithFruitsAndPowerUps();
    void resetGame();
    void FPS(const double &fps);
    void frameTime(const double &frame_time);
private:
    void renderHUD();
    void renderLevelBackground();
    void generateBackground();
    double _fps, _frame_time;
    std::shared_ptr<Win> _window;
    std::shared_ptr<Settings> _settings;
    std::shared_ptr<LTexture> _player_head, _player_tail, _background_texture,
        _bot_head, _bot_tail, _fruit, _powerup_textures;
    LTexture _score_texture, _fps_texture, _powerup_timer;
    std::vector<Fruit> _fruits;
    std::vector<PowerUp> _powerups;
    std::shared_ptr<Timer> _timer;
    int _level_width, _level_height, _powerups_count;
    const int &_text_size;
    std::vector<std::unique_ptr<Tile>> _background;
    SDL_Rect _camera, _level_size, _powerup_hud_clip;
    std::vector<SDL_Rect> _clip_bot_and_fruit,_clip_powerup;
    Player _player;
    std::vector<Bot> _bot;
    TTF_Font *_font;
    int TOTAL_FRUIT_SPRITES, TOTAL_POWERUP_SPRITES;
    template<typename S,typename F> friend void changeFruitPosition(S &snake, F &fruit);
    friend bool checkCollision(SDL_Rect a, SDL_Rect b);
};


#endif //SNAKER_GAME_H
