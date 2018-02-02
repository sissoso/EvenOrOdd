#include "UDPSocket.h"
#include <iostream>
#include "UDPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

namespace networkingLab {

UDPSocket::~UDPSocket() {


}


UDPSocket::UDPSocket(int port){

	    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

	    bzero((char *) &s_in, sizeof(s_in));

	    s_in.sin_family = (short)AF_INET;
	    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
	    s_in.sin_port = htons((u_short)port);

	    fsize = sizeof(from);

	    if(bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in))<0){
	        perror ("Error naming channel");
	    }
}

/**
 * 	- reads an incoming message from the UDP socket. the message is
 * 	copied into the given buffer up to the specified length.
 */
int UDPSocket::recv(char* buffer, int length){
	    return recvfrom(socket_fd,buffer,length,0,(struct sockaddr *)&from,&fsize);
}

/**
 * 	- sends the given message as a UDP message
 * 	to the given address specified by IP and port.
 *
 */
int UDPSocket::sendTo(string msg, string ip, int port){
	   struct sockaddr_in toAddr;
	    bzero((char *) &toAddr, sizeof(toAddr));
	    toAddr.sin_family = AF_INET;
	    toAddr.sin_addr.s_addr = inet_addr(ip.data());
	    toAddr.sin_port = htons(port);
	    return sendto(socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&toAddr,sizeof(toAddr));
}
/**
 * 	- reply to an incoming message,
 * 	this method will send the given message as a UDP
 * 	message to the peer from which the last message was received.
 *
 */
int UDPSocket::reply(string msg){
	return sendto(socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&from,sizeof(from));
}
/**
 * - close the UDP socket.
 */
void UDPSocket::close1(){
	// cout<<"closing socket"<<endl;
	    shutdown(socket_fd,SHUT_RDWR);
	    close(socket_fd);
}

/**
 * - return the sender IP of the last received message.
 */
string UDPSocket::fromAddr(){
	return inet_ntoa(from.sin_addr);
}

} /* namespace networkingLab */
