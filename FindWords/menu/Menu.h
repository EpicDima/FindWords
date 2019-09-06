#ifndef MENU_H
#define MENU_H


#include "MenuItem.h"

#include <conio.h>
#include <iostream>
#include <windows.h>


const WORD defaultFontAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const WORD activeFontAttribute = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;


class Menu
{
public:
    struct AnotherMenuItems
    {
        BaseMenuItem menu;
        BaseMenuItem choose;
        BaseMenuItem pause;
    };


    Menu(MenuItem* menuItems, AnotherMenuItems anotherMenuItems, uint64_t menuLength, uint64_t languagesNumber);

    int64_t click();

    void clear();
    uint64_t maxMenuItemLength();
    void draw();

    void enter();

    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void moveByNumber(uint64_t number);


    template<typename T> T chooseElementFromArrayByActiveLanguage(T* array)
    {
        return array[activeLanguage];
    }


private:
    uint64_t menuLength;
    uint64_t languagesNumber;
    MenuItem* menuItems;
    AnotherMenuItems anotherMenuItems;

    uint64_t activeItem = 0;
    uint64_t activeLanguage = 0;
    bool pause = false;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);


    void colorPrint(std::string&& s);
    void setCursorPosition(int64_t x, int64_t y);
    COORD getConsoleInfo();
};


#endif
