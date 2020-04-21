#include "Solver.h"


uint64_t Solver::openDictionary(const std::string& filename)
{
    std::basic_ifstream<char16_t> file(filename.data());
    if (file.is_open()) {
        vocabulary = new Trie();
        vocabularySize = 0;
        const uint64_t size = 256;
        char16_t* t = new char16_t[size];
        file.getline(t, size, u'\n');
        while (!file.fail()) {
            std::u16string s(t);
            if (s.size() > 0) {
                vocabulary->insert(s);
                vocabularySize++;
            }
            file.getline(t, size, u'\n');
        }
        delete [] t;
        file.close();
        return vocabularySize;
    }
    return 0;
}


void Solver::presolve(char16_t** table, uint64_t rows, uint64_t cols)
{
    matrix = table;
    numRows = rows;
    numCols = cols;

    originalBitMask = BitMask(numRows, numCols);
    for (uint64_t i = 0; i < numRows; i++) {
        for (uint64_t j = 0; j < numCols; j++) {
            char16_t t = matrix[i][j];
            bool b = t == u' ';
            originalBitMask[i][j] = b;
        }
    }
}


Solver::Solution Solver::postsolve(std::vector<StringAndBitMask>& possibleWordsAndBitmasks,
    std::vector<StringAndBitMask>& matchedWordsAndBitmasks, uint64_t calculationTime)
{
    std::vector<std::vector<uint64_t>> decodedResults = decodeResults(matchedWordsAndBitmasks);

    bitsetMasks.clear();
    results.clear();
    linearAllocator.reset();

    return {
        getUniqueWords(possibleWordsAndBitmasks),
        matchedWordsAndBitmasks,
        decodedResults,
        calculationTime,
        minLength,
        maxLength
    };
}


std::vector<std::u16string> Solver::getUniqueWords(std::vector<StringAndBitMask>& wordsAndBitmasks)
{
    std::vector<std::u16string> words(wordsAndBitmasks.size());
    for (auto & matchedWord : wordsAndBitmasks) {
        words.push_back(matchedWord.str);
    }
    sort(words.begin(), words.end(), comparator);
    words.resize(unique(words.begin(), words.end()) - words.begin());
    return words;
}


Solver::Solution Solver::solve(char16_t** table, uint64_t rows, uint64_t cols, bool needFindCombinations)
{
    if (vocabularySize == 0) {
        return {};
    }

    presolve(table, rows, cols);

    Timer timer;
    std::vector<StringAndBitMask> possibleWords = findAllPossibleWords();
    std::vector<StringAndBitMask> matchedWords;
    matchedWords.reserve(possibleWords.size());
    for (auto & possibleWord : possibleWords) {
        if (vocabulary->findWord(possibleWord.str)) {
            matchedWords.push_back(possibleWord);
        }
    }

    if (needFindCombinations) {
        ffff(matchedWords);
    }

    return postsolve(possibleWords, matchedWords, timer.elapsedNanoseconds());
}


std::vector<Solver::StringAndBitMask> Solver::findAllPossibleWords()
{
    std::vector<StringAndBitMask> a;
    a.reserve(vocabularySize);

    for (uint64_t i = 0; i < numRows; i++) {
        for (uint64_t j = 0; j < numCols; j++) {
            if (!originalBitMask[i][j]) {
                originalBitMask[i][j] = true;
                f(std::u16string(1, matrix[i][j]), i, j, originalBitMask, a);
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


void Solver::f(std::u16string&& word, uint64_t x, uint64_t y, BitMask mask, std::vector<StringAndBitMask>& possibleWords)
{
    if (word.size() >= minLength) {
        possibleWords.emplace_back(word, mask);
    }

    if (word.size() >= maxLength || !vocabulary->findSubstr(word)) {
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


void Solver::ffff(std::vector<StringAndBitMask> matchedWords)
{
    iterationSize = 2 * (sizeof(BitSet) + TYPESIZE_IN_BYTES * (numRows * numCols / TYPESIZE_IN_BITS + (numRows * numCols % TYPESIZE_IN_BITS == 0 ? 0 : 1)));

    BitSet originalMask(numRows * numCols, originalBitMask.getRawMask(), numRows, numCols);
    BitSet negOriginalMask = ~originalMask;

    matchedWordsSize = matchedWords.size();
    for (uint64_t i = 0; i < matchedWordsSize; i++) {
        bitsetMasks.push_back(
            BitSet(numRows * numCols, matchedWords[i].mask.getRawMask(), numRows, numCols) & negOriginalMask);
    }

    std::pair<uint64_t, uint64_t> zeroPoint = findMinimumMatchPoint(matchedWords);

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


std::pair<uint64_t, uint64_t> Solver::findMinimumMatchPoint(std::vector<StringAndBitMask>& matchedWords)
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
    return std::make_pair(x0, y0);
}


void Solver::f2(BitSet& mask, BitSet& indexes, uint64_t start)
{
    if (mask.isAllTrue()) {
        results.push_back(indexes);
        linearAllocator.deallocate(iterationSize);
        return;
    }
    for (uint64_t i = start; i < matchedWordsSize; i++) {
        if ((mask & bitsetMasks[i]).isAllFalse()) {
            auto* temp1 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(mask | bitsetMasks[i], linearAllocator);
            auto* temp2 = new (linearAllocator.allocate(sizeof(BitSet))) BitSet(indexes, linearAllocator);
            temp2->set(i, true);
            f2(*temp1, *temp2, i);
        }
    }
    linearAllocator.deallocate(iterationSize);
}


std::vector<std::vector<uint64_t>> Solver::decodeResults(std::vector<StringAndBitMask>& matchedWords)
{
    std::vector<std::vector<uint64_t>> decodedResults;
    for (auto & result : results) {
        std::vector<uint64_t> temp;
        for (uint64_t j = 0; j < matchedWords.size(); j++) {
            if (result[j]) {
                temp.push_back(j);
            }
        }
        decodedResults.push_back(temp);
    }
    return decodedResults;
}


bool Solver::comparator(const std::u16string& a, const std::u16string& b)
{
    if (a.size() > b.size()) {
        return true;
    } else if (a.size() < b.size()) {
        return false;
    } else {
        return a > b;
    }
}


bool Solver::setMinAndMaxWordLength(uint64_t min, uint64_t max)
{
    if (min <= max && min > 0) {
        minLength = min;
        maxLength = max;
        return true;
    }
    return false;
}


std::pair<uint64_t, uint64_t> Solver::getMinAndMaxWordLength()
{
    return std::pair<uint64_t, uint64_t>(minLength, maxLength);
}
