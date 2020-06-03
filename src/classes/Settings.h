//
// Created by lukasz on 03.06.2020.
//

#ifndef SNAKER_SETTINGS_H
#define SNAKER_SETTINGS_H
#include "../main.h"

class Settings{
public:
    bool loadSettings(const std::string& path);
    bool loadLanguageList(const std::string& path);
    void selectLanguage(const int& language);
    std::vector<std::string> availableTranslations();
    std::string Language();
    std::vector<std::string> Translation();
    int numberOfAvailableLanguages();
    void setTranslationDirectory(const std::string& path);
    bool loadTanslation();
    std::vector<int> settingsFromFile();
private:
    std::fstream _settings_file;
    std::fstream _language_list_file;
    std::fstream _language_file;
    std::string _settings_file_path;
    std::string _language_files_directory;
    std::vector<int> _settings_values;
    std::vector<std::string> _translation;
    std::vector<std::string> _language;
    int _selected_language;
    
};


#endif //SNAKER_SETTINGS_H
