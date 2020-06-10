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
        for(auto btn:_translation){
            if(btn.first.substr(0, 4)=="btn_"&&
               (btn.first=="btn_increment"||btn.first=="btn_decrement"||btn.first=="btn_next"||btn.first=="btn_prev")){
                if(btn.first=="btn_next"||btn.first=="btn_prev"){
                    _btn_value_change_mapping.emplace_back(btn.first);
                _btn_value_change.emplace(btn.first,
                                          Button(btn.first, btn.second, *_game_window, _text_font, *_text_size, false));
                }
                if(btn.first=="btn_decrement"||btn.first=="btn_increment"){
                    for(int i=0; i<4; i++){
                        _btn_value_change_mapping.emplace_back(btn.first+"_"+_mapping[i+8]);
                        _btn_value_change.emplace(btn.first+"_"+_mapping[i+8],
                                                  Button(btn.first+"_"+_mapping[i+8], btn.second, *_game_window, _text_font,
                                                         *_text_size, false));
                    }
                }
            }
        }
        return true;
    }
    return false;
}

void GameMenu::renderMainMenu(){
    int position_to_draw=23;
    renderBackground();
    renderGameTitle();
    //Main menu options are at positions 0-4
    std::map<std::string,Button>::iterator it=_buttons.end();
    //renderButton(it->second, _game_window->getWidth()/2, _game_window->getHeight()/6+*_text_size);
    _menu_text.find(_mapping[position_to_draw])->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[position_to_draw])->second.getWidth()/2, _game_window->getHeight()/6+*_text_size,*_game_window);
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
    int position_to_draw=22;
    renderBackground();
    renderGameTitle();
    _menu_text.find(_mapping[position_to_draw])->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[position_to_draw])->second.getWidth()/2, _game_window->getHeight()/6+*_text_size,*_game_window);
    auto text_it=_menu_text.end();
    auto btn_it=_buttons.end();
    for(int i=7;i<12;i++){
        text_it=_menu_text.find(_mapping[i]);
        text_it->second.render(_game_window->getWidth()/3, _game_window->getHeight()/3+(i-7)**_text_size, *_game_window);
        LTexture option_text;
        if(i==7){
            option_text.loadFromText(_game_settings->availableTranslations()[_game_settings->settingsFromFile()[i-7]], SDL_Color{255, 0, 0},
                                     _text_font, *_game_window);
        }else{
            option_text.loadFromText(std::to_string(_game_settings->settingsFromFile()[i-7]), SDL_Color{255, 0, 0},
                                     _text_font, *_game_window);
        }
        option_text.setWidth(2*option_text.getWidth());
        option_text.setHeight(*_text_size);
        option_text.render((_game_window->getWidth()/6)*3,(_game_window->getHeight()/3)+(i-7)**_text_size,*_game_window);
        if(i==7){
            renderButton(_btn_value_change[_btn_value_change_mapping[9]],option_text.getPosX()-_btn_value_change[_btn_value_change_mapping[9]].getButtonDims().w,option_text.getPosY());
            renderButton(_btn_value_change[_btn_value_change_mapping[8]],option_text.getPosX()+1.25*option_text.getWidth(),option_text.getPosY());
        }
        if(i!=7){
            renderButton(_btn_value_change[_btn_value_change_mapping[i-8]],option_text.getPosX()-_btn_value_change[_btn_value_change_mapping[i-8]].getButtonDims().w,option_text.getPosY());
            renderButton(_btn_value_change[_btn_value_change_mapping[i-4]],option_text.getPosX()+1.25*option_text.getWidth(),option_text.getPosY());
        }
    }
}

void GameMenu::renderPauseDialogue(){

}

void GameMenu::renderExitDialogue(){
    renderBackground();
    renderGameTitle();
    int position_to_draw=18;
    auto it=_menu_text.find(_mapping[position_to_draw]);
    auto btn_it=_buttons.find(_mapping[5]);
    _dialog_box.w=1.25*_game_title.getWidth();
    _dialog_box.x=(_game_window->getWidth()-_dialog_box.w)/2;
    _dialog_box.y=(_game_window->getHeight()-it->second.getHeight())/2;
    _dialog_box.h=1.25*it->second.getHeight()+*_text_size;
    SDL_RenderFillRect(_game_window->getRenderer(),&_dialog_box);
    it->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[position_to_draw])->second.getWidth()/2, (_game_window->getHeight()-*_text_size)/2,*_game_window);
    btn_it->second.render(_dialog_box.x,_dialog_box.y+1.25**_text_size,*_game_window);
    btn_it=_buttons.find(_mapping[6]);
    btn_it->second.render(_dialog_box.x+_dialog_box.w-btn_it->second.getButtonDims().w,_dialog_box.y+1.25**_text_size,*_game_window);
}

void GameMenu::eventHandler(SDL_Event &event){
    std::map<std::string,Button>::iterator it=_buttons.end();
    int begin=0, end=0;
    if(_game_state==_mapping[23]||_game_state==_mapping[1]||_game_state==_mapping[2]||_game_state==_mapping[6]){
        if(_played){
            begin=0;
        }else{
            begin=2;
        }
        end=4;
    }
    for(int i=begin;i<end;i++){
        it=_buttons.find(_mapping[i]);
        if(_played&&_mapping[i]==_mapping[2]){
            it=_buttons.end();
        }
        if(it!=_buttons.end()){
            if(event.type==SDL_MOUSEMOTION){
                SDL_GetMouseState(&_mouse_rect.x, &_mouse_rect.y);
                it->second.eventHandler(event);
            }
            if(checkCollision(it->second.getButtonDims(), _mouse_rect)){
                if(event.type==SDL_MOUSEBUTTONUP){
                    _game_state=it->second.getButtonTextID();
                }
            }
        }
    }
    if(_game_state==_mapping[3]){
        it=_btn_value_change.end();
        for(int i=0; i<_btn_value_change_mapping.size(); i++){
            it=_btn_value_change.find(_btn_value_change_mapping[i]);
            if(it!=_btn_value_change.end()){
                if(event.type==SDL_MOUSEMOTION){
                    SDL_GetMouseState(&_mouse_rect.x, &_mouse_rect.y);
                    it->second.eventHandler(event);
                }
                if(checkCollision(it->second.getButtonDims(), _mouse_rect)){
                    if(event.type==SDL_MOUSEBUTTONUP){
                        std::cerr<<it->second.getButtonTextID()<<"\n";
//                    _game_state=it->second.getButtonTextID();
                    }
                }
            }
        }
    }
    it=_btn_value_change.end();
    for(int i=14;i<16;i++){
        for(int j=8;j<12;j++){
            it=_btn_value_change.find(_mapping[i]+"_"+_mapping[j]);
            if(it!=_btn_value_change.end()){
                if(event.type==SDL_MOUSEMOTION){
                    SDL_GetMouseState(&_mouse_rect.x, &_mouse_rect.y);
                    it->second.eventHandler(event);
                }
                if(checkCollision(it->second.getButtonDims(), _mouse_rect)){
                    if(event.type==SDL_MOUSEBUTTONUP){
//                    _game_state=it->second.getButtonTextID();
                    }
                }
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
    const int *text_size, const int *title_size, Settings *game_settings):
    _played(false), _game_window(&window), _text_font(text),
    _title_font(title), _translation(translation),
    _text_size(text_size), _title_size(title_size), _game_settings(game_settings){
    _settings_values=game_settings->settingsFromFile();
    _game_title.loadFromText("Snaker", SDL_Color{255,0,0},_title_font,*_game_window);
    _game_title.setWidth(3*_game_title.getWidth());
    _game_title.setHeight(*_title_size);
    for(auto btn:_translation){
        if(btn.first.substr(0,4)=="btn_"&&btn.first!="btn_increment"&&btn.first!="btn_decrement"&&btn.first!="btn_next"&&btn.first!="btn_prev"){
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

void GameMenu::renderGameTitle(){
    _game_title.render((_game_window->getWidth() - _game_title.getWidth()) / 2, 0, *_game_window);
}
