#ifndef FINDWORDSCONSTANTS_H
#define FINDWORDSCONSTANTS_H


#include <string>

using namespace std;


namespace fw
{
extern const unsigned int LanguagesNumber = 2;
extern const unsigned int MenuItemsNumber = 5;
extern const unsigned int AuxiliaryMenuItemsNumber = 3;


class MenuConstants
{
public:
    static string MenuItemsStrings[MenuItemsNumber][LanguagesNumber];
    static string AuxiliaryMenuItemsStrings[AuxiliaryMenuItemsNumber][LanguagesNumber];
};

string MenuConstants::MenuItemsStrings[MenuItemsNumber][LanguagesNumber] = {
    {"1. Ввести таблицу букв", "1. Enter letter table"},
    {"2. Изменить необходимость нахождения комбинации", "2. Change the need to find a combination"},
    {"3. Изменить диапазон длин слов", "3. Change default length range of words"},
    {"4. Выбрать другой файл-словарь", "4. Select another dictionary file"}, {"5. Выход", "5. Exit"}};
string MenuConstants::AuxiliaryMenuItemsStrings[AuxiliaryMenuItemsNumber][LanguagesNumber] = {{"Меню", "Menu"},
    {"Выберите пункт меню: ", "Select menu item: "},
    {"Для продолжения нажмите любую клавишу...", "Press any key now to continue..."}};

extern const string PathToTheDictionaryString[LanguagesNumber] = {
    "Введите путь к файлу-словарю: ", "Enter the path to the dictionary file: "};
extern const string CalculationTimeMicrosecondsString[LanguagesNumber] = {
    "Время расчёта (микросекунды):                            ", "Calculation time (microseconds):                 "};
extern const string CalculationTimeMillisecondsString[LanguagesNumber] = {
    "Время расчёта (миллисекунды):                            ", "Calculation time (milliseconds):                 "};
extern const string CalculationTimeSecondsString[LanguagesNumber] = {
    "Время расчёта (секунды):                                 ", "Calculation time (seconds):                      "};
extern const string VocabularyWordsSizeString[LanguagesNumber] = {
    "Количество словарных слов:                               ", "Number of vocabulary words:                      "};
extern const string PossibleWordsSizeString[LanguagesNumber] = {
    "Количество возможных слов в таблице:                     ", "Number of possible words in the table:           "};
extern const string MatchedWordsSizeString[LanguagesNumber] = {
    "Количество совпавших словарных и возможных слов:         ", "Number of matched dictionary and possible words: "};
extern const string CombinedWordsSizeString[LanguagesNumber] = {
    "Количество совпавших слов, которые можно скомбинировать: ", "Number of matched words that can be combined:    "};
extern const string ColoredTablesString[LanguagesNumber] = {"Раскрашенные таблицы", "Colored tables"};
extern const string MatchedWordsString[LanguagesNumber] = {
    "Список совпавших слов (без повторений): ", "A list of matching words (without repetition): "};
extern const string CombinedWordsString[LanguagesNumber] = {
    "Список совпавших слов, которые можно скомбинировать (без повторений): ",
    "List of matching words that can be combined (without repetition): "};
extern const string EnterLinesString[LanguagesNumber] = {"Введите количество строк: ", "Enter the number of lines: "};
extern const string EnterColumnsString[LanguagesNumber] = {
    "Введите количество столбцов: ", "Enter the number of columns: "};
extern const string EnterLettersString[LanguagesNumber] = {
    "Вводите буквы (пробел является ячейкой без буквы)", "Enter letters (a space is a cell without a letter)"};
extern const string EnterMinLengthString[LanguagesNumber] = {
    "Введите минимальную длину слова: ", "Enter the minimum word length: "};
extern const string EnterMaxLengthString[LanguagesNumber] = {
    "Введите максимальную длину слова: ", "Enter the maximum word length: "};
extern const string EnabledStateString[LanguagesNumber] = {"включено", "enabled"};
extern const string DisabledStateString[LanguagesNumber] = {"отключено", "disabled"};
extern const string MinimumString[LanguagesNumber] = {"минимум: ", "minimum: "};
extern const string MaximumString[LanguagesNumber] = {";  максимум: ", ";  maximum: "};
extern const string VocabularySizeString[LanguagesNumber] = {"размер словаря: ", "vocabulary size: "};


extern const unsigned int attributesLength = 15;
extern const WORD attributesBackground[attributesLength] = {BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE,
    BACKGROUND_INTENSITY, BACKGROUND_RED | BACKGROUND_GREEN, BACKGROUND_RED | BACKGROUND_BLUE,
    BACKGROUND_RED | BACKGROUND_INTENSITY, BACKGROUND_GREEN | BACKGROUND_BLUE, BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    BACKGROUND_BLUE | BACKGROUND_INTENSITY, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY};


extern const WORD attributesForeground[attributesLength] = {FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE,
    FOREGROUND_INTENSITY, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_BLUE | FOREGROUND_INTENSITY, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY};

}   // namespace fw

#endif
