#include "service/Timer.h"
#include "bit_classes/BitMask.h"
#include "bit_classes/BitSet.h"
#include "menu/Menu.h"

#include "FindWordsConstants.h"
#include "Trie.h"

#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>



void createMenu();
vector<pair<string, BitMask>> findAllPossibleWords();
vector<pair<string, BitMask>> f(string word, int x, int y, BitMask mask);
void openDictionary(string filename = "dictionary.txt");
void changeDictionary();
void outputResult(vector<pair<string, BitMask>> matchedWords, vector<pair<string, BitMask>> possibleWords);
void printColorChar(char ch, WORD attribute);
pair<pair<vector<string>, vector<vector<unsigned int>>>, unsigned int> decodeResults(vector<pair<string, BitMask>> matchedWords);
void outputMatchedWords(vector<pair<string, BitMask>> matchedWords);
WORD* createAttributesArray(unsigned int max);
void outputTables(vector<pair<string, BitMask>> matchedWords, vector<vector<unsigned int>> decodedResults, WORD* attrs);
void outputCombinationResults(vector<pair<string, BitMask>> matchedWords);
bool comparator(const string &a, const string &b);
void f2(BitSet &mask, BitSet indexes, unsigned int start);
void ffff(vector<pair<string, BitMask>> matchedWords);
void calculate();
unsigned int inputPositiveNumber();
void inputTable();
string getStateOfNeedFindCombination();
string getMinMaxLengthsStr();
string getVocabularySizeStr();
void changeMinAndMaxWordLength();


const unsigned int attributesLength = 8;
const WORD attributes[attributesLength] = {FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE, FOREGROUND_INTENSITY, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE,  BACKGROUND_INTENSITY};	

const int dx[4] = { 1, -1, 0, 0 };
const int dy[4] = { 0, 0, 1, -1 };

unsigned int n;
unsigned int m;
char** matrix;

unsigned int minLength = 4;
unsigned int maxLength = 10;

Trie* vocabularyWords;
unsigned int vocabularyWordsSize;

unsigned long long calculationTimeInNanoseconds;

bool needFindCombination = false;

vector<BitSet> bitsetMasks;
vector<BitSet> results;
unsigned int matchedWordsSize = 0;

Menu *mainmenu;


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	openDictionary();
	createMenu();
	
	while (true) {
		mainmenu->click();
	}

	return 0;
}


void createMenu()
{
	function<void()> funcs[fw::MenuItemsNumber] = { []() { inputTable(); calculate(); }, 
		[]() { needFindCombination = !needFindCombination; }, 
		changeMinAndMaxWordLength, changeDictionary, []() { exit(0); } };
		
	MenuItem* items = new MenuItem[fw::MenuItemsNumber];
	for (unsigned int i = 0; i < fw::MenuItemsNumber; i++) {
		if (i + 1 == 2) {
			items[i] = MenuItem(fw::MenuConstants::MenuItemsStrings[i], fw::LanguagesNumber, funcs[i], getStateOfNeedFindCombination);
		} else if (i + 1 == 3) {
			items[i] = MenuItem(fw::MenuConstants::MenuItemsStrings[i], fw::LanguagesNumber, funcs[i], getMinMaxLengthsStr);
		} else if (i + 1 == 4) {
			items[i] = MenuItem(fw::MenuConstants::MenuItemsStrings[i], fw::LanguagesNumber, funcs[i], getVocabularySizeStr);
		} else {
			items[i] = MenuItem(fw::MenuConstants::MenuItemsStrings[i], fw::LanguagesNumber, funcs[i]);
		}
	}
	
	BaseMenuItem auxItems[fw::AuxiliaryMenuItemsNumber];
	for (unsigned int i = 0; i < fw::AuxiliaryMenuItemsNumber; i++) {
		auxItems[i] = BaseMenuItem(fw::MenuConstants::AuxiliaryMenuItemsStrings[i], fw::LanguagesNumber);
	}
	Menu::AnotherMenuItems auxItemsStruct = { auxItems[0], auxItems[1], auxItems[2] };
	
	mainmenu = new Menu(items, auxItemsStruct, fw::MenuItemsNumber, fw::LanguagesNumber);
	mainmenu->draw();
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
	cout << endl << mainmenu->chooseElementFromArrayByActiveLanguage(fw::PathToTheDictionaryString);
	cin >> filename;
	openDictionary(filename);
}


pair<unsigned long long, string> getCalculationTime()
{
	unsigned long long calculationTime;
	string strCalculationTime;
	if (calculationTimeInNanoseconds > 100000000000) {
		calculationTime = calculationTimeInNanoseconds / 1000000000;
		strCalculationTime = mainmenu->chooseElementFromArrayByActiveLanguage(fw::CalculationTimeSecondsString);
	} else if (calculationTimeInNanoseconds > 100000000) {
		calculationTime = calculationTimeInNanoseconds / 1000000;
		strCalculationTime = mainmenu->chooseElementFromArrayByActiveLanguage(fw::CalculationTimeMillisecondsString);
	} else {
		calculationTime = calculationTimeInNanoseconds / 1000;
		strCalculationTime = mainmenu->chooseElementFromArrayByActiveLanguage(fw::CalculationTimeMicrosecondsString);
	}
	return make_pair(calculationTime, strCalculationTime);
}


void outputResult(vector<pair<string, BitMask>> matchedWords, vector<pair<string, BitMask>> possibleWords)
{
	pair<unsigned long long, string> calculationTimePair = getCalculationTime();
	cout << endl << calculationTimePair.second
	     << setw(14) << calculationTimePair.first << endl << endl;
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::VocabularyWordsSizeString)
		 << setw(14) << vocabularyWordsSize << endl;
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::PossibleWordsSizeString)
		 << setw(14) << possibleWords.size() << endl;
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::MatchedWordsSizeString)
		 << setw(14) << matchedWords.size() << endl;
		 
	if (needFindCombination) {
		outputCombinationResults(matchedWords);
	} else {
		outputMatchedWords(matchedWords);
	}
}


void printColorChar(char ch, WORD attribute)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, attribute);
	cout << ch;
}


void outputMatchedWords(vector<pair<string, BitMask>> matchedWords)
{
	vector<string> tempwords;
	for (unsigned int i = 0; i < matchedWords.size(); i++) {
		tempwords.push_back(matchedWords[i].first);
	}
	sort(tempwords.begin(), tempwords.end(), comparator);
	tempwords.resize(unique(tempwords.begin(), tempwords.end()) - tempwords.begin());
	
	if (tempwords.size() > 0) {
		cout << "\n\n\n" << mainmenu->chooseElementFromArrayByActiveLanguage(fw::MatchedWordsString) << endl;
		for (unsigned int i = 0; i < tempwords.size(); i++) {
			cout << tempwords[i] << endl;
		}
	}
}


pair<pair<vector<string>, vector<vector<unsigned int>>>, unsigned int> decodeResults(vector<pair<string, BitMask>> matchedWords)
{
	vector<string> words;
	vector<vector<unsigned int>> decodedResults;
	unsigned int max = 0;
	for (unsigned int i = 0; i < results.size(); i++) {
		unsigned int k = 0;
		vector<unsigned int> temp;
		for (unsigned int j = 0; j < matchedWords.size(); j++) {
			if (results[i][j]) {
				temp.push_back(j);
				words.push_back(matchedWords[j].first);
				k++;
			}
		}
		decodedResults.push_back(temp);
		max = k > max ? k : max;
	}
	sort(words.begin(), words.end(), comparator);
	words.resize(unique(words.begin(), words.end()) - words.begin());
	return make_pair(make_pair(words, decodedResults), max);
}


WORD* createAttributesArray(unsigned int max)
{
	WORD* attrs = new WORD[max];
	unsigned int counter = 0;
	for (unsigned int i = 0; i < attributesLength; i++) {
		for (unsigned int j = 0; j < attributesLength; j++) {
			for (unsigned int k = 0; k < attributesLength; k++) {
				if (counter >= max) {
					break;
				}
				if (!(i % (attributesLength / 2) == j % (attributesLength / 2) 
				      || i % (attributesLength / 2) == k % (attributesLength / 2) 
				      || j % (attributesLength / 2) == k % (attributesLength / 2))) {
					attrs[counter] = attributes[i] | attributes[j] | attributes[k];
					counter++;
				}
			}
		}
	}
	return attrs;
}


void outputTables(vector<pair<string, BitMask>> matchedWords, vector<vector<unsigned int>> decodedResults, WORD* attrs)
{
	cout << "\n\n\n" << mainmenu->chooseElementFromArrayByActiveLanguage(fw::ColoredTablesString) << "\n";
	for (unsigned int h = 0; h < decodedResults.size(); h++) {
		cout << "\n" << h + 1 << ")\n"; 
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
				if (matrix[i][j] != ' ') {
					for (unsigned int k = 0; k < decodedResults[h].size(); k++) {
						if (matchedWords[decodedResults[h][k]].second[i][j]) {
							printColorChar(matrix[i][j], attrs[k]);
							break;
						}
					}
				} else {
					cout << " ";
				}
			}
			cout << endl;
		}
		printColorChar('\n', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
}


void outputCombinationResults(vector<pair<string, BitMask>> matchedWords)
{
	pair<pair<vector<string>, vector<vector<unsigned int>>>, unsigned int> p = decodeResults(matchedWords);
	vector<string> words = p.first.first;
	vector<vector<unsigned int>> decodedResults = p.first.second;
	unsigned int max = p.second;
	
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::CombinedWordsSizeString)
		 << setw(14) << words.size();
	
	if (words.size() > 0) {
		cout << "\n\n\n" << mainmenu->chooseElementFromArrayByActiveLanguage(fw::CombinedWordsString) << endl;
		for (unsigned int i = 0; i < words.size(); i++) {
			cout << words[i] << endl;
		}
		
		WORD* attrs = createAttributesArray(max);
		outputTables(matchedWords, decodedResults, attrs);
	} else {
		outputMatchedWords(matchedWords);
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


void f2(BitSet &mask, BitSet indexes, unsigned int start)
{
	for (unsigned int i = start; i < matchedWordsSize; i++) {
		if ((mask & bitsetMasks[i]).isAllFalse()) {
			BitSet temp(mask);
			temp |= bitsetMasks[i];
			if (temp.isAllTrue()) {
				indexes.set(i, true);
				results.push_back(indexes);
				return;
			}
			BitSet tempIndexes(indexes);
			tempIndexes.set(i, true);
			f2(temp, tempIndexes, i);
		}
	}
}


void ffff(vector<pair<string, BitMask>> matchedWords)
{
	BitSet originalMask(n * m);
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			originalMask.set(i * m + j, (matrix[i][j] == ' '));
		}
	}
	
	matchedWordsSize = matchedWords.size();
	for (unsigned int i = 0; i < matchedWordsSize; i++) {
		bitsetMasks.push_back(BitSet(n * m, matchedWords[i].second.getRawMask(), n, m) & ~originalMask);
	}
	
	unsigned int min = 0, x0 = 0, y0 = 0;
	min = ~min;
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			unsigned int s = 0;
			for (unsigned int k = 0; k < matchedWordsSize; k++) {
				s += matchedWords[k].second[i][j];
			}
			if (s < min && s != 0) {
				min = s;
				x0 = i;
				y0 = j;
			}
		}
	}
	
	for (unsigned int i = 0; i < matchedWordsSize; i++) {
		if (matchedWords[i].second[x0][y0]) {
			BitSet indexes(matchedWordsSize);
			indexes.set(i, true);
			BitSet tempMask(bitsetMasks[i] | originalMask);
			if (!(tempMask).isAllTrue()) {
				f2(tempMask, indexes, 0);
			} else {
				results.push_back(indexes);
			}
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

	if (needFindCombination) {
		ffff(matchedWords);
	}

	calculationTimeInNanoseconds = timer.elapsedNanoseconds();
	outputResult(matchedWords, possibleWords);
	
	bitsetMasks.clear();
	results.clear();
}


unsigned int inputPositiveNumber()
{
	long long k = -1;
	while (k <= 0) {
		while (!(cin >> k)) {
			cin.clear();
	        cin.sync();
		}
	}
	return static_cast<unsigned int>(k);
}


void inputTable()
{
	cout << endl << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterLinesString);
	n = inputPositiveNumber();
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterColumnsString);
	m = inputPositiveNumber();

	matrix = new char*[n];
	for (unsigned int i = 0; i < n; i++) {
		matrix[i] = new char[m];
		for (unsigned int j = 0; j < m; j++) {
			matrix[i][j] = ' ';
		}
	}

	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterLettersString) << endl << endl;
	
	cin.ignore();
	for (unsigned int i = 0; i < n; i++) {
		cin.getline(matrix[i], m + 1, '\n');
	}
	cout << endl;
}


string getStateOfNeedFindCombination()
{
	if (needFindCombination) {
		return mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnabledStateString);
	} else {
		return mainmenu->chooseElementFromArrayByActiveLanguage(fw::DisabledStateString);
	}
}


string getMinMaxLengthsStr()
{
	return mainmenu->chooseElementFromArrayByActiveLanguage(fw::MinimumString) + to_string(minLength)
		+ mainmenu->chooseElementFromArrayByActiveLanguage(fw::MaximumString) + to_string(maxLength);
}


string getVocabularySizeStr()
{
	return mainmenu->chooseElementFromArrayByActiveLanguage(fw::VocabularySizeString) + to_string(vocabularyWordsSize);
}


void changeMinAndMaxWordLength()
{
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterMinLengthString);
	minLength = inputPositiveNumber();
	cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterMaxLengthString);
	maxLength = inputPositiveNumber();
}
