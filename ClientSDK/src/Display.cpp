/*
 * Display.cpp
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#include "Display.h"
#include <stdlib.h>

namespace networkingLab {

Display::Display() {

}

Display::~Display() {
}

void Display::print(string s) {
	cout << s << endl;
}

bool compareByScoreDescending(pair<string, string> p1,
		pair<string, string> p2) {
	return atoi(p1.second.c_str()) > atoi(p2.second.c_str());
}

void Display::showScoreboard(map<string, string> data) {
	vector<pair<string, string> > scoreboard(data.begin(), data.end());
	sort(scoreboard.begin(), scoreboard.end(), compareByScoreDescending);
	ostringstream scoreStream;
	scoreStream
			<< "=============================================================="
			<< endl;
	scoreStream << "Scoreboard" << endl;
	scoreStream
			<< "=============================================================="
			<< endl;
	scoreStream << "Username Score" << endl;
	for (vector<pair<string, string> >::iterator it = scoreboard.begin();
			it != scoreboard.end(); ++it) {
		pair<string, string> p = *it;
		string userName = p.first;
		string score = p.second;
		scoreStream << userName << " " << score << endl;
	}
	scoreStream
			<< "=============================================================="
			<< endl;
	cout << scoreStream.str() << endl;
}

void Display::printConnectedUsersList(map<string, string> data) {
	Global::delim();
	cout << "Connected users: ";
	;
	for (map<string, string>::iterator it = data.begin(); it != data.end();
			++it) {
		cout << it->second << " ";
	}
	cout << endl;
}

} /* namespace networkingLab */
