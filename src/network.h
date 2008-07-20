#ifndef NETWORK_H
#define NETWORK_H

class ezSockets;

class GameNetwork {
	protected:
		bool is_server;
		ezSockets* socket;
		int port;
		const char* host;

		int SendHandShake();
		int WaitForHandShake();
		int CommonInit(int _port, CString _host);

	public:
		int InitClient(int _port, CString _host);
		int InitServer(int _port);

		void Shutdown();

		~GameNetwork();
		GameNetwork();
};

#endif // NETWORK_H
