/*
 * Display.h
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <iostream>
#include <sstream>
#include <map>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include "Global.h"

using namespace std;
namespace networkingLab {

class Display {
public:
	Display();
	void print(string s);
	void printConnectedUsersList(map<string, string> data);
	void showScoreboard(map<string, string> data);
	virtual ~Display();
};

} /* namespace networkingLab */

#endif /* DISPLAY_H_ */

