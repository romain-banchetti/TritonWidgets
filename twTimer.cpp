#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <commctrl.h>

#include "twWindow.h"
#include "twTimer.h"

twTimer::twTimer(twObject *parent, int interval)
{
    twWindow *window = static_cast<twWindow*>(parent);
    setType(tw::Timer);
    setParent(parent);
    m_timer = SetTimer(window->getWindow(), 0, interval, (TIMERPROC)NULL);
    window->addChild(tw::Timer, this);
}

twTimer::~twTimer()
{
	twWindow *window = static_cast<twWindow*>(m_parent);
	KillTimer(window->getWindow(), m_timer);
	window->removeChild(this);
}
