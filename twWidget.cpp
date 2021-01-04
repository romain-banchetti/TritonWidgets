#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "twSize.h"
#include "twPosition.h"
#include "twWindow.h"
#include "twWidget.h"

extern HINSTANCE instance;

twWidget::twWidget(HWND *handle, twPosition position)
{
    m_handle = handle;
    m_styles = 0;
    m_enable = true;
    m_position = position;
    m_relativePosition = twPosition(-1, -1);
    m_relativeSize = twSize(-1, -1);
    m_border = false;
    m_visible = true;
}

bool twWidget::getFocus()
{
    if (GetFocus() == *m_handle)
        return true;
    return false;
}

twWidget::~twWidget()
{
    DestroyWindow(*m_handle);
}

void twWidget::setToolTip(twString text)
{
	HWND tip = CreateWindow(TOOLTIPS_CLASS, NULL, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, GetParent(*m_handle), NULL, instance, NULL);
	RECT rect;
	TOOLINFO ti;
	GetClientRect (GetParent(*m_handle), &rect);
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = *m_handle;
	ti.lpszText = (char*)text.c_str();
	ti.rect.left = rect.left;
	ti.rect.top = rect.top;
	ti.rect.right = rect.right;
	ti.rect.bottom = rect.bottom;
	SendMessage(tip, TTM_ADDTOOL, 0, (LPARAM) &ti);
}

void twWidget::setPosition(twPosition point, bool redray)
{
    m_position = point;
    if (getType() == tw::Area)
        return;
	if (redray)
		SetWindowPos(*m_handle, NULL, point.X, point.Y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	else
		SetWindowPos(*m_handle, NULL, point.X, point.Y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
}

void twWidget::redraw()
{
	SetWindowPos(*m_handle, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE);
}

void twWidget::setRelativePosition(twPosition relativePosition)
{
	m_relativePosition = relativePosition;
}

void twWidget::setSize(twSize size, bool redray)
{
    m_size = size;
    if (getType() == tw::Area)
        return;
    if (getType() == tw::GraphicsArea)
    {
        twGraphicsArea *widget = static_cast<twGraphicsArea*>(this);
        widget->setSize(size, redray);
        return;
    }
	if (redray)
		SetWindowPos(*m_handle, NULL, 0, 0, size.width, size.height, SWP_NOZORDER|SWP_NOMOVE);
	else
		SetWindowPos(*m_handle, NULL, 0, 0, size.width, size.height, SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);
}

void twWidget::setRelativeSize(twSize relativeSize)
{
	m_relativeSize = relativeSize;
}

void twWidget::setEnable(bool enable)
{
    /*
    if (m_styles == 0)
        m_styles = GetWindowLong(*m_handle, GWL_STYLE);
    if (enable)
        SetWindowLong(*m_handle, GWL_STYLE, m_styles);
    else
        SetWindowLong(*m_handle, GWL_STYLE, m_styles|WS_DISABLED);
    m_enable = enable;
    */
    EnableWindow(*m_handle, enable);
}

void twWidget::setVisible(bool visible)
{
    m_visible = visible;
    if (getType() == tw::Area)
    {
        for (unsigned int i = 0; i < children->size(); i++)
        {
            twWidget *widget = (twWidget*)children->at(i).child;
            widget->setVisible(visible);
        }
        return;
    }
    if (visible)
        ShowWindow(*m_handle, SW_SHOW);
    else
        ShowWindow(*m_handle, SW_HIDE);
    UpdateWindow(*m_handle);
}

bool twWidget::getVisible()
{
    return m_visible;
}

void twWidget::setBorder(bool border)
{
    if (!m_border)
        m_windowStyle = GetWindowLong(*m_handle, GWL_STYLE);
    if (border)
        m_border = WS_BORDER;
    else
        m_border = false;
    if (border)
        SetWindowLong(*m_handle, GWL_STYLE, m_windowStyle|m_border);
    else
        SetWindowLong(*m_handle, GWL_STYLE, m_windowStyle);
}

bool twWidget::getBorder()
{
    return m_border;
}

bool twWidget::getEnable()
{
    return IsWindowEnabled(*m_handle);
}

void twWidget::focus()
{
    SetFocus(*m_handle);
}

twPosition twWidget::getPosition()
{
    if (getType() == tw::Area)
        return m_position;
    RECT rect;
    GetWindowRect(*m_handle, &rect);
    HWND parent = GetParent(*m_handle);
    //if (parent == NULL)
        return twPosition(rect.left, rect.top);

	/*
    RECT rect2;
    GetWindowRect(parent, &rect2);
    return twPosition(rect.left-rect2.left, rect.top-rect2.top);
    */
    //return twPosition();
}

twSize twWidget::getSize()
{
    if (getType() == tw::Area)
        return m_size;
    RECT rect;
    GetWindowRect(*m_handle, &rect);
    return twSize(rect.right-rect.left, rect.bottom-rect.top);
}

twSize twWidget::getCurrentSize()
{
    return m_size;
}

twPosition twWidget::getCurrentPosition()
{
    return m_position;
}

twSize twWidget::getRelativeSize()
{
	return m_relativeSize;
}

twRect twWidget::getInteriorRect()
{
    RECT rect;
    GetClientRect(*m_handle, &rect);
    return twRect(rect.left, rect.top, rect.right, rect.bottom);
}

twPosition twWidget::getRelativePosition()
{
	return m_relativePosition;
}

twSize twWidget::getInteriorSize()
{
    if (getType() == tw::Area)
        return m_size;
    RECT rect;
    GetClientRect(*m_handle, &rect);
    return twSize(rect.right, rect.bottom);
}

twPosition twWidget::getInteriorPosition()
{
    if (getType() == tw::Area)
        return m_position;
    RECT rect;
    GetClientRect(*m_handle, &rect);
    return twPosition(rect.left, rect.top);
}
