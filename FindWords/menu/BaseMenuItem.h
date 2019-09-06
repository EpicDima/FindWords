#ifndef BASEMENUITEM_H
#define BASEMENUITEM_H


#include <cassert>
#include <string>


class BaseMenuItem
{
public:
    BaseMenuItem();
    BaseMenuItem(std::string* item, uint64_t languagesNumber);
    BaseMenuItem(const BaseMenuItem& b) ;

    BaseMenuItem& operator=(const BaseMenuItem& b);
    std::string operator[](const uint64_t languageIndex);


    std::string* item;
    uint64_t languagesNumber;
};

#endif
