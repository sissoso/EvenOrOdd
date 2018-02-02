/*
 * PeerUser.h
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#ifndef PEERUSER_H_
#define PEERUSER_H_

#include <string>
#include <iostream>
#include "TCPSocket.h"
#include "Global.h"

typedef enum {
	PEER_FREE, PEER_IN_GAME
} PeerStatus;

using namespace std;
namespace networkingLab {
class PeerName {
private:
	TCPSocket* peerSock;
	string userName;
	int score;
	//if true-> add the score.
	bool shouldAddScore;
public:
	PeerStatus status;
	PeerName(string userName, TCPSocket* peerSock);
	virtual ~PeerName();

	TCPSocket* getPeer();
	string getUserName();
	void addScore(int n);
	int getScore();

};
}
#endif /* PEERUSER_H_ */
