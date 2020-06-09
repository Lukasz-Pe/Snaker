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
        const int* text_size, const int* title_size);
    bool loadMappingFile(const std::string& path);
    void renderMainMenu();
    void renderOptionsScreen();
    void renderPauseDialogue();
    void renderExitDialogue();
    void eventHandler(SDL_Event &event);
    void setBackgroundTexture(LTexture background_texture);
    std::string getGameState();
    std::vector<std::string> getMapping();
private:
    void renderText(std::string& text, const int& posX, const int& posY);
    void renderButton(Button &btn, const int &posX, const int &posY);
    void renderBackground();
    std::string _path_to_mapping_file;
    std::fstream _mapping_file;
    std::vector<std::string> _mapping;
    std::map<std::string,std::string> _translation;
    std::map<std::string,Button> _buttons;
    bool _played;
    Win* _game_window;
    TTF_Font *_text_font, *_title_font;
    const int *_text_size, *_title_size;
    LTexture _menu_background_texture;
    std::vector<std::unique_ptr<Tile>> _menu_background;
    SDL_Rect _menu_camera, _mouse_rect{0,0,1,1};
    friend bool checkCollision(SDL_Rect a, SDL_Rect b);
    std::string _menu_level;
    std::string _game_state;
};


#endif //SNAKER_GAMEMENU_H
