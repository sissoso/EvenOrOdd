/*
 * PeerUser.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include "PeerName.h"

namespace networkingLab {
PeerName::PeerName(string userName, TCPSocket* peerSock) {
	this->userName = userName;
	this->peerSock = peerSock;
	this->status = PEER_FREE;
	this->score = 0;
	this->shouldAddScore = true;
}

TCPSocket* PeerName::getPeer() {
	return this->peerSock;
}

string PeerName::getUserName() {
	return this->userName;
}

int PeerName::getScore() {
	return this->score;
}

void PeerName::addScore(int n) {
	if (shouldAddScore)
		this->score += n;
	shouldAddScore = !shouldAddScore;
}

PeerName::~PeerName() {
}

}
