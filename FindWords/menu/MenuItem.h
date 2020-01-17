#ifndef MENUITEM_H
#define MENUITEM_H

#include "BaseMenuItem.h"

#include <functional>


class MenuItem : public BaseMenuItem
{
public:
    MenuItem() {};
    MenuItem(std::string key, std::function<void()> func);
    MenuItem(std::string key, std::function<void()> func, std::function<std::string()> valueFunc);
    MenuItem(std::string key, std::function<void()> func, Localizer* localizer);
    MenuItem(std::string key, std::function<void()> func, std::function<std::string()> valueFunc, Localizer* localizer);
    MenuItem(const MenuItem& b);

    MenuItem& operator=(const MenuItem& b);

    std::string getString(uint64_t maxLength);


    std::function<void()> func;
    bool realTimeValue = false;
    std::function<std::string()> valueFunc;
    uint64_t offset = 10;
};

#endif
