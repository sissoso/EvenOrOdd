/*
 * Server.h
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "LoginAndRegister.h"


using namespace std;
namespace networkingLab{

class Server : public MThread{

private:
	TCPSocket* socket;
	LoginAndRegister * loginAndRegister;

public:
	Server(int port);
	void run();
	void shutdown();
	virtual ~Server();
};

}
#endif /* SERVER_H_ */
