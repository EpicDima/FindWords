#include <iostream>
#include <bitset>

using namespace std;


template <typename T, unsigned int N>
class Bitset
{
public:
	Bitset()
	{
		unsigned int typesize = sizeof(T) * 8;
		count = static_cast<unsigned int>(N / typesize);
		if (N % typesize != 0) {
			count++;
		}
		data = new T[count];
		for (unsigned int i = 0; i < count; i++) {
			data[i] = 0;
		}
	}
	
	Bitset(bool** a, unsigned int n, unsigned int m) : Bitset()
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

	Bitset(const Bitset<T, N>& b) : Bitset()
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
		Bitset<T, N> temp(*this);
		temp |= b;
		return temp;
	}
	
	Bitset operator& (const Bitset& b) 
	{
		Bitset<T, N> temp(*this);
		temp &= b;
		return temp;
	}
	
    bool isAllTrue() const
    {
    	for (unsigned int i = 0; i < count; i++) {
			if (~data[i] != 0) {
				return false;
			}
		}
		return true;
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
	T* data;
	unsigned int count;
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
	
	Bitset<char, 64> b(mask, 8, 8);
	cout << "\n------------------------\n";
	for (unsigned int i = 0; i < b.count; i++) {
		cout << bitset<8>(b.data[i]) << endl;
	}
	Bitset<char, 64> a(b);
	b.data[5] = 0;
	for (unsigned int i = 0; i < a.count; i++) {
		cout << bitset<8>(a.data[i]) << endl;
	}
	
	for (unsigned int i = 0; i < b.count; i++) {
		cout << bitset<8>(b.data[i]) << endl;
	}
	Bitset<char, 64> c = b | a;
	
	for (unsigned int i = 0; i < c.count; i++) {
		cout << bitset<8>(c.data[i]) << endl;
	}
	cout << b.isAllTrue() << endl;
	
	
	
	
	
	return 0;
}
