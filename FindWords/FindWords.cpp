#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

/*
vector<string> f(vector<string> words, string chars, unsigned int index)
{
	vector<string> newWords;
	unsigned int size = words.size();
	unsigned int charSize = chars.size();
	for (unsigned int i = 0; i < size; i++) {
		if (words[i].size() < index + 1) {
			newWords.push_back(words[i]);
		} else {
			for (unsigned int j = 0; j < charSize; j++) {
				if (words[i][index] == chars[j])
			words[i][index] == ch || w
			}
		}
	}
	return newWords;
}*/

unsigned int n;
unsigned int m;
string* matrix;

const unsigned int minLength = 4;
const unsigned int maxLength = 10;

class Node
{
public:

	Node(string word, int x, int y, bool** mask)
	{
		this->word = word;
		this->x = x;
		this->y = y;
		this->mask = mask;

		pressF();
	}

	vector<string> getWords()
	{
		vector<string> a;
		if (this->word.size() >= minLength) {
			a.push_back(this->word);
		}
		
		for (int i = 0; i < this->nodes.size(); i++) {
			vector<string> t = this->nodes[i]->getWords();
			for (int j = 0; j < t.size(); j++) {
				a.push_back(t[j]);
			}
		}
		return a;
	}

	static int dx[4];
	static int dy[4];


private:

	void pressF()
	{
		if (this->word.size() >= maxLength) {
			return;
		}
		int x2;
		int y2;
		for (int i = 0; i < 4; i++) {
			x2 = this->x + this->dx[i];
			y2 = this->y + this->dy[i];
			if (x2 >= 0 && x2 < n && y2 >= 0 && y2 < m) {
				if (!this->mask[x2][y2]) {
					bool** mask2 = new bool*[n];
					for (int j = 0; j < n; j++) {
						mask2[j] = new bool[m];
						for (int h = 0; h < m; h++) {
							mask2[j][h] = this->mask[j][h];
						}
					}
					mask2[x2][y2] = true;
					string word2 = this->word + matrix[x2][y2];
					this->nodes.push_back(new Node(word2, x2, y2, mask2));
				}
			}
		}
	}

	int x, y;
	bool** mask;
	string word;
	vector<Node*> nodes;
};

int Node::dx[] = { 1, -1, 0, 0 };
int Node::dy[] = { 0, 0, 1, -1 };


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
			Node tempNode(string(1, matrix[i][j]), i, j, mask);
			vector<string> q = tempNode.getWords();
			for (int h = 0; h < q.size(); h++) {
				a.push_back(q[h]);
			}
			mask[i][j] = false;
		}
	}

	return a;
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	//fstream vocabularyFile("C:/Users/Dima/Desktop/C++/Филворды/dictionary cp1251.txt");
	fstream vocabularyFile("C:/Users/Dima/Desktop/C++/Филворды/word_rus.txt");
	const unsigned int vocabularySize = 34010;

	vector<string> vocabularyWords;
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
	
	

	//for (unsigned int i = 0; i < possibleWords.size(); i++) {
	//	cout << possibleWords[i] << endl;
	//}

	for (unsigned int i = 0; i < matchedWords.size(); i++) {
		cout << matchedWords[i] << endl;
	}

	return 0;
}
