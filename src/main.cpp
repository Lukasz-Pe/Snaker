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
#include "classes/Game.h"
#include "classes/GameMenu.h"
#include "functions/initsdl.h"
//TODO think about concurrency
//----------------------------------------------------------------Declarations of variables
int main() {
    const double FPS_RECALC_PER_SECOND=4.0;
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
              game_settings, text_font.get(), TEXT_SIZE, frame_timer);
    gContinue=game.setLevelSize(4098,2160);
    game.generatePlayers();
    double frame_time{0}, time_on_loop_begining{0}, FPS_recalc_treshhold{1.0/FPS_RECALC_PER_SECOND};
	while(gContinue){
	    if(frame_timer->isStarted()&&!frame_timer->isPaused()){
            if((frame_timer->getSeconds<double>()-time_on_loop_begining)>FPS_recalc_treshhold){
                game.FPS(1.0/frame_time);
                time_on_loop_begining=frame_timer->getSeconds<double>();
            }
	        frame_time=frame_timer->getSeconds<double>();
	    }
        state=game_menu.getGameState();
        game_window->prepareRenderer(0,0,0);
        while(SDL_PollEvent(&event)){
            game_window->eventHandler(event);
            game_menu.eventHandler(event);
            game.eventHandler(event);
        }
        if(state==game_menu.getMapping()[23]|| state==game_menu.getMapping()[6]){
            if(frame_timer->isStarted()||frame_timer->isPaused()){
                frame_timer->pause();
            }
            game_menu.renderMainMenu();
        }
        if(state==game_menu.getMapping()[4]){
            if(frame_timer->isStarted()||frame_timer->isPaused()){
                frame_timer->stop();
            }
            game_menu.renderExitDialogue();
        }
        if(state==game_menu.getMapping()[5]){
            if(frame_timer->isStarted()||frame_timer->isPaused()){
                frame_timer->stop();
            }
            gContinue=false;
        }
        if(state==game_menu.getMapping()[3]){
            if(frame_timer->isStarted()||frame_timer->isPaused()){
                frame_timer->stop();
            }
            game_menu.renderOptionsScreen();
        }
        if(state==game_menu.getMapping()[24]){
            if(frame_timer->isStarted()||frame_timer->isPaused()){
                frame_timer->stop();
            }
            game_menu.renderInfo();
        }
        if(state==game_menu.getMapping()[19]){
            if(frame_timer->isStarted()){
                frame_timer->pause();
            }
            game_menu.renderPauseDialogue();
        }
        if(state==game_menu.getMapping()[2]||state==game_menu.getMapping()[0]){
            if(!frame_timer->isStarted()&&!frame_timer->isPaused()){
                frame_timer->start();
            }
            if(frame_timer->isPaused()){
                frame_timer->unpause();
            }
            game.recalculateVariables();
            game.render();
        }
        if(state==game_menu.getMapping()[1]){
            if(!frame_timer->isStarted()||frame_timer->isPaused()){
                frame_timer->stop();
            }
            game.resetGame();
            game_menu.goToMainMenu();
        }
        game_window->render();
        if(frame_timer->isStarted()&&!frame_timer->isPaused()){
            frame_time=frame_timer->getSeconds<double>()-frame_time;
            game.frameTime(frame_time);
        }
	}
	return 0;
}