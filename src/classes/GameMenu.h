//
// Created by lukasz on 06.06.2020.
//

#ifndef SNAKER_GAMEMENU_H
#define SNAKER_GAMEMENU_H

#include "../main.h"
#include "Button.h"

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
    const LTexture* _menu_backgroud;
};


#endif //SNAKER_GAMEMENU_H
