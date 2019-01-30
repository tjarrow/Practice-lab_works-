#include "stdafx.h"
#include <iostream>
using namespace std;

long double fact(int N)
{
	if (N < 0) 
		return 0; 
	if (N == 0)
		return 1;
	else 
		return N * fact(N - 1); 
}

int main()
{
	int N;
	setlocale(0, ""); 
	cout << "Enter number to count factorial: ";
	cin >> N;
	cout << "Factorial for number: " << N << " = " << fact(N) << endl << endl; 
	return 0;
}