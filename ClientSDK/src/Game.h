#ifndef GAME_H_
#define GAME_H_

#include "UDPClient.h"
#include <stdlib.h>

using namespace std;
namespace networkingLab {

class Game: public UDPClient::Notify, public MThread {
public:
	class GameNotify {
	public:
		virtual void onGameEnded(string winnerUsername)=0;
	};
private:
	UDPClient* udpClient;
	GameNotify* notify;//in our case this is the client
	map<string, string> gameDetails;
	string username;
	bool gotResultFromOpponent;
	int opponentChoice;
	string winner;
	string opponentUsername;
	void onReadCommandFromPeer(UDPClient* tcpClient, string msg);
public:
	Game(int port, GameNotify* notify);
	void close();
	void run();
	void startGame(map<string, string> gameDetails, string username);
	virtual ~Game();
};

} /* namespace networkingLab */

#endif /* GAME_H_ */
