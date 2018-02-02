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
#include <map>
#include <sstream>
#include <vector>

using namespace std;

namespace networkingLab {

#define USER_PASS_FILE "Users.db"

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
//return map of the users and passwords
	static map<string, string> LoadUserPassFile() {
		map<string, string> userPassMap;
		ifstream file(USER_PASS_FILE, ios::in);
		if (file.is_open()) {
			string size, buffer, username, password;
			getline(file, size);
			int Mapsize = atoi(size.c_str());
			for (int i = 0; i < Mapsize; i++) {
				getline(file, buffer);
				username = buffer;
				getline(file, buffer);
				password = buffer;
				userPassMap[username] = password;
			}
			file.close();
		}
		return userPassMap;
	}

	static void SaveMap(map<string, string>* outputMap) {
		ofstream file(USER_PASS_FILE, ios::out | ios::trunc);
		if (file.is_open()) {
			file << outputMap->size() << "\n";
			for (map<string, string>::iterator it = outputMap->begin();
					it != outputMap->end(); it++) {
				file << it->first << endl;
				file << it->second << endl;
			}
			file.close();
		} else
			cout << "ERROR: Unable to open UserPass file" << endl;
	}

	static map<string, string> getUserPassMap() {
		map<string, string> userPassMap = LoadUserPassFile();
		map<string, string> ret;
		int i = 0;
		for (map<string, string>::iterator it = userPassMap.begin();
				it != userPassMap.end(); ++it) {
			string userName = it->first;
			string UserIndex = convertTostring(i);
			ret[UserIndex] = userName;
			i++;
		}
		return ret;

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

	static void printLog(string header, string data) {
		cout << "[" << header << "] - " << data << endl;
	}
	//print to screen that there is new connection
	static void printLog(string data) {
		cout << "[LOG] - " << data << endl;
	}

	static void printMap(map<string, string> data) {
		cout << "{" << endl;
		for (map<string, string>::iterator it = data.begin(); it != data.end();
				it++) {
			cout << "   " << it->first << " " << it->second << endl;
		}
		cout << "}" << endl;
	}
};

} /* namespace networkingLab */

#endif /* GLOBAL_H_ */
