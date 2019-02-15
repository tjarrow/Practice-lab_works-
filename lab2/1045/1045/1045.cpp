#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool DepthFirstSearch(int currentVertice) {		
  bool isUsed[1001];
  vector<int> airports[1001];
  int level = 0;
  int secondVertice = 0;
  bool secondVerticeFound = false;
  bool isSorted[1001];
  bool firstWin = true, secondWin = false;
  isUsed[currentVertice] = true;
  bool child;
  level++;
  if (level == 2) {
  	secondVertice = currentVertice;
  }
  if (!isSorted[currentVertice]) {
	sort(airports[currentVertice].begin(), airports[currentVertice].end());
	isSorted[currentVertice] = true;
   }

  int countVertices = 0;				
  for (int i = 0; i < airports[currentVertice].size(); i++) {			
    if (!isUsed[airports[currentVertice][i]]) {
      child = DepthFirstSearch(airports[currentVertice][i]);
    	if (level % 2 == 1 ? child == firstWin : child == secondWin) {
		  if (level == 1) {
			if (child == firstWin) {
			  cout << "First player wins flying to airport " << secondVertice << endl;
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
	    countVertices++;
	  }
	}

	isUsed[currentVertice] = false;

	if (level != 1) 
	  return ((level--) % 2 == 1 ? secondWin : firstWin);
	else {
	  if (child == firstWin) {
		cout << "First player wins flying to airport " << secondVertice << endl;
		}
		else {
		  cout << "First player loses" << endl;
	   }
	}
}

int main() {
	vector<int> airports[1001];
	int secondVertice = 0;
	int n, k;
	cin >> n >> k;
	int u, v;
	for (int i = 0; i < n - 1; i++) {
		cin >> u >> v;
		airports[u].push_back(v);
		airports[v].push_back(u);
	}
	secondVertice = k;
	DepthFirstSearch(k);
}


