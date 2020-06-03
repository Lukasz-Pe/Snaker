//
// Created by lukasz on 03.06.2020.
//

#include "Settings.h"

bool Settings::loadSettings(const std::string &path){
    _settings_file_path=path;
    _settings_file.open(_settings_file_path);
    if(_settings_file.is_open()){
        std::string value;
        while(std::getline(_settings_file,value,'/')){
            _settings_file.ignore(1024, '\n');
            _settings_values.emplace_back(std::stoi(value));
        }
        _settings_file.close();
        return true;
    }
    std::cout << "Unable to load settings file from: "<<path<<"\n";
    return false;
}

bool Settings::loadLanguageList(const std::string &path){
    _language_list_file.open(path);
    if(_language_list_file.is_open()){
        std::string line;
        while(std::getline(_language_list_file,line)){
            _language.emplace_back(line);
        }
        _language_list_file.close();
        return true;
    }
    std::cout << "Unable to load language list file from: "<<path<<"\n";
    return false;
}

void Settings::selectLanguage(const int &language){
    if(language<_language.size()&&language>=0){
        _selected_language=language;
    }else{
        _selected_language=0;
    }
}

std::string Settings::Language(){
    return _language[_selected_language];
}

bool Settings::loadTanslation(){
    std::string path;
    path=_language_files_directory+_language[_selected_language]+".txt";
    _language_file.open(path);
    if(_language_file.is_open()){
        std::string line;
        while(std::getline(_language_file,line)){
            _translation.emplace_back(line);
        }
        _language_file.close();
        return true;
    }
    std::cout << "Unable to load translation file from: "<<path<<"\n";
    return false;
}

std::vector<std::string> Settings::availableTranslations(){
    return _language;
}

int Settings::numberOfAvailableLanguages(){
    return _language.size();
}

std::vector<std::string> Settings::Translation(){
    return _translation;
}

void Settings::setTranslationDirectory(const std::string &path){
    _language_files_directory=path;
}

std::vector<int> Settings::settingsFromFile(){
    return _settings_values;
}
