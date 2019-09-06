#include "MenuItem.h"


MenuItem::MenuItem(std::string* item, uint64_t languagesNumber, std::function<void()> func)
 : BaseMenuItem(item, languagesNumber), func(func)
{}


MenuItem::MenuItem(std::string* item, uint64_t languagesNumber, std::function<void()> func, std::function<std::string()> valueFunc)
 : MenuItem(item, languagesNumber, func)
{
    this->valueFunc = valueFunc;
    this->realTimeValue = true;
}


MenuItem::MenuItem(const MenuItem& b) : MenuItem(b.item, b.languagesNumber, b.func)
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


std::string MenuItem::getString(uint64_t languageIndex, uint64_t maxLength)
{
    assert(languageIndex >= 0 && languageIndex < languagesNumber);
    std::string leftPart = item[languageIndex];
    if (realTimeValue) {
        std::string middlePart = std::string(maxLength - leftPart.size() + offset, ' ');
        return leftPart + middlePart + '(' + valueFunc() + ')';
    } else {
        return leftPart;
    }
}