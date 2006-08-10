#ifndef SERVER_H
#define SERVER_H

#include "network.h"

class UDPServer {
	protected:
		int sock;
		struct sockaddr_in echoserver;
		struct sockaddr_in echoclient;
		char buffer[BUFFSIZE];
		unsigned int echolen, clientlen, serverlen;
		int received;

	public:
		int Init(int port);
		void ServerLoop();
		void Shutdown();
};

#endif // SERVER_H
