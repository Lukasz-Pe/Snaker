//
// Created by lukasz on 18.06.2020.
//

#ifndef SNAKER_PLAYER_H
#define SNAKER_PLAYER_H
#include "Snake.h"

class Player:public Snake{
public:
    Player()=default;
    Player(std::shared_ptr<LTexture> &head, std::shared_ptr<LTexture> &tail, const SDL_Point &start_position,
           const std::shared_ptr<Win> &window, const std::shared_ptr<Settings> &settings, const SDL_Rect &level_size,
           SDL_Rect &camera, const std::shared_ptr<Timer> &timer);
    void render() override;
    void eventHandler(SDL_Event& event);
    SDL_Rect headAndBodyRects(const int &body_part) override;
    void addLength() override;
    Player(const Player &source);//copy constructor
    Player &operator=(const Player &source);//copy assigment operator
    Player(Player &&source);//move constructor
    Player &operator=(Player &&source);//move assigment operator
    ~Player();
    void move() override;
    void frameTime(const double &frame_time);
private:
    void updateSnake() override;
    double _frame_time;
    std::shared_ptr<LTexture> _head,_tail;
    SDL_Point _mouse_position;
    std::unique_ptr<SDL_Rect> _camera;
};


#endif //SNAKER_PLAYER_H
