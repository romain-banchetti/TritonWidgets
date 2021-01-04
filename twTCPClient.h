#ifndef TWTCPCLIENT_H_INCLUDED
#define TWTCPCLIENT_H_INCLUDED

#include "twNetwork.h"

#include "twObject.h"

class twTCPClient : public twObject
{
	friend class twTCPServer;
    public:
        enum Event{NewData, Disconnected};
        twTCPClient();
        virtual ~twTCPClient();
        bool connectToHost(twString host, int port);
        bool send(twString data);
        bool send(char *data, int size);
        bool isConnect();
        twString receive();
        bool getReadyToRead();
        void tryReading();
        void close();
        void update();
        void setSocket(SOCKET socket);
        void setBlocking(bool blocking);
        bool getBlocking();
        void setProtectedMode(bool value);
        bool getProtectedMode();

    private:
        twString m_data;
        std::vector<twString> m_valideData;
        bool m_connected;
        bool m_blocking;
        SOCKET m_socket;
        bool m_protectedMode;

};

#endif // TWTCPCLIENT_H_INCLUDED
