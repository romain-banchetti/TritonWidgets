#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <uxtheme.h>
#include <Commctrl.h>

#include "twToolBar.h"

extern HINSTANCE instance;

twToolBar::twToolBar(twWidget *parent, twPosition position) : twWidget(&toolBar, position)
{
    toolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_TRANSPARENT | CCS_NODIVIDER | CCS_NOPARENTALIGN, position.X, position.Y, 0, 0, *parent->m_handle, NULL, instance, NULL);

    setType(tw::ToolBar);
    setParent(parent);
    SetWindowLong(toolBar, GWL_USERDATA, (long)this);

    m_clickedButton = -1;

	imageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 0);
	SendMessage(toolBar, TB_SETIMAGELIST, 0, (LPARAM)imageList);

	ShowWindow(toolBar, true);
}

twToolBar::~twToolBar()
{
	ImageList_Destroy(imageList);
}

void twToolBar::setIconSize(twSize size)
{
	ImageList_SetIconSize(imageList, size.width, size.height);
	SendMessage(toolBar, TB_SETIMAGELIST, 0, (LPARAM)imageList);
}

void twToolBar::addButton(twImage *icon, twString text)
{
	int id = ImageList_AddIcon(imageList, icon->toIcon());
	TBBUTTON tbButton;
	if (text.empty())
		tbButton.iString = 0;
	else
		tbButton.iString = (int)text.c_str();
	tbButton.fsState = TBSTATE_ENABLED;
	tbButton.fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
	tbButton.idCommand = buttons.size() + 1;
	tbButton.iBitmap = id;
	SendMessage(toolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(toolBar, TB_ADDBUTTONS, 1, (LPARAM)&tbButton);
	SendMessage(toolBar, TB_AUTOSIZE, 0, 0);
	Button button;
	button.text = text;
	button.image = icon;
	buttons.push_back(button);
	setPosition(getCurrentPosition());
	SIZE size;
	SendMessage(toolBar, TB_GETIDEALSIZE, false, (LPARAM)&size);
	SendMessage(toolBar, TB_GETIDEALSIZE, true, (LPARAM)&size);
	setSize(twSize(size.cx, size.cy));
}

void twToolBar::setIcon(int index, twImage *icon)
{
	ImageList_ReplaceIcon(imageList, index, icon->toIcon());
	TBBUTTONINFO tbbi;
	tbbi.cbSize = sizeof(TBBUTTONINFO);
	tbbi.dwMask = TBIF_BYINDEX | TBIF_IMAGE;
	tbbi.iImage = index;
	SendMessage(toolBar, TB_SETBUTTONINFO, index, (LPARAM)&tbbi);
	buttons[index].image = icon;
}

void twToolBar::setText(int index, twString text)
{
	TBBUTTONINFO tbbi;
	tbbi.cbSize = sizeof(TBBUTTONINFO);
	tbbi.dwMask = TBIF_BYINDEX | TBIF_TEXT;
	tbbi.pszText = (CHAR*)text.c_str();
	SendMessage(toolBar, TB_SETBUTTONINFO, index, (LPARAM)&tbbi);
	buttons[index].text = text;
}

int twToolBar::getClickedButton()
{
	return m_clickedButton;
}

twString twToolBar::getText(int index)
{
	if (index < 0)
		index = getClickedButton();
	if (index > -1 || buttons.size() >= index + 1)
		return buttons[index].text;
}

twImage* twToolBar::getIcon(int index)
{
	if (index < 0)
		index = getClickedButton();
	if (index > -1 || buttons.size() >= index + 1)
		return buttons[index].image;
}
