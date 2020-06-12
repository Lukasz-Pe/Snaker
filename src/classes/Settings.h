//
// Created by lukasz on 03.06.2020.
//

#ifndef SNAKER_SETTINGS_H
#define SNAKER_SETTINGS_H
#include "../main.h"

class Settings{
public:
    Settings():_settingsInstructions({"/Languages, as they are present in the list\n", "/Enemies count - original 20\n", "/Fruits count - original 200\n", "/Powerups count, cannot be 0 - original 25\n", "/Snake speed - original 300" }){};
    bool loadSettings(const std::string& path);
    bool saveSettings();
    bool loadLanguageList(const std::string& path);
    void selectLanguage(const int& language);
    void setBotsCount(const int& count);
    void setFruitsCount(const int& count);
    void setPowerupsCount(const int& count);
    void setSpeed(const int& speed);
    std::vector<std::string> availableTranslations();
    std::string Language();
    std::map<std::string,std::string> Translation();
    int numberOfAvailableLanguages();
    void setMappingFilePath(const std::string& path);
    void setTranslationDirectory(const std::string& path);
    bool loadTanslation();
    std::vector<int> settingsFromFile();
    bool reloadSettings();
private:
    std::vector<std::string> _settingsInstructions;
    std::fstream _settings_file;
    std::fstream _language_list_file;
    std::fstream _language_file;
    std::fstream _mapping_file;
    std::string _settings_file_path;
    std::string _language_files_directory;
    std::string _btn_and_text_mapping_file_path;
    std::vector<int> _settings_values;
    std::map<std::string,std::string> _translation;
    std::vector<std::string> _language;
    int _selected_language;
    
};


#endif //SNAKER_SETTINGS_H
