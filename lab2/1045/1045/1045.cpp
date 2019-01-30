#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool is_use[1001];
vector<int> airports[1001];
int level = 0;					
int second_ver = 0;					
bool is_second_ver_find = false;
bool is_sort[1001];
bool FIRST_WIN = true, SECOND_WIN = false;

bool dfs(int now_ver) {			
	is_use[now_ver] = true;
	bool child;
	level++;
	if (level == 2) {
		second_ver = now_ver;
	}
	if (!is_sort[now_ver]) {
		sort(airports[now_ver].begin(), airports[now_ver].end());
		is_sort[now_ver] = true;
	}

	int count_ver = 0;				
	for (int i = 0; i<airports[now_ver].size(); i++) {			
		if (!is_use[airports[now_ver][i]]) {
			child = dfs(airports[now_ver][i]);
			if (level % 2 == 1 ? child == FIRST_WIN : child == SECOND_WIN) {
				if (level == 1) {
					if (child == FIRST_WIN) {
						cout << "First player wins flying to airport " << second_ver << endl;
					}
					else {		
						cout << "First player loses" << endl;
					}
					return true;
				}
				else {
					level--;
					return child;
				}
			}	
			count_ver++;
		}
	}

	is_use[now_ver] = false;

	if (level != 1)return ((level--) % 2 == 1 ? SECOND_WIN : FIRST_WIN);
	else {
		if (child == FIRST_WIN) {
			cout << "First player wins flying to airport " << second_ver << endl;
		}
		else {
			cout << "First player loses" << endl;
		}
	}
}

int main() {
	int n, k;
	cin >> n >> k;
	int u, v;
	for (int i = 0; i<n - 1; i++) {
		cin >> u >> v;
		airports[u].push_back(v);
		airports[v].push_back(u);
	}
	second_ver = k;
	dfs(k);
}