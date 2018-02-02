/*
 * Dispatcher.h
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketListener.h"
#include <vector>
#include "PeerName.h"
#include "Protocol.h"
#include "Global.h"

using namespace std;
namespace networkingLab {
class Dispatcher: public MThread {

private:
	vector<PeerName*> peerlist;
	pthread_mutex_t peerlistlock;
	map<string, string> getScoreboard();
	string pickRandomFreePlayer(string exclude);
public:

	Dispatcher();

	void printOnLineUsers();
	void removePeer(PeerName* peerName);
	void add(PeerName *thisPeer);
	void SendGameRequest(PeerName* sock, map<string, string> data);
	PeerName* FindPeer(TCPSocket* sock);
	PeerName* FindPeer(string usrname);
	void run();
	vector<TCPSocket*> getPeersVector();
	virtual ~Dispatcher();
	map<string, string> getMapOfAllConnectPeers();

};
}
#endif /* DISPATCHER_H_ */
