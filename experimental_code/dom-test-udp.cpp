#include "ezSockets.h" 
#include <iostream>

#define STR_IS_EQUAL 0

using namespace std;


void * ReadThread( void * pData )
{
	ezSockets * pSock = (ezSockets*)pData;
	ezSocketsPacket server2client;
	cout<<"Thread Created."<<endl;
	while ( 1 )
	{
		if ( pSock->ReadPack( server2client ) )
		{
			cout<<"FROM:"<<server2client.PositionTAG<<endl;
			int pActualSize = server2client.Read4();
			if ( pActualSize != server2client.Size-4 )
				cout<<"WARN: Merged packets!"<<endl;
			cout<<server2client.Read2()<<endl;
			cout<<server2client.Read4()<<endl;
			cout<<server2client.ReadNT().c_str()<<endl;
		}
	}
	return NULL;
}

void do_client() {
	ezSockets client;
	client.mode = ezSockets::skUDP;

	client.Create( IPPROTO_UDP, SOCK_DGRAM );
	client.Bind( 1234 );

	ezSocketsPacket server2client;
	while ( 1 )
	{
		if ( client.ReadPack( server2client ) )
		{
			cout<<"FROM:"<<server2client.PositionTAG<<endl;
			int pActualSize = server2client.Read4();
			if ( pActualSize != server2client.Size-4 )
				cout<<"WARN: Merged packets!"<<endl;
			cout<<server2client.Read2()<<endl;
			cout<<server2client.Read4()<<endl;
			cout<<server2client.ReadNT()<<endl;
		}
	}
}

void do_server() {
	ezSockets server;
	ezSocketsPacket client2server;
	
	server.mode = ezSockets::skUDP;
	server.Create( IPPROTO_UDP, SOCK_DGRAM );
	cout<<"Connect:"<<server.Connect( "192.168.1.40", 1234 )<<endl;
	
	client2server.Write2(1010);
	client2server.Write4( 100234555 );
	client2server.WriteNT( "HELLO WORLD!" );
	server.SendPack( client2server );
}

int main(int argc, char* argv[])
{ 
	if (strcmp(basename(argv[0]), "server") == STR_IS_EQUAL) {
		printf("starting server!\n");
		do_server();		
	} else if (strcmp(basename(argv[0]), "client") == STR_IS_EQUAL) {
	printf("starting client!\n");
		do_client();
	} else {
		printf("unknown name!\n");
		return -1;
	}

	/*ezSocketsPacket client2server;

	char buff[1024];


	g.Create( ReadThread, (void*)&client );

	WaitForKey();
	while(1)

	WaitForKey();
	server.SendPack( client2server );

	WaitForKey();
*/
	return 0;
}
