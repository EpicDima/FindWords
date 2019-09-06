#include "BaseMenuItem.h"


BaseMenuItem::BaseMenuItem() : item(nullptr), languagesNumber(0) {}


BaseMenuItem::BaseMenuItem(std::string* item, uint64_t languagesNumber) : languagesNumber(languagesNumber)
{
    this->item = new std::string[languagesNumber];
    for (uint64_t i = 0; i < languagesNumber; i++) {
        this->item[i] = item[i];
    }
}


BaseMenuItem::BaseMenuItem(const BaseMenuItem& b) : BaseMenuItem(b.item, b.languagesNumber) {}


BaseMenuItem& BaseMenuItem::operator=(const BaseMenuItem& b)
{
    if (this != &b) {
        this->~BaseMenuItem();
        new (this) BaseMenuItem(b);
    }
    return *this;
}


std::string BaseMenuItem::operator[](const uint64_t languageIndex)
{
    assert(languageIndex >= 0 && languageIndex < languagesNumber);
    return item[languageIndex];
}