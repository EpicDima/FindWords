#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <chrono>
#include <windows.h>


using namespace std;


vector<string> findAllPossibleWords();
vector<string> f(string word, int x, int y, bool** mask);
void openDictionary(string filename);
void changeDictionary();
void outputResult(vector<string> matchedWords, vector<string> possibleWords);
bool comparator(const string &a, const string &b);
void calculate();
void inputTable();
void printMenu();
void menu();


class Timer
{
public:
	Timer() { reset(); } 
	void reset() { start = chrono::high_resolution_clock::now(); }
	unsigned long long elapsedNanoseconds() const
	{
		return chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();
	}
	
private:
	chrono::time_point<chrono::high_resolution_clock> start;
};


class Trie
{
public:
	Trie() { root = new Node(); }
	void insert(string word) { root->insert(word, word.size(), 0); }
	bool findSubstr(string substr) { return root->findSubstr(substr, substr.size(), 0); }
	bool findWord(string word) { return root->findWord(word, word.size(), 0); }
	
private:
	
	class Node 
	{
	public:	
		Node(bool wholeWord = false) { this->wholeWord = wholeWord; }
		
		void insert(string str, unsigned int size, unsigned int index)
		{
			if (letters.count(str[index])) {
			    if (index < size - 1) {
					this->letters.at(str[index])->insert(str, size, index + 1);
				} else {
					this->letters.at(str[index])->wholeWord = true;
				}
			} else {
				if (index < size - 1) {
					Node* node = new Node(false);
					this->letters.insert(pair<char, Node*>(str[index], node));
					node->insert(str, size, index + 1);
				} else {
					this->letters.insert(pair<char, Node*>(str[index], new Node(true)));
				}
			}
		}
		
		bool findSubstr(string substr, unsigned int size, unsigned int index)
		{
			if (index == size) {
				return true;
			}
			if (this->letters.count(substr[index])) {
				return this->letters.at(substr[index])->findSubstr(substr, size, index + 1);
			}
			return false;
		}
		
		bool findWord(string word, unsigned int size, unsigned int index)
		{
			if (index == size && this->wholeWord) {
				return true;
			}
			if (this->letters.count(word[index])) {
				return this->letters.at(word[index])->findWord(word, size, index + 1);
			}
			return false;
		}
		
	private:
		map<char, Node*> letters;
		bool wholeWord;
	};
	
	Node* root;
};


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


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	openDictionary(defaultFilename);
	menu();

	return 0;
}


vector<string> findAllPossibleWords()
{
	vector<string> a;

	bool** mask = new bool* [n];
	for (unsigned int i = 0; i < n; i++) {
		mask[i] = new bool[m];
		for (unsigned int j = 0; j < m; j++) {
			
			if (matrix[i][j] == ' ') {
				mask[i][j] = true;
			} else {
				mask[i][j] = false;
			}
		}
	}
	
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			if (!mask[i][j]) {
				mask[i][j] = true;
				vector<string> q = f(string(1, matrix[i][j]), i, j, mask);
				for (int h = 0; h < q.size(); h++) {
					a.push_back(q[h]);
				}
				mask[i][j] = false;
			}
		}
	}

	return a;
}


vector<string> f(string word, int x, int y, bool** mask)
{
	vector<string> newWords;
	
	if (word.size() >= minLength) {
		newWords.push_back(word);
	}
	
	if (word.size() >= maxLength || !vocabularyWords->findSubstr(word)) {
		return newWords;
	}
	
	int x2;
	int y2;
	
	bool** mask2 = new bool*[n];
	for (unsigned int j = 0; j < n; j++) {
		mask2[j] = new bool[m];
		for (unsigned int h = 0; h < m; h++) {
			mask2[j][h] = mask[j][h];
		}
	}
	
	string word2;
	vector<string> temp;
	
	for (unsigned int i = 0; i < 4; i++) {
		x2 = x + dx[i];
		y2 = y + dy[i];
		if (x2 >= 0 && x2 < n && y2 >= 0 && y2 < m) {
			if (!mask[x2][y2]) {
				mask2[x2][y2] = true;
				word2 = word + matrix[x2][y2];
				temp = f(word2, x2, y2, mask2);
				for (unsigned int i = 0; i < temp.size(); i++) {
					newWords.push_back(temp[i]);
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


void outputResult(vector<string> matchedWords, vector<string> possibleWords)
{
	cout << (russianLanguage ? "Время расчёта (микросекунды):                                         "
	                         : "Calculation time (microseconds):                                      " )
	                         << setw(14) << static_cast<double>(calculationTimeInNanoseconds / 1000.0) << endl << endl;
	cout << (russianLanguage ? "Количество словарных слов, подходящих под условия длины:              "
		                     : "The number of vocabulary words that fit the conditions of the length: ") 
							 << setw(14) << vocabularyWordsSize << endl;
	cout << (russianLanguage ? "Количество возможных слов в таблице:                                  " 
							 : "Number of possible words in the table:                                ") 
							 << setw(14) << possibleWords.size() << endl;
	cout << (russianLanguage ? "Количество совпавших словарных и возможных слов:                      " 
	                         : "Number of matched dictionary and possible words:                      ") 
							 << setw(14) << matchedWords.size() << endl << endl;

	for (unsigned int i = 0; i < matchedWords.size(); i++) {
		cout << matchedWords[i] << endl;
	}
	cout << endl;
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


void calculate()
{
	Timer timer;
	vector<string> possibleWords = findAllPossibleWords();
	vector<string> matchedWords;
	for (unsigned int i = 0; i < possibleWords.size(); i++) {
		if (vocabularyWords->findWord(possibleWords[i])) {
			matchedWords.push_back(possibleWords[i]);
		}
	}
	sort(matchedWords.begin(), matchedWords.end(), comparator);
	matchedWords.resize(unique(matchedWords.begin(), matchedWords.end()) - matchedWords.begin());
	calculationTimeInNanoseconds = timer.elapsedNanoseconds();
	outputResult(matchedWords, possibleWords);
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

