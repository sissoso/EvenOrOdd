/*
 * Server.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include "Server.h"
namespace networkingLab {

Server::Server(int port) {
	socket = new TCPSocket(port);
	loginAndRegister = new LoginAndRegister();
}

Server::~Server() {

}

void Server::run() {
	while (true) {
		TCPSocket * peer = socket->listenAndAccept();
		if (peer == NULL) {
			break;
		}
		Global::printLog("Got new connection");
		map<string, string> portMap;
		portMap[PORT_KEY] = Global::convertTostring(peer->getDestPort());
		Protocol::SendMSG(peer, PORT_EXCHANGE, portMap);
		loginAndRegister->add(peer);
	}
}

void Server::shutdown() {
	socket->close();
	delete socket;
	loginAndRegister->shutdown();
	delete loginAndRegister;
}

}
