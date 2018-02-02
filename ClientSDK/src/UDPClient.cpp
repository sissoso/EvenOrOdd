/*
 0 * UDPClient.cpp
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#include "UDPClient.h"

namespace networkingLab {

UDPClient::UDPClient(int port, Notify* notify) {
	this->udpSocket = new UDPSocket(port);
	this->notify = notify;
	this->running = true;
}

UDPClient::~UDPClient() {
}

void UDPClient::run() {
	while (running) {
		char buff[1024];
		int rc = udpSocket->recv(buff, sizeof(buff));
		if (rc < 0) {
			cout << "Error: Msg error " << endl;
		}
		buff[rc] = '\0';
		string msg(buff);
		notify->onReadCommandFromPeer(this, msg);
	}
}

void UDPClient::close() {
	running = false;
}

void UDPClient::sendMsg(string msg, string ip, string sport) {
	int port = atoi(sport.c_str());
	if (this->udpSocket->sendTo(msg, ip, port) == -1)
		cout << "send msg failed" << endl;

}

} /* namespace networkingLab */

