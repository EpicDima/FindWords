#include "service/LinearAllocator.h"
#include "service/Timer.h"

#include "bit_classes/BitMask.h"
#include "bit_classes/BitSet.h"

#include "menu/Menu.h"

#include "FindWordsConstants.h"
#include "Trie.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>


struct StringAndBitMask
{
    string str;
    BitMask mask;

    StringAndBitMask(string str, BitMask mask) : str(str), mask(mask) {}
};


void createMenu();
vector<StringAndBitMask> findAllPossibleWords();
void f(string&& word, unsigned int x, unsigned int y, BitMask mask, vector<StringAndBitMask>& possibleWords);
void openDictionary(string filename = "dictionary.txt");
void changeDictionary();
void outputResult(vector<StringAndBitMask>& matchedWords, vector<StringAndBitMask>& possibleWords);
void printColorChar(char ch, WORD attribute);
pair<pair<vector<string>, vector<vector<unsigned int>>>, unsigned int> decodeResults(
    vector<StringAndBitMask>& matchedWords);
void outputMatchedWords(vector<StringAndBitMask>& matchedWords);
WORD* createAttributesArray(unsigned int max);
void outputTables(vector<StringAndBitMask>& matchedWords, vector<vector<unsigned int>>& decodedResults, WORD* attrs);
void outputCombinationResults(vector<StringAndBitMask>& matchedWords);
bool comparator(const string& a, const string& b);
void f2(BitSet& mask, BitSet& indexes, unsigned int start);
pair<unsigned int, unsigned int> findMinimumMatchPoint(vector<StringAndBitMask>& matchedWords);
void ffff(vector<StringAndBitMask> matchedWords);
void calculate();
unsigned int inputPositiveNumber();
void inputTable();
string getStateOfNeedFindCombination();
string getMinMaxLengthsStr();
string getVocabularySizeStr();
void changeMinAndMaxWordLength();


const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};

unsigned int numRows;
unsigned int numCols;
char** matrix;
BitMask originalBitMask;

unsigned int minLength = 4;
unsigned int maxLength = 10;

Trie* vocabularyWords;
unsigned int vocabularyWordsSize;

unsigned long long calculationTimeInNanoseconds;

bool needFindCombination = false;

vector<BitSet> bitsetMasks;
vector<BitSet> results;
unsigned int matchedWordsSize = 0;

Menu* mainmenu;

LinearAllocator linearAllocator(25 * 1024 * 1024);   // 25Mb just in case
unsigned int iterationSize;


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
    MenuItem* items = new MenuItem[fw::MenuItemsNumber];
    items[0] = MenuItem(fw::MenuConstants::MenuItemsStrings[0], fw::LanguagesNumber, []() {
        inputTable();
        calculate();
    });
    items[1] = MenuItem(
        fw::MenuConstants::MenuItemsStrings[1], fw::LanguagesNumber,
        []() { needFindCombination = !needFindCombination; }, getStateOfNeedFindCombination);
    items[2] = MenuItem(
        fw::MenuConstants::MenuItemsStrings[2], fw::LanguagesNumber, changeMinAndMaxWordLength, getMinMaxLengthsStr);
    items[3] =
        MenuItem(fw::MenuConstants::MenuItemsStrings[3], fw::LanguagesNumber, changeDictionary, getVocabularySizeStr);
    items[4] = MenuItem(fw::MenuConstants::MenuItemsStrings[4], fw::LanguagesNumber, []() { exit(0); });

    Menu::AnotherMenuItems auxItems = {
        BaseMenuItem(fw::MenuConstants::AuxiliaryMenuItemsStrings[0], fw::LanguagesNumber),
        BaseMenuItem(fw::MenuConstants::AuxiliaryMenuItemsStrings[1], fw::LanguagesNumber),
        BaseMenuItem(fw::MenuConstants::AuxiliaryMenuItemsStrings[2], fw::LanguagesNumber)};

    mainmenu = new Menu(items, auxItems, fw::MenuItemsNumber, fw::LanguagesNumber);
    mainmenu->draw();
}


vector<StringAndBitMask> findAllPossibleWords()
{
    vector<StringAndBitMask> a;
    a.reserve(vocabularyWordsSize);

    for (unsigned int i = 0; i < numRows; i++) {
        for (unsigned int j = 0; j < numCols; j++) {
            if (!originalBitMask[i][j]) {
                originalBitMask[i][j] = true;
                f(string(1, matrix[i][j]), i, j, originalBitMask, a);
                originalBitMask[i][j] = false;
            }
        }
    }

    for (unsigned int k = 0; k < a.size(); k++) {
        for (unsigned int i = 0; i < numRows; i++) {
            for (unsigned int j = 0; j < numCols; j++) {
                a[k].mask[i][j] = static_cast<bool>(a[k].mask[i][j] - originalBitMask[i][j]);
            }
        }
    }

    return a;
}


void f(string&& word, unsigned int x, unsigned int y, BitMask mask, vector<StringAndBitMask>& possibleWords)
{
    if (word.size() >= minLength) {
        possibleWords.push_back(StringAndBitMask(word, mask));
    }

    if (word.size() >= maxLength || !vocabularyWords->findSubstr(word)) {
        return;
    }

    for (unsigned int i = 0; i < 4; i++) {
        unsigned int x2 = x + dx[i];
        unsigned int y2 = y + dy[i];
        if (x2 < numRows && y2 < numCols) {
            if (!mask[x2][y2]) {
                mask[x2][y2] = true;
                f(word + matrix[x2][y2], x2, y2, mask, possibleWords);
                mask[x2][y2] = false;
            }
        }
    }
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


void outputResult(vector<StringAndBitMask>& matchedWords, vector<StringAndBitMask>& possibleWords)
{
    pair<unsigned long long, string> calculationTimePair = getCalculationTime();
    cout << endl << calculationTimePair.second << setw(14) << calculationTimePair.first << endl << endl;
    cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::VocabularyWordsSizeString) << setw(14)
         << vocabularyWordsSize << endl;
    cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::PossibleWordsSizeString) << setw(14)
         << possibleWords.size() << endl;
    cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::MatchedWordsSizeString) << setw(14)
         << matchedWords.size() << endl;

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


void outputMatchedWords(vector<StringAndBitMask>& matchedWords)
{
    vector<string> tempwords;
    for (unsigned int i = 0; i < matchedWords.size(); i++) {
        tempwords.push_back(matchedWords[i].str);
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


pair<pair<vector<string>, vector<vector<unsigned int>>>, unsigned int> decodeResults(
    vector<StringAndBitMask>& matchedWords)
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
                words.push_back(matchedWords[j].str);
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
    unsigned int biggerThan = max / (fw::attributesLength * fw::attributesLength - fw::attributesLength) + 1;
    unsigned int counter = 0;
    for (unsigned int k = 0; k < biggerThan; k++) {
        for (unsigned int i = 0; i < fw::attributesLength + 1; i++) {
            for (unsigned int j = 0; j < fw::attributesLength; j++) {
                if ((i - 1) != j) {
                    if (i == 0) {
                        attrs[counter] = fw::attributesForeground[j];
                    } else {
                        attrs[counter] = fw::attributesBackground[i - 1] | fw::attributesForeground[j];
                    }

                    if (++counter >= max) {
                        return attrs;
                    }
                }
            }
        }
    }
    return attrs;
}


void outputTables(vector<StringAndBitMask>& matchedWords, vector<vector<unsigned int>>& decodedResults, WORD* attrs)
{
    cout << "\n\n\n" << mainmenu->chooseElementFromArrayByActiveLanguage(fw::ColoredTablesString) << "\n";
    for (unsigned int h = 0; h < decodedResults.size(); h++) {
        cout << "\n" << h + 1 << ")\n";
        for (unsigned int i = 0; i < numRows; i++) {
            for (unsigned int j = 0; j < numCols; j++) {
                if (matrix[i][j] != ' ') {
                    for (unsigned int k = 0; k < decodedResults[h].size(); k++) {
                        if (matchedWords[decodedResults[h][k]].mask[i][j]) {
                            printColorChar(matrix[i][j], attrs[k]);
                            break;
                        }
                    }
                } else {
                    printColorChar(' ', 0);
                }
            }
            cout << endl;
        }
        printColorChar('\n', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}


void outputCombinationResults(vector<StringAndBitMask>& matchedWords)
{
    pair<pair<vector<string>, vector<vector<unsigned int>>>, unsigned int> p = decodeResults(matchedWords);
    vector<string> words = p.first.first;
    unsigned int max = p.second;

    cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::CombinedWordsSizeString) << setw(14) << words.size();

    if (words.size() > 0) {
        cout << "\n\n\n" << mainmenu->chooseElementFromArrayByActiveLanguage(fw::CombinedWordsString) << endl;
        for (unsigned int i = 0; i < words.size(); i++) {
            cout << words[i] << endl;
        }

        WORD* attrs = createAttributesArray(max);
        outputTables(matchedWords, p.first.second, attrs);
    } else {
        outputMatchedWords(matchedWords);
    }
}


bool comparator(const string& a, const string& b)
{
    if (a.size() > b.size()) {
        return true;
    } else if (a.size() < b.size()) {
        return false;
    } else {
        return a > b;
    }
}


//  8x8 3-10 109: 312728077 iters    484.00 sec     109
//  8x8 4-10  57:    279677 iters      0.27 sec     57
// 11x8 5-10  63:   1547300 iters      1.64 sec     63
void f2(BitSet& mask, BitSet& indexes, unsigned int start)
{
    if (mask.isAllTrue()) {
        results.push_back(indexes);
        linearAllocator.deallocate(iterationSize);
        return;
    }
    for (unsigned int i = start; i < matchedWordsSize; i++) {
        if ((mask & bitsetMasks[i]).isAllFalse()) {
            BitSet* temp1 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(mask | bitsetMasks[i], linearAllocator);
            BitSet* temp2 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(indexes, linearAllocator);
            temp2->set(i, true);
            f2(*temp1, *temp2, i);
        }
    }
    linearAllocator.deallocate(iterationSize);
}


pair<unsigned int, unsigned int> findMinimumMatchPoint(vector<StringAndBitMask>& matchedWords)
{
    unsigned int min = ~0, x0 = 0, y0 = 0;
    for (unsigned int i = 0; i < numRows; i++) {
        for (unsigned int j = 0; j < numCols; j++) {
            unsigned int t = 0;
            for (unsigned int k = 0; k < matchedWords.size(); k++) {
                t += matchedWords[k].mask[i][j];
            }
            if (t < min && t != 0) {
                min = t;
                x0 = i;
                y0 = j;
            }
        }
    }
    return make_pair(x0, y0);
}


void ffff(vector<StringAndBitMask> matchedWords)
{
    iterationSize = 2 * (sizeof(BitSet) + numRows * numCols / TYPESIZE_IN_BYTES + (numRows * numCols % TYPESIZE_IN_BITS == 0 ? 0 : 1));
    
    BitSet originalMask(numRows * numCols, originalBitMask.getRawMask(), numRows, numCols);
    BitSet negOriginalMask = ~originalMask;

    matchedWordsSize = matchedWords.size();
    for (unsigned int i = 0; i < matchedWordsSize; i++) {
        bitsetMasks.push_back(
            BitSet(numRows * numCols, matchedWords[i].mask.getRawMask(), numRows, numCols) & negOriginalMask);
    }

    pair<unsigned int, unsigned int> zeroPoint = findMinimumMatchPoint(matchedWords);

    BitSet indexes(matchedWordsSize);
    for (unsigned int i = 0; i < matchedWordsSize; i++) {
        if (matchedWords[i].mask[zeroPoint.first][zeroPoint.second]) {
            BitSet* temp1 =
                new (linearAllocator.allocate(sizeof(BitSet))) BitSet(bitsetMasks[i] | originalMask, linearAllocator);
            BitSet* temp2 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(indexes, linearAllocator);
            temp2->set(i, true);
            f2(*temp1, *temp2, 0);
        }
    }
    matchedWordsSize = 0;
}


void calculate()
{
    Timer timer;
    vector<StringAndBitMask> possibleWords = findAllPossibleWords();
    vector<StringAndBitMask> matchedWords;
    matchedWords.reserve(possibleWords.size());
    for (unsigned int i = 0; i < possibleWords.size(); i++) {
        if (vocabularyWords->findWord(possibleWords[i].str)) {
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
    linearAllocator.reset();
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
    return k;
}


void inputTable()
{
    cout << endl << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterLinesString);
    numRows = inputPositiveNumber();
    cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterColumnsString);
    numCols = inputPositiveNumber();

    matrix = new char*[numRows];
    for (unsigned int i = 0; i < numRows; i++) {
        matrix[i] = new char[numCols];
        for (unsigned int j = 0; j < numCols; j++) {
            matrix[i][j] = ' ';
        }
    }

    cout << mainmenu->chooseElementFromArrayByActiveLanguage(fw::EnterLettersString) << endl << endl;

    cin.ignore();
    for (unsigned int i = 0; i < numRows; i++) {
        cin.getline(matrix[i], numCols + 1, '\n');
    }
    cout << endl;

    originalBitMask = BitMask(numRows, numCols);
    for (unsigned int i = 0; i < numRows; i++) {
        for (unsigned int j = 0; j < numCols; j++) {
            originalBitMask[i][j] = matrix[i][j] == ' ';
        }
    }
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
    return mainmenu->chooseElementFromArrayByActiveLanguage(fw::MinimumString) + to_string(minLength) +
        mainmenu->chooseElementFromArrayByActiveLanguage(fw::MaximumString) + to_string(maxLength);
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
    while (maxLength < minLength) {
        maxLength = inputPositiveNumber();
    }
}
