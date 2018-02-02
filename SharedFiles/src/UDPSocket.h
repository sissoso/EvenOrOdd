#ifndef SRC_UDPSOCKET_H_
#define SRC_UDPSOCKET_H_
#include <strings.h>
#include <stdlib.h>
#include <iostream>
#include <netinet/in.h>
#include <inttypes.h>
#include <stdio.h>
#include <iostream>

using namespace std;
namespace networkingLab {

class UDPSocket {
	   struct sockaddr_in  s_in;
	    struct sockaddr_in from;
	    unsigned int fsize;
	    int socket_fd;
public:
	 ~UDPSocket();
	/**
	 * 	-  initialize a UDP socket and binds it on
	 * 	the given port. if no port is specified a default
	 * 	port 9999 is set. This UDP socket can be used both
	 * 	for server socket and client socket
	 *
	 */
	UDPSocket(int port = 9999);

	/**
	 * 	- reads an incoming message from the UDP socket. the message is
	 * 	copied into the given buffer up to the specified length.
	 */
	int recv(char* buffer, int length);
	/**
	 * 	- sends the given message as a UDP message
	 * 	to the given address specified by IP and port.
	 *
	 */
	int sendTo(string msg, string ip, int port);
	/**
	 * 	- reply to an incoming message,
	 * 	this method will send the given message as a UDP
	 * 	message to the peer from which the last message was received.
	 *
	 */
	int reply(string msg);
	/**
	 * - close the UDP socket.
	 */
	void close1();

	/**
	 * - return the sender IP of the last received message.
	 */
	string fromAddr();

};

} /* namespace networkingLab */

#endif /* SRC_UDPSOCKET_H_ */
