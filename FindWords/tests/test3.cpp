#include "../Menu.h"

#include <ctime>


void f1() { cout << "\n---------1----------\n"; }
void f2() { cout << "\n****2***\n"; }
void f3() { cout << "\n+++++++++++3++++++\n"; }
void f4() { cout << "\n@@@@@@@4@@@@@@@@@\n"; }
void f5() { cout << "\n#########5######\n"; exit(0); }



string strtime = "hellp";


void cycle(Menu& menu)
{
	int k;
	while (true) {
		k = 0;
		strtime = to_string(time(NULL));
		while (k == 224 || k == 0) 
	    {
	        k = menu.click();
	    }

	}
}


int main()
{
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	MenuItem* items = new MenuItem[5];
	
	unsigned int lngs = 1;
	unsigned int mnlen = 5;
	unsigned int anmnlen = 3;
	string s[lngs][mnlen] = {{"1. Первый пункт", "2. Второй пункт", "3. Третий (как неожиданно) пункт", "4. Четвёртый пункт", "5. Выход"}};
	string s1[lngs][anmnlen] = {{"Меню", "Выберите пункт меню: ", "Для продолжения нажмите любую клавишу..."}};
	
	function<void()> fs[5] = { f1, function<void()>(f2), function<void()>(f3), function<void()>(f4), function<void()>(f5) };
	
	for (unsigned int i = 0; i < mnlen; i++) {
		string t[lngs];
		for (unsigned int j = 0; j < lngs; j++) {
			t[j] = s[j][i];
		}
		items[i] = MenuItem(t, lngs, fs[i]);
	}
	
	string t[lngs];
	t[0] = s[0][0];
	items[0] = MenuItem(t, 1, fs[0], strtime);
	
	BaseMenuItem anItems[3];
	
	for (unsigned int i = 0; i < anmnlen; i++) {
		string t[lngs];
		for (unsigned int j = 0; j < lngs; j++) {
			t[j] = s1[j][i];
		}
		anItems[i] = BaseMenuItem(t, lngs);
	}
	
	Menu::AnotherMenuItems anItemsStruct = {anItems[0], anItems[1], anItems[2]};
	
	Menu menu(items, anItemsStruct, mnlen, lngs);
	menu.draw();
	cycle(menu);
	
	
	return 0;
}
