/*
 * Client.h
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#ifndef CLIENT_H_
#define CLIENT_H_
#include "TCPClient.h"
#include "UDPClient.h"
#include "Display.h"
#include "Game.h"
#include "EventHandler.h"

typedef enum {
	NOT_CONNECTED,
	CONNECTED,
	WAITING_FOR_YER_OR_NO,
	WAITING_FOR_USER_ANSWER,
	IN_GAME,
	WAITING_FOR_RANDOM_PLAYER,
	WAITING_FOR_SCOREBOARD
} Status;

using namespace std;
namespace networkingLab {

class Client : public TCPClient::Notify, Game::GameNotify {
public:
	TCPClient* tcpClient;
	int serverPort;
	Display* display;
	Game* game;
	string username;
	Status status;
	string fromUser;
	EventHandler* handler;

	void onReadCommandFromServer(TCPClient* tcpClient, Command command,
			map<string, string> data);
	void onGameEnded(string winnerUsername);

	void connect(string ip);
	void disconnect();
	void shutdown();
	void yesAnswer();
	void noAnswer();
	void listOnLineUsers();
	void login(string UserDetials);
	void regiester(string UserDetials);
	void sendGameRequest(string userName);
	void sendRandomPlayerRequest();
	Client(int serverPort,EventHandler* handler);
	Client(int serverPort);
	virtual ~Client();
};

} /* namespace networkingLab */

#endif /* CLIENT_H_ */
