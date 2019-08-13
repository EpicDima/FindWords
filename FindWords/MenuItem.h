#ifndef MENUITEM_H
#define MENUITEM_H

#include "BaseMenuItem.h"

#include <functional>


class MenuItem : public BaseMenuItem
{
public:
	MenuItem() {}
	
	MenuItem(string* item, unsigned int languagesNumber, function<void()> func) 
		: BaseMenuItem(item, languagesNumber)
	{
		this->func = func;
	}
	
	MenuItem(string* item, unsigned int languagesNumber, function<void()> func, string& value) 
		: MenuItem(item, languagesNumber, func)
	{
		this->value = &value;
		this->realTimeValue = true;
	}
	
	MenuItem(const MenuItem& b) 
		: MenuItem(b.item, b.languagesNumber, b.func)
	{
		if (b.realTimeValue) {
			this->value = b.value;
			this->realTimeValue = true;
		}
	}
	
	string getString(unsigned int languageIndex, unsigned int maxLength)
	{
		assert(languageIndex >= 0 && languageIndex < languagesNumber);
		string leftPart = item[languageIndex];
		if (realTimeValue) {
			string middlePart = string(maxLength + 5 - leftPart.size(), ' ');
			return leftPart + middlePart + '(' + *value + ')';
		} else {
			return leftPart;
		}
	}
	
	function<void()> func;
	bool realTimeValue = false;
	string* value;
};

#endif
