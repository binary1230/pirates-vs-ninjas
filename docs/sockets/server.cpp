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

void recv_data(int sock, char* buf) {
	struct sockaddr* si = (struct sockaddr*)&si_other;
	socklen_t slen = sizeof(si_other);
	if (recvfrom(sock, buf, BUFLEN, 0, si, &slen)==-1)
		diep("recvfrom()");
}

int main(void)
{
  struct sockaddr_in si_me;
	struct sockaddr* p_si_me = (struct sockaddr*)&si_me;
  int sock;
  char buf[BUFLEN];

  if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_me, sizeof(si_me), 0);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, p_si_me, sizeof(si_me))==-1)
      diep("bind");

	while(1) {
		recv_data(sock, buf);

		if (strcmp(buf, "exit") == 0)
			break;

    printf("Received packet from %s:%d\nData: %s\n\n", 
           inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

		sleep(1);
  }

  close(sock);
  return 0;
}
