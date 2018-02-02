/*
 * UDPClient.h
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#include "MThread.h"
#include "UDPSocket.h"
#include <stdlib.h>
#include <string>
#include <map>
#include "Global.h"
#include "Protocol.h"

using namespace std;
namespace networkingLab {

class UDPClient: public MThread {
public:
	class Notify {
	public:
		virtual void onReadCommandFromPeer(UDPClient* udpClient, string msg)=0;
	};
private:
	UDPSocket* udpSocket;
	Notify* notify;
	bool running;
public:
	UDPClient(int port, Notify* notify);
	void run();
	void close();
	void sendMsg(string msg, string ip, string sport);
	virtual ~UDPClient();
};

} /* namespace networkingLab */

#endif /* UDPCLIENT_H_ */
