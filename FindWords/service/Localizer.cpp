#include "Localizer.h"


uint64_t Localizer::LOCALES = 2;


Localizer::Localizer(Locale locale)
{
    changeLocale(locale);
}


Localizer::Locale Localizer::getLocale()
{
    return locale;
}


void Localizer::changeLocale(Locale locale)
{
    map.clear();
    std::string filepath = getLocaleFilePath(locale);
    parseFile(filepath);
}


std::string Localizer::get(std::string key)
{
    return map[key];
}


std::string Localizer::getLocaleFilePath(Locale locale)
{
    std::string localeString;
    switch (locale) {
        case Locale::EN:
            localeString = "en";
            break;
        case Locale::RU: // так и должно быть, в непонятном случае будет по стандарту русская локализация
        default:
            localeString = "ru";
            break;
    }
    this->locale = locale;
    return "locale/locale." + localeString + ".txt";
}


void Localizer::parseFile(std::string filepath)
{
    std::ifstream file(filepath);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::size_t index = line.find_first_of('=');
            if (index != std::string::npos) {
                std::string key = line.substr(0, index);
                std::string value = line.substr(index + 1, std::string::npos);
                map[key] = value;
            }
        }
    }
    file.close();
}