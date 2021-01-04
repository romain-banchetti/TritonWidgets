#ifndef TWTCPSERVER_H_INCLUDED
#define TWTCPSERVER_H_INCLUDED

#include "twNetwork.h"

#include "twTCPClient.h"

class twTCPServer : public twObject
{
    public:
        enum Event{NewConnection};
        twTCPServer();
        virtual ~twTCPServer();
        void close();
        bool listen(int port);
        bool accept(twTCPClient *client, twString *address);
        void update();

    private:
		bool getNewConnection();
		SOCKET m_connection;
		twString m_address;
        SOCKET m_socket;
        void setBlocking(bool blocking);
};

#endif // TWTCPSERVER_H_INCLUDED
