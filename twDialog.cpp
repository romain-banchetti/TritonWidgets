#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include "twString.h"
#include <vector>
#include <sstream>
#include <Shlobj.h>

#include "twDialog.h"

using namespace std;

/*
twDialog::twDialog(twObject *parent, std::twString title, twSize size)
{
	twWindow *window = static_cast<twWindow*>(parent);
	DLGTEMPLATE dialogTemplate;
	dialogTemplate.style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
	dialogTemplate.x = 10;
	dialogTemplate.y = 10;
	dialogTemplate.cx = 100;
	dialogTemplate.cy = 100;
	DialogBoxIndirect(NULL, &dialogTemplate, window->getWindow(), dialogProcedure);
}

INT_PTR CALLBACK twDialog::dialogProcedure(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	}
	return false;
}
*/

void twDialog::information(twObject *parent, twString title, twString message)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    MessageBox(hwnd, message.c_str(), title.c_str(), MB_OK|MB_ICONINFORMATION);
}

void twDialog::information(twObject *parent, twString title, int message)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    MessageBox(hwnd, twString::intToString(message).c_str(), title.c_str(), MB_OK|MB_ICONINFORMATION);
}

void twDialog::error(twObject *parent, twString title, twString message)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    MessageBox(hwnd, message.c_str(), title.c_str(), MB_OK|MB_ICONERROR);
}

void twDialog::warning(twObject *parent, twString title, twString message)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    MessageBox(hwnd, message.c_str(), title.c_str(), MB_OK|MB_ICONWARNING);
}

bool twDialog::question(twObject *parent, twString title, twString message)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    if (MessageBox(hwnd, message.c_str(), title.c_str(), MB_YESNO|MB_ICONQUESTION) == IDYES)
		return true;
	else
		return false;
}
twString twDialog::getOpenFileName(twObject *parent, twString filter, twString initialDir)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    twString path;
    OPENFILENAME openFileName;
    char filePath[MAX_PATH] = "";
    ZeroMemory(&openFileName, sizeof(OPENFILENAME));
    openFileName.lStructSize = sizeof(OPENFILENAME);
    openFileName.hwndOwner = hwnd;
    openFileName.lpstrFile = filePath;
    if (!initialDir.empty())
		openFileName.lpstrInitialDir = (char*)initialDir.c_str();
    openFileName.nMaxFile = MAX_PATH;
    if (filter.empty())
		openFileName.lpstrFilter = "All\0*\00";
	else
	{
		char strFilter[1000] = {0};
		strcpy(strFilter, filter.data());
		for (unsigned int i = 0; i < 1000; i++)
			if (strFilter[i] == '\n')
				strFilter[i] = '\0';
		openFileName.lpstrFilter = strFilter;
	}
    openFileName.nFilterIndex = 1;
    openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileName(&openFileName) == TRUE)
    {
        path = filePath;
    }
    return path;
}

vector<twString> twDialog::getOpenFileNames(twObject *parent, twString filter, twString initialDir)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    vector<twString> paths;
    OPENFILENAME openFileName;
    char filePath[4096] = "";
    ZeroMemory(&openFileName, sizeof(OPENFILENAME));
    openFileName.lStructSize = sizeof(OPENFILENAME);
    openFileName.hwndOwner = hwnd;
    openFileName.lpstrFile = filePath;
    if (!initialDir.empty())
		openFileName.lpstrInitialDir = (char*)initialDir.c_str();
    openFileName.nMaxFile = 4096;
    if (filter.empty())
		openFileName.lpstrFilter = "All\0*\00";
	else
	{
		char strFilter[1000] = {0};
		strcpy(strFilter, filter.data());
		for (unsigned int i = 0; i < 1000; i++)
			if (strFilter[i] == '\n')
				strFilter[i] = '\0';
		openFileName.lpstrFilter = strFilter;
	}
    openFileName.nFilterIndex = 1;
    openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
    if (GetOpenFileName(&openFileName) == TRUE)
    {
    	if (filePath[openFileName.nFileOffset - 1] != '\0')
			paths.push_back(filePath);
		else
		{
			twString dir;
			twString currentFile;
			for (int i = 0;; i++)
			{
				if (filePath[i] == '\0')
				{
					if (dir.empty())
						dir = currentFile;
					else
						paths.push_back(dir + "\\" + currentFile);
					currentFile.clear();
					if (filePath[i + 1] == '\0')
						break;
				}
				else
					currentFile += filePath[i];
			}
		}
    }
    return paths;
}

twString twDialog::getExistingDirectory(twWidget *parent, twString text, twString initialDir)
{
    BROWSEINFO bi = {0};
    if (parent != NULL)
        bi.hwndOwner = *parent->m_handle;
    char buffer[MAX_PATH] = {0};
    bi.pszDisplayName = buffer;
    if (text != "")
        bi.lpszTitle = text.c_str();
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    SHGetPathFromIDList(pidl, buffer);
    twString path = buffer;
    IMalloc * imalloc = 0;
    if (SUCCEEDED(SHGetMalloc(&imalloc)))
    {
        imalloc->Free(pidl);
        imalloc->Release();
    }
    return path;
}

twString twDialog::getText(twWidget *parent, twString title)
{
    twWindow *parent2 = (twWindow*)parent;
    twWindow *window = new twWindow(title, twSize(300, 47), tw::NoSizingWindow, parent2);
    window->setInteriorMode(true);
    window->setPosition(parent2->getPosition() + twPosition((parent2->getSize().width / 2) - (window->getSize().width / 2), (parent2->getSize().height / 2) - (window->getSize().height / 2)));
    twTextEdit *textEdit = new twTextEdit(window, false);
    textEdit->setRelativePosition(twPosition(0, 0));
    textEdit->setRelativeSize(twSize(100, -1));
    textEdit->focus();
    textEdit->connect(twTextEdit::EnterPressed, window, SLOT(twWindow::closeModal));
    twButton *buttonOK = new twButton(window, "OK");
    buttonOK->setRelativePosition(twPosition(0, 0));
    buttonOK->setRelativeSize(twSize(50, 100));
    buttonOK->connect(twButton::Clicked, window, SLOT(twWindow::closeModal));
    twButton *buttonCancel = new twButton(window, "Annuler");
    buttonCancel->setRelativePosition(twPosition(50, 0));
    buttonCancel->setRelativeSize(twSize(50, 100));
    buttonCancel->connect(twButton::Clicked, window, SLOT(twWindow::closeModal));
    window->modal();
    twString text = textEdit->getText();
    window->close();
    return text;
}

twString twDialog::getSaveFileName(twObject *parent, twString filter)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    twString path;
    OPENFILENAME openFileName;
    char filePath[MAX_PATH] = "";
    ZeroMemory(&openFileName, sizeof(OPENFILENAME));
    openFileName.lStructSize = sizeof(OPENFILENAME);
    openFileName.hwndOwner = hwnd;
    openFileName.lpstrFile = filePath;
    openFileName.nMaxFile = MAX_PATH;
    if (filter.empty())
		openFileName.lpstrFilter = "All\0*\00";
	else
	{
		char strFilter[1000] = {0};
		strcpy(strFilter, filter.data());
		for (unsigned int i = 0; i < 1000; i++)
			if (strFilter[i] == '\n')
				strFilter[i] = '\0';
		openFileName.lpstrFilter = strFilter;
	}
    openFileName.nFilterIndex = 1;
    openFileName.Flags = OFN_OVERWRITEPROMPT;
    if(GetSaveFileName(&openFileName) == TRUE)
    {
        path = filePath;
    }
    return path;
}

twColor twDialog::getColor(twObject *parent)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
    static COLORREF color = 0;
	CHOOSECOLOR chooseColor;
	static COLORREF acrCustClr[16];
	ZeroMemory(&chooseColor, sizeof(CHOOSECOLOR));
	chooseColor.lStructSize = sizeof(CHOOSECOLOR);
	chooseColor.hwndOwner = hwnd;
	chooseColor.lpCustColors = NULL;
	chooseColor.lpCustColors = (LPDWORD) acrCustClr;
	chooseColor.rgbResult = color;
	chooseColor.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&chooseColor))
	{
		color = chooseColor.rgbResult;
		return twColor(GetRValue(color), GetGValue(color), GetBValue(color));
	}
	else
		return twColor();
}

twFont twDialog::getFont(twObject *parent)
{
    HWND hwnd = NULL;
    if (parent != NULL)
    {
        twWindow *window = static_cast<twWindow*>(parent);
        hwnd = window->getWindow();
    }
	static HFONT hfont;
	static LOGFONT logFont;
	CHOOSEFONT chooseFont;
	ZeroMemory(&chooseFont, sizeof(CHOOSEFONT));
	chooseFont.lStructSize = sizeof (CHOOSEFONT);
	chooseFont.hwndOwner = hwnd;
	chooseFont.lpLogFont = &logFont;
	chooseFont.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	if (ChooseFont(&chooseFont))
	{
		DeleteObject(hfont);
		hfont = CreateFontIndirect(&logFont);
		return twFont(logFont.lfFaceName, logFont.lfHeight);
	}
	else
		return twFont();
}
