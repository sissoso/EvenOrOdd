#include "TestRunner.h"



namespace networkingLab {

void TestHandler::onMessageRecvFromServer(Command cmd) {
	this->lastCmdRecv = cmd;
}
void TestHandler::onMessageSentToServer(Command cmd) {
	this->lastCmdSent = cmd;
}
TestHandler::TestHandler() {
	this->lastCmdRecv = NONE;
	this->lastCmdSent = NONE;
}
TestHandler::~TestHandler() {

}

TestRunner::TestRunner() {
}

TestRunner::~TestRunner() {
}

class ServerRunner: public MThread {
	Server* server;
public:
	void run() {
		this->server->run();
	}
	ServerRunner(int serverPort) {
		server = new Server(serverPort);
	}
	~ServerRunner() {
		delete server;
	}
};

bool TestRunner::test() {

	TestHandler testHandler1;
	TestHandler testHandler2;

	// create server
	int serverPort = (rand() % 1000) + 6000;
	//run the server
	ServerRunner server(serverPort);
	server.start();

	print("Server started at port " + Global::convertTostring(serverPort));

	// create client1
	Client client1(serverPort, &testHandler1);
	// connect client1 to the server
	client1.connect("127.0.0.1");
	sleep(1);
	if (testHandler1.lastCmdRecv == PORT_EXCHANGE)
		print("Client 1 is connected to the server");
	else {
		print("Client 1 connect to server failed");
		return false;
	}

	// login client1 to the server with user a
	client1.regiester("a a");
	client1.login("a a");
	sleep(1);
	if (testHandler1.lastCmdRecv == SUCCESS_LOGIN
			&& testHandler1.lastCmdSent == LOGIN)
		print("Client 1 logged in");
	else {
		print("Client 1 failed to log in");
		return false;
	}

	// create client2
	 Client client2(serverPort, &testHandler2);
	// connect client2 to the server
	client2.connect("127.0.0.1");
	sleep(1);
	if (testHandler2.lastCmdRecv == PORT_EXCHANGE)
		print("Client 2 is connected to the server");
	else {
		print("Client 2 connect to server failed");
		return false;

	}

	// login client2 to the server with user b
	client2.regiester("b b");
	client2.login("b b");
	sleep(1);
	if (testHandler2.lastCmdRecv == SUCCESS_LOGIN
			&& testHandler2.lastCmdSent == LOGIN)
		print("Client 2 logged in");
	else {
		print("Client 2 failed to log in");
		return false;
	}

	// client1 send a game request to client2
	client1.sendGameRequest("b");
	sleep(1);
	if (testHandler1.lastCmdSent == SEND_GAME_REQUEST
			&& testHandler2.lastCmdRecv == SEND_GAME_REQUEST)
		print("Client 1 sent game request to Client 2");
	else {
		print("Client 1 failed to send a game request to Client 2");
		return false;
	}

	// client2 accept the request
	client2.yesAnswer();
	sleep(1);
	if (testHandler2.lastCmdSent == GAME_REQUEST_ACCEPTED
			&& testHandler1.lastCmdRecv == GAME_REQUEST_ACCEPTED)
		print("Client 2 accept the request from Client 1");
	else {
		print("Client 2 failed to accept the request from Client 1");
		return false;
	}

	// now they are playing the game...
	// the game ended, the winner is a
	client1.onGameEnded("a");
	client2.onGameEnded("a");
	sleep(1);
	if (testHandler1.lastCmdSent == GAME_ENDED
			&& testHandler2.lastCmdSent == GAME_ENDED
			&& testHandler1.lastCmdRecv == SCOREBOARD
			&& testHandler2.lastCmdRecv == SCOREBOARD)
		print("Client 2 accept the request from Client 1");
	else {
		print("Client 2 failed to accept the request from Client 1");
		return false;
	}

	return true;
}

} /* namespace networkingLab */

void networkingLab::TestRunner::print(string str) {
	cout<<str<<endl;
}
