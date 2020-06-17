//
// Created by lukasz on 06.06.2020.
//

#include "GameMenu.h"

#include <utility>

bool GameMenu::loadMappingFile(const std::string &path){
    _path_to_mapping_file=path;
    _mapping_file.open(_path_to_mapping_file);
    if(_mapping_file.is_open()){
        std::string line;
        while(std::getline(_mapping_file, line)){
            _mapping.emplace_back(line);
        }
        _game_state=_mapping[23];
        createMenuItems();
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
    _menu_text.find(_mapping[position_to_draw])->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[position_to_draw])->second.getWidth()/2, _game_window->getHeight()/6+*_text_size,_game_window);
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
    if(_game_state==_btn_value_change_mapping[8]){
        _settings_values[0]++;
        if(_settings_values[0]>=_game_settings->numberOfAvailableLanguages()){
            _settings_values[0]=0;
        }
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[9]){
        _settings_values[0]--;
        if(_settings_values[0]<0){
            _settings_values[0]=_game_settings->numberOfAvailableLanguages()-1;
        }
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[4]){
        _settings_values[1]++;
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[0]){
        _settings_values[1]--;
        if(_settings_values[1]<0){
            _settings_values[1]=0;
        }
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[5]){
        _settings_values[2]++;
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[1]){
        _settings_values[2]--;
        if(_settings_values[2]<0){
            _settings_values[2]=0;
        }
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[6]){
        _settings_values[3]++;
        if(_settings_values[3]>100){
            _settings_values[3]=100;
        }
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[2]){
        _settings_values[3]--;
        if(_settings_values[3]<0){
            _settings_values[3]=0;
        }
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[7]){
        _settings_values[4]++;
        _game_state=_mapping[3];
    }
    if(_game_state==_btn_value_change_mapping[3]){
        _settings_values[4]--;
        if(_settings_values[4]<0){
            _settings_values[4]=0;
        }
        _game_state=_mapping[3];
    }
    int position_to_draw=22;
    renderBackground();
    renderGameTitle();
    _menu_text.find(_mapping[position_to_draw])->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[position_to_draw])->second.getWidth()/2, _game_window->getHeight()/6+*_text_size,_game_window);
    auto text_it=_menu_text.end();
    for(int i=7;i<12;i++){
        text_it=_menu_text.find(_mapping[i]);
        text_it->second.render(_game_window->getWidth()/3, _game_window->getHeight()/3+(i-7)**_text_size, _game_window);
        LTexture option_text;
        if(i==7){
            _game_settings->selectLanguage(_settings_values[i-7]);
            option_text.loadFromText(_game_settings->Language(), SDL_Color{255, 0, 0},
                                     _text_font, _game_window);
        }else{
            if((i-7)==3){
                option_text.loadFromText(std::to_string(_settings_values[i-7])+"%", SDL_Color{255, 0, 0},
                                         _text_font, _game_window);
            }else{
                option_text.loadFromText(std::to_string(_settings_values[i-7]), SDL_Color{255, 0, 0},
                                         _text_font, _game_window);
            }
        }
        option_text.setWidth(2*option_text.getWidth());
        option_text.setHeight(*_text_size);
        option_text.render((_game_window->getWidth()/6)*3,(_game_window->getHeight()/3)+(i-7)**_text_size,_game_window);
        if(i==7){
            renderButton(_btn_value_change[_btn_value_change_mapping[9]],option_text.getPosX()-_btn_value_change[_btn_value_change_mapping[9]].getButtonDims().w,option_text.getPosY());
            renderButton(_btn_value_change[_btn_value_change_mapping[8]],option_text.getPosX()+1.25*option_text.getWidth(),option_text.getPosY());
        }
        if(i!=7){
            renderButton(_btn_value_change[_btn_value_change_mapping[i-8]],option_text.getPosX()-_btn_value_change[_btn_value_change_mapping[i-8]].getButtonDims().w,option_text.getPosY());
            renderButton(_btn_value_change[_btn_value_change_mapping[i-4]],option_text.getPosX()+1.25*option_text.getWidth(),option_text.getPosY());
        }
    }
    renderButton(_buttons[_mapping[12]],text_it->second.getPosX()+text_it->second.getWidth()/2,text_it->second.getPosY()+*_text_size);
    renderButton(_buttons[_mapping[13]],_buttons[_mapping[12]].getButtonDims().x+2*_buttons[_mapping[12]].getButtonDims().w,text_it->second.getPosY()+*_text_size);
}

void GameMenu::renderPauseDialogue(){
    renderBackground();
    renderGameTitle();
    _dialog_box.w=1.5*_menu_text[_mapping[19]].getWidth();
    _dialog_box.h=1.5*_menu_text[_mapping[19]].getHeight();
    _dialog_box.x=_game_window->getWidth()/2-_dialog_box.w/2;
    _dialog_box.y=_game_window->getHeight()/2-_dialog_box.h/2;
    SDL_RenderFillRect(_game_window->getRenderer(),&_dialog_box);
    _menu_text[_mapping[19]].render(_game_window->getWidth()/2-_menu_text[_mapping[19]].getWidth()/2,
                                    _game_window->getHeight()/2-*_title_size/2,
                                    _game_window);
    _menu_text[_mapping[26]].render(_game_window->getWidth()/2-_menu_text[_mapping[26]].getWidth()/2,
                                    _game_window->getHeight()/2+*_text_size,
                                    _game_window);
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
    it->second.render(_game_window->getWidth()/2-_menu_text.find(_mapping[position_to_draw])->second.getWidth()/2, (_game_window->getHeight()-*_text_size)/2,_game_window);
    btn_it->second.render(_dialog_box.x, _dialog_box.y+1.25**_text_size);
    btn_it=_buttons.find(_mapping[6]);
    btn_it->second.render(_dialog_box.x+_dialog_box.w-btn_it->second.getButtonDims().w, _dialog_box.y+1.25**_text_size);
}

void GameMenu::eventHandler(SDL_Event &event){
    std::map<std::string,Button>::iterator it=_buttons.end();
    int begin=0, end=0;
    bool options=false;
    //Buttons to check for main menu
//    if(_game_state!=_mapping[0]||_game_state!=_mapping[2]){
        if(_game_state==_mapping[23]||_game_state==_mapping[1]||_game_state==_mapping[2]||_game_state==_mapping[6]){
            if(_played){
                begin=0;
            }else{
                begin=2;
            }
            end=5;
        }
        //Buttons for quit dialogue
        if(_game_state==_mapping[4]){
            begin=5;
            end=7;
        }
        //Save and exit and exit buttons in options menu
        if(_game_state==_mapping[3]){
            begin=12;
            end=14;
            options=true;
        }
        //Reboot info
        if(_game_state==_mapping[24]){
            begin=25;
            end=26;
        }
        //Logic
        for(int i=begin; i<end; i++){
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
                        if(!options){
                            _game_state=it->second.getButtonTextID();
                        }else{
                            if(it->second.getButtonTextID()==_mapping[12]){
                                saveSettings();
                            }
                            if(it->second.getButtonTextID()==_mapping[13]){
                                _game_state=_mapping[23];
                                _game_settings->reloadSettings();
                                _settings_values=_game_settings->settingsFromFile();
                            }
                        }
                    }
                }
            }
        }
        if(_game_state==_mapping[3]&&options){
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
                            _game_state=it->second.getButtonTextID();
                        }
                    }
                }
            }
            it=_btn_value_change.end();
            for(int i=14; i<16; i++){
                for(int j=8; j<12; j++){
                    it=_btn_value_change.find(_mapping[i]+"_"+_mapping[j]);
                    if(it!=_btn_value_change.end()){
                        if(event.type==SDL_MOUSEMOTION){
                            SDL_GetMouseState(&_mouse_rect.x, &_mouse_rect.y);
                            it->second.eventHandler(event);
                        }
                        if(checkCollision(it->second.getButtonDims(), _mouse_rect)){
                            if(event.type==SDL_MOUSEBUTTONUP||event.type==SDL_MOUSEWHEEL){
                                _game_state=it->second.getButtonTextID();
                            }
                        }
                    }
                }
            }
        }
//    }
    if(_game_state==_mapping[2]){
        _played=true;
    }
    if(_game_state==_mapping[1]){
        _played=false;
    }
    if(_game_state==_mapping[25]){
        _game_state=_mapping[23];
    }
    if(event.type==SDL_KEYDOWN&&event.key.repeat == 0){
        switch(event.key.keysym.sym){
            case SDLK_p:
            case SDLK_PAUSE:{
                if(_game_state!=_mapping[19]){
                    _tmp_state=_game_state;
                    _game_state=_mapping[19];
                }else{
                    _game_state=_tmp_state;
                }
                break;
            }
            case SDLK_q:
            case SDLK_ESCAPE:{
                if(_game_state==_mapping[0]||_game_state==_mapping[2]||_game_state==_mapping[19]){
                    _game_state=_mapping[23];
                }else{
                    _game_state=_mapping[4];
                }
                break;
            }
            case SDLK_o:{
                if(_game_state!=_mapping[0]||_game_state!=_mapping[2]){
                    _game_state=_mapping[3];
                }
                break;
            }
            case SDLK_c:{
                if(_played){
                    _game_state=_mapping[0];
                }
                break;
            }
            case SDLK_r:{
                if(_played){
                    _game_state=_mapping[2];
                }
                break;
            }
            case SDLK_s:{
                if(!_played){
                    _game_state=_mapping[2];
                }
                break;
            }
            case SDLK_y:{
                if(_game_state==_mapping[4]){
                    _game_state=_mapping[5];
                }
                break;
            }
            case SDLK_n:{
                if(_game_state==_mapping[4]){
                    _game_state=_mapping[6];
                }
                break;
            }
        }
    }
}

void GameMenu::renderButton(Button &btn, const int &posX, const int &posY){
    btn.render(posX-btn.getButtonDims().w/2, posY);
}

void GameMenu::renderBackground(){
    _menu_camera={0,0,_game_window->getWidth(),_game_window->getHeight()};
    for(int i=0;i<_menu_background.size();i++){
        _menu_background[i]->render(_menu_camera,_game_window,_menu_background_texture);
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

GameMenu::GameMenu(std::shared_ptr<Win> window, TTF_Font *text, TTF_Font *title, const int *text_size, const int *title_size,
                   const std::shared_ptr<Settings> &game_settings):
    _played(false), _game_window(std::move(window)), _text_font(text),
    _title_font(title), _text_size(text_size), _title_size(title_size), _game_settings(game_settings){
    _settings_values=game_settings->settingsFromFile();
    _game_title.loadFromText("Snaker", SDL_Color{255,0,0},_title_font,_game_window);
    _game_title.setWidth(3*_game_title.getWidth());
    _game_title.setHeight(*_title_size);
}

std::string GameMenu::getGameState(){
    return _game_state;
}

std::vector<std::string> GameMenu::getMapping(){
    return _mapping;
}

void GameMenu::renderGameTitle(){
    _game_title.render((_game_window->getWidth() - _game_title.getWidth()) / 2, 0, _game_window);
}

void GameMenu::saveSettings(){
    _game_settings->selectLanguage(_settings_values[0]);
    _game_settings->setBotsCount(_settings_values[1]);
    _game_settings->setFruitsCount(_settings_values[2]);
    _game_settings->setPowerupsCount(_settings_values[3]);
    _game_settings->setSpeed(_settings_values[4]);
    if(_game_settings->saveSettings()){
        _game_state=_mapping[23];
        _game_settings->reloadSettings();
        _game_settings->loadTanslation();
        createMenuItems();
    }else{
        _game_state=_mapping[24];
    }
}

void GameMenu::renderInfo(){
    renderBackground();
    renderGameTitle();
    _dialog_box.w=_game_window->getWidth()/3*2;
    _dialog_box.x=_game_window->getWidth()/2-_dialog_box.w/2;
    _dialog_box.y=_game_window->getHeight()/3;
    _dialog_box.h=_game_window->getHeight()/3;
    SDL_RenderFillRect(_game_window->getRenderer(),&_dialog_box);
    auto text_it=_menu_text.find(_mapping[24]);
    auto btn_it=_buttons.find(_mapping[25]);
    text_it->second.render(_dialog_box.x+_dialog_box.w/2-text_it->second.getWidth()/2,
        _dialog_box.y+*_text_size,_game_window);
    renderButton(btn_it->second,_dialog_box.x+_dialog_box.w/2,_dialog_box.y+_dialog_box.h-*_text_size);
}

void GameMenu::createMenuItems(){
    if(!_buttons.empty()){
        _buttons.clear();
    }
    if(!_menu_text.empty()){
        _menu_text.clear();
    }
    if(!_btn_value_change.empty()){
        _btn_value_change.clear();
    }
    
    for(auto btn:_game_settings->Translation()){
        if(btn.first.substr(0,4)=="btn_"&&btn.first!="btn_increment"&&btn.first!="btn_decrement"&&btn.first!="btn_next"&&btn.first!="btn_prev"){
            _buttons.emplace(btn.first,Button(btn.first,btn.second,_game_window,_text_font,*_text_size));
        }
        if(btn.first.substr(0,5)=="text_"){
            LTexture text_texture;
            text_texture.loadFromText(btn.second,SDL_Color{255,0,0},_text_font,_game_window);
            text_texture.setWidth(3*text_texture.getWidth());
            text_texture.setHeight(*_text_size);
            _menu_text.emplace(btn.first,std::move(text_texture));
        }
        if(btn.first.substr(0, 4)=="btn_"&&
           (btn.first=="btn_increment"||btn.first=="btn_decrement"||btn.first=="btn_next"||btn.first=="btn_prev")){
            if(btn.first=="btn_next"||btn.first=="btn_prev"){
                _btn_value_change_mapping.emplace_back(btn.first);
                _btn_value_change.emplace(btn.first,
                                          Button(btn.first, btn.second, _game_window, _text_font, *_text_size, false));
            }
            if(btn.first=="btn_decrement"||btn.first=="btn_increment"){
                for(int i=0; i<4; i++){
                    _btn_value_change_mapping.emplace_back(btn.first+"_"+_mapping[i+8]);
                    _btn_value_change.emplace(btn.first+"_"+_mapping[i+8],
                                              Button(btn.first+"_"+_mapping[i+8], btn.second, _game_window, _text_font,
                                                     *_text_size, false));
                }
            }
        }
    }
    auto text_it=_menu_text.find(_mapping[24]);
    text_it->second.setWidth(4*text_it->second.getWidth());
    _menu_text[_mapping[19]].loadFromText(_game_settings->Translation()[_mapping[19]],SDL_Color{255,255,0},_title_font,_game_window);
    _menu_text[_mapping[19]].setHeight(*_title_size);
    _menu_text[_mapping[19]].setWidth(3*_menu_text[_mapping[19]].getWidth());
    _menu_text[_mapping[26]].loadFromText(_game_settings->Translation()[_mapping[26]],SDL_Color{255,255,0},_text_font,_game_window);
    _menu_text[_mapping[26]].setWidth(12*_menu_text[_mapping[26]].getWidth());
    _menu_text[_mapping[26]].setHeight(*_text_size);
}
