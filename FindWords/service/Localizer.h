#ifndef LOCALIZER_H
#define LOCALIZER_H


#include <fstream>
#include <map>


class Localizer
{
public:
    enum Locale : uint64_t
    {
        RU = 0, EN
    };

    static uint64_t LOCALES;

    Localizer(Locale locale = Locale::RU);

    Locale getLocale();
    void changeLocale(Locale locale = Locale::RU);

    std::string get(std::string key);

private:
    Locale locale;
    std::map<std::string, std::string> map;

    std::string getLocaleFilePath(Locale locale);
    void parseFile(std::string filepath);
};

#endif