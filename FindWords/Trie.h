#ifndef TRIE_H
#define TRIE_H


#include <string>
//#include <map>
#include <unordered_map>

using namespace std;



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
	
	
	~Trie()
	{
		delete root;
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
		
		
		bool findSubstr(string &substr, unsigned int size, unsigned int index)
		{
			if (index == size) {
				return true;
			}
			if (this->letters.count(substr[index])) {
				return this->letters.at(substr[index])->findSubstr(substr, size, index + 1);
			}
			return false;
		}
		
		
		bool findWord(string &word, unsigned int size, unsigned int index)
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
		unordered_map<char, Node*> letters;
		bool wholeWord;
	};
	
	Node* root;
};

#endif
