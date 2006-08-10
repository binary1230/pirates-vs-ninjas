#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "server.h"
#include "client.h"

#define STR_IS_EQUAL 0

#define SERVER_PORT 7777

void do_server() {
	UDPServer serv;
	serv.Init(SERVER_PORT);
	serv.ServerLoop();
}

void do_client(char* ip) {

	if (!ip)
		ip = "127.0.0.1";

	UDPClient client;
	client.Init("127.0.0.1", SERVER_PORT);
	client.SendMsg("CRAP1!");
	client.SendMsg("CRAP2!");
	client.SendMsg("CRAP3!");
}

int main(int argc, char* argv[]) {
	printf("starting!\n");

	if (strcmp(basename(argv[0]), "server") == STR_IS_EQUAL) {
		do_server();		
	} else if (strcmp(basename(argv[0]), "client") == STR_IS_EQUAL) {
		// sleep(1);

		if (argc == 2)
			do_client(argv[1]);
		else
			do_client(NULL);

	} else {
		printf("unknown name!\n");
		return -1;
	}

	return 0;
}
