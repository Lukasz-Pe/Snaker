//
// Created by lukasz on 06.06.2020.
//

#ifndef SNAKER_GAMEMENU_H
#define SNAKER_GAMEMENU_H

#include "../main.h"
#include "Button.h"
#include "Tile.h"
class GameMenu{
public:
    GameMenu(Win& window, TTF_Font* text, TTF_Font* title,
        const std::map<std::string,std::string>& translation,
        const int* text_size, const int* title_size):
        _played(false), _game_window(&window), _text_font(text),
        _title_font(title), _translation(translation),
        _text_size(text_size), _title_size(title_size){};
    bool loadMappingFile(const std::string& path);
    void renderMainMenu();
    void renderOptionsScreen();
    void renderPauseDialogue();
    void renderExitDialogue();
    void eventHandler(SDL_Event &event);
    void setBackgroundTexture(LTexture background_texture);
private:
    void renderText(std::string& text, const int& posX, const int& posY);
    void renderButton(std::string& text, const int& posX, const int& posY, const bool& activated);
    void renderBackground();
    std::string _path_to_mapping_file;
    std::fstream _mapping_file;
    std::vector<std::string> _mapping;
    std::map<std::string,std::string> _translation;
    bool _played;
    Win* _game_window;
    TTF_Font *_text_font, *_title_font;
    const int *_text_size, *_title_size;
    LTexture _menu_background_texture;
    std::vector<std::unique_ptr<Tile>> _menu_background;
    SDL_Rect _menu_camera, _mouse_rect{0,0,1,1};
    friend bool checkCollision(SDL_Rect a, SDL_Rect b);
};


#endif //SNAKER_GAMEMENU_H
