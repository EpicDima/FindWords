#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;







void setCursorOn(int x, int y)
{
	COORD xory;
	xory.Y = y;
	xory.X = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xory);
}


class Menu
{
public:
	Menu(string** menuItems, unsigned int menuLength, unsigned int languagesNumber) {
		this->menuLength = menuLength;
		this->languagesNumber = languagesNumber;
		for (unsigned int i = 0; i < menuLength; i++) {
		}
		this->menuItems = new string*[languagesNumber];
		for (unsigned int i = 0; i < languagesNumber; i++) {
			this->menuItems[i] = new string[menuLength];
			for (unsigned int j = 0; j < menuLength; j++) {
				this->menuItems[i][j] = menuItems[i][j];
			}
		}
	}
	
	~Menu()
	{
		for (unsigned int i = 0; i < menuLength; i++) {
			delete[] menuItems[i];
		}
		delete[] menuItems;
	}
	
	void draw()
	{
		system("cls");
		cout << "Меню\n";
		for (int i = 0; i < menuLength; i++) {
			if (i == activeItem) {
				colorPrint(menuItems[activeLanguage][i]);
				cout << endl;
			} else {
				cout << menuItems[activeLanguage][i] << endl;
			}
		}
		cout << "Выберите номер пункта меню: " << activeItem + 1;
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
	
	void moveByNumber(unsigned int number)
	{
		if (number >= 1 && number <= menuLength && menuLength < 10) {
			activeItem = number - 1;
			draw();
		}
	}

private:
	unsigned int menuLength;
	unsigned int languagesNumber;
	string** menuItems;
	
	unsigned int activeItem = 0;
	unsigned int activeLanguage = 0;
	
	WORD defaultFontAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	WORD activeFontAttribute = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	void colorPrint(string s)
	{	
		SetConsoleTextAttribute(handle, activeFontAttribute);
		cout << s;
		SetConsoleTextAttribute(handle, defaultFontAttribute);
	}
};


int main()
{
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	
	string s1[] = {"1. Первый пункт", "2. Второй пункт", "3. Третий (как неожиданно) пункт", "4. Четвёртый пункт", "5. Выход"};
	string** s = new string*[1];
	for (unsigned int i = 0; i < 1; i++) {
		s[i] = new string[5];
		for (unsigned int j = 0; j < 5; j++) {
			s[i][j] = s1[j];
		}
	}
	
	Menu menu(s, 5, 1);
	menu.draw();
	
	int n = 0;
	while (true) {
		n = 0;
		
		while (n == 224 || n == 0) 
	    {
	        n = _getch();
	    }
	    
	    if (n == 80) {
	    	menu.moveDown();
		} else if (n == 72) {
			menu.moveUp();
		} else if (n >= 48 && n <= 57) {
			menu.moveByNumber(n - 48);
		}
	}
	
	return 0;
}
