#ifndef TRIE_H
#define TRIE_H


#include <string>
#include <unordered_map>


class Trie
{
public:
    Trie();

    ~Trie();

    void insert(std::string& word);
    bool findSubstr(std::string& substr);
    bool findWord(std::string& word);


private:
    class Node
    {
    public:
        Node(bool wholeWord = false);

        void insert(std::string& str, uint64_t size, uint64_t index);
        bool findSubstr(std::string& substr, uint64_t size, uint64_t index);
        bool findWord(std::string& word, uint64_t size, uint64_t index);

    private:
        std::unordered_map<char, Node*> letters;
        bool wholeWord;
    };

    Node* root;
};

#endif
