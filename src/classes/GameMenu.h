//
// Created by lukasz on 06.06.2020.
//

#ifndef SNAKER_GAMEMENU_H
#define SNAKER_GAMEMENU_H

#include "../main.h"
#include "Button.h"

class GameMenu{
public:
    bool loadMappingFile(const std::string& path);
    void renderMainMenu();
    void renderOptionsScreen();
    void renderPauseDialogue();
    void renderExitDialogue();
private:
    std::string _path_to_mapping_file;
    std::vector<std::string> _mapping;
    bool _played;
    
};


#endif //SNAKER_GAMEMENU_H
