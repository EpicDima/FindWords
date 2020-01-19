#include "MenuItem.h"

#include <utility>


MenuItem::MenuItem(std::string key, std::function<void()> func)
    : BaseMenuItem(std::move(key)), func(std::move(func))
{
}


MenuItem::MenuItem(std::string key, std::function<void()> func, std::function<std::string()> valueFunc)
    : MenuItem(std::move(key), std::move(func))
{
    this->valueFunc = std::move(valueFunc);
    this->realTimeValue = true;
}


MenuItem::MenuItem(const MenuItem& b) : MenuItem(b.key, b.func)
{
    this->localizer = b.localizer;
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