#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <iostream>
#include <string>
#include <time.h>

#include "twApplication.h"

using namespace std;

#ifdef WIN32
HINSTANCE instance;
twWindow *g_mainWindow = NULL;
int display;
HDC hDC;
bool alInitialised = false;
bool g_graphicsMode = false;
#endif

twApplication *g_application = NULL;
std::vector<twString> m_arg;

twApplication::twApplication(int argc, char *argv[])
{
    #ifdef WIN32
	InitCommonControls();
	networkTimer = NULL;
	msNetworkTimer = 0;
	#endif
    g_application = this;
    if (argc > 0)
		setArg(argc, argv);
	m_sleepMode = true;
	srand(time(NULL));
}

int twApplication::show()
{
    #ifdef WIN32
	if (m_sleepMode)
	{
		while (GetMessage(&message, NULL, 0, 0))
		{
		    if (g_mainWindow != NULL)
		    {
                if (!TranslateAccelerator(g_mainWindow->getWindow(), g_mainWindow->haccel, &message))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
					g_mainWindow->triggerEvent(twWindow::Event);
				}
                if (g_graphicsMode)
                {
                    g_mainWindow->onUpdate();
                    for (unsigned int i=0;i<g_mainWindow->events.size();i++)
                    {
                        if (g_mainWindow->events[i].event == twWindow::Update)
                            (g_mainWindow->events[i].object->*g_mainWindow->events[i].function)();
                    }
                    for (unsigned int i=0;i<g_mainWindow->children->size();i++)
                    {
                        twWidget *child = static_cast<twWidget*>(g_mainWindow->children->at(i).child);
                        if (child->getType() == tw::GraphicsArea)
                        {
                            twGraphicsArea *ga = (twGraphicsArea*)child;
                            SwapBuffers(ga->hDC);
                        }
                        for (unsigned int j=0;j<child->children->size();j++)
                        {
                            if (child->children->at(j).child->getType() == tw::GraphicsArea)
                            {
                                twGraphicsArea *ga = (twGraphicsArea*)child->children->at(j).child;
                                SwapBuffers(ga->hDC);
                            }
                        }
                    }
                }
		    }
		}
	}
	else
	{
		bool quit = false;
		while (!quit)
		{
            update();
            for (unsigned int i=0;i<events.size();i++)
            {
                if (events[i].event == Update)
                    (events[i].object->*events[i].function)();
            }
		    if (g_mainWindow != NULL)
		    {
		    	//cout << "corentin est" << endl;
                if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
                {
                	//cout << "un belge" << endl;
                    if (message.message == WM_QUIT)
                    {
                        quit = TRUE;
                    }
                    else
                    {
                        TranslateAccelerator(g_mainWindow->getWindow(), g_mainWindow->haccel, &message);
                        TranslateMessage(&message);
                        DispatchMessage(&message);
                        for (unsigned int i=0;i<g_mainWindow->events.size();i++)
                        {
                            if (g_mainWindow->events[i].event == twWindow::Event)
                                (g_mainWindow->events[i].object->*g_mainWindow->events[i].function)();
                        }
                    }
                }
                else
                {
                	//cout << "ah ah" << endl;
                    g_mainWindow->onUpdate();
                    for (unsigned int i=0;i<g_mainWindow->events.size();i++)
                    {
                        if (g_mainWindow->events[i].event == twWindow::Update)
                            (g_mainWindow->events[i].object->*g_mainWindow->events[i].function)();
                    }
                    for (unsigned int i=0;i<g_mainWindow->children->size();i++)
                    {
                        twWidget *child = static_cast<twWidget*>(g_mainWindow->children->at(i).child);
                        if (child->getType() == tw::GraphicsArea)
                        {
                            twGraphicsArea *ga = (twGraphicsArea*)child;
                            SwapBuffers(ga->hDC);
                        }
                        for (unsigned int j=0;j<child->children->size();j++)
                        {
                            if (child->children->at(j).child->getType() == tw::GraphicsArea)
                            {
                                twGraphicsArea *ga = (twGraphicsArea*)child->children->at(j).child;
                                SwapBuffers(ga->hDC);
                            }
                        }
                    }
                }
		    }
		    /*
		    else
		    {
				while (GetMessage(&message, NULL, 0, 0))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
		    }
		    */
		}
	}
	#endif

	#ifdef LINUX
    bool quit = false;
    while (!quit)
    {
        update();
        for (unsigned int i=0;i<events.size();i++)
        {
            if (events[i].event == Update)
                (events[i].object->*events[i].function)();
        }
    }
	#endif
	return 0;
}

#ifdef WIN32
void twApplication::setNetworkUpdateDelay(int ms)
{
    if (g_mainWindow == NULL)
        return;
    if (networkTimer != NULL)
        delete networkTimer;
    networkTimer = new twTimer(g_mainWindow, ms);
    networkTimer->connect(twTimer::TimeOut, this, SLOT(twApplication::update));
    msNetworkTimer = ms;
}

int twApplication::getNetworkUpdateDelay()
{
    return msNetworkTimer;
}

twString twApplication::getComputerName()
{
    char str[1000];
    DWORD size = 1000;
    GetComputerName(str, &size);
    return str;
}
twString twApplication::getUserName()
{
    char str[1000];
    DWORD size = 1000;
    GetUserName(str, &size);
    return str;
}
twString twApplication::getSystemDirectory()
{
    char str[1000];
    GetSystemDirectory(str, 1000);
    return str;
}
twString twApplication::getWindowsDirectory()
{
    char str[1000];
    GetWindowsDirectory(str, 1000);
    return str;
}
twString twApplication::getCurrentDirectory()
{
    char str[1000];
    GetCurrentDirectory(1000, str);
    return str;
}

twPosition twApplication::getCursorPosition()
{
	POINT position;
	GetCursorPos(&position);
	return twPosition(position.x, position.y);
}

void twApplication::open(twString file)
{
	ShellExecute(NULL, "open", file.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

twSize twApplication::getScreenSize()
{
    return twSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

twSize twApplication::getWorkArea()
{
   RECT workArea;
   SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
   return twSize(workArea.right, workArea.bottom);
}

namespace tw
{
    char keyToChar(Key key)
    {
        BYTE keyState[256];
        GetKeyboardState(keyState);
        WORD string = 0;
        if (ToAscii(key, 0, keyState, &string, 0))
            return char(string);
        else
            return '\0';
    }
}
#endif

int twApplication::addTCPClient(twObject *client)
{
	m_TCPClient.push_back(client);
	return m_TCPClient.size()-1;
}

int twApplication::addUDPSocket(twObject *socket)
{
	m_UDPSocket.push_back(socket);
	return m_UDPSocket.size()-1;
}

int twApplication::addTCPServer(twObject *server)
{
	m_TCPServer.push_back(server);
	return m_TCPServer.size()-1;
}

void twApplication::removeTCPClient(twObject *client)
{
	for (unsigned int i=0;i<m_TCPClient.size();i++)
	{
		if (m_TCPClient[i] == client)
			m_TCPClient.erase(m_TCPClient.begin()+i);
	}
}

void twApplication::removeTCPServer(twObject *server)
{
	for (unsigned int i=0;i<m_TCPServer.size();i++)
	{
		if (m_TCPServer[i] == server)
			m_TCPServer.erase(m_TCPServer.begin()+i);
	}
}

void twApplication::removeUDPSocket(twObject *socket)
{
	for (unsigned int i=0;i<m_UDPSocket.size();i++)
	{
		if (m_UDPSocket[i] == socket)
		m_UDPSocket.erase(m_UDPSocket.begin()+i);
	}
}

void twApplication::update()
{
	for (unsigned int i=0;i<m_TCPClient.size();i++)
	{
		twTCPClient *client = static_cast<twTCPClient*>(m_TCPClient[i]);
		client->update();
	}
	for (unsigned int i=0;i<m_TCPServer.size();i++)
	{
		twTCPServer *server = static_cast<twTCPServer*>(m_TCPServer[i]);
		server->update();
	}
	for (unsigned int i=0;i<m_UDPSocket.size();i++)
	{
		twUDPSocket *socket = static_cast<twUDPSocket*>(m_UDPSocket[i]);
		socket->update();
	}
}

void twApplication::setSleepMode(bool sleepMode)
{
	m_sleepMode = sleepMode;
}

bool twApplication::getSleepMode()
{
    return m_sleepMode;
}

twString twApplication::getArg(int index)
{
	if (index < (int)m_arg.size())
		return m_arg[index];
	return "";
}

void twApplication::quit()
{
    #ifdef WIN32
    PostQuitMessage(0);
    #endif

    #ifdef LINUX
    exit(0);
    #endif
}

void twApplication::sleep(int ms)
{
    #ifdef WIN32
    Sleep(ms);
    #endif

    #ifdef LINUX
    usleep(ms);
    #endif
}

int twApplication::getElapsedTime()
{
	return clock();
}

void twApplication::setArg(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
		m_arg.push_back(argv[i]);
}

int twRand(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

