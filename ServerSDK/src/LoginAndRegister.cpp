/*
 * LoginAndRegister.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include "LoginAndRegister.h"
#include "Md5.h"

namespace networkingLab {

LoginAndRegister::LoginAndRegister() {
	dispatcher = new Dispatcher();
	this->userPassMap = Global::LoadUserPassFile();

}

void LoginAndRegister::add(TCPSocket* peer) {
	pthread_mutex_lock(&peersLock);
	peers.push_back(peer);
	//the thread start when the first user added
	if (peers.size() == 1) {
		start();
	}
	pthread_mutex_unlock(&peersLock);
}
//remove user by socket
void LoginAndRegister::remove(TCPSocket* peer) {
	pthread_mutex_lock(&peersLock);
	vector<TCPSocket*>::iterator it = findPeer(peer);
	if (it != peers.end())
		peers.erase(it);
	pthread_mutex_unlock(&peersLock);
}
//find the user by peer
vector<TCPSocket*>::iterator LoginAndRegister::findPeer(TCPSocket* peer) {
	if (peers.size() > 0) {
		vector<TCPSocket*>::iterator it;
		for (it = peers.begin(); it != peers.end(); it++) {
			if (*it == peer) {
				return it;
			}
		}
	}
	return peers.end();
}

void LoginAndRegister::run() {
	while (peers.size() > 0) {
		MultipleTCPSocketListener listener;
		listener.addSockets(this->peers);
		TCPSocket * peer = listener.listen(1);

		if (peer != NULL) {
			Command command;
			//data key username,data value pass
			map<string, string> data;
			Protocol::readMSG(peer, command, data);
			switch (command) {

			case REGISTER:
				Global::printLog("got REGISTER command");
				//check if the user name is available
				Global::printLog("REGISTER",
						"Checking if " + data.find(USER_NAME_KEY)->second
								+ " is already taken");
				if (this->Register(data)) {
					Global::printLog("REGISTER", "Success");
					//send to the user that success to register
					Protocol::SendMSG(peer, SUCCESS_REGISTER, data);
				} else {
					//send to the user cannot register
					Global::printLog("REGISTER", "Failure");
					Protocol::SendMSG(peer, ERROR_REGISTER);

				}
				break;

			case LOGIN:
				Global::printLog("got LOGIN command");
				Global::printLog("LOGIN",
						"Checking credentials (" + data[USER_NAME_KEY] + ","
								+ data[PASSWORD_KEY] + ")");

				if (this->Login(data)) {
					//if the login success ->create a peerName(User)
					PeerName * thisPeer = new PeerName(
							data.find(USER_NAME_KEY)->second, peer);
					//add the user to the dispatcher
					dispatcher->add(thisPeer);
					//remove the peer from the peers vector
					this->remove(peer);
					Global::printLog("LOGIN", "Success");
					Protocol::SendMSG(peer, SUCCESS_LOGIN, data);
				} else {
					Global::printLog("LOGIN", "Failure");
					Protocol::SendMSG(peer, ERROR_LOGIN);
				}
				break;

			case DISCONNECT:
				Global::printLog("got DISCONNECT command");
				this->remove(peer);
				break;

			case NONE:
				Global::printLog("ERROR","got NONE command, this should not happen");
				this->remove(peer);
				break;

			default:
				Global::printLog("got UNKOWN command with value = "+command);
				Protocol::SendMSG(peer, COMMAND_NOT_FOUND);
				break;
			}
		}
	}
}

bool LoginAndRegister::Login(map<string, string>& data) {
	bool ret = false;
	//get user name
	string userName = data.find(USER_NAME_KEY)->second;
	//get password and decrypt
	string passwordMd5 = md5(data.find(PASSWORD_KEY)->second);
	//check if the user name already logged in
	if (this->dispatcher->FindPeer(userName))
		return false;

	pthread_mutex_lock(&userPassMapLock);
	map<string, string>::iterator iter = FindUser(userName);
	//checking if the password is correct
	if (iter != userPassMap.end()) {
		if (passwordMd5 == (string) iter->second) {
			ret = true;
		}
	}
	pthread_mutex_unlock(&userPassMapLock);
	return ret;
}

bool LoginAndRegister::Register(map<string, string> data) {
	string userName = data.find(USER_NAME_KEY)->second;
	string password = data.find(PASSWORD_KEY)->second;

	bool ret = false;
	pthread_mutex_lock(&userPassMapLock);
	//check if the user name already exist(if the last iterator return -> the user name doesn't exist)
	if (this->FindUser(userName) == userPassMap.end()) {
		//encrypt the password with MD 5
		userPassMap[userName] = md5(password);
		//save the the user name and the encrypt pass to file
		saveToFile();
		//register success
		ret = true;
	}
	pthread_mutex_unlock(&userPassMapLock);
	return ret;
}

map<string, string>::iterator LoginAndRegister::FindUser(string username) {
	map<string, string>::iterator it = userPassMap.find(username);
	return it;
}

void LoginAndRegister::shutdown() {
	pthread_mutex_lock(&peersLock);
	for (vector<TCPSocket*>::iterator it = peers.begin(); it != peers.end();
			++it) {
		TCPSocket* p = *it;
		p->close();
		delete p;
	}
	pthread_mutex_unlock(&peersLock);
	delete dispatcher;
}

Dispatcher * LoginAndRegister::getDispatcher() {
	return dispatcher;
}

void LoginAndRegister::saveToFile() {
	Global::SaveMap(&userPassMap);
}

LoginAndRegister::~LoginAndRegister() {
}

}
