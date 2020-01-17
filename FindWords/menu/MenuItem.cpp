#include "MenuItem.h"


MenuItem::MenuItem(std::string key, std::function<void()> func)
    : BaseMenuItem(key), func(func)
{
}


MenuItem::MenuItem(std::string key, std::function<void()> func, std::function<std::string()> valueFunc)
    : MenuItem(key, func)
{
    this->valueFunc = valueFunc;
    this->realTimeValue = true;
}


MenuItem::MenuItem(std::string key, std::function<void()> func, Localizer* localizer)
    : BaseMenuItem(key, localizer), func(func)
{
}


MenuItem::MenuItem(std::string key, std::function<void()> func, std::function<std::string()> valueFunc, Localizer* localizer)
    : MenuItem(key, func, localizer)
{
    this->valueFunc = valueFunc;
    this->realTimeValue = true;
}


MenuItem::MenuItem(const MenuItem& b) : MenuItem(b.key, b.func, b.localizer)
{
    if (b.realTimeValue) {
        this->valueFunc = b.valueFunc;
        this->realTimeValue = true;
    }
}


MenuItem& MenuItem::operator=(const MenuItem& b)
{
    if (this != &b) {
        this->~MenuItem();
        new (this) MenuItem(b);
    }
    return *this;
}


std::string MenuItem::getString(uint64_t maxLength)
{
    std::string leftPart = get();
    if (realTimeValue) {
        std::string middlePart = std::string(maxLength - leftPart.size() + offset, ' ');
        return leftPart + middlePart + '(' + valueFunc() + ')';
    } else {
        return leftPart;
    }
}