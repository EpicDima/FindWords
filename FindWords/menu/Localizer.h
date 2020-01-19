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

    explicit Localizer(Locale locale = Locale::RU);

    Locale getLocale();
    void changeLocale(Locale locale = Locale::RU);

    std::string get(const std::string& key);

private:
    Locale locale;
    std::map<std::string, std::string> map;

    std::string getLocaleFilePath();
    void parseFile(const std::string& filepath);
};

#endif