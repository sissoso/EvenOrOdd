#include "Client.h"
#include "Protocol.h"

namespace networkingLab {

Client::Client(int serverPort, EventHandler* handler) {
	this->status = NOT_CONNECTED;
	this->serverPort = serverPort;
	this->display = new Display();
	this->tcpClient = NULL;
	this->game = NULL;
	this->handler = handler;
}

Client::Client(int serverPort) {
	this->status = NOT_CONNECTED;
	this->serverPort = serverPort;
	this->display = new Display();
	this->tcpClient = NULL;
	this->game = NULL;
	this->handler = NULL;
}

void Client::onGameEnded(string winnerUsername) {
	map<string, string> data;
	data[USER_NAME_KEY] = winnerUsername;
	//send to the server game ended and who is the winner
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), GAME_ENDED, data);

	if (this->handler != NULL)
		this->handler->onMessageSentToServer(GAME_ENDED);
	this->status = WAITING_FOR_SCOREBOARD;
}

void Client::onReadCommandFromServer(TCPClient* tcpClient, Command cmd,
		map<string, string> data) {
	int com = cmd;
	if (com < 0) {
		this->display->print("[ERROR] Error in connection to server");
		this->shutdown();
		return;
	}

	if (this->handler != NULL)
		this->handler->onMessageRecvFromServer(cmd);

	switch (cmd) {

	case ERROR_REGISTER:
		this->display->print("User In Use");
		this->status = CONNECTED;
		break;

	case SUCCESS_REGISTER:
		this->display->print("Register success");
		this->status = CONNECTED;
		break;

	case SEND_GAME_REQUEST: {
		this->status = WAITING_FOR_YER_OR_NO;
		this->fromUser = data[USER_NAME_KEY];
		cout << this->fromUser << " Sent you a game request." << endl;
		cout << "Do you want to play with him? (enter y/n)" << endl;
	}
		break;

	case GAME_REQUEST_REFUSED: {
		cout << "The user refused your request" << endl;
		this->status = CONNECTED;
	}
		break;

	case SCOREBOARD: {
		this->display->showScoreboard(data);
		this->status = CONNECTED;
	}
		break;

	case BRING_RANDOM_PLAYER: {
		string chosen = data[USER_NAME_KEY];
		cout << "The server pick the player: " << chosen << endl;
		this->sendGameRequest(chosen);
	}
		break;

	case NO_PLAYERS_AVAILABLE: {
		this->display->print("No users available to play with");
		this->status = CONNECTED;
	}
		break;

	case NO_SUCH_USER:
		this->display->print("User does not exist in the list");
		this->status = CONNECTED;
		break;

	case YOU_CANT_START_A_GAME_WITH_YOURSELF:
		this->display->print("You cant start a game with yourself");
		this->status = CONNECTED;
		break;

	case USER_ALREADY_IN_GAME:
		this->display->print("User is already in a game with someone");
		this->status = CONNECTED;
		break;

	case COMMAND_NOT_FOUND:
		this->display->print("Command Not Found (Server)");
		break;

	case ERROR_LOGIN:
		this->display->print(
				"Bad credentials or the user is already connected");
		break;

	case SUCCESS_LOGIN:
		this->display->print("Login success");
		this->status = CONNECTED;
		this->username = data[USER_NAME_KEY];
		break;

	case LIST_CONNECTED_USERS:
		//the server sent map  <string(index),string(user name)> of all users
		this->display->printConnectedUsersList(data);
		break;

	case PORT_EXCHANGE: {
		//the udpPort (same like tcp port of the tcpSocket )
		int intPort = atoi(data.find(PORT_KEY)->second.c_str());
		this->game = new Game(intPort, this);
	}
		break;

	case GAME_REQUEST_ACCEPTED: {
		this->status = IN_GAME;
		//the data is the gameDetails (name,ip,port,even or odd)
		this->game->startGame(data, this->username);
	}
		break;

	default:
		this->display->print("Command Not Found");
		break;
	}

}

Client::~Client() {

}

void Client::yesAnswer() {
	map<string, string> data;
	data[USER_NAME_KEY] = this->fromUser;
	//sending to the server that we accept play against the player
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), GAME_REQUEST_ACCEPTED,
			data);

	if (this->handler != NULL)
		this->handler->onMessageSentToServer(GAME_REQUEST_ACCEPTED);
	this->status = IN_GAME;

}
void Client::noAnswer() {
	map<string, string> data;
	data[USER_NAME_KEY] = this->fromUser;
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), GAME_REQUEST_REFUSED,
			data);

	if (this->handler != NULL)
		this->handler->onMessageSentToServer(GAME_REQUEST_REFUSED);
	this->status = CONNECTED;
}

void Client::listOnLineUsers() {
	if (this->status == CONNECTED) {
		Protocol::SendMSG(this->tcpClient->getTcpSocket(),
				LIST_CONNECTED_USERS);

		if (this->handler != NULL)
			this->handler->onMessageSentToServer(LIST_CONNECTED_USERS);
	}

	else {
		cout << "[ERROR] You are NOT Connected" << endl;
	}
}

void Client::login(string UserDetials) {
	if (this->status == CONNECTED) {
		string delimiter = " ";
		string userName = UserDetials.substr(0, UserDetials.find(delimiter));
		string passWord = UserDetials.substr(UserDetials.find(delimiter) + 1);
		map<string, string> UserDetialsMap;
		UserDetialsMap[USER_NAME_KEY] = userName;
		UserDetialsMap[PASSWORD_KEY] = passWord;
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), LOGIN,
				UserDetialsMap);

		if (this->handler != NULL)
			this->handler->onMessageSentToServer(LOGIN);
	} else {
		if (this->status == NOT_CONNECTED) {
			cout << "[ERROR] You are NOT Connected" << endl;
		} else {
			cout << "[ERROR] You are already logged in" << endl;
		}
	}

}

void Client::regiester(string UserDetials) {
	if (this->status == CONNECTED) {
		string delimiter = " ";
		string userName = UserDetials.substr(0, UserDetials.find(delimiter));
		string passWord = UserDetials.substr(UserDetials.find(delimiter) + 1);
		map<string, string> UserDetialsMap;
		UserDetialsMap[USER_NAME_KEY] = userName;
		UserDetialsMap[PASSWORD_KEY] = passWord;
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), REGISTER,
				UserDetialsMap);

		if (this->handler != NULL)
			this->handler->onMessageSentToServer(REGISTER);
	} else {
		if (this->status == NOT_CONNECTED) {
			cout << "[ERROR] You are NOT Connected" << endl;
		} else {
			cout << "[ERROR] You are already logged in" << endl;
		}
	}

}

void Client::sendRandomPlayerRequest() {
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), BRING_RANDOM_PLAYER);

	if (this->handler != NULL)
		this->handler->onMessageSentToServer(BRING_RANDOM_PLAYER);
	cout << "Waiting for the server to choose a random player" << endl;
	this->status = WAITING_FOR_RANDOM_PLAYER;
}

void Client::sendGameRequest(string userName) {
	if (this->status == CONNECTED
			|| this->status == WAITING_FOR_RANDOM_PLAYER) {
		map<string, string> UserDetialsMap;
		//the details of the user component
		UserDetialsMap[USER_NAME_KEY] = userName;
		//send against who we want to play
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), SEND_GAME_REQUEST,
				UserDetialsMap);

		if (this->handler != NULL)
			this->handler->onMessageSentToServer(SEND_GAME_REQUEST);
		cout << "Game request sent, waiting " << userName << " to response..."
				<< endl;
		this->status = WAITING_FOR_USER_ANSWER;
	} else {
		if (this->status == NOT_CONNECTED) {
			cout << "you are not connected" << endl;
		} else {
			cout << "you are already in session" << endl;
		}
	}

}

void Client::disconnect() {
	if (this->status != NOT_CONNECTED) {
		//send to the server msg that we want to disconnect and the server will remove us from his list
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), DISCONNECT);

		if (this->handler != NULL)
			this->handler->onMessageSentToServer(DISCONNECT);
		this->tcpClient->close();
		this->status = NOT_CONNECTED;
	} else
		cout << "[ERROR] You are not connected to the server" << endl;

}

void Client::connect(string ip) {
	if (this->status == NOT_CONNECTED) {
		cout << "Connecting to " << ip << endl;
		this->tcpClient = new TCPClient(this, ip, this->serverPort);
		this->status = CONNECTED;
	} else {
		cout << "[ERROR] You are Already Connected" << endl;
	}
}

void Client::shutdown() {
	if (status == CONNECTED) {
		this->disconnect();
	}
	this->status = NOT_CONNECTED;
	this->display->~Display();
}

} /* namespace networkingLab */

