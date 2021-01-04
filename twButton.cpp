#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <uxtheme.h>

#include <Commctrl.h>
#include "twButton.h"

extern HINSTANCE instance;

using namespace std;

twButton::twButton(twWidget *parent, twString text, twPosition position, twSize size) : twWidget(&button, position)
{
    button = CreateWindow("BUTTON", text.c_str(), WS_CHILD | WS_VISIBLE, position.X, position.Y, size.width, size.height, *parent->m_handle, NULL, instance, NULL);
    LOGFONT structFont;
    HFONT font;
    ZeroMemory(&structFont, sizeof(LOGFONT));
    strcpy(structFont.lfFaceName, "Arial");
    structFont.lfHeight = 15;
    font = CreateFontIndirect(&structFont);
    SendMessage(button, WM_SETFONT, (WPARAM)font, 0);
    setType(tw::Button);
    setParent(parent);
    SetWindowLong(button, GWL_USERDATA, (long)this);
}

twColor twButton::getBackgroundColor()
{
	return m_backgroundColor;
}

twColor twButton::getTextColor()
{
	return m_textColor;
}

void twButton::setBackgroundColor(twColor backgroundColor)
{
	m_backgroundColor = backgroundColor;
}

void twButton::setTextColor(twColor textColor)
{
	m_textColor = textColor;
}

void twButton::setText(twString text)
{
    SetWindowText(button, text.c_str());
}

twString twButton::getText()
{
    char buffer[99];
    GetWindowText(button, buffer, 98);
    return buffer;
}

HWND twButton::getButton()
{
    return button;
}
