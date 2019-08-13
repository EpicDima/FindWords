#include "Trie.h"
#include "Timer.h"
#include "BitMask.h"
#include "Bitset.h"
#include "FindWordsConstants.h"
#include "Menu.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <windows.h>



vector<pair<string, BitMask>> findAllPossibleWords();
vector<pair<string, BitMask>> f(string word, int x, int y, BitMask mask);
void openDictionary(string filename);
void changeDictionary();
void outputResult(vector<pair<string, BitMask>> matchedWords, vector<pair<string, BitMask>> possibleWords);
void colorPrint(string s, WORD attribute);
void outputMatrix(vector<pair<string, BitMask>> matchedWords);
bool comparator(const string &a, const string &b);
template<typename T1>
void f2(Bitset<T1> &mask, Bitset<T1> indexes, unsigned int start);
void ffff(vector<pair<string, BitMask>> matchedWords);
void calculate();
void inputTable();
void changeMinAndMaxWordLength();
void printMenu();
void menu();


WORD attributes[] = {FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE, FOREGROUND_INTENSITY, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE,  BACKGROUND_INTENSITY};	

const int dx[] = { 1, -1, 0, 0 };
const int dy[] = { 0, 0, 1, -1 };

unsigned int n;
unsigned int m;
char** matrix;

unsigned int minLength = 4;
unsigned int maxLength = 10;

Trie* vocabularyWords;
unsigned int vocabularyWordsSize;

bool russianLanguage = true;
const string defaultFilename = "dictionary.txt";

unsigned long long calculationTimeInNanoseconds;

vector<Bitset<unsigned long long>> bitsetMasks;
vector<Bitset<unsigned long long>> results;
unsigned int matchedWordsSize = 0;


Menu* createMenu()
{
	function<void()> funcs[CONSTANTS::MenuItemsNumber] = {[]() { inputTable(); calculate(); }, 
		changeDictionary, changeMinAndMaxWordLength,      []() { exit(0); } };
		
	MenuItem* items = new MenuItem[CONSTANTS::MenuItemsNumber];
	for (unsigned int i = 0; i < CONSTANTS::MenuItemsNumber; i++) {
		items[i] = MenuItem(CONSTANTS::MenuItemsStrings[i], CONSTANTS::LanguagesNumber, funcs[i]);
	}
	
	BaseMenuItem auxItems[3];
	for (unsigned int i = 0; i < CONSTANTS::AuxiliaryMenuItemsNumber; i++) {
		auxItems[i] = BaseMenuItem(CONSTANTS::AuxiliaryMenuItemsStrings[i], CONSTANTS::LanguagesNumber);
	}
	Menu::AnotherMenuItems auxItemsStruct = { auxItems[0], auxItems[1], auxItems[2] };
	
	return new Menu(items, auxItemsStruct, CONSTANTS::MenuItemsNumber, CONSTANTS::LanguagesNumber);
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	openDictionary(defaultFilename);
	Menu mainmenu = *createMenu();
	mainmenu.draw();
	while (true) {
		mainmenu.click();
	}
	
	//menu();

	return 0;
}


vector<pair<string, BitMask>> findAllPossibleWords()
{
	vector<pair<string, BitMask>> a;

	BitMask mask(n, m);
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			mask[i][j] = (matrix[i][j] == ' ');
		}
	}
	
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			if (!mask[i][j]) {
				mask[i][j] = true;
				vector<pair<string, BitMask>> q = f(string(1, matrix[i][j]), i, j, mask);
				mask[i][j] = false;
				for (unsigned int h = 0; h < q.size(); h++) {
					for (unsigned int k = 0; k < n; k++) {
						for (unsigned int g = 0; g < n; g++) {
							q[h].second[k][g] -= mask[k][g];
						}
					}
					a.push_back(q[h]);
				}
			}
		}
	}

	return a;
}


vector<pair<string, BitMask>> f(string word, int x, int y, BitMask mask)
{
	vector<pair<string, BitMask>> newWords;
	
	if (word.size() >= minLength) {
		newWords.push_back(make_pair(word, mask));
	}
	
	if (word.size() >= maxLength || !vocabularyWords->findSubstr(word)) {
		return newWords;
	}
	
	int x2, y2;
	
	BitMask mask2(mask);
	
	string word2;
	vector<pair<string, BitMask>> temp; 
	
	for (unsigned int i = 0; i < 4; i++) {
		x2 = x + dx[i];
		y2 = y + dy[i];
		if (x2 >= 0 && x2 < n && y2 >= 0 && y2 < m) {
			if (!mask[x2][y2]) {
				mask2[x2][y2] = true;
				word2 = word + matrix[x2][y2];
				temp = f(word2, x2, y2, mask2);
				for (unsigned int j = 0; j < temp.size(); j++) {
					newWords.push_back(temp[j]);
				}
				mask2[x2][y2] = false;
			}
		}
	}
	return newWords;
}


void openDictionary(string filename)
{
	ifstream vocabularyFile(filename);
	vocabularyWordsSize = 0;
	vocabularyWords = new Trie();
	string t;
	while (getline(vocabularyFile, t)) {
		vocabularyWords->insert(t);
		vocabularyWordsSize++;
	}
}


void changeDictionary()
{
	string filename;
	cout << endl << (russianLanguage ? "Введите путь к файлу-словарю: " 
                                     : "Enter the path to the dictionary file: ");
	cin >> filename;
	openDictionary(filename);
}



void outputResult(vector<pair<string, BitMask>> matchedWords, vector<pair<string, BitMask>> possibleWords)
{
	cout << (russianLanguage ? "Время расчёта (миллисекунды):                                         "
	                         : "Calculation time (milliseconds):                                      " )
	                         << setw(14) << static_cast<double>(calculationTimeInNanoseconds / 1000000.0) << endl << endl;
	cout << (russianLanguage ? "Количество словарных слов, подходящих под условия длины:              "
		                     : "The number of vocabulary words that fit the conditions of the length: ") 
							 << setw(14) << vocabularyWordsSize << endl;
	cout << (russianLanguage ? "Количество возможных слов в таблице:                                  " 
							 : "Number of possible words in the table:                                ") 
							 << setw(14) << possibleWords.size() << endl;
	cout << (russianLanguage ? "Количество совпавших словарных и возможных слов:                      " 
	                         : "Number of matched dictionary and possible words:                      ") 
							 << setw(14) << matchedWords.size() << endl;
	outputMatrix(matchedWords);
}


void colorPrint(string s, WORD attribute)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, attribute);
	cout << s;
}


void outputMatrix(vector<pair<string, BitMask>> matchedWords)
{
	vector<string> words;
	vector<vector<unsigned int>> v;
	unsigned int max = 0;
	for (unsigned int i = 0; i < results.size(); i++) {
		unsigned int k = 0;
		vector<unsigned int> temp;
		for (unsigned int j = 0; j < n * m; j++) {
			if (results[i][j]) {
				temp.push_back(j);
				words.push_back(matchedWords[j].first);
				k++;
			}
		}
		v.push_back(temp);
		if (k > max) {
			max = k;
		}
	}
	
	sort(words.begin(), words.end(), comparator);
	words.resize(unique(words.begin(), words.end()) - words.begin());
	
	cout << (russianLanguage ? "Количество совпавших слов, которые можно скомбинировать:              " 
	                         : "Number of matched words that can be combined:                         ") 
							 << setw(14) << words.size() << endl << endl;
	
	cout << "\n\n";
	for (unsigned int i = 0; i < words.size(); i++) {
		cout << words[i] << endl;
	}
	cout << "\n\n";
	
	WORD* attrs = new WORD[max];
	unsigned int counter = 0;
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			for (unsigned int k = 0; k < 8; k++) {
				if (counter >= max) {
					break;
				}
				if (!(i % 4 == j % 4 || i % 4 == k % 4 || j % 4 == k % 4)) {
					attrs[counter] = attributes[i] | attributes[j] | attributes[k];
					counter++;
				}
			}
		}
	}
	for (unsigned int h = 0; h < v.size(); h++) {
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
				for (unsigned int k = 0; k < v[0].size(); k++) {
					if (matchedWords[v[0][k]].second[i][j]) {
						colorPrint(string(1, matrix[i][j]), attrs[k]);
					}
				}
			}
			cout << endl;
		}
		colorPrint("\n\n", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
}


bool comparator(const string &a, const string &b)
{
	if (a.size() > b.size()) {
		return true;
	} else if (a.size() < b.size()) {
		return false;
	} else {
		return a > b;
	}
}


template<typename T1>
void f2(Bitset<T1> &mask, Bitset<T1> indexes, unsigned int start)
{
	for (unsigned int i = start; i < matchedWordsSize; i++) {
		if ((mask & bitsetMasks[i]).isAllFalse()) {
			Bitset<T1> temp(mask);
			temp |= bitsetMasks[i];
			
			if (temp.isAllTrue()) {
				indexes.set(i, true);
				results.push_back(indexes);
				return;
			}
			
			Bitset<T1> tempIndexes(indexes);
			tempIndexes.set(i, true);
			f2(temp, tempIndexes, i);
		}
	}
}



void ffff(vector<pair<string, BitMask>> matchedWords)
{
	matchedWordsSize = matchedWords.size();	
	for (unsigned int i = 0; i < matchedWordsSize; i++) {
		bitsetMasks.push_back(Bitset<unsigned long long>(n * m, matchedWords[i].second.getRawMask(), n, m));
	}
	
	
	unsigned int min = 0, x0 = 0, y0 = 0;
	min = ~min;
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			unsigned int s = 0;
			for (unsigned int k = 0; k < matchedWordsSize; k++) {
				s += matchedWords[k].second[i][j];
			}
			if (s < min) {
				min = s;
				x0 = i;
				y0 = j;
			}
		}
	}
	
	for (unsigned int i = 0; i < matchedWordsSize; i++) {
		if (matchedWords[i].second[x0][y0]) {
			Bitset<unsigned long long> indexes(matchedWordsSize);
			indexes.set(i, true);
			f2(bitsetMasks[i], indexes, 0);
		}
	}
	matchedWordsSize = 0;
}


void calculate()
{
	Timer timer;
	vector<pair<string, BitMask>> possibleWords = findAllPossibleWords();
	vector<pair<string, BitMask>> matchedWords;
	for (unsigned int i = 0; i < possibleWords.size(); i++) {
		if (vocabularyWords->findWord(possibleWords[i].first)) {
			matchedWords.push_back(possibleWords[i]);
		}
	}

	ffff(matchedWords);

	calculationTimeInNanoseconds = timer.elapsedNanoseconds();
	outputResult(matchedWords, possibleWords);
	
	results.clear();
}


void inputTable()
{
	cout << endl << (russianLanguage ? "Введите количество строк: " 
	                                 : "Enter the number of lines: ");
	cin >> n;
	cout << (russianLanguage ? "Введите количество столбцов: " 
	                         : "Enter the number of columns: ");
	cin >> m;

	matrix = new char*[n];
	for (unsigned int i = 0; i < n; i++) {
		matrix[i] = new char[m];
	}

	cout << (russianLanguage ? "Вводите буквы (строка пишется без пробелов)" 
	                         : "Enter letters (the string is written without spaces)") << endl << endl;
	
	cin.ignore();
	for (unsigned int i = 0; i < n; i++) {
		cin.getline(matrix[i], m + 1, '\n');
	}
	cout << endl;
}


void changeMinAndMaxWordLength()
{
	unsigned int t;
	cout << (russianLanguage ? "Введите минимальную длину слова: " 
                             : "Enter the minimum word length: ");
	if (!(cin >> t)) {
        cin.clear();
        cin.sync();
        t = minLength;
    } else {
    	minLength = t;
	}
	cout << (russianLanguage ? "Введите максимальную длину слова: " 
                             : "Enter the maximum word length: ");
	if (!(cin >> t)) {
        cin.clear();
        cin.sync();
        t = maxLength;
    } else {
    	maxLength = t;
	}
}


void printMenu()
{
	system("cls");
	cout << (russianLanguage ? "Меню" 
	                         : "Menu") << endl;
	cout << (russianLanguage ? "1. Ввод таблицы букв" 
	                         : "1. Entering a table of letters") << endl;
	cout << (russianLanguage ? "2. Выбор другого файла-словаря" 
	                         : "2. Selecting another dictionary file") << endl;
	cout << (russianLanguage ? "3. Смена языка" 
	                         : "3. Language change") << endl;
	cout << (russianLanguage ? "4. Изменить диапазон длин слов"
	                         : "4. Change default length range of words") << endl;
	cout << (russianLanguage ? "5. Выход" 
	                         : "5. Exit") << endl;
	cout << (russianLanguage ? "Выберите пункт меню: " 
	                         : "Select menu item: ");
}


void menu()
{
	unsigned int k = 0;
	bool flag = false;
	while (!flag) {
		printMenu();
		if (!(cin >> k)) {
	        cin.clear();
	        cin.sync();
	        k = 0;
	    }
	    switch (k) {
	    	case 1:
	    		inputTable();
	    		calculate();
	    		system("pause");
	    		break;
	    	case 2: 
	    		changeDictionary();
	    		break;
	    	case 3:
	    		russianLanguage = !russianLanguage;
	    		break;
	    	case 4:	
	    		changeMinAndMaxWordLength();
	    		break;
	    	case 5:
	    		flag = true;
				break;	
		}
	}
}

