#ifndef FINDWORDS_SOLVER_H
#define FINDWORDS_SOLVER_H


#include "BitMask.h"
#include "BitSet.h"
#include "LinearAllocator.h"
#include "Trie.h"
#include "Timer.h"

#include <vector>
#include <fstream>
#include <algorithm>


class Solver
{
public:

    struct StringAndBitMask
    {
        std::u16string str;
        BitMask mask;

        StringAndBitMask(std::u16string str, const BitMask& mask) : str(std::move(str)), mask(mask) {}
    };

    struct Solution
    {
        std::vector<std::u16string> possibleWords;
        std::vector<StringAndBitMask> matchedWordsWithMasks;
        std::vector<std::vector<uint64_t>> combinedWordsIndexes;
        uint64_t calculationTimeInNanoseconds;
        uint64_t minWordLength;
        uint64_t maxWordLength;
    };


    uint64_t openDictionary(const std::string& filename);
    Solution solve(char16_t** table, uint64_t rows, uint64_t cols, bool needFindCombinations = false);
    bool setMinAndMaxWordLength(uint64_t min, uint64_t max);
    std::pair<uint64_t, uint64_t> getMinAndMaxWordLength();

private:

    static const uint64_t NUMBER_OF_DIRECTIONS = 4;
    constexpr static const int64_t DX[NUMBER_OF_DIRECTIONS] = {1, -1, 0, 0};
    constexpr static const int64_t DY[NUMBER_OF_DIRECTIONS] = {0, 0, 1, -1};

    static const uint64_t ALLOCATOR_BUFFER_SIZE = 25 * 1024 * 1024;

    uint64_t numRows;
    uint64_t numCols;
    char16_t** matrix;
    BitMask originalBitMask;

    uint64_t minLength = 4;
    uint64_t maxLength = 10;

    Trie* vocabulary;
    uint64_t vocabularySize;

    std::vector<BitSet> bitsetMasks;
    std::vector<BitSet> results;
    uint64_t matchedWordsSize = 0;

    LinearAllocator linearAllocator = LinearAllocator(ALLOCATOR_BUFFER_SIZE);
    uint64_t iterationSize;


    void presolve(char16_t** table, uint64_t rows, uint64_t cols);
    Solution postsolve(std::vector<StringAndBitMask>& possibleWords, std::vector<StringAndBitMask>& matchedWords, uint64_t calculationTimeInNanoseconds);
    static std::vector<std::u16string> getUniqueWords(std::vector<StringAndBitMask>& wordsAndBitmasks);

    std::vector<StringAndBitMask> findAllPossibleWords();
    void f(std::u16string&& word, uint64_t x, uint64_t y, BitMask mask, std::vector<StringAndBitMask>& possibleWords);
    void ffff(std::vector<StringAndBitMask> matchedWords);
    std::pair<uint64_t, uint64_t> findMinimumMatchPoint(std::vector<StringAndBitMask>& matchedWords);
    void f2(BitSet& mask, BitSet& indexes, uint64_t start);

    std::vector<std::vector<uint64_t>> decodeResults(std::vector<StringAndBitMask>& matchedWords);
    static bool comparator(const std::u16string& a, const std::u16string& b);
};


#endif   // FINDWORDS_SOLVER_H
