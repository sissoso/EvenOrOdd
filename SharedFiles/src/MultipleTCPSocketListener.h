#ifndef SRC_MULTIPLETCPSOCKETLISTENER_H_
#define SRC_MULTIPLETCPSOCKETLISTENER_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include "TCPSocket.h"

using namespace std;
using namespace networkingLab;

class MultipleTCPSocketListener {

	typedef vector<TCPSocket*> tSockets;
	vector<TCPSocket*> sockets;

public:
	/**
	 * add socket to listen to
	 */
	void add(TCPSocket* socket);
	void addSockets(vector<TCPSocket*> sockets);
	TCPSocket* listen(int timeout);
};

#endif /* SRC_MULTIPLETCPSOCKETLISTENER_H_ */
