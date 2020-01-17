#ifndef BASEMENUITEM_H
#define BASEMENUITEM_H


#include "../service/Localizer.h"

#include <cassert>
#include <string>


class BaseMenuItem
{
public:
    BaseMenuItem() {};
    BaseMenuItem(std::string key, Localizer* localizer);
    BaseMenuItem(std::string key);
    BaseMenuItem(const BaseMenuItem& b);

    BaseMenuItem& operator=(const BaseMenuItem& b);

    void setLocalizer(Localizer* localizer);
    std::string get();

protected:
    std::string key;
    Localizer* localizer;
};

#endif
