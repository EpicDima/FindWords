#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;


int dx[] = { 1, -1, 0, 0 };
int dy[] = { 0, 0, 1, -1 };

unsigned int n;
unsigned int m;
string* matrix;

const unsigned int minLength = 4;
const unsigned int maxLength = 10;

vector<string> vocabularyWords;


void input()
{
	cout << "Введите количество строк: ";
	cin >> n;
	cout << "Введите количество столбцов: ";
	cin >> m;

	matrix = new string[n];

	cout << "Вводите буквы (строка пишется без пробелов)\n";
	for (unsigned int i = 0; i < n; i++) {
		cin >> matrix[i];
	}
}


bool isSubstringInDictionary(string str)
{
	unsigned int size = vocabularyWords.size();
	for (unsigned int i = 0; i < size; i++) {
		if (vocabularyWords[i].find(str) == 0) {
			return true;
		}
	}
	return false;
}


vector<string> f(string word, int x, int y, bool** mask)
{
	vector<string> newWords;
	
	if (word.size() >= minLength) {
		newWords.push_back(word);
	}
	
	if (word.size() >= maxLength) {
		return newWords;
	}
	
	if (!isSubstringInDictionary(word)) {
		return newWords;
	}
	
	int x2;
	int y2;
	for (int i = 0; i < 4; i++) {
		x2 = x + dx[i];
		y2 = y + dy[i];
		if (x2 >= 0 && x2 < n && y2 >= 0 && y2 < m) {
			if (!mask[x2][y2]) {
				bool** mask2 = new bool*[n];
				for (int j = 0; j < n; j++) {
					mask2[j] = new bool[m];
					for (int h = 0; h < m; h++) {
						mask2[j][h] = mask[j][h];
					}
				}
				mask2[x2][y2] = true;
				string word2 = word + matrix[x2][y2];
				
				vector<string> temp = f(word2, x2, y2, mask2);
				for (int i = 0; i < temp.size(); i++) {
					newWords.push_back(temp[i]);
				}
			}
		}
	}

	return newWords;
}


vector<string> findAllPossibleWords()
{
	vector<string> a;

	bool** mask = new bool* [n];
	for (int i = 0; i < n; i++) {
		mask[i] = new bool[m];
		for (int j = 0; j < m; j++) {
			mask[i][j] = false;
		}
	}
	
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			mask[i][j] = true;
			vector<string> q = f(string(1, matrix[i][j]), i, j, mask);
			for (int h = 0; h < q.size(); h++) {
				a.push_back(q[h]);
			}
			mask[i][j] = false;
		}
	}

	return a;
}


bool comparator(const string &a, const string &b)
{
    if (a.size() > b.size()) return true;
    return false;
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	fstream vocabularyFile("dictionary.txt");
	const unsigned int vocabularySize = 56290;

	vocabularyWords.reserve(vocabularySize);

	for (int i = 0; i < vocabularySize; i++) {
		string t;
		vocabularyFile >> t;
		int size = t.size();
		if (size >= minLength && size <= maxLength) {
			vocabularyWords.push_back(t);
		}
	}
	
	input();

	cout << "\n\n";

	vector<string> possibleWords = findAllPossibleWords();
	
	cout << "Нашли возможные слова в количестве: " << possibleWords.size() << "\n\n";

	vector<string> matchedWords;
	
	unsigned int vocabularyWordsSize = vocabularyWords.size();
	unsigned int possibleWordsSize = possibleWords.size();
	
	for (unsigned int i = 0; i < vocabularyWordsSize; i++) {
		for (unsigned int j = 0; j < possibleWordsSize; j++) {
			if (vocabularyWords[i].compare(possibleWords[j]) == 0) {
				matchedWords.push_back(possibleWords[j]);
			}
		}
	}
	

	sort(matchedWords.begin(), matchedWords.end(), comparator);

	for (unsigned int i = 0; i < matchedWords.size(); i++) {
		cout << matchedWords[i] << endl;
	}

	return 0;
}
