/*
 * Global.h
 *
 *  Created on: May 24, 2016
 *      Author: user
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <vector>

using namespace std;
namespace networkingLab {

class Global {
public:

	Global();

	virtual ~Global();

	static string convertTostring(int i) {
		string Result;
		ostringstream convert;
		convert << i;
		Result = convert.str();
		return Result;
	}

	static void delim()
	{
		cout << "=============================================================="
				<< endl;
	}

	static void printClientInstructions() {
		Global::delim();
		cout << "============================ MENU ============================"
				<< endl;
		Global::delim();
		cout << "cl                         - Connect to Server at localhost" << endl;
		cout << "c <Server IP>              - Connect to Server at <Server IP>"
				<< endl;
		cout << "r <Username> <Password>    - Register a new user" << endl;
		cout << "l <Username> <Password>    - Login" << endl;
		cout << "u                          - Show connected users" << endl;
		cout
				<< "g <Username>               - Start a new game with a specific user"
				<< endl;
		cout
				<< "rg                         - Start a new game with a random user"
				<< endl;
		cout << "q                          - Quit" << endl;
		Global::delim();

	}

	static string* splitString(string data) {
		bool flag = false;
		string * myStr = new string[2];
		string str;
		istringstream iss(data);
		while (iss) {
			string temp;
			iss >> temp;
			if (flag == false) {
				myStr[0] = temp;
				flag = true;
			} else {
				str.append(temp);
			}
		}
		myStr[1] = str;
		return myStr;
	}

	static void printMap(map<string, string> data) {
		cout << "{" << endl;
		for (map<string, string>::iterator it = data.begin(); it != data.end();
				it++) {
			cout << "   " << it->first << " " << it->second << endl;
		}
		cout << "}" << endl;
	}

	static void printMapInt(map<int, string> data) {
		cout << "{" << endl;
		for (map<int, string>::iterator it = data.begin(); it != data.end();
				it++) {
			cout << "   " << it->first << " " << it->second << endl;
		}
		cout << "}" << endl;
	}
};

} /* namespace networkingLab */

#endif /* GLOBAL_H_ */
