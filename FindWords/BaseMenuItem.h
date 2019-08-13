#ifndef BASEMENUITEM_H
#define BASEMENUITEM_H


#include <string>
#include <cassert>

using namespace std;



class BaseMenuItem
{
public:
	BaseMenuItem() {}
	
	BaseMenuItem(string* item, unsigned int languagesNumber)
	{
		this->languagesNumber = languagesNumber;
		this->item = new string[languagesNumber];
		for (unsigned int i = 0; i < languagesNumber; i++) {
			this->item[i] = item[i];
		}
	}
	
	BaseMenuItem(const BaseMenuItem& b) 
		: BaseMenuItem(b.item, b.languagesNumber)
	{
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
