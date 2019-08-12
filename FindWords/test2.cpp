#include <iostream>
#include <bitset>
#include <vector>

using namespace std;


template <typename T>
class Bitset
{
public:
	Bitset(unsigned int nBits)
	{
		this->nBits = nBits; 
		unsigned int typesize = sizeof(T) * 8;
		count = static_cast<unsigned int>(nBits / typesize);
		lastmask = 0;
		unsigned int difference = nBits % typesize;
		if (difference != 0) {
			count++;
			lastmask = ~((~lastmask) >> (typesize - difference));
		}
		data = new T[count];
		for (unsigned int i = 0; i < count; i++) {
			data[i] = 0;
		}
	}
	
	Bitset(unsigned int nBits, bool** a, unsigned int n, unsigned int m) : Bitset(nBits)
	{
		unsigned int typesize = sizeof(T) * 8;
		T mask = 1;
		unsigned int k = 0;
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
				unsigned int t = i * n + j;
				if (t % typesize == 0 && t != 0) {
					k++;
					if (k == count) {
						throw logic_error("Не хватает памяти для такого числа элементов");
					}
					mask = 1;
				}
				if (a[i][j]) {
					data[k] |= mask;
				} else {
					data[k] &= ~mask;
				}
				mask <<= 1;
			}
		}
	}

	Bitset(const Bitset<T>& b) : Bitset(b.nBits)
	{
		for (unsigned int i = 0; i < count; i++) {
			this->data[i] = b.data[i];
		}
	}
	
	~Bitset()
	{
		delete[] data;
	}
	
	Bitset& operator&= (const Bitset& b) 
	{
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] &= b.data[i];
		}
		return *this;
	}
	
	Bitset& operator|= (const Bitset& b) 
	{
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] |= b.data[i];
		}
		return *this;
	}
	
	Bitset& operator^= (const Bitset& b) 
	{
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] ^= b.data[i];
		}
		return *this;
	}
	
	Bitset operator| (const Bitset& b) 
	{
		Bitset<T> temp(*this);
		temp |= b;
		return temp;
	}
	
	Bitset operator& (const Bitset& b) 
	{
		Bitset<T> temp(*this);
		temp &= b;
		return temp;
	}
	
	
	bool operator[] (const unsigned int index) const
	{
		pair<unsigned int, T> p = getMaskByIndex(index);
		return (data[p.first] & p.second) != 0;
	}
	
	void set(const unsigned int index, const bool value) 
	{
		pair<unsigned int, T> p = getMaskByIndex(index);
		if (value) {
			data[p.first] |= p.second;
		} else {
			data[p.first] &= ~p.second;
		}
	}
	
    bool isAllTrue() const
    {
    	for (unsigned int i = 0; i < (count - 1); i++) {
			if (~data[i] != 0) {
				return false;
			}
		}
		return ~(data[count - 1] | lastmask) == 0;
	}
	
	bool isAllFalse() const
    {
    	for (unsigned int i = 0; i < count; i++) {
			if (data[i] != 0) {
				return false;
			}
		}
		return true;
	}
	
//private:
	unsigned int nBits;
	unsigned int count;
	T lastmask;
	T* data;
	
	pair<unsigned int, T> getMaskByIndex(const unsigned int index) const
	{
		unsigned int typesize = sizeof(T) * 8;
		unsigned int typeindex = static_cast<unsigned int>(index / typesize);
		if (typeindex >= count) {
			throw logic_error("Индекс вне границы массива");
		}
		T mask = 1;
		mask <<= (index % typesize);
		return make_pair(typeindex, mask);
	}
};


int main()
{
	
	bool t[8][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,1,1},{0,0,0,0,1,1,1,0},{0,0,0,0,1,1,1,0},{0,0,0,0,0,0,0,0}};
	
	bool** mask = new bool*[8];
	for (unsigned int i = 0; i < 8; i++) {
		mask[i] = new bool[8];
		for (unsigned int j = 0; j < 8; j++) {
			mask[i][j] = t[i][j];
		}
	}
	
	Bitset<char> b(64, mask, 8, 8);
	cout << "\n------------------------\n";
	for (unsigned int i = 0; i < b.count; i++) {
		cout << bitset<8>(b.data[i]) << endl;
	}
	Bitset<char> a(b);
	b.data[5] = 0;
	for (unsigned int i = 0; i < a.count; i++) {
		cout << bitset<8>(a.data[i]) << endl;
	}
	
	for (unsigned int i = 0; i < b.count; i++) {
		cout << bitset<8>(b.data[i]) << endl;
	}
	Bitset<char> c = b | a;
	
	for (unsigned int i = 0; i < c.count; i++) {
		cout << bitset<8>(c.data[i]) << endl;
	}
	cout << b.isAllTrue() << endl;
	
	
	Bitset<unsigned long long> d(65);
	d.data[0] = ~d.data[0];
	d.data[1]++;
	cout << "what? " << d.isAllTrue() << endl;
	cout << "0    " << bitset<64>(d.data[0]) << endl;
	cout << "1    " << bitset<64>(d.data[1]) << endl;
	cout << "mask " << bitset<64>(d.lastmask) << endl;
	
	cout << "\nbit    " << d[4] << endl;
	d.set(4, false);
	cout << "bit2   " << d[4] << endl;
	d.set(4, true);
	cout << "what2? " << d.isAllTrue() << endl;
	
	vector<Bitset<char>> v;
	v.push_back(c);
	v.push_back(b);
	v.push_back(a);
	v.push_back(c);
	v.push_back(b);
	v.push_back(a);
	
	
	return 0;
}
