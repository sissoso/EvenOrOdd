#include "Game.h"
#include <unistd.h>

namespace networkingLab {

Game::Game(int port, GameNotify* notify) {

	this->notify = notify;
	this->udpClient = new UDPClient(port, this);
	this->opponentChoice = -1;
	this->gotResultFromOpponent = false;
}
void Game::onReadCommandFromPeer(UDPClient* tcpClient, string msg) {
	this->opponentChoice = atoi(msg.c_str());
	this->gotResultFromOpponent = true;
}

void Game::run() {

	this->udpClient->start();
	string ipToSend = this->gameDetails[IP_KEY];
	string portToSend = this->gameDetails[PORT_KEY];
	string evenOrOdd = this->gameDetails[GAME_CATEGORY];

	this->opponentUsername = this->gameDetails[OPPONENT_USER_NAME];

	Global::delim();
	cout << "Even-Odd game" << endl;
	cout << "Opponent: " << this->opponentUsername << endl;
	cout << "You are: " << evenOrOdd << endl;
	Global::delim();

	cout << "Choose 1 or 2:" << endl;
	string input = "";
	getline(cin, input);

	while (input.length() != 1 || (input != "1" && input != "2")) {
		cout << "[ERROR] Please choose 1 or 2:" << endl;
		getline(cin, input);
	}

	this->udpClient->sendMsg(input, ipToSend, portToSend);

	int choice = atoi(input.c_str());
	cout << "Your choice is " << choice << endl;

	cout << "Waiting for opponent choice.." << endl;
	//waiting for opononet choice
	while (!this->gotResultFromOpponent) {
		sleep(1);
	}
	cout << "Opponent choice is " << this->opponentChoice << endl;
	//calculate who is the winner
	if (((choice + this->opponentChoice) % 2 == 0 && evenOrOdd == GAME_EVEN)
			|| ((choice + this->opponentChoice) % 2 == 1 && evenOrOdd == GAME_ODD)) {
		this->winner = this->username;
		cout << "You win the game" << endl;
	} else {
		this->winner = this->opponentUsername;
		cout << "You lost the game" << endl;
	}

	notify->onGameEnded(this->winner);
}

void Game::startGame(map<string, string> gameDetails, string username) {
	this->gameDetails = gameDetails;
	this->username = username;
	start();
}

void Game::close() {
	this->udpClient->close();
}

Game::~Game() {
}

} /* namespace networkingLab */

