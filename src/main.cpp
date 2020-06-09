/*
 * main.cpp
 *
 *  Created on: 23.08.2018
 *      Author: Łukasz Pękalski
 */

#include "main.h"
#include "classes/Win.h"
#include "classes/LTexture.h"
#include "classes/Snake.h"
#include "classes/Fruit.h"
#include "classes/Tile.h"
#include "classes/Button.h"
#include "classes/Timer.h"
#include "classes/Settings.h"
#include "classes/GameMenu.h"
#include "functions/initsdl.h"
#include "functions/eventHandler.h"
//#include "functions/collisions.h"
//TODO think about memory management
//TODO think about concurrency
//TODO no global variables!
//----------------------------------------------------------------Declarations of variables
const std::string PATH_TO_TRANSLATION_LIST{"../assets/lang/_LangList.txt"},
PATH_TO_SETTINGS{"../assets/settings.txt"},
PATH_TO_TRANSLATIONS{"../assets/lang/"},
PATH_TO_FONT_FILE{"../assets/fonts/Horta.ttf"},
MAPPING_FILE_PATH{"../assets/menu_structure.txt"};
const int TEXT_SIZE=50, TITLE_SIZE=150;
int main() {
    bool gContinue = false;
    Settings game_settings;
    Win game_window;
    game_window.setWidth(1366);
    game_window.setHeight(720);
    
//Load language list, setting, translation and assets
    gContinue=game_settings.loadLanguageList(PATH_TO_TRANSLATION_LIST);
    gContinue=game_settings.loadSettings(PATH_TO_SETTINGS);
    if(gContinue){
        game_settings.setTranslationDirectory(PATH_TO_TRANSLATIONS);
        game_settings.setMappingFilePath(MAPPING_FILE_PATH);
        game_settings.selectLanguage(game_settings.settingsFromFile()[0]);
    }
    gContinue=game_settings.loadTanslation();
    if (gContinue) {
        gContinue = initSDL(&game_window, game_window.getWidth(), game_window.getHeight());
    }
    LTexture fruits_and_powerups_textures, player_head_textures, player_tail_textures, level_background, menu_background, snake_head_textures, snake_tail_textures, icons_of_active_powerups;
    if (gContinue) {
        gContinue = fruits_and_powerups_textures.loadFromFile("../assets/images/fruits.png", game_window);
    }
    if (gContinue) {
        gContinue = player_head_textures.loadFromFile("../assets/images/face.png", game_window);
    }
    if (gContinue) {
        gContinue = player_tail_textures.loadFromFile("../assets/images/dot.png", game_window);
    }
    if (gContinue) {
        gContinue = level_background.loadFromFile("../assets/images/bo_play_pattern.png", game_window);
    }
    if (gContinue) {
        gContinue = menu_background.loadFromFile("../assets/images/escheresque.png", game_window);
    }
    if (gContinue) {
        gContinue = snake_head_textures.loadFromFile("../assets/images/faces.png", game_window);
    }
    if (gContinue) {
        gContinue = snake_tail_textures.loadFromFile("../assets/images/tails.png", game_window);
    }
    if (gContinue) {
        gContinue = icons_of_active_powerups.loadFromFile("../assets/images/powerups.png", game_window);
    }
    std::unique_ptr<TTF_Font,SDLTTFDestroyer>text_font(TTF_OpenFont(PATH_TO_FONT_FILE.c_str(), TEXT_SIZE));
    std::unique_ptr<TTF_Font,SDLTTFDestroyer>title_font(TTF_OpenFont(PATH_TO_FONT_FILE.c_str(), TITLE_SIZE));
    if (text_font == NULL || title_font == NULL) {
        std::cerr<<"Failed to load font! SDL_ttf Error: "<<TTF_GetError()<<"\n";
        gContinue = false;
    }
//	gLTOptionsText.setWidth(5 * gLTOptionsText.getWidth());
    //ToDo: Write object creation and sprite assignment code
	//TODO Build class for powerups - derived from Fruits
    //TODO build class for menu generation
	game_window.setTitle("Snaker");
    GameMenu game_menu(game_window,text_font.get(),title_font.get(),game_settings.Translation(),&TEXT_SIZE,&TITLE_SIZE);
    game_menu.loadMappingFile(MAPPING_FILE_PATH);
    game_menu.setBackgroundTexture(std::move(menu_background));
    SDL_Event event;
    std::string state{game_menu.getGameState()};
	while(gContinue){
        game_window.prepareRenderer(0,0,0);
        while(SDL_PollEvent(&event)){
            game_window.eventHandler(event);
            game_menu.eventHandler(event);
        }
        if(state==game_menu.getMapping()[23]){
            game_menu.renderMainMenu();
        }
        if(state==game_menu.getMapping()[4]){
            gContinue=false;
        }
        game_window.render();
        state=game_menu.getGameState();
	}
	return 0;
}