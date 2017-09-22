#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <assert.h>
#include "assign.h"
#include "libxl.h"

using namespace std;
using namespace libxl;

void getTable(vector<vector<int> >& table, vector<string>& nameMap, vector<string>& dayMap) {
	Book* book = xlCreateXMLBook();
	if(book) {
		book->load("table.xlsx");
		Sheet* sheet = book->getSheet(0);
		int rows = 1, cols = 1;
		while(true) {
			CellType type = sheet->cellType(0, cols);
			if(type == CELLTYPE_EMPTY || type == CELLTYPE_BLANK) break;
			dayMap.push_back(sheet->readStr(0, cols));
			++cols;
		}
		while(true) {
			CellType type = sheet->cellType(rows, 0);
			if(type == CELLTYPE_EMPTY || type == CELLTYPE_BLANK) break;
			nameMap.push_back(sheet->readStr(rows, 0));
			++rows;
		}
		--rows; --cols;
		// cout << rows << " " << cols << endl;
		assert(rows * 2 == cols);
		table.resize(cols, vector<int>(cols));
		int i, j;
		for(i = 0; i < rows; ++i) {
			for(j = 0; j < cols; ++j) {
				table[i][j] = (sheet->cellType(i+1, j+1) == CELLTYPE_EMPTY ? 0 : int(sheet->readNum(i+1, j+1)));
			}
		}
		for(i = rows; i < table.size(); ++i) {
			for(j = 0; j < cols; ++j) {
				table[i][j] = table[i-rows][j];
			}
		}
		book->release();
	}
}

bool cmp(const pair<int, int>& p1, const pair<int, int>& p2) {
	return p1.second < p2.second;
}

int main() {
	vector<vector<int> > table;
	vector<string> nameMap;
	vector<string> dayMap;
	getTable(table, nameMap, dayMap);
	// printTable(table);
	// for(auto s:nameMap) cout << s << " ";
	// cout << endl;
	// for(auto s:dayMap) cout << s << " ";
	// cout << endl;
	try {
		vector<pair<int, int> > res = assign(table, true);
		sort(res.begin(), res.end(), cmp);
		int nameCnt = table.size() / 2;
		for(auto r:res) {
			cout << dayMap[r.second] << " " << nameMap[r.first % nameCnt] << endl;
		}
	}	
	catch(const invalid_argument& e) {
		cout << e.what() << endl;
	}
}