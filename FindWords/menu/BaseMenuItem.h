#ifndef BASEMENUITEM_H
#define BASEMENUITEM_H


#include "Localizer.h"

#include <cassert>
#include <string>


class BaseMenuItem
{
public:
    BaseMenuItem() = default;;
    BaseMenuItem(std::string key, Localizer* localizer);
    explicit BaseMenuItem(std::string key);
    BaseMenuItem(const BaseMenuItem& b);

    BaseMenuItem& operator=(const BaseMenuItem& b);

    void setLocalizer(Localizer* pLocalizer);
    std::string get();

protected:
    std::string key;
    Localizer* localizer{};
};

#endif
