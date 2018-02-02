//============================================================================
// Name        : ServerMain.cpp
// Author      : Amit And Eilon
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;

#include "Server.h"
//if the port ocupid can take the port from the argv else take from the TCP_PORT
int main(int argc, char* argv[]) {
	Server* server;
	if (argc == 2) {
		server = new Server(atoi(argv[1]));
	} else {
		server = new Server(TCP_PORT);
	}

	server->run();
	return 0;
}
