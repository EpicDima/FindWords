#ifndef TRIE_H
#define TRIE_H


#include <string>
#include <unordered_map>


class Trie
{
public:
    Trie();

    ~Trie();

    void insert(std::u16string& word);
    bool findSubstr(std::u16string& substr);
    bool findWord(std::u16string& word);


private:
    class Node
    {
    public:
        explicit Node(bool wholeWord = false);

        void insert(std::u16string& str, uint64_t size, uint64_t index);
        bool findSubstr(std::u16string& substr, uint64_t size, uint64_t index);
        bool findWord(std::u16string& word, uint64_t size, uint64_t index);

    private:
        std::unordered_map<char16_t, Node*> letters;
        bool wholeWord;
    };

    Node* root;
};

#endif
