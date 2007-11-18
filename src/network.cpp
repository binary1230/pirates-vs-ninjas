#include "stdafx.h"
#include "network.h"
#include "ezSockets.h"

#define PVN_NETWORK_MAGIC_GREETING 123454321

int GameNetwork::SendHandShake() {
	ezSocketsPacket packet;
 	packet.Write4(PVN_NETWORK_MAGIC_GREETING);
	socket->SendPack(packet);
	TRACE("NET: Sent initial handshake greeting.\n");
	return 0;
}

int GameNetwork::WaitForHandShake() {
	ezSocketsPacket packet;
	bool got_greeting = false;

	TRACE("NET: Waiting for handshake response...\n");

	packet.ClearPacket();

	while (!got_greeting) {
		if (socket->ReadPack(packet)) {
			int size = packet.Read4();
      if (size != packet.Size-4)
        TRACE("NET: WARN: Merged packets!\n");

			if (packet.Read4() != PVN_NETWORK_MAGIC_GREETING) {
				TRACE("Incorrect MAGIC recieved, aborting!\n");
				return -1;	
			} else {
				got_greeting = true;
			}
    } 
	}
		
	TRACE("NET: Got response! Handshake Complete!\n");

	return 0;
}

int GameNetwork::CommonInit(int _port, CString _host) {
	host = _host.c_str();
	port = _port;

	socket = new ezSockets();
	if (!socket) {
		Shutdown();
		TRACE("NET: ERROR: Can't allocate memory for socket!.\n");
		return -1;
	}
  socket->mode = ezSockets::skUDP;

	return 0;
}

int GameNetwork::InitClient(int _port, CString _host) {
	Shutdown();

	TRACE("NET: Starting UDP network client:\n");

	if (CommonInit(_port, _host) == -1)
		return -1;
  
	TRACE("NET: Trying to connect to: %s:%i\n", host, port);

	if (!socket->Create( IPPROTO_UDP, SOCK_DGRAM )) {
		TRACE("NET: ERROR Can't create socket.\n");
		Shutdown();
		return -1;
	}
  
  if (!socket->Connect(host,port)) {
		TRACE("NET: ERROR Can't connect to server.\n");
		Shutdown();
		return -1;
	} else {
		TRACE("NET: Connected to server.\n");
	}

	SendHandShake();
	WaitForHandShake();
	
	TRACE("NET: Client: Server connection succeeded!\n");

	return 0;
}

int GameNetwork::InitServer(int _port) {
	Shutdown();

	is_server = true;

	if (CommonInit(_port, "") == -1)
		return -1;
	
	TRACE("NET: Starting UDP network server on port %i\n", port);
  
	socket->mode = ezSockets::skUDP;
  if (!socket->Create(IPPROTO_UDP, SOCK_DGRAM)) {
		TRACE("NET: ERROR: Can't create server socket!\n");
		return -1;
	}

  if (!socket->Bind(port)) {
		TRACE("NET: ERROR: Can't bind() server socket!\n");
		return -1;
	}

	WaitForHandShake();
	rest(1000);
	SendHandShake();
	
	TRACE("NET: Server: Client connection succeeded!\n");

	return 0;
}

void GameNetwork::Shutdown() {
	if (socket) {
		socket->Close();	
		delete socket;
	}

	socket = NULL;
	port = -1;
	host = NULL;
	is_server = false;
}

GameNetwork::~GameNetwork() {
	Shutdown();
}

GameNetwork::GameNetwork() {
	socket=NULL;
	host=NULL;
	Shutdown();
}
