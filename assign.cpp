#include <vector>
#include <set>
#include <iostream>
#include <stdexcept>
#include <climits>
#include <algorithm>
#include "assign.h"
using namespace std;

void printTable(const vector<vector<int> >& table) {
	for(auto r:table) {
		for(auto i:r) {
			if(i == -1) cout << "O ";
			else if(i == -2) cout << "X ";
			else cout << i << " ";
		}
		cout << endl;
	}
}

static void rowReduce(vector<vector<int> >& table) {
	int m = table.size();
	for(int i = 0; i < m; ++i) {
		int minval = INT_MAX;
		for(int j = 0; j < m; ++j) {
			minval = min(minval, table[i][j]);
		}
		for(int j = 0; j < m; ++j) {
			table[i][j] -= minval;
		}
	}
}

static void colReduce(vector<vector<int> >& table) {
	int m = table.size();
	for(int j = 0; j < m; ++j) {
		int minval = INT_MAX;
		for(int i = 0; i < m; ++i) {
			minval = min(minval, table[i][j]);
		}
		for(int i = 0; i < m; ++i) {
			table[i][j] -= minval;
		}
	}
}

// 叉0，将要叉的0改为-2
static void crossZero(vector<vector<int> >& table, int p, int q) {
	int n = table.size();
	table[p][q] = CIRCLE;
	for(int i = 0; i < n; ++i) {
		table[i][q] = (table[i][q] == 0) ? CROSS : table[i][q];
		table[p][i] = (table[p][i] == 0) ? CROSS : table[p][i];
	}
}

static void encircleZero(vector<vector<int> >& table, vector<pair<int, int> >& res, bool needCheckZero = true) {
	int n = table.size();

	int encircleCnt = 1;
	while(encircleCnt != 0) {
		encircleCnt = 0;
		for(int i = 0; i < n; ++i) {
			int p = 0, q = 0, cnt = 0;
			for(int j = 0; j < n; ++j) {
				cnt += (table[i][j] == 0 ? (p=i, q=j, 1) : 0);
			}
			if(cnt == 1) {
				++encircleCnt;
				res.push_back(make_pair(p, q));
				crossZero(table, p, q);
			}
		}
		for(int j = 0; j < n; ++j) {
			int p = 0, q = 0, cnt = 0;
			for(int i = 0; i < n; ++i) {
				cnt += (table[i][j] == 0 ? (p=i, q=j, 1) : 0);
			}
			if(cnt == 1) {
				++encircleCnt;
				res.push_back(make_pair(p, q));
				crossZero(table, p, q);
			}
		}
	}

	if(!needCheckZero) return;

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			if(table[i][j] == 0) {
				res.push_back(make_pair(i, j));
				crossZero(table, i, j);
				encircleZero(table, res, false);
			}
		}
	}
}

static void checkRowFunc(vector<vector<int> >& table, set<int>& rows, set<int>& cols, set<int>& checkCol) {
	int n = table.size();
	cols.clear();
	for(auto r:rows) {
		for(int i = 0; i < n; ++i) {
			if(table[r][i] <= 0) {
				if(checkCol.find(i) == checkCol.end()) {
					cols.insert(i);
					checkCol.insert(i);
				}
			}
		}
	}
}

static void checkColFunc(vector<vector<int> >& table, set<int>& rows, set<int>& cols, set<int>& checkRow) {
	int n = table.size();
	rows.clear();
	for(auto c:cols) {
		for(int i = 0; i < n; ++i) {
			if(table[i][c] == CIRCLE) {
				if(checkRow.find(i) == checkRow.end()) {
					rows.insert(i);
					checkRow.insert(i);
				}
			}
		}
	}
}

static void minLineCover(vector<vector<int> >& table) {
	int n = table.size(), i, j;
	set<int> checkRow, checkCol;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j) {
			if(table[i][j] == CIRCLE) {
				break;
			}
		}
		if(j == n) {
			checkRow.insert(i);
		}
	}
	set<int> rows = checkRow, cols;
	while(true) {
		checkRowFunc(table, rows, cols, checkCol);
		if(cols.size() == 0) break;
		checkColFunc(table, rows, cols, checkRow);
		if(rows.size() == 0) break;
	}
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j) {
			if(table[i][j] < 0) table[i][j] = 0;
		}
	}
	int minval = INT_MAX;
	for(i = 0; i < n; ++i) {
		if(checkRow.find(i) == checkRow.end()) continue;
		for(j = 0; j < n; ++j) {
			if(checkCol.find(j) != checkCol.end()) continue;
			minval = min(minval, table[i][j]);
		}
	}
	for(auto r:checkRow) {
		for(int i = 0; i < n; ++i) {
			table[r][i] -= minval;
		}
	}
	for(auto c:checkCol) {
		for(int i = 0; i < n; ++i) {
			table[i][c] += minval;
		}
	}
}

vector<pair<int, int> > assign(vector<vector<int> >& table, bool maxMode) {
	vector<pair<int, int> > res;
	int m = table.size();
	if(m == 0) return res;
	int n = table[0].size();
	if(m != n) throw std::invalid_argument("error: rownum != colnum");

	if(maxMode) {
		int maxval = 0;
		for(auto r:table) {
			for(auto i:r) {
				maxval = max(maxval, i);
			}
		}
		for(auto& r:table) {
			for(auto& i:r) {
				i = maxval - i;
			}
		}
	}

	for(auto r:table) {
		for(auto i:r) {
			if(i < 0) {
				throw std::invalid_argument("error: table elements should be all greater than or equal to zero");
			}
		}
	}

	auto printTableAndWait = [&](string title){
#ifdef DEBUG_MODE
		cout << title << endl;
		printTable(table);
		string s;
		getline(cin, s);
#endif
	};

	printTableAndWait("Init Matrix:");

	rowReduce(table);
	colReduce(table);
	printTableAndWait("Reduced Matrix:");

	while(true) {
		res.clear();
		encircleZero(table, res);
		printTableAndWait("After encircleZero");
		if(res.size() == m) break;
		minLineCover(table);
		printTableAndWait("After minLineCover:");
	}

	return res;
}
