/*
 * CLI.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

//#include "Game.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "ClientApplication.h"

using namespace std;
using namespace networkingLab;

int main(int argc, char* argv[]) {
	Client* client;
	if (argc == 2) {
		client = new Client(atoi(argv[1]));
	} else {
		client = new Client(TCP_PORT);
	}

	//in this cases we don't want to show the instructions
	while (true) {
		if (client->status == WAITING_FOR_USER_ANSWER
				|| client->status == IN_GAME
				|| client->status == WAITING_FOR_SCOREBOARD
				|| client->status == WAITING_FOR_RANDOM_PLAYER) {
			sleep(1);
			continue;
		}

		if (client->status != WAITING_FOR_YER_OR_NO) {
			Global::printClientInstructions();
			cout << "Please enter your choice: " << endl;
		}
		string command;
		getline(cin, command);
		string delimiter = " ";
		string beginCommand = command.substr(0, command.find(delimiter));
		string endCommand = command.substr(command.find(delimiter) + 1);

		if (client->status == WAITING_FOR_YER_OR_NO) {
			if (beginCommand == Yes) {
				client->yesAnswer();
			} else if (beginCommand == No) {
				client->noAnswer();
			} else {
				cout << "Please choose yes or no!" << endl;
			}
			continue;
		}

		if (beginCommand == ConnectDefaultServer) {
			client->connect("127.0.0.1");
		} else if (beginCommand == ConnectServer) {
			client->connect(endCommand);
		} else if (beginCommand == ListConnectedUsers) {
			client->listOnLineUsers();
		} else if (beginCommand == Login) {
			client->login(endCommand);
		} else if (beginCommand == Register) {
			client->regiester(endCommand);
		} else if (beginCommand == StartGameWithSpecificUser) {
			client->sendGameRequest(endCommand);
		} else if (beginCommand == StartGameWithRandomUser) {
			client->sendRandomPlayerRequest();
		} else if (beginCommand == Quit) {
			client->shutdown();
			break;
		} else {
			cout << "[ERROR] Unknown command " << beginCommand << endl;
		}
		Global::delim();
	}
	return 0;
}

