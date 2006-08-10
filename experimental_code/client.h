#ifndef CLIENT_H
#define CLIENT_H

#include "network.h"

class UDPClient {
	protected:
		int sock;
		struct sockaddr_in echoserver;
		struct sockaddr_in echoclient;
		char buffer[BUFFSIZE];
		unsigned int echolen, clientlen;
		int received;

	public:
		int Init(char* server_ip, int server_port);
		void SendMsg(char* buf);
		void Shutdown();
};

#endif // CLIENT_h
