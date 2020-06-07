//
// Created by lukasz on 06.06.2020.
//

#include "GameMenu.h"

bool GameMenu::loadMappingFile(const std::string &path){
    _path_to_mapping_file=path;
    _mapping_file.open(_path_to_mapping_file);
    if(_mapping_file.is_open()){
        std::string line;
        while(std::getline(_mapping_file, line)){
            _mapping.emplace_back(line);
        }
        return true;
    }
    return false;
}

void GameMenu::renderMainMenu(){
    renderBackground();
    //Main menu options are at positions 0-4
    for(int i=0;i<5;i++){
        auto it=_translation.find(_mapping[i]);
        if(it!=_translation.end()){
            renderButton(it->second,_game_window->getWidth() / 2, _game_window->getHeight() / 3 + i**_text_size,false);
        }
    }
}

void GameMenu::renderOptionsScreen(){

}

void GameMenu::renderPauseDialogue(){

}

void GameMenu::renderExitDialogue(){

}

void GameMenu::eventHandler(SDL_Event &event){
    while(SDL_PollEvent(&event)){
    
    }
}

void GameMenu::renderText(std::string &text, const int &posX, const int &posY){

}

void GameMenu::renderButton(std::string &text, const int &posX, const int &posY, const bool &activated){
    Button menu_button;
    //TODO add collision checking
    menu_button.setButtonText(text,*_game_window,_text_font,*_text_size);
    menu_button.render(posX-menu_button.getButtonDims().w/2, posY, *_game_window);
}

void GameMenu::renderBackground(){
    _menu_camera={0,0,_game_window->getWidth(),_game_window->getHeight()};
    for(int i=0;i<_menu_background.size();i++){
        _menu_background[i]->render(_menu_camera,*_game_window,_menu_background_texture);
    }
}

void GameMenu::setBackgroundTexture(LTexture background_texture){
    _menu_background_texture=std::move(background_texture);
    int tile_count_x{2*(_game_window->getWidth()/_menu_background_texture.getWidth())},
        tile_count_y{2*(_game_window->getHeight()/_menu_background_texture.getHeight())};
    for(int i=0;i<tile_count_y;i++){
        for(int j=0;j<tile_count_x;j++){
            _menu_background.emplace_back(std::make_unique<Tile>(j*_menu_background_texture.getWidth(),
                i*_menu_background_texture.getHeight(),_menu_background_texture.getWidth(),
                _menu_background_texture.getHeight()));
        }
    }
}
