#include "Menu.h"


Menu::Menu(
    MenuItem* menuItems, AnotherMenuItems anotherMenuItems, uint64_t menuLength, uint64_t languagesNumber)
 : menuLength(menuLength), languagesNumber(languagesNumber), anotherMenuItems(anotherMenuItems)
{
    this->menuItems = new MenuItem[menuLength];
    for (uint64_t i = 0; i < menuLength; i++) {
        this->menuItems[i] = MenuItem(menuItems[i]);
    }
}


int64_t Menu::click()
{
    bool enterBeenCalled = false;
    int64_t k = _getch();
    switch (k) {
        case 13:
            if (!pause) {
                enterBeenCalled = true;
                enter();
            }
            break;
        case 75: moveLeft(); break;
        case 77: moveRight(); break;
        case 80: moveDown(); break;
        case 72: moveUp(); break;
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57: moveByNumber(k - 48); break;
        default: break;
    }

    if (!enterBeenCalled && pause) {
        pause = false;
        draw();
    }
    return k;
}


void Menu::clear()
{
    COORD info = getConsoleInfo();
    setCursorPosition(0, 0);
    DWORD written;
    FillConsoleOutputCharacterA(handle, ' ', info.X * info.Y, {0, 0}, &written);
    FillConsoleOutputAttribute(handle, defaultFontAttribute, info.X * info.Y, {0, 0}, &written);
    setCursorPosition(0, 0);
}


uint64_t Menu::maxMenuItemLength()
{
    size_t maxLength = anotherMenuItems.menu[activeLanguage].size();
    if (anotherMenuItems.choose[activeLanguage].length() > maxLength) {
        maxLength = anotherMenuItems.choose[activeLanguage].size();
    }
    for (uint64_t i = 0; i < menuLength; i++) {
        if (menuItems[i][activeLanguage].length() > maxLength) {
            maxLength = menuItems[i][activeLanguage].size();
        }
    }
    return static_cast<uint64_t>(maxLength);
}


void Menu::draw()
{
    clear();
    uint64_t maxLength = maxMenuItemLength();
    std::cout << anotherMenuItems.menu[activeLanguage] << std::endl;
    for (uint64_t i = 0; i < menuLength; i++) {
        if (i == activeItem) {
            colorPrint(menuItems[i].getString(activeLanguage, maxLength));
        } else {
            std::cout << menuItems[i].getString(activeLanguage, maxLength);
        }
        std::cout << std::endl;
    }
    std::cout << anotherMenuItems.choose[activeLanguage] << activeItem + 1;
}


void Menu::enter()
{
    if (!pause) {
        std::cout << "\n\n\n";
        menuItems[activeItem].func();
        std::cout << "\n\n" << anotherMenuItems.pause[activeLanguage];
        pause = true;
    }
}


void Menu::moveUp()
{
    if (activeItem > 0) {
        activeItem--;
    } else {
        activeItem = menuLength - 1;
    }
    draw();
}


void Menu::moveDown()
{
    if (activeItem < menuLength - 1) {
        activeItem++;
    } else {
        activeItem = 0;
    }
    draw();
}


void Menu::moveRight()
{
    if (activeLanguage > 0) {
        activeLanguage--;
    } else {
        activeLanguage = languagesNumber - 1;
    }
    draw();
}


void Menu::moveLeft()
{
    if (activeLanguage < languagesNumber - 1) {
        activeLanguage++;
    } else {
        activeLanguage = 0;
    }
    draw();
}


void Menu::moveByNumber(uint64_t number)
{
    if (number >= 1 && number <= menuLength && menuLength < 10) {
        activeItem = number - 1;
        draw();
    }
}


void Menu::colorPrint(std::string&& s)
{
    SetConsoleTextAttribute(handle, activeFontAttribute);
    std::cout << s;
    SetConsoleTextAttribute(handle, defaultFontAttribute);
}


void Menu::setCursorPosition(int64_t x, int64_t y)
{
    COORD xy{static_cast<int16_t>(x), static_cast<int16_t>(y)};
    SetConsoleCursorPosition(handle, xy);
}


COORD Menu::getConsoleInfo()
{
    CONSOLE_SCREEN_BUFFER_INFO bi;
    GetConsoleScreenBufferInfo(handle, &bi);
    COORD info = {bi.dwSize.X, static_cast<int16_t>(bi.dwCursorPosition.Y + 1)};
    return info;
}