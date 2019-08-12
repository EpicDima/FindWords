#include <iostream>
#include <vector>
#include "../BitMask.h"

using namespace std;


int main()
{
	int n = 5;
	BitMask a(n, n);
	
	string s = "hello";
	
	pair<string, BitMask> p = make_pair(s, a);
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			p.second[i][j] = true;//i * j % 2 == 0;
		}
	}
	
	p.second[0][0] = false;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << p.second[i][j];
		}
		cout << endl;
	}
	
	cout << p.first << endl;
	
	vector<pair<string, BitMask>> v;
	v.push_back(p);
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			v[0].second[i][j] = i * j % 2 != 0;
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << v[0].second[i][j];
		}
		cout << endl;
	}
	
	BitMask q(v[0].second);
	v[0].second[3][3] = true;
	v[0].second[3][4] = true;
	v[0].second[3][2] = true;
	
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << v[0].second[i][j];
		}
		cout << endl;
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << q[i][j];
		}
		cout << endl;
	}
	
	return 0;
}
