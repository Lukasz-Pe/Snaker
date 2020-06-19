/*
 * main.cpp
 *
 *  Created on: 23.08.2018
 *      Author: Łukasz Pękalski
 */

#include "main.h"
#include "classes/Win.h"
#include "classes/LTexture.h"
#include "classes/Timer.h"
#include "classes/PowerUp.h"
#include "classes/Game.h"
#include "classes/GameMenu.h"
#include "functions/initsdl.h"
//#include "functions/collisions.h"
//TODO think about memory management
//TODO think about concurrency
//----------------------------------------------------------------Declarations of variables
int main() {
    const int TEXT_SIZE=50, TITLE_SIZE=150;
    const std::string PATH_TO_TRANSLATION_LIST{"../assets/lang/_LangList.txt"},
        PATH_TO_SETTINGS{"../assets/settings.txt"},
        PATH_TO_TRANSLATIONS{"../assets/lang/"},
        PATH_TO_FONT_FILE{"../assets/fonts/Horta.ttf"},
        MAPPING_FILE_PATH{"../assets/menu_structure.txt"};
    bool gContinue = false;
    auto game_settings=std::make_shared<Settings>();
    auto game_window=std::make_shared<Win>();
    game_window->setWidth(1366);
    game_window->setHeight(720);
//Load language list, setting, translation and assets
    gContinue=game_settings->loadLanguageList(PATH_TO_TRANSLATION_LIST);
    if(gContinue){
        gContinue=game_settings->loadSettings(PATH_TO_SETTINGS);
    }
    if(gContinue){
        game_settings->setTranslationDirectory(PATH_TO_TRANSLATIONS);
        game_settings->setMappingFilePath(MAPPING_FILE_PATH);
        game_settings->selectLanguage(game_settings->settingsFromFile()[0]);
    }
    gContinue=game_settings->loadTanslation();
    if (gContinue) {
        gContinue = initSDL(game_window, game_window->getWidth(), game_window->getHeight());
    }
//    LTexture fruits_and_powerups_textures, player_head_textures, player_tail_textures,
//             level_background, menu_background, snake_head_textures, snake_tail_textures, icons_of_active_powerups;
    auto fruits_and_powerups_textures = std::make_shared<LTexture>();
    auto player_head_textures = std::make_shared<LTexture>();
    auto player_tail_textures = std::make_shared<LTexture>();
    auto level_background = std::make_shared<LTexture>();
    auto menu_background = std::make_shared<LTexture>();
    auto snake_head_textures = std::make_shared<LTexture>();
    auto snake_tail_textures = std::make_shared<LTexture>();
    auto icons_of_active_powerups = std::make_shared<LTexture>();
    if (gContinue) {
        gContinue = fruits_and_powerups_textures->loadFromFile("../assets/images/fruits.png", game_window);
    }
    if (gContinue) {
        gContinue = player_head_textures->loadFromFile("../assets/images/face.png", game_window);
    }
    if (gContinue) {
        gContinue = player_tail_textures->loadFromFile("../assets/images/dot.png", game_window);
    }
    if (gContinue) {
        gContinue = level_background->loadFromFile("../assets/images/bo_play_pattern.png", game_window);
    }
    if (gContinue) {
        gContinue = menu_background->loadFromFile("../assets/images/escheresque.png", game_window);
    }
    if (gContinue) {
        gContinue = snake_head_textures->loadFromFile("../assets/images/faces.png", game_window);
    }
    if (gContinue) {
        gContinue = snake_tail_textures->loadFromFile("../assets/images/tails.png", game_window);
    }
    if (gContinue) {
        gContinue = icons_of_active_powerups->loadFromFile("../assets/images/powerups.png", game_window);
    }
    std::unique_ptr<TTF_Font,SDLTTFDestroyer>text_font(TTF_OpenFont(PATH_TO_FONT_FILE.c_str(), TEXT_SIZE));
    std::unique_ptr<TTF_Font,SDLTTFDestroyer>title_font(TTF_OpenFont(PATH_TO_FONT_FILE.c_str(), TITLE_SIZE));
    if (text_font == NULL || title_font == NULL) {
        std::cerr<<"Failed to load font! SDL_ttf Error: "<<TTF_GetError()<<"\n";
        gContinue = false;
    }
	game_window->setTitle("Snaker");
    GameMenu game_menu(game_window, text_font.get(), title_font.get(),
        &TEXT_SIZE, &TITLE_SIZE, game_settings);
    game_menu.loadMappingFile(MAPPING_FILE_PATH);
    game_menu.setBackgroundTexture(menu_background);
    SDL_Event event;
    std::string state{game_menu.getGameState()}, tmp_state{};
    auto frame_timer=std::make_shared<Timer>();
    Game game(game_window, level_background, player_head_textures, player_tail_textures,
              snake_head_textures, snake_tail_textures, fruits_and_powerups_textures, icons_of_active_powerups,
              frame_timer, game_settings);
    gContinue=game.setLevelSize(6830,3600);
    game.generatePlayer();
	while(gContinue){
        state=game_menu.getGameState();
        game_window->prepareRenderer(0,0,0);
        while(SDL_PollEvent(&event)){
            game_window->eventHandler(event);
            game_menu.eventHandler(event);
            game.eventHandler(event);
        }
        if(state==game_menu.getMapping()[23]||state==game_menu.getMapping()[1]||
           state==game_menu.getMapping()[6]){
            game_menu.renderMainMenu();
        }
        if(state==game_menu.getMapping()[4]){
            game_menu.renderExitDialogue();
        }
        if(state==game_menu.getMapping()[5]){
            gContinue=false;
        }
        if(state==game_menu.getMapping()[3]){
            game_menu.renderOptionsScreen();
        }
        if(state==game_menu.getMapping()[24]){
            game_menu.renderInfo();
        }
        if(state==game_menu.getMapping()[19]){
            game_menu.renderPauseDialogue();
        }
        if(state==game_menu.getMapping()[2]){
            game.render();
        }
        game_window->render();
	}
	return 0;
}