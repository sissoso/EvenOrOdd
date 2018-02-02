/*
 * Dispatcher.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include "Dispatcher.h"

namespace networkingLab {

Dispatcher::Dispatcher() {
}

PeerName* Dispatcher::FindPeer(TCPSocket* sock) {
	PeerName* ret = NULL;
	pthread_mutex_lock(&peerlistlock);
	vector<PeerName*>::iterator it;
	for (it = peerlist.begin(); it != peerlist.end(); it++) {
		PeerName* usr = *it;

		if (usr->getPeer() == sock) {
			ret = *it;
		}
	}
	pthread_mutex_unlock(&peerlistlock);
	return ret;
}

PeerName* Dispatcher::FindPeer(string usrname) {
	PeerName* ret = NULL;
	pthread_mutex_lock(&peerlistlock);
	vector<PeerName*>::iterator it;
	for (it = peerlist.begin(); it != peerlist.end(); it++) {
		PeerName* usr = *it;

		if (usr->getUserName() == usrname) {
			ret = *it;
		}
	}
	pthread_mutex_unlock(&peerlistlock);
	return ret;
}

Dispatcher::~Dispatcher() {
}

void Dispatcher::add(PeerName* thisPeer) {
	pthread_mutex_lock(&peerlistlock);
	this->peerlist.push_back(thisPeer);
	pthread_mutex_unlock(&peerlistlock);
	//the thread start just when the first user logged in
	if (peerlist.size() == 1)
		start();
}

map<string, string> Dispatcher::getScoreboard() {
	map<string, string> scoreboard;
	pthread_mutex_lock(&peerlistlock);
	for (vector<PeerName*>::iterator it = peerlist.begin();
			it != peerlist.end(); ++it) {
		PeerName* usr = *it;
		string userName = usr->getUserName();
		string score = Global::convertTostring(usr->getScore());
		scoreboard.insert(pair<string, string>(userName, score));
	}
	pthread_mutex_unlock(&peerlistlock);
	return scoreboard;
}
//search free user
string Dispatcher::pickRandomFreePlayer(string exclude) {
	string chosen = "";
	pthread_mutex_lock(&peerlistlock);
	vector<PeerName*> available;
	for (vector<PeerName*>::iterator it = peerlist.begin();
			it != peerlist.end(); ++it) {
		PeerName* usr = *it;
		if (usr->status == PEER_FREE && usr->getUserName() != exclude)
			available.push_back(usr);
	}
	chosen =
			available.empty() ?
					"" : available[rand() % available.size()]->getUserName();
	pthread_mutex_unlock(&peerlistlock);
	return chosen;

}

void Dispatcher::run() {
	while (peerlist.size() > 0) {
		MultipleTCPSocketListener listener;
		//add all the sockets of the users
		listener.addSockets(getPeersVector());
		TCPSocket * peer = listener.listen(1);
		if (peer != NULL) {
			PeerName* peerName = FindPeer(peer);
			Command command = NONE;
			map<string, string> data;
			Protocol::readMSG(peer, command, data);

			switch (command) {
			case SEND_GAME_REQUEST:
				Global::printLog("got SEND_GAME_REQUEST command");
				Global::printLog(
						"Sending a game request from " + peerName->getUserName()
								+ " to " + data[USER_NAME_KEY]);
				this->SendGameRequest(peerName, data);
				break;

			case GAME_REQUEST_REFUSED: {
				Global::printLog("got GAME_REQUEST_REFUSED command");
				PeerName* destUser = FindPeer(data.find(USER_NAME_KEY)->second);
				Global::printLog(
						peerName->getUserName() + " refused "
								+ destUser->getUserName() + " to play a game");
				Protocol::SendMSG(destUser->getPeer(), GAME_REQUEST_REFUSED,
						data);
			}
				break;
				//auto start
			case BRING_RANDOM_PLAYER: {
				Global::printLog("got BRING_RANDOM_PLAYER command");
				//return the name of the random available user
				string chosen = this->pickRandomFreePlayer(
						peerName->getUserName());

				if (chosen.empty()) {
					Global::printLog(
							"No players that are available for a game");
					Protocol::SendMSG(peerName->getPeer(),
							NO_PLAYERS_AVAILABLE);
				} else {
					map<string, string> data;
					data[USER_NAME_KEY] = chosen;
					Global::printLog(
							"The random player that was picked is " + chosen);
					Protocol::SendMSG(peerName->getPeer(), BRING_RANDOM_PLAYER,
							data);
				}
			}
				break;

			case GAME_ENDED: {
				Global::printLog("got GAME_ENDED command");
				string winner = data[USER_NAME_KEY];
				PeerName* peer = FindPeer(winner);
				Global::printLog(
						"the winner is " + winner + ". adding him 1 score.");
				//add 1 to the winner
				peer->addScore(1);

				map<string, string> scoreboard = this->getScoreboard();
				Global::printLog(
						"Sending scoreboard to " + peerName->getUserName());
				Protocol::SendMSG(peerName->getPeer(), SCOREBOARD, scoreboard);
				peerName->status = PEER_FREE;
			}
				break;

			case GAME_REQUEST_ACCEPTED: {
				Global::printLog("got GAME_REQUEST_ACCEPTED command");
				PeerName* destUser = FindPeer(data.find(USER_NAME_KEY)->second);

				//details of the peer
				map<string, string> peerGameDetails;
				peerGameDetails[USER_NAME_KEY] = destUser->getUserName();
				peerGameDetails[IP_KEY] = destUser->getPeer()->fromAddr();
				peerGameDetails[PORT_KEY] = Global::convertTostring(
						destUser->getPeer()->getDestPort());
				peerGameDetails[GAME_CATEGORY] = GAME_ODD;
				peerGameDetails[OPPONENT_USER_NAME] = destUser->getUserName();

				//details of the dest peers
				map<string, string> destUserGameDetails;
				destUserGameDetails[USER_NAME_KEY] = peerName->getUserName();
				destUserGameDetails[IP_KEY] = peerName->getPeer()->fromAddr();
				destUserGameDetails[PORT_KEY] = Global::convertTostring(
						peerName->getPeer()->getDestPort());
				destUserGameDetails[GAME_CATEGORY] = GAME_EVEN;
				destUserGameDetails[OPPONENT_USER_NAME] =
						peerName->getUserName();

				Global::printLog("GAME",
						"A game is starting between " + peerName->getUserName()
								+ " and " + destUser->getUserName());
				//change the statuses
				destUser->status = PEER_IN_GAME;
				peerName->status = PEER_IN_GAME;

				Global::printLog("GAME",
						"Sending game details to " + peerName->getUserName());
				Protocol::SendMSG(peerName->getPeer(), GAME_REQUEST_ACCEPTED,
						peerGameDetails);

				Global::printLog("GAME",
						"Sending game details to " + destUser->getUserName());
				Protocol::SendMSG(destUser->getPeer(), GAME_REQUEST_ACCEPTED,
						destUserGameDetails);
			}
				break;

			case LIST_CONNECTED_USERS:
				Global::printLog("got LIST_CONNECTED_USERS command");
				Global::printLog(
						"Sending list of connected users to "
								+ peerName->getUserName());
				Protocol::SendMSG(peer, LIST_CONNECTED_USERS,
						this->getMapOfAllConnectPeers());
				break;

			case DISCONNECT:
				Global::printLog("got DISCONNECT command");
				this->removePeer(peerName);
				break;
			case NONE:
				Global::printLog("ERROR",
						"got NONE command, this should not happen");
				this->removePeer(peerName);
				break;

			default:
				Global::printLog("got UNKOWN command with value = " + command);
				Protocol::SendMSG(peer, COMMAND_NOT_FOUND);

			}
		}
	}
}

void Dispatcher::SendGameRequest(PeerName* peerName, map<string, string> data) {
	PeerName* destUser = FindPeer(data.find(USER_NAME_KEY)->second);
	if (destUser == NULL) {
		Protocol::SendMSG(peerName->getPeer(), NO_SUCH_USER);
	} else if (peerName->getUserName().compare(destUser->getUserName()) == 0) {
		Protocol::SendMSG(peerName->getPeer(),
				YOU_CANT_START_A_GAME_WITH_YOURSELF);
	} else if (destUser->status == PEER_IN_GAME) {
		Protocol::SendMSG(peerName->getPeer(), USER_ALREADY_IN_GAME);
	} else {
		map<string, string> data;
		data.insert(
				pair<string, string>(USER_NAME_KEY, peerName->getUserName()));
		Protocol::SendMSG(destUser->getPeer(), SEND_GAME_REQUEST, data);
	}
}

void Dispatcher::removePeer(PeerName* peerName) {
	vector<PeerName*>::iterator it;
	pthread_mutex_lock(&peerlistlock);
	for (it = peerlist.begin(); it != peerlist.end(); it++) {
		PeerName* usr = *it;
		if (usr->getPeer() == peerName->getPeer())
			break;
	}
	if (it != peerlist.end()) {
		peerlist.erase(it);
	}
	pthread_mutex_unlock(&peerlistlock);
}
//retrun vector of all the peerName(users) sockets
vector<TCPSocket*> Dispatcher::getPeersVector() {

	vector<TCPSocket*> ret;
	pthread_mutex_lock(&peerlistlock);
	for (unsigned int i = 0; i < peerlist.size(); i++) {
		ret.push_back(peerlist[i]->getPeer());
	}
	pthread_mutex_unlock(&peerlistlock);
	return ret;
}

map<string, string> Dispatcher::getMapOfAllConnectPeers() {
	map<string, string> usrList; //index, name
	int i = 0;
	pthread_mutex_lock(&peerlistlock);
	for (vector<PeerName*>::iterator it = peerlist.begin();
			it != peerlist.end(); ++it) {
		PeerName* usr = *it;
		string userName = usr->getUserName();
		string UserIndex = Global::convertTostring(i);
		usrList.insert(pair<string, string>(UserIndex, userName));
		i++;
	}
	pthread_mutex_unlock(&peerlistlock);
	return usrList;

}

}
