#include "client.h"

int UDPClient::Init(char* server_ip, int server_port) {
	printf(".. starting client!\n");

	received = 0;

  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    Die("Failed to create socket");
  }

  memset(&echoserver, 0, sizeof(echoserver));
  echoserver.sin_family = AF_INET;        

  echoserver.sin_addr.s_addr = inet_addr(server_ip);
  echoserver.sin_port = htons(server_port);      

	return 0;
}

void UDPClient::Shutdown() {

}

void UDPClient::SendMsg(char* msg) {
  echolen = strlen(msg);
  if (sendto(sock, msg, echolen, 0,
   (struct sockaddr *) &echoserver,
   sizeof(echoserver)) != int(echolen)) {
    Die("Mismatch in number of sent bytes");
  }
}
