#ifndef BASEMENUITEM_H
#define BASEMENUITEM_H


#include <string>
#include <cassert>

using namespace std;



class BaseMenuItem
{
public:
	BaseMenuItem() : item(nullptr), languagesNumber(0)
	{}
	

	BaseMenuItem(string* item, unsigned int languagesNumber) 
		: languagesNumber(languagesNumber)
	{
		this->item = new string[languagesNumber];
		for (unsigned int i = 0; i < languagesNumber; i++) {
			this->item[i] = item[i];
		}
	}
	

	BaseMenuItem(const BaseMenuItem& b) 
		: BaseMenuItem(b.item, b.languagesNumber)
	{
	}


	BaseMenuItem& operator=(const BaseMenuItem& b)
	{
		if (this != &b) {
			this->~BaseMenuItem();
			new (this) BaseMenuItem(b);
		}
		return *this;
	}
	

	string operator[] (const unsigned int languageIndex)
	{
		assert(languageIndex >= 0 && languageIndex < languagesNumber);
		return item[languageIndex];
	}
	

	string* item;
	unsigned int languagesNumber;
};

#endif
