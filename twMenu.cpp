#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <commctrl.h>

#include "twWindow.h"
#include "twMenu.h"

extern HINSTANCE instance;

using namespace std;
using namespace tw;

twMenu::twMenu(twObject *parent, twString text, twImage *icon)
{
    m_menu = CreatePopupMenu();
    m_text = text;
    setType(tw::Menu);
    m_enable = true;
	m_icon = icon;
	m_parentMenu = NULL;
    if (parent->getType() == tw::Menu)
    {
    	setParent(parent->getParent());
    	m_id = getChildId();
    	((twMenu*)parent)->addMenu(this);
    }
    else
    	setParent(parent);
	m_id = getChildId();
}

twMenu::~twMenu()
{
	DestroyMenu(m_menu);
}

void twMenu::setIcon(twImage *icon)
{
	m_icon = icon;
}

twImage* twMenu::getIcon()
{
	return m_icon;
}

void twMenu::display(twPosition position, bool byWindow)
{
	if (getParent()->getType() != tw::Window)
		return;
	twWindow *window = (twWindow*)getParent();
	POINT pt;
	pt.x = position.X;
	pt.y = position.Y;
	if (byWindow)
		ClientToScreen(*window->m_handle, &pt);
	TrackPopupMenu(m_menu, 0, pt.x, pt.y, 0, *window->m_handle, 0);
}

void twMenu::addMenu(twMenu *menu)
{
	AppendMenu(m_menu, MF_STRING, menu->getId(), menu->getText().c_str());
	if (menu->getIcon() != NULL)
		SetMenuItemBitmaps(m_menu, menu->getId(), MF_BYCOMMAND, menu->getIcon()->toBitmap(), menu->getIcon()->toBitmap());
    menu->m_parentMenu = m_menu;
    if (menu->m_enable == false)
        EnableMenuItem(m_menu, menu->m_id, MF_GRAYED);
}

void twMenu::addSeparator()
{
    AppendMenu(m_menu, MF_SEPARATOR, (UINT)NULL, "");
}

void twMenu::setEnable(bool enable)
{
    m_enable = enable;
    if (m_parentMenu == NULL)
        return;
    if (enable)
        EnableMenuItem(m_parentMenu, m_id, MF_ENABLED);
    else
        EnableMenuItem(m_parentMenu, m_id, MF_GRAYED);
}

void twMenu::setShortcut(Key key, SpecialKey specialKey)
{
    ACCEL accel;
    BYTE word = 0;
    if (specialKey == Ctrl)
        word = FCONTROL;
    if (specialKey == Alt)
        word = FALT;
    if (specialKey == Shift)
        word = FSHIFT;
    accel.fVirt = word|FVIRTKEY;
    accel.key = key;
    accel.cmd = m_id;
    twWindow *window = static_cast<twWindow*>(getParent());
    window->accels.push_back(accel);
    ACCEL accels[window->accels.size()];
    for (unsigned int i=0;i<window->accels.size();i++)
    {
        accels[i] = window->accels[i];
    }
    window->haccel = CreateAcceleratorTable(accels, window->accels.size());
    string strWord;
    if (specialKey == Ctrl)
        strWord = "Ctrl+";
    if (specialKey == Alt)
        strWord = "Alt+";
    if (specialKey == Shift)
        strWord = "Shift+";
    string strKey;
    if (key == Key_F1)
        strKey = "F1";
    else if (key == Key_F2)
        strKey = "F2";
    else if (key == Key_F3)
        strKey = "F3";
    else if (key == Key_F4)
        strKey = "F4";
    else if (key == Key_F5)
        strKey = "F5";
    else if (key == Key_F6)
        strKey = "F6";
    else if (key == Key_F7)
        strKey = "F7";
    else if (key == Key_F8)
        strKey = "F8";
    else if (key == Key_F9)
        strKey = "F9";
    else if (key == Key_F10)
        strKey = "F10";
    else if (key == Key_F11)
        strKey = "F11";
    else if (key == Key_F12)
        strKey = "F12";
    else if (key == Key_Space)
        strKey = "Space";
    else if (key == Key_Left)
        strKey = "Left";
    else if (key == Key_Right)
        strKey = "Right";
    else if (key == Key_Up)
        strKey = "Up";
    else if (key == Key_Down)
        strKey = "Down";
    else if (key == Key_Escape)
        strKey = "Escape";
    else if (key == Key_Delete)
        strKey = "Delete";
    else
    {
        setText(getText()+"\t"+strWord+(char)key);
        return;
    }
    setText(getText()+"\t"+strWord+strKey);
}

void twMenu::setText(twString text)
{
	if (getText().find("\t") != twString::npos)
		text += getText().substr(getText().find("\t"));
    m_text = text;
    if (m_parentMenu == NULL)
        return;
    ModifyMenu(m_parentMenu, m_id, MF_STRING, m_id, text.c_str());
}

HMENU twMenu::getMenu()
{
    return m_menu;
}

int twMenu::getId()
{
    return m_id;
}
twString twMenu::getText()
{
    return m_text;
}
