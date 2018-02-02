/*
 * LoginAndRegister.h
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#ifndef LOGINANDREGISTER_H_
#define LOGINANDREGISTER_H_
#include "Dispatcher.h"
#include <fstream>
#include <stdlib.h>



using namespace std;
namespace networkingLab{
class LoginAndRegister : public MThread {

private:
	map<string, string> userPassMap;
	vector<TCPSocket*> peers;
	Dispatcher * dispatcher;
	pthread_mutex_t peersLock;
	pthread_mutex_t userPassMapLock;

	bool Login(map<string,string>& data);
	bool Register(map<string,string> data);
	map<string, string>::iterator FindUser(string username);
	vector<TCPSocket*>::iterator findPeer(TCPSocket* peer);


public:

	LoginAndRegister();

	Dispatcher * getDispatcher();
	void remove(TCPSocket* peer);
	void add(TCPSocket * peer);
	void saveToFile();
	void run();
	void shutdown();
	virtual ~LoginAndRegister();
};
}

#endif /* LOGINANDREGISTER_H_ */
