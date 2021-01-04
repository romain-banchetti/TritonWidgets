#ifndef TWAPPLICATION_H_INCLUDED
#define TWAPPLICATION_H_INCLUDED

#define SLOT(slot) (bool (twObject::*)(void)) &slot

#include <iostream>
#include <vector>

#ifdef WIN32

#include <windows.h>

#include "GLEW/glew.h"

#include "twSize.h"
#include "twPosition.h"
#include "twColor.h"
#include "twArea.h"
#include "twButton.h"
#include "twWindow.h"
#include "twDialog.h"
#include "twText.h"
#include "twSlider.h"
#include "twTimer.h"
#include "twTextEdit.h"
#include "twRichEdit.h"
#include "twMenu.h"
#include "twToolBar.h"
#include "twTabsBox.h"
#include "twImage.h"
#include "twListView.h"
#include "twSprite.h"
#include "twShape.h"
#include "twSound.h"
#include "twCheckBox.h"
#include "twListBox.h"
#include "twProgressBar.h"
#include "twStatusBar.h"
#include "twTrayIcon.h"
#include "twGraphicsArea.h"
#include "twCalendar.h"
#include "twFTP.h"
#include "twMysql.h"
#include "twFileInfo.h"
#include "twDir.h"

#endif

#include "twString.h"
#include "twObject.h"
#include "twNetwork.h"
#include "twTCPClient.h"
#include "twUDPSocket.h"
#include "twTCPServer.h"
#include "twHTTP.h"


class twApplication : public twObject
{
    public:
        enum event{Update};
        twApplication(int argc = 0, char *argv[] = 0);
        int show();
        void setSleepMode(bool sleepMode);
        bool getSleepMode();
        static twString getArg(int index = 1);
        static void quit();
        static void sleep(int ms);
        static void setArg(int argc, char *argv[]);
        static int getElapsedTime();
        void update();
        int addTCPClient(twObject *client);
        int addTCPServer(twObject *server);
        int addUDPSocket(twObject *socket);
        void removeTCPClient(twObject *client);
        void removeTCPServer(twObject *server);
        void removeUDPSocket(twObject *socket);
        #ifdef WIN32
        static twString getComputerName();
        static twString getUserName();
        static twString getSystemDirectory();
        static twString getWindowsDirectory();
        static twString getCurrentDirectory();
        static void open(twString file);
        static twSize getScreenSize();
        static twSize getWorkArea();
        void setNetworkUpdateDelay(int ms);
        int getNetworkUpdateDelay();
        static twPosition getCursorPosition();
        #endif

    private:
        std::vector<twObject*> m_TCPClient;
        std::vector<twObject*> m_UDPSocket;
        std::vector<twObject*> m_TCPServer;
        #ifdef WIN32
        twTimer *networkTimer;
        MSG message;
        #endif
        int modeDAffichage;
        bool m_sleepMode;
        int msNetworkTimer;
};

int twRand(int min, int max = 0);

#endif // TWAPPLICATION_H_INCLUDED
