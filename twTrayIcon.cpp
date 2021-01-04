#define _WIN32_IE 0x0600

#define NOTIFYICONDATA_V3_SIZE 504

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <Windowsx.h>

#include "twTrayIcon.h"

extern HINSTANCE instance;

using namespace std;

twTrayIcon::twTrayIcon(twObject *parent, twString text)
{
	systemTrayIcon = (NOTIFYICONDATA*)malloc(sizeof(NOTIFYICONDATA));
	systemTrayIcon->cbSize = NOTIFYICONDATA_V3_SIZE;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        systemTrayIcon->hWnd = window->getWindow();
        setType(tw::SystemTrayIcon);
        setParent(parent);
        systemTrayIcon->uCallbackMessage = WM_USER + 1;
        systemTrayIcon->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
    }
    else
        systemTrayIcon->uFlags = NIF_ICON | NIF_TIP;
    strcpy(systemTrayIcon->szInfo, "");
    strcpy(systemTrayIcon->szTip, "");
    //systemTrayIcon->uVersion = NOTIFYICON_VERSION_4;
    m_text = "";
    m_message = "";

    if (!text.empty())
        setText(text);
    systemTrayIcon->uID = 0;
}

twTrayIcon::~twTrayIcon()
{
    setVisible(false);
}

void twTrayIcon::setIcon(twImage *image)
{
    systemTrayIcon->hIcon = image->toIcon();
    Shell_NotifyIcon(NIM_MODIFY, systemTrayIcon);
}

void twTrayIcon::setMessage(twString message, twString title, int timeout)
{
	if (timeout != 0)
		systemTrayIcon->uTimeout = timeout;
	strcpy(systemTrayIcon->szInfo, message.c_str());
    strcpy(systemTrayIcon->szTip, m_text.c_str());
    strcpy(systemTrayIcon->szInfoTitle, title.c_str());
    systemTrayIcon->dwInfoFlags = NIIF_NONE;
    m_message = message;
    m_title = title;
    m_timeOut = timeout;
    Shell_NotifyIcon(NIM_MODIFY, systemTrayIcon);
}

void twTrayIcon::setText(twString text)
{
    m_text = text;
    strcpy(systemTrayIcon->szTip, text.c_str());
    strcpy(systemTrayIcon->szInfo, m_message.c_str());
    strcpy(systemTrayIcon->szInfoTitle, m_title.c_str());
	if (m_timeOut != 0)
		systemTrayIcon->uTimeout = m_timeOut;
    systemTrayIcon->dwInfoFlags = NIIF_NONE;
    Shell_NotifyIcon(NIM_MODIFY, systemTrayIcon);
}

twString twTrayIcon::getText()
{
    return m_text;
}

twString twTrayIcon::getMessage()
{
    return m_message;
}

void twTrayIcon::setVisible(bool visible)
{
    if (visible)
        Shell_NotifyIcon(NIM_ADD, systemTrayIcon);
    else
        Shell_NotifyIcon(NIM_DELETE, systemTrayIcon);
}
