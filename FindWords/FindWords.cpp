#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <windows.h>

using namespace std;


int dx[] = { 1, -1, 0, 0 };
int dy[] = { 0, 0, 1, -1 };

unsigned int n;
unsigned int m;
string* matrix;

const unsigned int minLength = 4;
const unsigned int maxLength = 10;


class Trie
{
public:
	
	Trie()
	{
		root = new Node();
	}
	
	void insert(string word)
	{
		root->insert(word, word.size(), 0);
	}
	
	bool findSubstr(string substr)
	{
		return root->findSubstr(substr, substr.size(), 0);
	}
	
	bool findWord(string word)
	{
		return root->findWord(word, word.size(), 0);
	}
	
private:
	
	class Node 
	{
	public:	
		Node(bool wholeWord = false)
		{
			this->wholeWord = wholeWord;
		}
		
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


Trie* vocabularyWords = new Trie();


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


vector<string> findAllPossibleWords()
{
	vector<string> a;

	bool** mask = new bool* [n];
	for (unsigned int i = 0; i < n; i++) {
		mask[i] = new bool[m];
		for (unsigned int j = 0; j < m; j++) {
			mask[i][j] = false;
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


bool comparator(const string &a, const string &b)
{
    if (a.size() > b.size()) {
    	return true;
	}
    return false;
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	

	fstream vocabularyFile("dictionary.txt");
	const unsigned int vocabularySize = 56255;
	unsigned int vocabularyWordsSize = 0;
	
	for (unsigned int i = 0; i < vocabularySize; i++) {
		string t;
		vocabularyFile >> t;
		int size = t.size();
		if (size >= minLength && size <= maxLength) {
			vocabularyWords->insert(t);
			vocabularyWordsSize++;
		}
	}
	

	input();

	cout << "\n\n";
	
	vector<string> possibleWords = findAllPossibleWords();
	vector<string> matchedWords;

	unsigned int possibleWordsSize = possibleWords.size();
	
	for (unsigned int i = 0; i < possibleWordsSize; i++) {
		if (vocabularyWords->findWord(possibleWords[i])) {
			matchedWords.push_back(possibleWords[i]);
		}
	}
	
	
	unsigned int matchedWordsSize = matchedWords.size();
	
	cout << "Количество словарных слов, подходящих под условия длины: " << setw(8) << vocabularyWordsSize << endl;
	cout << "Количество возможных слов в таблице:                     " << setw(8) << possibleWordsSize << endl;
	cout << "Количество совпавших словарных и возможных слов:         " << setw(8) << matchedWordsSize << endl << endl;

	sort(matchedWords.begin(), matchedWords.end(), comparator);

	for (unsigned int i = 0; i < matchedWordsSize; i++) {
		cout << matchedWords[i] << endl;
	}

	return 0;
}
