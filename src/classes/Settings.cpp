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
    std::cerr << "Unable to load settings file from: "<<path<<"\n";
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
    std::cerr << "Unable to load language list file from: "<<path<<"\n";
    return false;
}

void Settings::selectLanguage(const int &language){
    _selected_language=language;
    _settings_values[0]=_selected_language;
}

std::string Settings::Language(){
    return _language[_selected_language];
}

bool Settings::loadTanslation(){
    std::string path;
    path=_language_files_directory+_language[_selected_language]+".txt";
    _language_file.open(path);
    _mapping_file.open(_btn_and_text_mapping_file_path);
    if(_language_file.is_open()&&_mapping_file.is_open()){
        std::string lang_line;
        std::string mapping_line;
        while(std::getline(_language_file,lang_line)&&std::getline(_mapping_file,mapping_line)){
            _translation.emplace(std::pair<std::string,std::string>(mapping_line, lang_line));
        }
        _language_file.close();
        _mapping_file.close();
        return true;
    }
    std::cerr << "Unable to load translation file from: "<<path<<"\n";
    return false;
}

std::vector<std::string> Settings::availableTranslations(){
    return _language;
}

int Settings::numberOfAvailableLanguages(){
    return _language.size();
}

std::map<std::string,std::string> Settings::Translation(){
    return _translation;
}

void Settings::setTranslationDirectory(const std::string &path){
    _language_files_directory=path;
}

std::vector<int> Settings::settingsFromFile(){
    return _settings_values;
}

bool Settings::saveSettings(){
    _settings_file.open(_settings_file_path);
    if(_settings_file.is_open()){
        for(int i=0;i<_settings_values.size();i++){
            _settings_file<<_settings_values[i]<<"\t"<<_settingsInstructions[i];//<<"\n";
        }
        _settings_file.close();
        return true;
    }
    std::cerr << "Unable to save settings file to: "<<_settings_file_path<<"\n";
    return false;
}

void Settings::setBotsCount(const int &count){
    _settings_values[1]=count;
}

void Settings::setFruitsCount(const int &count){
    _settings_values[2]=count;
}

void Settings::setPowerupsCount(const int &count){
    _settings_values[3]=count;
}

void Settings::setSpeed(const int &speed){
    _settings_values[4]=speed;
}

void Settings::setMappingFilePath(const std::string& path){
    _btn_and_text_mapping_file_path=path;
}
