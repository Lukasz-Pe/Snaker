//
// Created by lukasz on 06.06.2020.
//

#ifndef SNAKER_GAMEMENU_H
#define SNAKER_GAMEMENU_H

#include "../main.h"
#include "Button.h"
#include "Tile.h"
#include "Settings.h"
class GameMenu{
public:
    GameMenu(Win& window, TTF_Font* text, TTF_Font* title,
        const std::map<std::string,std::string>& translation,
        const int* text_size, const int* title_size, Settings *game_settings);
    bool loadMappingFile(const std::string& path);
    void renderMainMenu();
    void renderOptionsScreen();
    void renderPauseDialogue();
    void renderExitDialogue();
    void eventHandler(SDL_Event &event);
    void setBackgroundTexture(LTexture background_texture);
    void renderInfo();
    std::string getGameState();
    std::vector<std::string> getMapping();
private:
    void renderButton(Button &btn, const int &posX, const int &posY);
    void renderBackground();
    void renderGameTitle();
    void saveSettings();
    std::string _path_to_mapping_file;
    std::fstream _mapping_file;
    std::vector<std::string> _mapping, _btn_value_change_mapping;
    std::map<std::string,std::string> _translation;
    std::map<std::string,Button> _buttons, _btn_value_change;
    bool _played;
    Win* _game_window;
    TTF_Font *_text_font, *_title_font;
    const int *_text_size, *_title_size;
    LTexture _menu_background_texture,_game_title;
    std::vector<std::unique_ptr<Tile>> _menu_background;
    SDL_Rect _menu_camera, _mouse_rect{0,0,1,1}, _dialog_box{0,0,0,0};
    friend bool checkCollision(SDL_Rect a, SDL_Rect b);
    std::string _game_state, _tmp_state;
    std::map<std::string, LTexture> _menu_text;
    Settings *_game_settings;
    std::vector<int> _settings_values;
};


#endif //SNAKER_GAMEMENU_H
