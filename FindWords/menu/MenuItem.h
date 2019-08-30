#ifndef MENUITEM_H
#define MENUITEM_H

#include "BaseMenuItem.h"

#include <functional>


class MenuItem : public BaseMenuItem
{
public:
    MenuItem() {}


    MenuItem(string* item, unsigned int languagesNumber, function<void()> func)
     : BaseMenuItem(item, languagesNumber), func(func)
    {}


    MenuItem(string* item, unsigned int languagesNumber, function<void()> func, function<string()> valueFunc)
     : MenuItem(item, languagesNumber, func)
    {
        this->valueFunc = valueFunc;
        this->realTimeValue = true;
    }


    MenuItem(const MenuItem& b) : MenuItem(b.item, b.languagesNumber, b.func)
    {
        if (b.realTimeValue) {
            this->valueFunc = b.valueFunc;
            this->realTimeValue = true;
        }
    }


    MenuItem& operator=(const MenuItem& b)
    {
        if (this != &b) {
            this->~MenuItem();
            new (this) MenuItem(b);
        }
        return *this;
    }


    string getString(unsigned int languageIndex, unsigned int maxLength)
    {
        assert(languageIndex >= 0 && languageIndex < languagesNumber);
        string leftPart = item[languageIndex];
        if (realTimeValue) {
            string middlePart = string(maxLength - leftPart.size() + offset, ' ');
            return leftPart + middlePart + '(' + valueFunc() + ')';
        } else {
            return leftPart;
        }
    }


    function<void()> func;
    bool realTimeValue = false;
    function<string()> valueFunc;
    unsigned int offset = 10;
};

#endif
