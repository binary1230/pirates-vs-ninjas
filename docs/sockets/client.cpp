// Based on code by Gunnar Gunnarsson [http://www.abc.se/~m6695/udp.html]
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void diep(char *s)
{
  perror(s);
  exit(1);
}

struct sockaddr_in si_other;

void send_data(int sock, const char* buf) {
	int slen = sizeof(si_other);

  if (sendto(sock, buf, strlen(buf) + 1, 0, (const struct sockaddr*)&si_other, slen)==-1)
		diep("sendto()");
}

int main(int argc, char* argv[]) {

	const char* server_address;

	if (argc == 2) {
		// printf("usage: ./client server_ip_address\n");
		// return 0;
		server_address = argv[1];
	} else {
		server_address = "127.0.0.1";
	}

  int sock, i;
  char buf[BUFLEN];

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_other, sizeof(si_other), 0);
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(server_address, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  for (i=0; i<NPACK; i++) {
    printf("Sending packet %d\n", i);
    sprintf(buf, "This is packet %d\n", i);
		send_data(sock, buf);
  }
	
	send_data(sock, "exit");

  close(sock);
  return 0;
}
