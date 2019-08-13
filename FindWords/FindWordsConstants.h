#ifndef FINDWORDSCONSTANTS_H
#define FINDWORDSCONSTANTS_H


#include <string>

using namespace std;


class CONSTANTS {   
public:
	static const unsigned int LanguagesNumber;
	static const unsigned int MenuItemsNumber;
	static const unsigned int AuxiliaryMenuItemsNumber;
	static string MenuItemsStrings[4][2];
	static string AuxiliaryMenuItemsStrings[3][2];
	
};

const unsigned int CONSTANTS::LanguagesNumber = 2;
const unsigned int CONSTANTS::MenuItemsNumber = 4;
const unsigned int CONSTANTS::AuxiliaryMenuItemsNumber = 3;

string CONSTANTS::MenuItemsStrings[4][2] = {{"1. Ввод таблицы букв", "1. Entering a table of letters"},
                                            {"2. Выбор другого файла-словаря", "2. Selecting another dictionary file"}, 
										    {"3. Изменить диапазон длин слов", "3. Change default length range of words"},
										    {"4. Выход", "4. Exit"}};
string CONSTANTS::AuxiliaryMenuItemsStrings[3][2] = {{"Меню", "Menu"},
												     {"Выберите пункт меню: ", "Select menu item: "},
												     {"Для продолжения нажмите любую клавишу...", "Press any key now to continue..."}};

#endif
