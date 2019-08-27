#ifndef MENU_H
#define MENU_H


#include "MenuItem.h"

#include <iostream>
#include <windows.h>
#include <conio.h>


const WORD defaultFontAttribute 
	= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const WORD activeFontAttribute 
	= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;


class Menu
{
public:
	
	struct AnotherMenuItems
	{
		BaseMenuItem menu;
		BaseMenuItem choose;
		BaseMenuItem pause;
	};
	

	Menu(MenuItem* menuItems, AnotherMenuItems anotherMenuItems, 
		unsigned int menuLength, unsigned int languagesNumber)
		: anotherMenuItems(anotherMenuItems), menuLength(menuLength), 
		languagesNumber(languagesNumber)
	{
		this->menuItems = new MenuItem[menuLength];
		for (unsigned int i = 0; i < menuLength; i++) {
			this->menuItems[i] = MenuItem(menuItems[i]);
		}
	}
	
	
	int click()
	{
		bool enterBeenCalled = false;
		int k = _getch();
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
			case 49: case 50: case 51: case 52: case 53: case 54: 
			case 55: case 56: case 57: moveByNumber(k - 48); break;
			default: break;
		}
		
		if (!enterBeenCalled && pause) {
			pause = false;
			draw();
		}
		return k;
	}
	
	
	void clear()
	{
		COORD info = getConsoleInfo();
		setCursorPosition(0, 0);
		DWORD written;
		FillConsoleOutputCharacterA(handle, ' ', info.X * info.Y, 
			{ 0, 0 }, &written);
	    FillConsoleOutputAttribute(handle, defaultFontAttribute, 
			info.X * info.Y, { 0, 0 }, &written);
		setCursorPosition(0, 0);
	}

	
	unsigned int maxMenuItemLength()
	{
		size_t maxLength = anotherMenuItems.menu[activeLanguage].size();
		if (anotherMenuItems.choose[activeLanguage].length() > maxLength) {
			maxLength = anotherMenuItems.choose[activeLanguage].size();
		}
		for (unsigned int i = 0; i < menuLength; i++) {
			if (menuItems[i][activeLanguage].length() > maxLength) {
				maxLength = menuItems[i][activeLanguage].size();
			}
		}
		return static_cast<unsigned int>(maxLength);
	}
	

	void draw()
	{
		clear();
		unsigned int maxLength = maxMenuItemLength();
		cout << anotherMenuItems.menu[activeLanguage] << endl;
		for (unsigned int i = 0; i < menuLength; i++) {
			if (i == activeItem) {
				colorPrint(menuItems[i].getString(activeLanguage, maxLength));
			} else {
				cout << menuItems[i].getString(activeLanguage, maxLength);
			}
			cout << endl;
		}
		cout << anotherMenuItems.choose[activeLanguage] << activeItem + 1;
	}

	
	void enter()
	{
		if (!pause) {
			cout << "\n\n\n";
			menuItems[activeItem].func();
			cout << "\n\n" << anotherMenuItems.pause[activeLanguage];
			pause = true;
		}
	}
	

	void moveUp()
	{
		if (activeItem > 0) {
			activeItem--;
		} else {
			activeItem = menuLength - 1;
		}
		draw();
	}

	
	void moveDown()
	{
		if (activeItem < menuLength - 1) {
			activeItem++;
		} else {
			activeItem = 0;
		}
		draw();
	}
	

	void moveRight()
	{
		if (activeLanguage > 0) {
			activeLanguage--;
		} else {
			activeLanguage = languagesNumber - 1;
		}
		draw();
	}
	

	void moveLeft()
	{
		if (activeLanguage < languagesNumber - 1) {
			activeLanguage++;
		} else {
			activeLanguage = 0;
		}
		draw();
	}

	
	void moveByNumber(unsigned int number)
	{
		if (number >= 1 && number <= menuLength && menuLength < 10) {
			activeItem = number - 1;
			draw();
		}
	}

	
	template<typename T>
	T chooseElementFromArrayByActiveLanguage(T* array)
	{
		return array[activeLanguage];
	}


private:
	unsigned int menuLength;
	unsigned int languagesNumber;
	MenuItem* menuItems;
	AnotherMenuItems anotherMenuItems;
	
	unsigned int activeItem = 0;
	unsigned int activeLanguage = 0;
	bool pause = false;
	
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	void colorPrint(string&& s)
	{	
		SetConsoleTextAttribute(handle, activeFontAttribute);
		cout << s;
		SetConsoleTextAttribute(handle, defaultFontAttribute);
	}
	
	void setCursorPosition(int x, int y)
	{
		COORD xy {static_cast<short>(x), static_cast<short>(y)};
		SetConsoleCursorPosition(handle, xy);
	}
	
	COORD getConsoleInfo() 
	{
		CONSOLE_SCREEN_BUFFER_INFO bi;
		GetConsoleScreenBufferInfo(handle, &bi);
		COORD info = { bi.dwSize.X, static_cast<short>(bi.dwCursorPosition.Y + 1) };
		return info;
	}
};


#endif
