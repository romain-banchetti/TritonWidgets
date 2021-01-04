#include <windows.h>
#include <iostream>
#include "twString.h"
#include <vector>
#include <sstream>
#include <commctrl.h>

#include "twTabsBox.h"

extern HINSTANCE instance;

extern twWindow *g_mainWindow;

using namespace std;

twTabsBox::twTabsBox(twObject *parent, twPosition position, twSize size) : twWidget(&m_tabsBox)
{
    m_tabsBox = CreateWindow(WC_TABCONTROL, "", WS_CHILD|WS_VISIBLE, position.X, position.Y, size.width, size.height, *((twWidget*)parent)->m_handle, NULL, instance, NULL);
    LOGFONT structFont;
    HFONT font;
    ZeroMemory(&structFont, sizeof(LOGFONT));
    strcpy(structFont.lfFaceName, "Arial");
    structFont.lfHeight = 16;
    font = CreateFontIndirect(&structFont);
    SendMessage(m_tabsBox, WM_SETFONT, (WPARAM)font, 0);
    InitCommonControls();
    tie.mask = TCIF_TEXT;
    setType(tw::TabsBox);
    setParent(parent);
    SetWindowLong(m_tabsBox, GWL_USERDATA, (long)this);
}

twTabsBox::~twTabsBox()
{
	/*
    for (unsigned int i = 0; i < m_tabs.size(); i++)
        delete m_tabs[i];
	*/
}

int twTabsBox::addWidget(twWidget *widget, twString title)
{
    if (m_tabs.size() != 0 && widget != NULL)
        widget->setVisible(false);
    m_tabs.push_back(widget);
    tie.pszText = (char*)title.c_str();
    TabCtrl_InsertItem(m_tabsBox, m_tabs.size(), &tie);
    g_mainWindow->recalculateSize();
    recalculateSize();
    return m_tabs.size()-1;
}

int twTabsBox::getCurrentIndex()
{
    int index = TabCtrl_GetCurSel(m_tabsBox);
    return index;
}

void twTabsBox::setCurrentIndex(int index)
{
    if (TabCtrl_GetCurSel(m_tabsBox) == index)
        return;
    TabCtrl_SetCurSel(m_tabsBox, index);
    HWND parent = GetParent(m_tabsBox);
    NMHDR nmh;
    nmh.code = TCN_SELCHANGE;
    nmh.idFrom = GetDlgCtrlID(m_tabsBox);
    nmh.hwndFrom = m_tabsBox;
    SendMessage(parent, WM_NOTIFY, (WPARAM)m_tabsBox, (LPARAM)&nmh);
    WINDOWPOS winpos;
    SendMessage(parent, WM_WINDOWPOSCHANGED, 0, (LPARAM)&winpos);
    recalculateSize();
}

void twTabsBox::removeTab(int index)
{
    TabCtrl_DeleteItem(m_tabsBox, index);
    //delete m_tabs[index];
    m_tabs[index]->setVisible(false);
    m_tabs.erase(m_tabs.begin()+index);
    if (getCount() > 0)
        setCurrentIndex(getCount()-1);
    recalculateSize();
}

int twTabsBox::getCount()
{
    int count = TabCtrl_GetItemCount(m_tabsBox);
    return count;
}

twWidget *twTabsBox::getWidget(int index)
{
    twWidget *widget = m_tabs[index];
    return widget;
}

void twTabsBox::setText(int index, twString text)
{
    TCITEM item;
    TabCtrl_GetItem(m_tabsBox, index, &item);
    item.pszText = (char*)text.c_str();
    TabCtrl_SetItem(m_tabsBox, index, &item);
}

void twTabsBox::recalculateSize()
{
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = getSize().width;
    rect.bottom = getSize().height;
    TabCtrl_AdjustRect(m_tabsBox, false, &rect);
    twPosition position(getCurrentPosition().X + rect.left-2, rect.top-1);
    twSize size(rect.right-rect.left+5, rect.bottom-rect.top+5);
    for (unsigned int j = 0; j < m_tabs.size(); j++)
    {
        if (m_tabs[j] == NULL)
            continue;
        m_tabs[j]->setPosition(position, false);
        m_tabs[j]->setSize(size);
        if (m_tabs[j]->getType() == tw::Area)
            g_mainWindow->recalculateSize(m_tabs[j]);
    }
    g_mainWindow->redraw();
}

twString twTabsBox::getText(int index)
{
    TCITEM item;
    item.mask = TCIF_TEXT;
    char buffer[128];
    item.pszText = buffer;
    item.cchTextMax = 127;
    TabCtrl_GetItem(m_tabsBox, index, &item);
    return item.pszText;
}
