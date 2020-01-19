#include "BaseMenuItem.h"

#include <utility>


BaseMenuItem::BaseMenuItem(std::string key, Localizer* localizer)
    : key(std::move(key)), localizer(localizer)
{
}


BaseMenuItem::BaseMenuItem(std::string key)
    : key(std::move(key)), localizer(nullptr)
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


void BaseMenuItem::setLocalizer(Localizer* pLocalizer)
{
    this->localizer = pLocalizer;
}


std::string BaseMenuItem::get()
{
    return localizer->get(key);
}