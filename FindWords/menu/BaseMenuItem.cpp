#include "BaseMenuItem.h"


BaseMenuItem::BaseMenuItem(std::string key, Localizer* localizer)
    : key(key), localizer(localizer)
{
}


BaseMenuItem::BaseMenuItem(std::string key)
    : key(key), localizer(nullptr)
{
}


BaseMenuItem::BaseMenuItem(const BaseMenuItem& b) 
    : BaseMenuItem(b.key, b.localizer)
{
}


BaseMenuItem& BaseMenuItem::operator=(const BaseMenuItem& b)
{
    if (this != &b) {
        this->~BaseMenuItem();
        new (this) BaseMenuItem(b);
    }
    return *this;
}


void BaseMenuItem::setLocalizer(Localizer* localizer)
{
    this->localizer = localizer;
}


std::string BaseMenuItem::get()
{
    return localizer->get(key);
}