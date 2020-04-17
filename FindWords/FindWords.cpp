#include "service/LinearAllocator.h"
#include "service/Timer.h"

#include "bit_classes/BitMask.h"
#include "bit_classes/BitSet.h"

#include "menu/Menu.h"

#include "FindWordsConstants.h"
#include "Solver.h"
#include "Trie.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <utility>
#include <vector>

using namespace std;


struct StringAndBitMask
{
    string str;
    BitMask mask;

    StringAndBitMask(string str, const BitMask& mask) : str(std::move(str)), mask(mask) {}
};


void createMenu();
vector<StringAndBitMask> findAllPossibleWords();
void f(string&& word, uint64_t x, uint64_t y, BitMask mask, vector<StringAndBitMask>& possibleWords);
void openDictionary(const string& filename = "dictionary.txt");
void changeDictionary();
void outputResult(vector<StringAndBitMask>& matchedWords, vector<StringAndBitMask>& possibleWords);
void printColorChar(char ch, WORD attribute);
pair<pair<vector<string>, vector<vector<uint64_t>>>, uint64_t> decodeResults(vector<StringAndBitMask>& matchedWords);
void outputMatchedWords(vector<StringAndBitMask>& matchedWords);
WORD* createAttributesArray(uint64_t max);
void outputTables(vector<StringAndBitMask>& matchedWords, vector<vector<uint64_t>>& decodedResults, WORD* attrs);
void outputCombinationResults(vector<StringAndBitMask>& matchedWords);
bool comparator(const string& a, const string& b);
void f2(BitSet& mask, BitSet& indexes, uint64_t start);
pair<uint64_t, uint64_t> findMinimumMatchPoint(vector<StringAndBitMask>& matchedWords);
void ffff(vector<StringAndBitMask> matchedWords);
void calculate();
uint64_t inputPositiveNumber();
void inputTable();
string getStateOfNeedFindCombination();
string getMinMaxLengthsStr();
string getVocabularySizeStr();
void changeMinAndMaxWordLength();


const uint64_t NUMBER_OF_DIRECTIONS = 4;
const int64_t DX[NUMBER_OF_DIRECTIONS] = {1, -1, 0, 0};
const int64_t DY[NUMBER_OF_DIRECTIONS] = {0, 0, 1, -1};

uint64_t numRows;
uint64_t numCols;
char** matrix;
BitMask originalBitMask;

uint64_t minLength = 4;
uint64_t maxLength = 10;

Trie* vocabularyWords;
uint64_t vocabularyWordsSize;

uint64_t calculationTimeInNanoseconds;

bool needFindCombination = false;

vector<BitSet> bitsetMasks;
vector<BitSet> results;
uint64_t matchedWordsSize = 0;

Menu* mainmenu;

LinearAllocator linearAllocator(25 * 1024 * 1024);   // 25Mb just in case
uint64_t iterationSize;


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Solver solver;
    solver.openDictionary("dictionary.txt");

    uint64_t rows = 3, cols = 7;

    char** table = new char*[rows];
    for (uint64_t i = 0; i < rows; i++) {
        table[i] = new char[cols];
        for (uint64_t j = 0; j < cols; j++) {
            table[i][j] = ' ';
        }
    }

    cin.ignore();
    for (uint64_t i = 0; i < rows; i++) {
        cin.getline(table[i], cols + 1, '\n');
    }
    cout << endl;

    solver.setMinAndMaxWordLength(rows, cols);
    Solver::Solution solution = solver.solve(table, rows, cols, true);

    cout << solution.possibleWords.size() << endl;



    return 0;

//    openDictionary();
//    createMenu();
//
//    while (true) {
//        mainmenu->click();
//    }
}


void createMenu()
{
    auto* localizer = new Localizer();

    vector<MenuItem> items;
    items.emplace_back("menu_item_1", []() {inputTable(); calculate();});
    items.emplace_back("menu_item_2", []() { needFindCombination = !needFindCombination; },
            getStateOfNeedFindCombination);
    items.emplace_back("menu_item_3", changeMinAndMaxWordLength, getMinMaxLengthsStr);
    items.emplace_back("menu_item_4", changeDictionary, getVocabularySizeStr);
    items.emplace_back("menu_item_5", []() { exit(0); });

    Menu::AnotherMenuItems auxItems = {
        BaseMenuItem("menu", localizer),
        BaseMenuItem("menu_choose_item", localizer),
        BaseMenuItem("menu_press_to_continue", localizer)};

    mainmenu = new Menu(items.data(), auxItems, items.size(), localizer);
    mainmenu->draw();
}


vector<StringAndBitMask> findAllPossibleWords()
{
    vector<StringAndBitMask> a;
    a.reserve(vocabularyWordsSize);

    for (uint64_t i = 0; i < numRows; i++) {
        for (uint64_t j = 0; j < numCols; j++) {
            if (!originalBitMask[i][j]) {
                originalBitMask[i][j] = true;
                f(string(1, matrix[i][j]), i, j, originalBitMask, a);
                originalBitMask[i][j] = false;
            }
        }
    }

    for (auto & k : a) {
        for (uint64_t i = 0; i < numRows; i++) {
            for (uint64_t j = 0; j < numCols; j++) {
                k.mask[i][j] = static_cast<bool>(k.mask[i][j] - originalBitMask[i][j]);
            }
        }
    }

    return a;
}


void f(string&& word, uint64_t x, uint64_t y, BitMask mask, vector<StringAndBitMask>& possibleWords)
{
    if (word.size() >= minLength) {
        possibleWords.emplace_back(word, mask);
    }

    if (word.size() >= maxLength || !vocabularyWords->findSubstr(word)) {
        return;
    }

    for (uint64_t i = 0; i < NUMBER_OF_DIRECTIONS; i++) {
        uint64_t x2 = x + DX[i];
        uint64_t y2 = y + DY[i];
        if (x2 < numRows && y2 < numCols) {
            if (!mask[x2][y2]) {
                mask[x2][y2] = true;
                f(word + matrix[x2][y2], x2, y2, mask, possibleWords);
                mask[x2][y2] = false;
            }
        }
    }
}


void openDictionary(const string& filename)
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
    cout << endl << mainmenu->getString("enter_path_to_dictionary");
    cin >> filename;
    openDictionary(filename);
}


pair<uint64_t, string> getCalculationTime()
{
    uint64_t calculationTime;
    string strCalculationTime;
    if (calculationTimeInNanoseconds > 1e11) {
        calculationTime = calculationTimeInNanoseconds / 1e9;
        strCalculationTime = mainmenu->getString("calculation_time_s");
    } else if (calculationTimeInNanoseconds > 1e8) {
        calculationTime = calculationTimeInNanoseconds / 1e6;
        strCalculationTime = mainmenu->getString("calculation_time_ms");
    } else {
        calculationTime = calculationTimeInNanoseconds / 1e3;
        strCalculationTime = mainmenu->getString("calculation_time_us");
    }
    return make_pair(calculationTime, strCalculationTime);
}


void outputResult(vector<StringAndBitMask>& matchedWords, vector<StringAndBitMask>& possibleWords)
{
    pair<uint64_t, string> calculationTimePair = getCalculationTime();
    cout << endl << calculationTimePair.second << setw(14) << calculationTimePair.first << endl << endl;
    cout << mainmenu->getString("vocabulary_words_count") << setw(14)
         << vocabularyWordsSize << endl;
    cout << mainmenu->getString("possible_words_count") << setw(14)
         << possibleWords.size() << endl;
    cout << mainmenu->getString("matched_words_count") << setw(14)
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
    for (auto & matchedWord : matchedWords) {
        tempwords.push_back(matchedWord.str);
    }
    sort(tempwords.begin(), tempwords.end(), comparator);
    tempwords.resize(unique(tempwords.begin(), tempwords.end()) - tempwords.begin());

    if (!tempwords.empty()) {
        cout << "\n\n\n" << mainmenu->getString("matched_words_list") << endl;
        for (const auto & tempword : tempwords) {
            cout << tempword << endl;
        }
    }
}


pair<pair<vector<string>, vector<vector<uint64_t>>>, uint64_t> decodeResults(vector<StringAndBitMask>& matchedWords)
{
    vector<string> words;
    vector<vector<uint64_t>> decodedResults;
    uint64_t max = 0;
    for (auto & result : results) {
        uint64_t k = 0;
        vector<uint64_t> temp;
        for (uint64_t j = 0; j < matchedWords.size(); j++) {
            if (result[j]) {
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


WORD* createAttributesArray(uint64_t max)
{
    WORD* attrs = new WORD[max];
    uint64_t biggerThan = max / (attributesLength * attributesLength - attributesLength) + 1;
    uint64_t counter = 0;
    for (uint64_t k = 0; k < biggerThan; k++) {
        for (uint64_t i = 0; i < attributesLength + 1; i++) {
            for (uint64_t j = 0; j < attributesLength; j++) {
                if ((i - 1) != j) {
                    if (i == 0) {
                        attrs[counter] = attributesForeground[j];
                    } else {
                        attrs[counter] = attributesBackground[i - 1] | attributesForeground[j];
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


void outputTables(vector<StringAndBitMask>& matchedWords, vector<vector<uint64_t>>& decodedResults, WORD* attrs)
{
    cout << "\n\n\n" << mainmenu->getString("colored_tables") << "\n";
    for (uint64_t h = 0; h < decodedResults.size(); h++) {
        cout << "\n" << h + 1 << ")\n";
        for (uint64_t i = 0; i < numRows; i++) {
            for (uint64_t j = 0; j < numCols; j++) {
                if (matrix[i][j] != ' ') {
                    for (uint64_t k = 0; k < decodedResults[h].size(); k++) {
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
    pair<pair<vector<string>, vector<vector<uint64_t>>>, uint64_t> p = decodeResults(matchedWords);
    vector<string> words = p.first.first;
    uint64_t max = p.second;

    cout << mainmenu->getString("combined_words_count") << setw(14) << words.size();

    if (!words.empty()) {
        cout << "\n\n\n" << mainmenu->getString("combined_words_list") << endl;
        for (const auto & word : words) {
            cout << word << endl;
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


void f2(BitSet& mask, BitSet& indexes, uint64_t start)
{
    if (mask.isAllTrue()) {
        results.push_back(indexes);
        linearAllocator.deallocate(iterationSize);
        return;
    }
    for (uint64_t i = start; i < matchedWordsSize; i++) {
        if ((mask & bitsetMasks[i]).isAllFalse()) {
            auto* temp1 =
                new (linearAllocator.allocate(sizeof(BitSet))) BitSet(mask | bitsetMasks[i], linearAllocator);
            auto* temp2 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(indexes, linearAllocator);
            temp2->set(i, true);
            f2(*temp1, *temp2, i);
        }
    }
    linearAllocator.deallocate(iterationSize);
}


pair<uint64_t, uint64_t> findMinimumMatchPoint(vector<StringAndBitMask>& matchedWords)
{
    uint64_t min = ~0ULL, x0 = 0, y0 = 0;
    for (uint64_t i = 0; i < numRows; i++) {
        for (uint64_t j = 0; j < numCols; j++) {
            uint64_t t = 0;
            for (auto & matchedWord : matchedWords) {
                t += matchedWord.mask[i][j];
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
    iterationSize = 2 *
        (sizeof(BitSet) + TYPESIZE_IN_BYTES * (numRows * numCols / TYPESIZE_IN_BITS + (numRows * numCols % TYPESIZE_IN_BITS == 0 ? 0 : 1)));

    BitSet originalMask(numRows * numCols, originalBitMask.getRawMask(), numRows, numCols);
    BitSet negOriginalMask = ~originalMask;

    matchedWordsSize = matchedWords.size();
    for (uint64_t i = 0; i < matchedWordsSize; i++) {
        bitsetMasks.push_back(
            BitSet(numRows * numCols, matchedWords[i].mask.getRawMask(), numRows, numCols) & negOriginalMask);
    }

    pair<uint64_t, uint64_t> zeroPoint = findMinimumMatchPoint(matchedWords);

    BitSet indexes(matchedWordsSize);
    for (uint64_t i = 0; i < matchedWordsSize; i++) {
        if (matchedWords[i].mask[zeroPoint.first][zeroPoint.second]) {
            auto* temp1 =
                new (linearAllocator.allocate(sizeof(BitSet))) BitSet(bitsetMasks[i] | originalMask, linearAllocator);
            auto* temp2 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(indexes, linearAllocator);
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
    for (auto & possibleWord : possibleWords) {
        if (vocabularyWords->findWord(possibleWord.str)) {
            matchedWords.push_back(possibleWord);
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


uint64_t inputPositiveNumber()
{
    int64_t k = -1;
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
    cout << endl << mainmenu->getString("enter_rows_count");
    numRows = inputPositiveNumber();
    cout << mainmenu->getString("enter_columns_count");
    numCols = inputPositiveNumber();

    matrix = new char*[numRows];
    for (uint64_t i = 0; i < numRows; i++) {
        matrix[i] = new char[numCols];
        for (uint64_t j = 0; j < numCols; j++) {
            matrix[i][j] = ' ';
        }
    }

    cout << mainmenu->getString("enter_letters") << endl << endl;

    cin.ignore();
    for (uint64_t i = 0; i < numRows; i++) {
        cin.getline(matrix[i], numCols + 1, '\n');
    }
    cout << endl;

    originalBitMask = BitMask(numRows, numCols);
    for (uint64_t i = 0; i < numRows; i++) {
        for (uint64_t j = 0; j < numCols; j++) {
            originalBitMask[i][j] = matrix[i][j] == ' ';
        }
    }
}


string getStateOfNeedFindCombination()
{
    if (needFindCombination) {
        return mainmenu->getString("enabled");
    } else {
        return mainmenu->getString("disabled");
    }
}


string getMinMaxLengthsStr()
{
    return mainmenu->getString("minimum") + to_string(minLength) +
        mainmenu->getString("maximum") + to_string(maxLength);
}


string getVocabularySizeStr()
{
    return mainmenu->getString("vocabulary_size") + to_string(vocabularyWordsSize);
}


void changeMinAndMaxWordLength()
{
    cout << mainmenu->getString("enter_min_length");
    minLength = inputPositiveNumber();
    cout << mainmenu->getString("enter_max_length");
    maxLength = inputPositiveNumber();
    while (maxLength < minLength) {
        maxLength = inputPositiveNumber();
    }
}
