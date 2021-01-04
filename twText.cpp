#include <windows.h>
#include <iostream>
#include "twString.h"
#include <vector>
#include <sstream>

#include "twWindow.h"
#include "twText.h"
#include "GLEW/glew.h"

extern HINSTANCE instance;
extern HDC hDC;

using namespace std;

twText::twText(twString text, twWidget *parent, twPosition position, twColor color, twFont *font) : m_position(position), m_font(font), m_color(color)
{
	m_parent = parent;
    if (parent != NULL && color == twColor::white())
        color = twColor::black();
    twPosition m_position = position;
    m_text = text;
    m_relativePosition = twPosition(-1, -1);
    m_relativeSize = twSize(-1, -1);
    m_color = color;
	if (m_parent != NULL)
	{
        setType(tw::Text);
        setParent(parent);
		twWindow *window = static_cast<twWindow*>(m_parent);
		window->redraw();
	}
}

twPosition twText::getRelativePosition()
{
    return m_relativePosition;
}

void twText::setRelativePosition(twPosition position)
{
    m_relativePosition = position;
}

twSize twText::getRelativeSize()
{
    return m_relativeSize;
}

void twText::setRelativeSize(twSize size)
{
    m_relativeSize = size;
}

twSize twText::getSize()
{
    return m_size;
}

void twText::setSize(twSize size)
{
    m_size = size;
}

twSize twText::getTextSize()
{
    SIZE size;
    SelectObject(hDC, getFont()->m_font);
    GetTextExtentPoint(hDC, m_text.c_str(), m_text.size(), &size);
    return twSize(size.cx, size.cy);
}

twText::~twText()
{

}

void twText::setPosition(twPosition position)
{
    //redraw();
    m_position = position;

    /*
    if (m_parent)
    {
        twWindow *window = static_cast<twWindow*>(m_parent);
        window->redraw();
    }
    */
}

void twText::setFont(twFont *font)
{
    m_font = font;
    if (m_parent)
    {
        twWindow *window = static_cast<twWindow*>(m_parent);
        window->redraw();
    }
}

void twText::setColor(twColor color)
{
    m_color = color;
    if (m_parent)
    {
        twWindow *window = static_cast<twWindow*>(m_parent);
        window->redraw();
    }
}

twFont* twText::getFont()
{
    return m_font;
}

twColor twText::getColor()
{
    return m_color;
}

twPosition twText::getPosition()
{
    return m_position;
}
twString twText::getText()
{
    return m_text;
}

void twText::redraw()
{

    RECT rect;
    rect.left = getPosition().X;
    rect.top = getPosition().Y;
    rect.right = getPosition().X + getTextSize().width;
    rect.bottom = getPosition().Y + getTextSize().height;
    if (getPosition().X + m_size.width > rect.right)
        rect.right = getPosition().X + m_size.width;
    if (getPosition().Y + m_size.height > rect.bottom)
        rect.bottom = getPosition().Y + m_size.height;

    //RedrawWindow(*m_parent->m_handle, &rect, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_NOFRAME);
    //UpdateWindow(*m_parent->m_handle);
    InvalidateRect(*m_parent->m_handle, &rect, true);

/*
    HRGN region = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

    HDC dc = GetDCEx(*m_parent->m_handle, region, DCX_PARENTCLIP);
    SendMessage(*m_parent->m_handle, WM_ERASEBKGND, (WPARAM)dc, 0);
	ReleaseDC(*m_parent->m_handle, dc);

    RedrawWindow(*m_parent->m_handle, &rect, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
*/


	//UpdateWindow(*m_parent->m_handle);
}

void twText::setText(twString text)
{
	if (m_text == text)
		return;
    twSize size = getTextSize();
    m_text = text;
	if (m_parent != NULL)
		redraw();
}
