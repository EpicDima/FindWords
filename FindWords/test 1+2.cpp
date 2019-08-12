#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

unsigned int n = 8;
unsigned int m = 8;
const unsigned int nm = n * m;

unsigned int size = 45;

bool masks[45][8][8] = {{{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,1,1,1,1,1,1,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,1,1,1,1},{0,0,0,0,1,1,1,1}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,1,1,1,0,0},{0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{1,1,1,0,0,0,0,0},{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{1,1,0,0,0,0,0,0},{0,1,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,0,0,1,0,0,0,0},{1,0,0,1,0,0,0,0},{1,1,1,1,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,1,0,0,0},{0,0,0,0,1,0,0,0},{0,0,0,0,1,1,1,0},{0,0,0,0,1,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,1,1,1,0},{0,0,0,0,1,0,0,0},{0,0,0,0,1,1,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,1,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{1,1,1,0,0,0,0,0},{1,1,0,0,0,0,0,0},{0,1,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,1,0},{0,0,0,0,0,0,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,1,0,0,0,0},{1,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,1,0,0,0,0,0,0},{0,1,0,0,0,0,0,0},{1,1,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,1,0,0,0,0,0,0},{0,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,1,0,0,0},{0,0,0,0,1,0,0,0},{0,0,0,0,1,0,0,0},{0,0,0,0,1,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,1,1,0,0,0,0,0},{0,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,1,1,1,0,0,0,0},{0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,1,1,1,0,0,0,0},{0,1,0,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,1,0,0,0,0},{0,0,0,1,1,0,0,0},{0,0,0,0,1,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,1,1,1,0},{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,1,0},{0,0,0,1,1,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,1,1,0,0,0,0,0},{0,1,1,0,0,0,0,0},{0,1,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,1,1,0,0,0,0,0},{1,1,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,1,1,0,0,0,0},{0,0,1,0,0,0,0,0},{0,1,1,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,1,1,1,1,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,1,1,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,1,1,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,1,1,1,0,0},{0,0,0,0,1,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,1,1,1,0},{0,0,0,0,0,0,1,0},{0,0,0,0,0,0,1,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1},{0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,1,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,1,1,0,0,0},{0,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{1,1,1,1,0,0,0,0},{0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{1,1,0,0,0,0,0,0},{0,1,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,1,1,1,1,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,1,0,0,0,0},{1,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
						{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,1,0,0},{0,0,0,0,1,1,0,0},{0,0,0,0,1,1,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}};


ofstream file("test1+2.txt", ios_base::out | ios_base::trunc);

// ����� ����� ��� ������ � ������, �� ����� ��������� �������� ���� ��������, �� ���� ������ ���� ����� ������ �����
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
						throw logic_error("�� ������� ������ ��� ������ ����� ���������");
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
		
private:
	T* data;
	unsigned int count;
};


vector<Bitset<unsigned long long, 64>> bitsetMasks;

template<typename T, unsigned int N>
void f(Bitset<T, N> mask, vector<unsigned int> indexes, vector<unsigned int> unusefulIndexes)
{	
	for (unsigned int k = 0; k < size; k++) {
		bool use = true;
		for (unsigned int i = 0; i < indexes.size(); i++) {
			if (k == indexes[i]) {
				use = false;
				break;
			}
		}
		if (use) {
			bool use2 = true;
			for (unsigned int i = 0; i < unusefulIndexes.size(); i++) {
				if (k == unusefulIndexes[i]) {
					use2 = false;
					break;
				}
			}
			if (use2) {
				if ((mask & bitsetMasks[k]).isAllFalse()) {					
					Bitset<unsigned long long, 64> temp(mask);
					temp |= bitsetMasks[k];
					
					if (temp.isAllTrue()) {
						for (unsigned int i = 0; i < indexes.size(); i++) {
							file << setw(3) << indexes[i];
						}
						file << "\n";
						return;
					}
					
					vector<unsigned int> tempIndexes = indexes;
					tempIndexes.push_back(k);
					f(temp, tempIndexes, vector<unsigned int>(unusefulIndexes));
				}
			}
		}
	}
}


void ffff(unsigned int x0, unsigned int y0)
{
	for (unsigned int k = 0; k < size; k++) {
		if (masks[k][x0][y0]) {
			vector<unsigned int> indexes = {k};
			f(bitsetMasks[k], indexes, vector<unsigned int>());
		}
	}
}




int main()
{	
	unsigned int** sum = new unsigned int* [n];
	for (unsigned int i = 0; i < n; i++) {
		sum[i] = new unsigned int[m];
		for (unsigned int j = 0; j < m; j++) {
			sum[i][j] = 0;
			for (unsigned int k = 0; k < size; k++) {
				sum[i][j] += masks[k][i][j];
			}
		}
	}
	
	for (unsigned int k = 0; k < size; k++) {
		bool** mask = new bool*[n];
		for (unsigned int i = 0; i < n; i++) {
			mask[i] = new bool[m];
			for (unsigned int j = 0; j < m; j++) {
				mask[i][j] = masks[k][i][j];
			}
		}
		
		bitsetMasks.push_back(Bitset<unsigned long long, 64>(mask, n, m));
		
		for (unsigned int i = 0; i < n; i++) {
			delete[] mask[i];
		}
		delete[] mask;
	}
	
	unsigned int min = 4000000000;
	unsigned int xMin = 0, yMin = 0;
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < m; j++) {
			cout << setw(3) << sum[i][j];
			if (sum[i][j] < min) {
				min = sum[i][j];
				xMin = i;
				yMin = j;
			}
		}
		cout << endl;
	}
	
	cout << endl << xMin << endl << yMin << endl << endl;
	
	ffff(xMin, yMin);
	
	return 0;
} 
