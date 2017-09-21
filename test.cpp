#include <iostream>
#include <vector>
#include <stdexcept>
#include "assign.h"
using namespace std;

int main() {
	//vector<vector<int> >test{{12,7,9,7,9}, {8,9,6,6,6},{7,17,12,14,9},{15,14,6,6,10},{4,10,7,10,9}};
	vector<vector<int> >test{{25,29,31,42,37},{39,38,26,20,33},{34,27,28,40,32},{24,42,36,23,45},{24,27,26,20,32}};
	try {
		vector<pair<int, int> > res = assign(test);
		for(auto r:res) {
			cout << r.first << " " << r.second << endl;
		}
	}
	catch(const invalid_argument& e) {
		cout << e.what() << endl;
	}
}