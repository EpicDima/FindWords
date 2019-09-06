#include "Trie.h"


Trie::Trie()
{
    root = new Node();
}


Trie::~Trie()
{
    delete root;
}


void Trie::insert(std::string& word)
{
    root->insert(word, word.size(), 0);
}


bool Trie::findSubstr(std::string& substr)
{
    return root->findSubstr(substr, substr.size(), 0);
}


bool Trie::findWord(std::string& word)
{
    return root->findWord(word, word.size(), 0);
}


Trie::Node::Node(bool wholeWord)
{
    this->wholeWord = wholeWord;
}


void Trie::Node::insert(std::string& str, uint64_t size, uint64_t index)
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
            this->letters.insert(std::pair<char, Node*>(str[index], node));
            node->insert(str, size, index + 1);
        } else {
            this->letters.insert(std::pair<char, Node*>(str[index], new Node(true)));
        }
    }
}


bool Trie::Node::findSubstr(std::string& substr, uint64_t size, uint64_t index)
{
    if (index == size) {
        return true;
    }
    if (this->letters.count(substr[index])) {
        return this->letters.at(substr[index])->findSubstr(substr, size, index + 1);
    }
    return false;
}


bool Trie::Node::findWord(std::string& word, uint64_t size, uint64_t index)
{
    if (index == size && this->wholeWord) {
        return true;
    }
    if (this->letters.count(word[index])) {
        return this->letters.at(word[index])->findWord(word, size, index + 1);
    }
    return false;
}