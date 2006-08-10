#include "server.h"

int min(int x, int y) {
	if (x < y) return x; else return y;
}

int UDPServer::Init(int port) {
	printf(".. starting server!\n");

	received = 0;

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	  Die("Failed to create socket");
	}

	memset(&echoserver, 0, sizeof(echoserver));   
	echoserver.sin_family = AF_INET;        
	echoserver.sin_addr.s_addr = htonl(INADDR_ANY);  
	echoserver.sin_port = htons(port); 

	serverlen = sizeof(echoserver);
	if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0) {
	  Die("Failed to bind server socket");
	}	

	return 0;
}

void UDPServer::Shutdown() {

}

void UDPServer::ServerLoop() {
  while (1) {
    
		/* Receive a message from the client */
    clientlen = sizeof(echoclient);
    if ((received = recvfrom(sock, buffer, BUFFSIZE, 0,
         (struct sockaddr *) &echoclient,
         &clientlen)) < 0) {
      Die("Failed to receive message");
    }

		buffer[min(received,255)] = 0;

    fprintf(stderr,
  	// "SERVER SAYS: Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
		"CLIENT SENT: '%s'\n", buffer);
    
		/* Send the message back to client */
    /*if (sendto(sock, buffer, received, 0,
     (struct sockaddr *) &echoclient,
     sizeof(echoclient)) != received) {
      Die("Mismatch in number of echo'd bytes");
    }*/
  }
}
