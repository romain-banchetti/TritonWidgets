#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include "twString.h"
#include <vector>
#include <sstream>
#include <uxtheme.h>
#include <richedit.h>
//#include <richole.h>

#include "twWindow.h"
#include "twTextEdit.h"

extern HINSTANCE instance;

using namespace std;

twTextStyle::twTextStyle()
{
    m_bold = false;
    m_italic = false;
}

void twTextStyle::setColor(twColor color)
{
    m_color = color;
}

void twTextStyle::setFont(twString font)
{
    m_font = font;
}

void twTextStyle::setBold(bool bold)
{
    m_bold = bold;
}

void twTextStyle::setItalic(bool italic)
{
    m_italic = italic;
}

twColor twTextStyle::getColor()
{
    return m_color;
}

twString twTextStyle::getFont()
{
    return m_font;
}

bool twTextStyle::getBold()
{
    return m_bold;
}

bool twTextStyle::getItalic()
{
    return m_italic;
}

void twTextEdit::setSelectionStyle(twTextStyle ts)
{
    CHARFORMAT cf = {0};
    cf.cbSize = sizeof(CHARFORMAT);
    int color = 0;
    if (ts.getColor().a == 255)
    {
        color = CFM_COLOR;
        cf.crTextColor = RGB(ts.getColor().r, ts.getColor().g, ts.getColor().b);
    }
    int font = 0;
    if (ts.getFont() != "")
    {
        font = CFM_FACE;
        strcpy(cf.szFaceName, ts.getFont().c_str());
    }
    int bold = 0;
    if (ts.getBold())
    {
        bold = CFE_BOLD;
    }
    int italic = 0;
    if (ts.getItalic())
    {
        italic = CFE_ITALIC;
    }
    cf.dwMask = color | font | CFM_BOLD | CFM_ITALIC;
    cf.dwEffects = bold | italic;
    SendMessage(textEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);//SCF_WORD |
}

void twTextEdit::setWordStyle(twString word, twTextStyle ts)
{
    WordStyle ws;
    ws.word = word;
    ws.ts = ts;
    wordsStyle.push_back(ws);
}

void twTextEdit::setTextStyle(twTextStyle ts)
{
    CHARFORMAT cf = {0};
    cf.cbSize = sizeof(CHARFORMAT);
    int color = 0;
    if (ts.getColor().a == 255)
    {
        color = CFM_COLOR;
        cf.crTextColor = RGB(ts.getColor().r, ts.getColor().g, ts.getColor().b);
    }
    int font = 0;
    if (ts.getFont() != "")
    {
        font = CFM_FACE;
        strcpy(cf.szFaceName, ts.getFont().c_str());
    }
    int bold = 0;
    int bold2 = 0;
    if (ts.getBold())
    {
        bold = CFM_BOLD;
        bold2 = CFE_BOLD;
    }
    int italic = 0;
    int italic2 = 0;
    if (ts.getItalic())
    {
        italic = CFM_ITALIC;
        italic2 = CFE_ITALIC;
    }
    cf.dwMask = color | font | bold | italic;
    cf.dwEffects = bold2 | italic2;
    SendMessage(textEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
}

twTextEdit::twTextEdit(twObject *parent, bool multiLine, twPosition position, twSize size) : twWidget(&textEdit, position)
{
    LoadLibrary(TEXT("Riched20.dll"));
    twWidget *widget = static_cast<twWidget*>(parent);
    if (multiLine)
        textEdit = CreateWindow(RICHEDIT_CLASS, (LPCTSTR)"", (DWORD)WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|ES_NOHIDESEL, position.X, position.Y, size.width, size.height, *widget->m_handle, NULL, instance, NULL);
    else
        textEdit = CreateWindow(RICHEDIT_CLASS, (LPCTSTR)"", (DWORD)WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL, position.X, position.Y, size.width, size.height, *widget->m_handle, NULL, instance, NULL);
    SendMessage(textEdit, EM_SETTEXTMODE, TM_PLAINTEXT, 0);
    setRichMode(true);
    LOGFONT structFont;
    HFONT font;
    ZeroMemory(&structFont, sizeof(LOGFONT));
    strcpy(structFont.lfFaceName, "Arial");
    structFont.lfHeight = 16;
    font = CreateFontIndirect(&structFont);
    SendMessage(textEdit, WM_SETFONT, (WPARAM)font, 0);
    SendMessage(textEdit, EM_LIMITTEXT, (WPARAM)0, 0);
    setType(tw::TextEdit);
    setParent(parent);
    SetWindowLong(textEdit, GWL_USERDATA, (long)this);
	m_multiLine = multiLine;
	m_margin = 3;
	SendMessage(textEdit, EM_SETEVENTMASK, 0, ENM_LINK | ENM_CHANGE);//ENM_KEYEVENTS | ENM_CHANGE |
	m_password = false;
}

void twTextEdit::clear()
{
	setText("");
}

void twTextEdit::setRichMode(bool richMode)
{
    if (richMode)
    {
        SendMessage(textEdit, EM_SETTEXTMODE, TM_RICHTEXT, 0);
        SendMessage(textEdit, EM_AUTOURLDETECT, true, 0);
    }
    else
    {
        SendMessage(textEdit, EM_AUTOURLDETECT, false, 0);
        CHARFORMAT cf = {0};
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_COLOR | CFM_UNDERLINE;
        cf.dwEffects = CFE_AUTOCOLOR;
        cf.crTextColor = RGB(0, 0, 0);
        SendMessage(textEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
        SendMessage(textEdit, EM_SETTEXTMODE, TM_PLAINTEXT, 0);
    }
}

void twTextEdit::setInteriorMargin(int margin)
{
    m_margin = margin;
}

twString twTextEdit::getSelection()
{
    return getText().substr(getStartingSelection(), getEndingSelection() - getStartingSelection());
}

void twTextEdit::setSelection(int start, int end)
{
    Edit_SetSel(textEdit, start, end);
}

void twTextEdit::setPassword(bool value)
{
    if (value)
        Edit_SetPasswordChar(textEdit, '*');
    else
        Edit_SetPasswordChar(textEdit, 0);
    m_password = value;
}

bool twTextEdit::getPassword()
{
    return m_password;
}

void twTextEdit::undo()
{
    Edit_Undo(textEdit);
}

bool twTextEdit::canUndo()
{
    return Edit_CanUndo(textEdit);
}

int twTextEdit::getStartingSelection()
{
    return LOWORD(Edit_GetSel(textEdit));
}

int twTextEdit::getEndingSelection()
{
    return HIWORD(Edit_GetSel(textEdit));
}

int twTextEdit::getInteriorMargin()
{
    return m_margin;
}

void twTextEdit::setScrollBar(bool scrollBar)
{
    if (scrollBar)
    {
        if (m_multiLine)
            m_scroolBar = ES_AUTOVSCROLL|WS_VSCROLL;
        else
            m_scroolBar = ES_AUTOHSCROLL|ES_AUTOVSCROLL;
    }
    else
        m_scroolBar = false;
    if (m_multiLine)
        SetWindowLong(*m_handle, GWL_STYLE, WS_CHILD|WS_VISIBLE|ES_MULTILINE|m_border|m_scroolBar);
    else
        SetWindowLong(*m_handle, GWL_STYLE, WS_CHILD|WS_VISIBLE|ES_MULTILINE|m_border|m_scroolBar);
}

twColor twTextEdit::getBackgroundColor()
{
	return m_backgroundColor;
}

twColor twTextEdit::getTextColor()
{
	return m_textColor;
}

void twTextEdit::setBackgroundColor(twColor backgroundColor)
{
    SendMessage(textEdit, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(backgroundColor.r, backgroundColor.g, backgroundColor.b));
	m_backgroundColor = backgroundColor;
}

void twTextEdit::setTextColor(twColor textColor)
{
    twTextStyle ts;
    ts.setColor(textColor);
    setTextStyle(ts);
	m_textColor = textColor;
}

bool twTextEdit::getMultiLine()
{
	return m_multiLine;
}

void twTextEdit::setText(twString text)
{
    Edit_SetText(textEdit, text.c_str());
}

twString twTextEdit::getLine(int line)
{
    char buffer[10000];
    int len = Edit_GetLine(textEdit, line, buffer, 9999);
    if (len > 1)
        buffer[len - 1] = 0;
    else
        buffer[len] = 0;
    return buffer;
}

void twTextEdit::setLine(int numberLine, twString line)
{
    twString data = getText();
    int index = Edit_LineIndex(textEdit, numberLine);
    if (index == -1)
        data += "\r\n" + line;
    else
    {
        /*
        if (numberLine + 1 >= getNumberLines())
            data.replace(index, data.size(), line);
        else
        {
        */
            //cout << "LONGUEUR LIGNE " << numberLine << " : " << Edit_LineLength(textEdit, numberLine) << endl;
            data.replace(index, getLengthLine(numberLine), line);
        //}
    }
    int currentLine = getCurrentLine();
    int currentCharacter = getCurrentCharacter();
    setText(data);
    goLine(currentLine);
    goCharacter(currentCharacter);
}

void twTextEdit::setFont(twFont font)
{
    SendMessage(textEdit, WM_SETFONT, (WPARAM)font.getFont(), 0);
}

twString twTextEdit::getText()
{
    char buffer[100000];
    GETTEXTEX gt = {0};
    gt.cb = 99999;
    SendMessage(textEdit, EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)buffer);
    return buffer;
}

void twTextEdit::setReadOnly(bool readOnly)
{
	Edit_SetReadOnly(textEdit, readOnly);
}

void twTextEdit::append(twString text)
{
    int length = getTextLength();
    setSelection(length, length);
    if (length != 0)
        text = "\n" + text;
    Edit_ReplaceSel(textEdit, text.c_str());
    goLine(-1);
}

int twTextEdit::getTextLength()
{
    int length = GetWindowTextLength(textEdit);
    return length;
}

int twTextEdit::getNumberLines()
{
    int lineCount = Edit_GetLineCount(textEdit);
    return lineCount;
}

int twTextEdit::getLengthLine(int line)
{
    return Edit_LineLength(textEdit, Edit_LineIndex(textEdit, line));
}

void twTextEdit::goLine(int line)
{
	if (line == -1)
	{
	    int length = getTextLength();
        setSelection(length, length);
		line = getNumberLines() - 1;
		return;
	}
    int currentLine = Edit_GetFirstVisibleLine(textEdit);
    Edit_SetSel(textEdit, Edit_LineIndex(textEdit, line), Edit_LineIndex(textEdit, line));
    if (line > currentLine)
        SendMessage(textEdit, EM_LINESCROLL, 0, line - currentLine);
    else
        SendMessage(textEdit, EM_LINESCROLL, 0, ((line - currentLine) * -1)) - 1;
}

void twTextEdit::goCharacter(int character)
{
	if (character == -1)
		character = getLengthLine(0);
    int index = Edit_LineIndex(textEdit, getCurrentLine());
    Edit_SetSel(textEdit, index + character, index + character);
}

int twTextEdit::getCurrentLine()
{
    int character = LOWORD(Edit_GetSel(textEdit));
    for (int i = 0; i < getNumberLines(); i++)
    {
        if (i + 1 == getNumberLines() || (Edit_LineIndex(textEdit, i) <= character && Edit_LineIndex(textEdit, i + 1) > character))
            return i;
    }
    return -1;
}

int twTextEdit::getCurrentCharacter()
{
    return LOWORD(Edit_GetSel(textEdit)) - Edit_LineIndex(textEdit, getCurrentLine());
}



/*
BOOL RichEditInsertBitmap(HWND hWndRichEdit, HBITMAP hBitmap)
{
        HRESULT hr;
        IDataObject* pDataObject;
        LPOLEOBJECT lpOleObject = NULL;
        LPSTORAGE lpStorage = NULL;
        LPOLECLIENTSITE lpOleClientSite = NULL;
        LPLOCKBYTES lpLockBytes = NULL;
        CLIPFORMAT cfFormat = 0;
            LPFORMATETC lpFormatEtc = NULL;
        FORMATETC formatEtc;
        LPRICHEDITOLE lpRichEditOle;
        SCODE sc;
        LPUNKNOWN lpUnknown;
        REOBJECT reobject;
        CLSID clsid;
        SIZEL sizel;
        DWORD dwStart, dwEnd;

        CoInitialize(NULL);
            if( OpenClipboard(NULL) )
        {
                EmptyClipboard();
                SetClipboardData( CF_BITMAP, hBitmap );
                CloseClipboard();
        }
        else
                return 0;

        OleGetClipboard(&pDataObject);
        if (pDataObject == NULL)
                return 0;
        sc = CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
        if (lpLockBytes == NULL)
                return 0;

            sc = StgCreateDocfileOnILockBytes(lpLockBytes,
STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
        if (lpStorage == NULL)
                return 0;

           lpFormatEtc = &formatEtc;
        lpFormatEtc->cfFormat = cfFormat;
        lpFormatEtc->ptd = NULL;
        lpFormatEtc->dwAspect = DVASPECT_CONTENT;
        lpFormatEtc->lindex = -1;
        lpFormatEtc->tymed = TYMED_NULL;

            SendMessage(hWndRichEdit, EM_GETOLEINTERFACE, 0,
(LPARAM)&lpRichEditOle);
        if (lpRichEditOle == NULL)
                return 0;

        lpRichEditOle->GetClientSite(&lpOleClientSite);
        if (lpOleClientSite == NULL)
                return 0;

            hr = OleCreateStaticFromData(pDataObject, IID_IUnknown, 0, lpFormatEtc, lpOleClientSite, lpStorage,
(void**)&lpOleObject);
        if (lpOleObject == NULL)
                return 0;

            lpUnknown = (LPUNKNOWN)lpOleObject;
            lpUnknown->QueryInterface(IID_IOleObject,
(void**)&lpOleObject);
            lpUnknown->Release();
        OleSetContainedObject((LPUNKNOWN)lpOleObject, TRUE);

        ZeroMemory(&reobject, sizeof(REOBJECT));
        reobject.cbStruct = sizeof(REOBJECT);

        sc = lpOleObject->GetUserClassID(&clsid);

        reobject.clsid = clsid;
        reobject.cp = REO_CP_SELECTION;
        reobject.dvaspect = DVASPECT_CONTENT;
        reobject.dwFlags = REO_BELOWBASELINE; //REO_RESIZABLE |
REO_BELOWBASELINE;
        reobject.dwUser = 0;
        reobject.poleobj = lpOleObject;
        reobject.polesite = lpOleClientSite;
        reobject.pstg = lpStorage;

        sizel.cx = sizel.cy = 0;
        reobject.sizel = sizel;

        SendMessage(hWndRichEdit, EM_SETSEL, 0, -1);
        SendMessage(hWndRichEdit, EM_GETSEL, (WPARAM)&dwStart,
(LPARAM)&dwEnd);
        SendMessage(hWndRichEdit, EM_SETSEL, dwEnd+1, dwEnd+1);

            lpRichEditOle->InsertObject(&reobject);

        lpLockBytes->Release();
        lpStorage->Release();
        pDataObject->Release();
        lpRichEditOle->Release();
        lpOleClientSite->Release();
        lpOleObject->Release();
        CoUninitialize();

        return 1;
}
*/
