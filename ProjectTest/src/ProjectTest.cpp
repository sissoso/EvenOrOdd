//============================================================================
// Name        : ProjectTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TestRunner.h"
using namespace std;
using namespace networkingLab;

int main() {
	TestRunner t;
	cout << "========================================================" <<endl;
	cout << "========================= TEST =========================" <<endl;
	cout << "========================================================" <<endl;
	if (t.test())
		cout << "========================= PASS =========================" <<endl;
	else
		cout << "========================= FAIL =========================" <<endl;
}
