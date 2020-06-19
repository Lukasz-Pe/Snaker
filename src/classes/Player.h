//
// Created by lukasz on 18.06.2020.
//

#ifndef SNAKER_PLAYER_H
#define SNAKER_PLAYER_H
#include "Snake.h"

class Player:public Snake{
public:
    Player()= default;;
    Player(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
           const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings,
           const SDL_Rect &level_size, const std::shared_ptr<Timer> &timer,
           const SDL_Rect &camera);
    void render() override;
    void eventHandler(SDL_Event& event);
    void updateSnake() override;
private:
    void move() override;
    std::shared_ptr<LTexture> _head,_tail;
    SDL_Point _mouse_position;
    int _speed;
    SDL_Rect _camera;
};


#endif //SNAKER_PLAYER_H
