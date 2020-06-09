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
        _game_state=_mapping[23];
        return true;
    }
    return false;
}

void GameMenu::renderMainMenu(){
    if(_menu_level!=_mapping[23]){
        _menu_level=_mapping[23];
    }
    renderBackground();
    _game_title.render((_game_window->getWidth() - _game_title.getWidth()) / 2, 0, *_game_window);
    
    //Main menu options are at positions 0-4
    std::map<std::string,Button>::iterator it=_buttons.end();
    //renderButton(it->second, _game_window->getWidth()/2, _game_window->getHeight()/6+*_text_size);
    _menu_text.find(_mapping[23])->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[23])->second.getWidth()/2, _game_window->getHeight()/6+*_text_size,*_game_window);
    for(int i=0;i<4;i++){
        if(_played){
            if(i>1){
                i++;
                it=_buttons.find(_mapping[i]);
                i--;
            }else{
                it=_buttons.find(_mapping[i]);
            }
        }else{
            if(i>0){
                i++;
                it=_buttons.find(_mapping[i]);
                i--;
            }
        }
        if(it!=_buttons.end()){
            renderButton(it->second, _game_window->getWidth()/2, _game_window->getHeight()/3+i**_text_size);
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
    std::map<std::string,Button>::iterator it=_buttons.end();
    for(int i=0;i<4;i++){
        if(_played){
            if(i>1){
                i++;
                it=_buttons.find(_mapping[i]);
                i--;
            }else{
                it=_buttons.find(_mapping[i]);
            }
        }else{
            if(i>0){
                i++;
                it=_buttons.find(_mapping[i]);
                i--;
            }
        }
    if(event.type==SDL_MOUSEMOTION){
        SDL_GetMouseState(&_mouse_rect.x, &_mouse_rect.y);
            if(it!=_buttons.end()){
                it->second.eventHandler(event);
            }
        }
        if(checkCollision(it->second.getButtonDims(), _mouse_rect)){
        if(event.type==SDL_MOUSEBUTTONUP){
            _game_state=it->second.getButtonTextID();
        }
        }
    }
    if(_game_state==_mapping[2]){
        _played=true;
    }
    if(_game_state==_mapping[1]){
        _played=false;
    }
}
void GameMenu::renderText(std::string &text, const int &posX, const int &posY){

}

void GameMenu::renderButton(Button &btn, const int &posX, const int &posY){
    btn.render(posX-btn.getButtonDims().w/2, posY, *_game_window);
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

GameMenu::GameMenu(Win &window, TTF_Font *text, TTF_Font *title,
    const std::map<std::string, std::string> &translation,
    const int *text_size, const int *title_size):
    _played(true), _game_window(&window), _text_font(text),
    _title_font(title), _translation(translation),
    _text_size(text_size), _title_size(title_size){
    _game_title.loadFromText("Snaker", SDL_Color{255,0,0},_title_font,*_game_window);
    _game_title.setWidth(3*_game_title.getWidth());
    _game_title.setHeight(*_title_size);
    for(auto btn:_translation){
        if(btn.first.substr(0,4)=="btn_"){
            _buttons.emplace(btn.first,Button(btn.first,btn.second,*_game_window,_text_font,*_text_size));
        }
        if(btn.first.substr(0,5)=="text_"){
            LTexture text_texture;
            text_texture.loadFromText(btn.second,SDL_Color{255,0,0},_text_font,*_game_window);
            text_texture.setWidth(3*text_texture.getWidth());
            text_texture.setHeight(*_text_size);
            _menu_text.emplace(btn.first,std::move(text_texture));
        }
    }
}

std::string GameMenu::getGameState(){
    return _game_state;
}

std::vector<std::string> GameMenu::getMapping(){
    return _mapping;
}
