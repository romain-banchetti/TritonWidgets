#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <windowsx.h>
#include <richedit.h>
#include <Shellapi.h>

#include "twButton.h"
#include "twSlider.h"
#include "twTimer.h"
#include "twMenu.h"
#include "twWindow.h"
#include "twApplication.h"

#include <algorithm>

#include "GLEW/glew.h"

extern HINSTANCE instance;
extern twWindow *g_mainWindow;
extern twApplication *g_application;
extern HDC hDC;

using namespace std;
using namespace tw;

twWindow::twWindow(twString title, twSize size, Style style, twWindow *parent) : twWidget(&m_window)
{
	//m_scaleView = 1;
	//m_graphicMode = false;
	m_backgroundColor.a = 0;
	m_lastTime = 0;
	m_interiorMode = false;
    mb_centralWidget = false;
    m_mouseLeave = true;

    mouseLeftClicked = false;
    mouseRightClicked = false;
    mouseMiddleClicked = false;

    WNDCLASS windowClass;

    windowClass.style = 0;//CS_OWNDC;
    windowClass.lpfnWndProc = windowProcedure;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = NULL;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);//(HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszMenuName =  NULL;
    windowClass.lpszClassName = title.c_str();
    RegisterClass(&windowClass);
    m_parent = parent;
    HWND parent2;
    if (parent != NULL)
        parent2 = parent->getWindow();
    else
        parent2 = NULL;
    m_window = CreateWindow(title.c_str(), title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, size.width, size.height, parent2, NULL, NULL, this);
    m_menu = CreateMenu();
    if (g_mainWindow == NULL)
		g_mainWindow = this;
    //m_flachingTimer = NULL;
    m_activated = false;
    setType(tw::Window);
    if (parent != NULL)
        setParent(parent);
    m_hdc = GetDC(m_window);
    if (parent == NULL)
        hDC = m_hdc;
	m_hide = true;
}

void twWindow::setDragFiles(bool dragFiles)
{
    DragAcceptFiles(m_window, dragFiles);
}

void twWindow::onUpdate()
{
	int elapsedTime = getElapsedTime();
	m_frameTime = elapsedTime - m_lastTime;
	//cout << elapsedTime << " - " << m_lastTime << "=" << m_frameTime << endl;
	if (m_frameTime > 0)
		m_fps = 1000 / m_frameTime;
	m_lastTime = elapsedTime;
	//cout << getElapsedTime() << endl;
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//m_viewPosition = twPosition(100, 100);
	//glOrtho(m_viewPosition.X, getInteriorSize().width + m_viewPosition.X, getInteriorSize().height + m_viewPosition.Y, m_viewPosition.Y, 0, 1);
    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
}

twWindow::~twWindow()
{
    ReleaseDC(m_window, m_hdc);
}

/*
void twWindow::setGraphicMode(bool graphicMode, twPosition point, twSize size)
{
	m_graphicMode = graphicMode;
	if (m_graphicMode)
	{
		HGLRC hrc;
		PIXELFORMATDESCRIPTOR pfd;
		int format;
		if (size.width == 0 || size.height == 0)
		{
			if (m_interiorMode)
				size = getSize();
			else
				size = twSize(getInteriorSize().width, getInteriorSize().height);
		}
		m_graphicArea = CreateWindow("STATIC", "", WS_CHILD|WS_VISIBLE, point.X, point.Y, size.width, size.height, window, 0, NULL, NULL);
        m_hdc = GetDC(m_graphicsArea);
        ZeroMemory( &pfd, sizeof( pfd ) );
        pfd.nSize = sizeof( pfd );
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;
        format = ChoosePixelFormat( m_hdc, &pfd );
        SetPixelFormat(m_hdc, format, &pfd );
        hrc = wglCreateContext( m_hdc);
        wglMakeCurrent(m_hdc, hrc);
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glMatrixMode(GL_PROJECTION);
        glOrtho(0, size.width, size.height, 0, 0, 1);
        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
	}
}
*/

void twWindow::setInteriorMode(bool interiorMode)
{
    twSize size = getSize();
	m_interiorMode = interiorMode;
	setSize(size);
}

bool twWindow::getVisible()
{
	return !m_hide;
}

twColor twWindow::getBackgroundColor()
{
	return m_backgroundColor;
}

void twWindow::setSize(twSize size)
{
	if (!m_interiorMode)
		SetWindowPos(*m_handle, NULL, 0, 0, size.width, size.height, SWP_NOZORDER|SWP_NOMOVE);
	else
	{
        twWidget *widget = (twWidget*)this;
        twSize size2 = widget->getSize();
        twSize borderSize = size2 - getInteriorSize();
		SetWindowPos(*m_handle, NULL, 0, 0, size.width + borderSize.width, size.height + borderSize.height, SWP_NOZORDER|SWP_NOMOVE);
	}
}

twSize twWindow::getSize()
{
    twWidget *widget = (twWidget*)this;
    twSize size = widget->getSize();
    //if (!m_interiorMode)
        return size;
    return size - twSize(size - widget->getInteriorSize());
}

/*
void twWindow::setGraphicAreaPosition(twPosition point)
{
	SetWindowPos(m_graphicArea, NULL, point.X, point.Y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void twWindow::setGraphicAreaSize(twSize size)
{
	glPushMatrix();
	SetWindowPos(m_graphicArea, NULL, 0, 0, size.width, size.height, SWP_NOZORDER|SWP_NOMOVE);
	glOrtho(0, size.width, size.height, 0, 0, 1);
	glPopMatrix();
}

twPosition twWindow::getGraphicAreaPosition()
{
    RECT rect;
    GetClientRect(m_graphicArea, &rect);
	return twPosition(rect.left - getInteriorPosition().X, rect.top - getInteriorPosition().Y);
}

twSize twWindow::getGraphicAreaSize()
{
    RECT rect;
    GetWindowRect(m_graphicArea, &rect);
    return twSize(rect.right-rect.left, rect.bottom-rect.top);
}
*/
void twWindow::setIcon(twImage *image)
{
	SetClassLong(m_window, GCL_HICON, (long)image->toIcon());
}

int twWindow::getFPS()
{
	return m_fps;
}

int twWindow::getFrameTime()
{
	return m_frameTime;
}

int twWindow::getElapsedTime()
{
    //SYSTEMTIME time;
    //GetLocalTime(&time);
	return clock();
}

void twWindow::setTop(bool top)
{
	if (top)
		SetWindowPos(getWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	else
		SetWindowPos(getWindow(), NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}

HWND twWindow::getWindow()
{
    return m_window;
}

void twWindow::close()
{
	DestroyWindow(m_window);
}

void twWindow::closeModal()
{
    EnableWindow(m_parent->getWindow(), true);
    //m_parent->setVisible(true);
}

void twWindow::setTitle(twString title)
{
    SetWindowText(m_window, title.c_str());
}

void twWindow::setCentralWidget(twWidget *widget)
{
    widget->setRelativePosition(twPosition(0, 0));
    widget->setRelativeSize(twSize(100, 100));
}

void twWindow::showCursor(bool showCursor)
{
	ShowCursor(showCursor);
}
/*
void twWindow::setViewPosition(twPosition position)
{
	glTranslated(m_viewPosition.X - position.X, m_viewPosition.Y - position.Y, 0);
	m_viewPosition = position;
}

void twWindow::setScaleView(double scale)
{
	//cout << scale + ((m_scaleView - 1) * -1) << endl;
	//glScaled(scale + ((m_scaleView - 1) * -1), scale + ((m_scaleView - 1) * -1), 1);
	//m_scaleView = scale;
	//glLoadIdentity();
	glScaled(1.1, 1.1, 1.1);
	glScaled(0.9, 0.9, 0.9);
}

double twWindow::getScaleView()
{
	return m_scaleView;
}

twPosition twWindow::getViewPosition()
{
	return m_viewPosition;
}
*/
twString twWindow::getTitle()
{
    char buffer[1024];
    GetWindowText(m_window, buffer, 1023);
    return buffer;
}

void twWindow::addMenu(twMenu *menu)
{
    AppendMenu(m_menu, MF_POPUP, (UINT)menu->getMenu(), menu->getText().c_str());
    SetMenu(m_window, m_menu);
    menu->m_parentMenu = m_menu;
    if (menu->m_enable == false)
        EnableMenuItem(m_menu, menu->m_id, MF_GRAYED);
}

void twWindow::flashingTimeOut()
{
	FlashWindow(m_window, true);
}

void twWindow::setMinimumSize(twSize minimumSize)
{
	m_minimumSize = minimumSize;
	if (m_minimumSize.width > 0 && getSize().width < m_minimumSize.width)
		setSize(twSize(m_minimumSize.width, getSize().height));
	if (m_minimumSize.height > 0 && getSize().height < m_minimumSize.height)
		setSize(twSize(getSize().width, m_minimumSize.height));
}

void twWindow::setMaximumSize(twSize maximumSize)
{
	m_maximumSize = maximumSize;
	if (m_maximumSize.width > 0 && getSize().width > m_maximumSize.width)
		setSize(twSize(m_maximumSize.width, getSize().height));
	if (m_maximumSize.height > 0 && getSize().height > m_maximumSize.height)
		setSize(twSize(getSize().width, m_maximumSize.height));
}

void twWindow::flash()
{
	/*
	if (flashing == 0)
	{
		delete m_flachingTimer;
		m_flachingTimer = NULL;
		return;
	}
	if (m_flachingTimer != NULL)
		return;
	m_flachingAutoStop = autoStop;
	if (autoStop && m_activated)
		return;
		*/
	//m_flachingTimer = new twTimer(this, flashing);
	//m_flachingTimer->connect(twTimer::TimeOut, this, SLOT(twWindow::flashingTimeOut));
	if (!m_activated)
		FlashWindow(m_window, true);
}

void twWindow::setBackgroundColor(twColor color)
{
	m_backgroundColor = color;
	//RedrawWindow(window, NULL, NULL, RDW_NOINTERNALPAINT|RDW_UPDATENOW|RDW_ALLCHILDREN|RDW_VALIDATE);
}

void twWindow::setVisible(bool visible)
{
	if (!visible)
		hide();
	else
		show();
}

void twWindow::show(State state)
{
	m_hide = false;
	if (state == Maximized)
		ShowWindow(m_window, SW_SHOWMAXIMIZED);
	else if (state == Minimized)
		ShowWindow(m_window, SW_SHOWMINIMIZED);
	else if (state == FullScreen)
	{
		SetWindowLong(m_window, GWL_STYLE, WS_POPUP);
		ShowWindow(m_window, SW_SHOWMAXIMIZED);
	}
	else
	{
		ShowWindow(m_window, SW_SHOWNORMAL);
		//SetWindowPos(m_window, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
	}
}

void twWindow::modal()
{
    if (m_parent != NULL)
        EnableWindow(m_parent->getWindow(), false);
    show();
    MSG message;
    while (GetMessage(&message, NULL, 0, 0))
    {
        if(message.message == WM_QUIT)
            break;
        TranslateMessage(&message);
        DispatchMessage(&message);
        if (IsWindowEnabled(*((twWidget*)getParent())->m_handle))
            return;
    }
    //if (m_parent != NULL)
    //    EnableWindow(m_parent->getWindow(), true);
}

void twWindow::setWindowState(State state)
{
	m_state = state;
	if (state == Maximized)
		ShowWindow(m_window, SW_MAXIMIZE);
	else if (state == Minimized)
		ShowWindow(m_window, SW_MINIMIZE);
	else if (state == FullScreen)
	{
		SetWindowLong(m_window, GWL_STYLE, WS_POPUP);
		ShowWindow(m_window, SW_MAXIMIZE);
	}
}

void twWindow::setWindowStyle(Style style)
{
	SetWindowLong(m_window, GWL_STYLE, style);
	ShowWindow(m_window, SW_SHOW);
}

void twWindow::hide()
{
	m_hide = true;
	ShowWindow(m_window, SW_HIDE);
	if (m_parent != NULL)
        if (!IsWindowEnabled(m_parent->getWindow()))
            EnableWindow(m_parent->getWindow(), true);
}

Key twWindow::getKeyDown()
{
	return keyDown;
}

Key twWindow::getKeyUp()
{
	return keyUp;
}

bool twWindow::isKeyDown(Key key)
{
	for (unsigned int i = 0;i < keysDown.size();i++)
	{
		if (keysDown[i] == key)
			return true;
	}
	return false;
}

bool twWindow::isMouseDown(tw::MouseButton mouseButton)
{
    if (mouseButton == tw::Left)
        return mouseLeftClicked;
    if (mouseButton == tw::Right)
        return mouseRightClicked;
    if (mouseButton == tw::Middle)
        return mouseMiddleClicked;
}
/*
void twWindow::dray(twSprite *sprite)
{
	twImage *image = sprite->getImage();
	if (image == NULL || !image->isLoaded())
		return;
	float width = image->getSize().width;
	float height = image->getSize().height;
	twPosition position = getViewPosition();
	twSize size = getGraphicAreaSize();
	if (sprite->getPosition().X + width < position.X || sprite->getPosition().X > position.X + size.width || sprite->getPosition().Y + height < position.Y || sprite->getPosition().Y > position.Y + size.height)
		return;
    glColor4ub(sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, sprite->getColor().a);
	unsigned int texture = image->m_texture;
	if (texture == 0)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)width, (int)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
		//glBindTexture(GL_TEXTURE_2D, texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (int)width, (int)height, GL_RGBA, GL_UNSIGNED_BYTE, &image->m_pixels[0]);
		//glLoadIdentity();
		//glBindTexture(GL_TEXTURE_2D, texture);
		image->m_texture = texture;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	int x = sprite->getPosition().X;
	int y = sprite->getPosition().Y;
	glPushMatrix();
	glScaled(sprite->getScale(), sprite->getScale(), 1);
	glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2d(x, y);
		glTexCoord2d(0, 1);
		glVertex2d(x, y + height);
		glTexCoord2d(1, 1);
		glVertex2d(x + width, y + height);
		glTexCoord2d(1, 0);
		glVertex2d(x + width, y);
	glEnd();
	glPopMatrix();
	glColor4ub(255, 255, 255, 255);
}

void twWindow::dray(twShape *shape)
{
	float width = shape->getSize().width;
	float height = shape->getSize().height;
	int x = shape->getPosition().X;
	int y = shape->getPosition().Y;
    glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
        glColor4ub(shape->getColor().r, shape->getColor().g, shape->getColor().b, shape->getColor().a);
		glVertex2d(x, y);
		glVertex2d(x, y + height);
		glVertex2d(x + width, y + height);
		glVertex2d(x + width, y);
	glEnd();
	if (shape->getBorderSize() > 0)
	{
	    int borderSize = shape->getBorderSize() - 1;
	    glLineWidth(borderSize + 1);
        glBegin(GL_LINE_LOOP);
            glColor4ub(shape->getBorderColor().r, shape->getBorderColor().g, shape->getBorderColor().b, shape->getBorderColor().a);
            glVertex2d(x - borderSize, y - borderSize);
            glVertex2d(x - borderSize, y + height + borderSize);
            glVertex2d(x + width + borderSize, y + height + borderSize);
            glVertex2d(x + width + borderSize, y - borderSize);
        glEnd();
	}
	glColor4ub(255, 255, 255, 255);
	glEnable(GL_TEXTURE_2D);
}

void twWindow::dray(twText *text)
{
    glColor4ub(text->getColor().r, text->getColor().g, text->getColor().b, text->getColor().a);
    glRasterPos2d(text->getPosition().X, text->getPosition().Y + 10);
    glListBase(text->getFont()->m_genLists);
    glCallLists(text->getText().size(), GL_UNSIGNED_BYTE, text->getText().c_str());
    glColor4ub(255, 255, 255, 255);
}

void twWindow::clear(twColor color)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a / 255);
}
*/
void twWindow::addSprite(twSprite *sprite)
{
	m_sprites.push_back(sprite);
}

void twWindow::redraw(bool erase)
{
    if (erase)
        RedrawWindow(m_window, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    else
        RedrawWindow(m_window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

twPosition twWindow::getCursorPosition()
{
	POINT position;
	GetCursorPos(&position);
	ScreenToClient(m_window, &position);
	return twPosition(position.x, position.y);
}

twImage* twWindow::capture()
{
    twImage *image = new twImage;

    BYTE *lpBits = NULL;
    int width = getInteriorSize().width;
    int height = getInteriorSize().height;

    //le HDC mémoire
    HDC hdcMemoire = CreateCompatibleDC(hDC);
    //le HBITMAP de l'image de l'écran
    HBITMAP hbitmap = CreateCompatibleBitmap(hDC, width, height);
    //selection de l'objet
    SelectObject(hdcMemoire, hbitmap);

    //copie de la surface
    BitBlt(hdcMemoire, 0, 0, width, height, hDC, 0, 0, SRCCOPY);

    //ici on a un hbitmap valide

    //création des variables pour getdiblits
    BITMAP bitmap;
    BITMAPINFO bitmapinfo;

    //initialisation des variables
    ZeroMemory(&bitmap,sizeof(BITMAP));
    ZeroMemory(&bitmapinfo,sizeof(BITMAPINFO));

    //
    GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bitmap);

    //initialisation de bitmapinfoheader
    bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapinfo.bmiHeader.biWidth = bitmap.bmWidth;
    bitmapinfo.bmiHeader.biHeight = bitmap.bmHeight;
    bitmapinfo.bmiHeader.biPlanes = 1;
    bitmapinfo.bmiHeader.biBitCount = 32;//affichage 32 bits
    bitmapinfo.bmiHeader.biCompression = BI_RGB;
    bitmapinfo.bmiHeader.biSizeImage = 0;
    bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
    bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
    bitmapinfo.bmiHeader.biClrUsed = 0;
    bitmapinfo.bmiHeader.biClrImportant = 0;

    //allocation
    lpBits = new BYTE[bitmapinfo.bmiHeader.biWidth*bitmapinfo.bmiHeader.biHeight*4];

    //on récupere ls données
    if(GetDIBits(hDC, hbitmap, 0, (UINT)bitmapinfo.bmiHeader.biHeight, lpBits, &bitmapinfo, DIB_RGB_COLORS)){

        //si GetDIBits à fonctionné

//lpBits
		//image->fromBitmap(hbitmap);

		image->setSize(twSize(width, height));
        image->m_pixels.resize(width * height);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                COLORREF color = GetPixel(hdcMemoire, i, j);
                image->setPixel(twPosition(i, j), twColor(GetRValue(color), GetGValue(color), GetBValue(color)));
            }
        }

    }

    //on libere
	*lpBits=(BYTE)NULL;
    lpBits=(BYTE)NULL;

    //on supprime le HBITMAP
    DeleteObject(hbitmap);
    //on libere le HDC mémoire
    DeleteDC(hdcMemoire);

    return image;
}

void twWindow::setAlwaysTop(bool alwaysTop)
{
    if (alwaysTop)
        SetWindowPos(m_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(m_window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool twWindow::getMouseWheel()
{
	return m_mouseWheel;
}

void twWindow::setCursorPosition(twPosition position)
{
	POINT pt;
	pt.x = position.X;
	pt.y = position.Y;
	ClientToScreen(m_window, &pt);
	SetCursorPos(pt.x, pt.y);
}

MouseButton twWindow::getMouseDown()
{
	return m_mouseDown;
}

MouseButton twWindow::getMouseUp()
{
	return m_mouseUp;
}

std::vector<twString> twWindow::getDropFiles()
{
	return dropFiles;
}

tw::State twWindow::getWindowState()
{
	return m_state;
}

/*
int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int number = twRand(-1, 1);
	//twDialog::information(NULL, "", number);
	cout << number << endl;
	return number;
}
*/

void twWindow::recalculateSize(twWidget *rw)
{
    if (rw == NULL)
        rw = this;

	if (rw->getType() == tw::SystemTrayIcon)
		return;

    int widthWindow = rw->getInteriorSize().width;
    int heightWindow = rw->getInteriorSize().height;

    if (widthWindow == 0 || heightWindow == 0)//Peut arriver par exemple avant que la fenêtre soit chargée
        return;

    int currentWidth = 0;
    bool statusBar = false;
    //Cette boucle sert à calculer le nombre de pixel en hauteur que 100% vaudra...
    for (unsigned int i = 0; i < rw->children->size(); i++)
    {
        if (rw->children->at(i).type == tw::StatusBar)
            statusBar = true;
        if (rw->children->at(i).type != tw::Timer && rw->children->at(i).type != tw::Menu && rw->children->at(i).type != tw::StatusBar && rw->children->at(i).type != tw::Text && rw->children->at(i).type != tw::SystemTrayIcon)
        {
            twWidget *widget = static_cast<twWidget*>(rw->children->at(i).child);
            if (widget->getRelativeSize().width > -1 && widget->getRelativeSize().height == -1)//Un widget ayant -1 en hauteur relative n'est pas pris en compte dans les pourcentages
            {
                currentWidth += widthWindow * widget->getRelativeSize().width / 100;//La largeur doit être calculée car plusieurs widgets peuvent être alignés en hauteur, donc sans compter plusieurs fois dans la hauteur
                if ((widthWindow * widget->getRelativeSize().width / 100) + (widthWindow * widget->getRelativePosition().X / 100) > currentWidth)
                    currentWidth = (widthWindow * widget->getRelativeSize().width / 100) + (widthWindow * widget->getRelativePosition().X / 100);
                if (currentWidth >= (widthWindow - 10))//10 = approximation %/px
                {
                    currentWidth = 0;
                    heightWindow -= widget->getSize().height;
                }
            }
        }
    }
    if (statusBar)
        heightWindow -= 22;
    int currentHeight = 0;
    currentWidth = 0;
    int maxHeight = 0;
    for (unsigned int i = 0; i < rw->children->size(); i++)
    {
        int newSizeWidth, newSizeHeight;
        int newPositionX, newPositionY;
        if (rw->children->at(i).type != tw::Timer && rw->children->at(i).type != tw::Menu && rw->children->at(i).type != tw::StatusBar && rw->children->at(i).type != tw::Text && rw->children->at(i).type != tw::SystemTrayIcon)
        {
            twWidget *widget = static_cast<twWidget*>(rw->children->at(i).child);
            if (widget->getRelativeSize().width > -1 || widget->getRelativeSize().height > -1)
            {
                newSizeWidth = widget->getSize().width;
                newSizeHeight = widget->getSize().height;
                if (widget->getRelativeSize().width > -1)
                    newSizeWidth = widthWindow * widget->getRelativeSize().width/100;
                if (widget->getRelativeSize().height > -1)
                    newSizeHeight = heightWindow * widget->getRelativeSize().height/100;
                widget->setSize(twSize(newSizeWidth, newSizeHeight), false);
            }
            newPositionX = widget->getPosition().X;
            newPositionY = widget->getPosition().Y;
            if (widget->getRelativePosition().X > -1 || widget->getRelativePosition().Y > -1)
            {
                if (widget->getRelativePosition().X > -1)
                    newPositionX = widthWindow * widget->getRelativePosition().X/100;
                if (widget->getRelativePosition().Y > -1)
                    newPositionY = heightWindow * widget->getRelativePosition().Y/100;
                if (rw->getType() == tw::Window)
				    widget->setPosition(twPosition(newPositionX, newPositionY + currentHeight), false);
                else
                    widget->setPosition(twPosition(newPositionX + rw->getPosition().X, newPositionY + currentHeight + rw->getPosition().Y), false);
            }

            if (widget->getRelativeSize().width > -1 && widget->getRelativeSize().height == -1)
            {
                currentWidth += newSizeWidth;
                if (newSizeWidth + newPositionX > currentWidth)
                    currentWidth = newSizeWidth + newPositionX;
                if (currentWidth >= (widthWindow - 10))//5 = approximation %/px
                {
                    currentWidth = 0;
                    currentHeight += newSizeHeight;
                }
            }
            if (maxHeight < newSizeHeight)
                maxHeight = newSizeHeight;
			widget->redraw();
        }
        if (rw->children->at(i).type == tw::Text)
        {
            twText *widget = static_cast<twText*>(rw->children->at(i).child);
            if (widget->getRelativeSize().width > -1 || widget->getRelativeSize().height > -1)
            {
                int newSizeWidth = widget->getSize().width, newSizeHeight = widget->getSize().height;
                if (widget->getRelativeSize().width > -1)
                    newSizeWidth = widthWindow * widget->getRelativeSize().width/100;
                if (widget->getRelativeSize().height > -1)
                    newSizeHeight = heightWindow * widget->getRelativeSize().height/100;
                widget->setSize(twSize(newSizeWidth, newSizeHeight));
            }
            if (widget->getRelativePosition().X > -1 || widget->getRelativePosition().Y > -1)
            {
                int newPositionX = widget->getPosition().X, newPositionY = widget->getPosition().Y;
                if (widget->getRelativePosition().X > -1)
                    newPositionX = widthWindow * widget->getRelativePosition().X/100;
                if (widget->getRelativePosition().Y > -1)
                    newPositionY = heightWindow * widget->getRelativePosition().Y/100;
                widget->setPosition(twPosition(newPositionX, newPositionY + currentHeight));
            }
            if (widget->getRelativeSize().width > -1 && widget->getRelativeSize().height == -1)
            {
                currentWidth += widget->getSize().width;
                if (widget->getSize().width + widget->getPosition().X > currentWidth)
                    currentWidth = widget->getSize().width + widget->getPosition().X;
                if (currentWidth >= (widthWindow - 10))//5 = approximation %/px
                {
                    currentWidth = 0;
                    currentHeight += widget->getSize().height;
                }
            }
            if (maxHeight < widget->getSize().height)
                maxHeight = widget->getSize().height;
            widget->redraw();
        }
        if (rw->children->at(i).type == tw::TabsBox)
        {
            twTabsBox *tabsBox = static_cast<twTabsBox*>(rw->children->at(i).child);
            tabsBox->recalculateSize();
        }
        if (rw->children->at(i).type == tw::StatusBar)
        {
            int interieurHeight = getInteriorSize().height;
            twStatusBar *statusBar = static_cast<twStatusBar*>(rw->children->at(i).child);
            MoveWindow(*statusBar->m_handle, 0, heightWindow - interieurHeight, widthWindow, interieurHeight, true);
        }
        if (rw->children->at(i).type == tw::TextEdit)
        {
            twTextEdit *textEdit = static_cast<twTextEdit*>(rw->children->at(i).child);
            RECT rect;
            rect.bottom = textEdit->getSize().height - textEdit->getInteriorMargin();
            rect.top = textEdit->getInteriorMargin();
            rect.left = textEdit->getInteriorMargin();
            rect.right = textEdit->getSize().width - textEdit->getInteriorMargin();
            textEdit->setSize(twSize(textEdit->getSize().width - 1, textEdit->getSize().height), false);
            textEdit->setSize(twSize(textEdit->getSize().width + 1, textEdit->getSize().height), false);
            Edit_SetRect(*textEdit->m_handle, &rect);
        }
        if (rw->children->at(i).type == tw::Area)
        {
            twArea *area = static_cast<twArea*>(rw->children->at(i).child);
            recalculateSize(area);
        }
    }
}

bool twWindow::isSpecialChar(char sc)
{
    if ((sc >= 0 && sc <= 47) || (sc >= 58 && sc <= 64) || (sc >= 91 && sc <= 96) || (sc >= 123 && sc <= 127))
        return true;
    return false;
}

LRESULT CALLBACK twWindow::windowProcedure(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    twWindow *pThis = NULL;
    if (umsg == WM_CREATE)
    {
        pThis=(twWindow*)((LPCREATESTRUCT(lparam))->lpCreateParams);
        SetWindowLong(hwnd, GWL_USERDATA, (long)pThis);
    }
    else
    {
        pThis=(twWindow*)GetWindowLong(hwnd,GWL_USERDATA);
    }
    /*
    if (g_mainWindow != NULL)
        return g_mainWindow->windowProcedure2(hwnd,umsg,wparam,lparam);
    */
    return pThis->windowProcedure2(hwnd,umsg,wparam,lparam);
}

LRESULT CALLBACK twWindow::windowProcedure2(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_CREATE:
		{
			return 0;//If an application processes this message, it should return zero to continue creation of the window. If the application returns –1, the window is destroyed and the CreateWindowEx or CreateWindow function returns a NULL handle.
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(m_window, &ps);
			ps.hdc = dc;
			for (unsigned int i = 0; i < m_sprites.size(); i++)
				DrawState(dc, NULL, NULL, (long)m_sprites[i]->getImage()->toBitmap(), NULL, m_sprites[i]->getPosition().X, m_sprites[i]->getPosition().Y, 0, 0, DST_BITMAP);
			for (unsigned int i=0;i<children->size();i++)
			{
				if (children->at(i).type == tw::Text)
				{
					SetBkMode(dc, TRANSPARENT);
					twText *text = static_cast<twText*>(children->at(i).child);
					LOGFONT structFont;
					HFONT font;
					ZeroMemory(&structFont, sizeof(LOGFONT));
					strcpy(structFont.lfFaceName, text->getFont()->getFontName().c_str());
					structFont.lfHeight = text->getFont()->getFontSize();
					font = CreateFontIndirect(&structFont);
					SetTextColor(dc, RGB(text->getColor().r, text->getColor().g, text->getColor().b));
					HGDIOBJ hfntPrev = SelectObject(dc, font);
					TextOut(dc, text->getPosition().X, text->getPosition().Y, text->getText().c_str(), text->getText().size());
					SelectObject(dc, hfntPrev);
					DeleteObject(font);
					SetBkMode(dc, OPAQUE);
				}
			}
			EndPaint(m_window, &ps);
			return 0;//An application returns zero if it processes this message.
		}
		case WM_SIZE:
		{
			recalculateSize();
			triggerEvent(Resize);
			redraw();
			//RedrawWindow(m_window, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
			//InvalidateRect(m_window, NULL, false);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_COMMAND:
		{
			if (lparam == 0 && (HIWORD(wparam) == 0 || HIWORD(wparam) == 1))
			{
				twMenu *menu = (twMenu*)children->at(LOWORD(wparam)).child;
				menu->sendEvent(twMenu::Clicked);
			}

			else if (HIWORD(wparam) == EN_CHANGE)
			{

				if (long(GetWindowLong((HWND)lparam, GWL_USERDATA)) == 0)
					return 0;

				twTextEdit *textEdit = (twTextEdit*)GetWindowLong((HWND)lparam, GWL_USERDATA);

				/*
				twString text = textEdit->getText();
				if (text.find(" ") != twString::npos)
				{
					twString lastWord = text.substr(text.rfind(" ") + 1);
					lastWord = lastWord.replaceAll("\r", "");
					lastWord = lastWord.replaceAll("\n", "");







				}


*/
/*
				if (IsWindowEnabled(*textEdit->m_handle))
				{
					bool haveFocus = textEdit->getFocus();
					EnableWindow(*textEdit->m_handle, false);
					int pos = textEdit->getStartingSelection();
					twString text = textEdit->getText();

					twString textBefore = textEdit->getText().substr(0, pos);

					int spacePos = textBefore.rfind(" ") + 1;
					int nrPos = textBefore.rfind("\r") + 1;
					if (nrPos > spacePos)
						spacePos = nrPos;
					if (spacePos != 0)
						textBefore = textBefore.substr(spacePos);
					bool trouve = false;
					for (unsigned int i = 0; i < textEdit->wordsStyle.size(); i++)
					{
						int findPos = textBefore.find(textEdit->wordsStyle[i].word);
						if (findPos == -1)
							continue;
						if ((textEdit->wordsStyle[i].word.size() == 1 && isSpecialChar(textEdit->wordsStyle[i].word[0])) || ((findPos == 0 || isSpecialChar(textBefore[findPos - 1])) && (findPos + textEdit->wordsStyle[i].word.size() == textBefore.size() || isSpecialChar(textBefore[findPos + textEdit->wordsStyle[i].word.size()]))))
						{
							findPos += spacePos;
							textEdit->setSelection(findPos, findPos + textEdit->wordsStyle[i].word.size());
							textEdit->setSelectionStyle(textEdit->wordsStyle[i].ts);

							textEdit->setSelection(findPos + textEdit->wordsStyle[i].word.size(), pos);

							twTextStyle ts;
							ts.setColor(twColor::black());
							ts.setBold(false);
							textEdit->setSelectionStyle(ts);

							trouve = true;
						}
					}


					if (!trouve)
					{
						twTextStyle ts;
						ts.setColor(twColor::black());
						ts.setBold(false);
						//if (textBefore.size() == 1)
						//    textEdit->setSelection(spacePos, spacePos + 1);
						//else
						textEdit->setSelection(spacePos, spacePos + textBefore.size());
						textEdit->setSelectionStyle(ts);
					}

					textEdit->setSelection(pos, pos);

					EnableWindow(*textEdit->m_handle, true);
					if (haveFocus)
						textEdit->focus();
				}
*/


				if (!textEdit->getMultiLine() && textEdit->getNumberLines() > 1)
				{
					twString text;
					for (int j = 0; j < textEdit->getNumberLines(); j++)
						text += textEdit->getLine(j);
					while (text.find(13) != twString::npos)
						text.replace(text.find(13), 1, "");
					textEdit->setText(text);
					textEdit->goCharacter(-1);
					textEdit->sendEvent(twTextEdit::EnterPressed);
				}
				for (unsigned int j=0;j<textEdit->events.size();j++)
				{
					if (textEdit->events[j].event == twTextEdit::TextChanged)// && umsg == EN_CHANGE
						(textEdit->events[j].object->*textEdit->events[j].function)();
				}
			}
			else if (HIWORD(wparam) == CBN_SELCHANGE)
			{
				twListBox *listBox = (twListBox*)GetWindowLong((HWND)lparam, GWL_USERDATA);
				listBox->triggerEvent(twListBox::ItemChanged);
			}
			else if (HIWORD(wparam) == BN_CLICKED)
			{
				if (LOWORD(wparam) == 0)
				{
					twButton *button = (twButton*)GetWindowLong((HWND)lparam, GWL_USERDATA);
					button->triggerEvent(twButton::Clicked);
				}
				else
				{
					twToolBar *tb = (twToolBar*)GetWindowLong((HWND)lparam, GWL_USERDATA);
					tb->m_clickedButton = LOWORD(wparam) - 1;
					tb->triggerEvent(twToolBar::Clicked);
				}
			}
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_KEYDOWN:
		{
			keyDown = (Key)wparam;
			bool alreadyAdded = false;
			for (int i = 0; i < keysDown.size(); i++)
			{
				if (keysDown[i] == keyDown)
					alreadyAdded = true;
			}
			if (!alreadyAdded)
				keysDown.push_back(keyDown);
			triggerEvent(KeyDown);
			return 0;//An application should return zero if it processes this message.
		}
		case WM_ACTIVATE:
		{
			if (wparam == WA_INACTIVE)
			{
				for (unsigned int i=0;i<events.size();i++)
				{
					if (events[i].event == twWindow::Deactivated)
						(events[i].object->*events[i].function)();
				}
			}
			else
			{
				for (unsigned int i=0;i<events.size();i++)
				{
					if (events[i].event == twWindow::Activated)
						(events[i].object->*events[i].function)();
				}
			}
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_MOUSEMOVE://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 512)
		{
			if (m_mouseLeave)
			{
				m_mouseLeave = false;
				for (unsigned int i=0;i<events.size();i++)
				{
					if (events[i].event == twWindow::MouseJoin)
						(events[i].object->*events[i].function)();
				}
				TRACKMOUSEEVENT mouseEvent;
				mouseEvent.dwFlags = TME_LEAVE;
				mouseEvent.hwndTrack = m_window;
				mouseEvent.cbSize = sizeof(mouseEvent);
				TrackMouseEvent(&mouseEvent);
			}
			m_cursorPosition.X = LOWORD(lparam);
			m_cursorPosition.Y = HIWORD(lparam);
			triggerEvent(MouseMove);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_MOUSELEAVE:
		{
			m_mouseLeave = true;
			triggerEvent(MouseLeave);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_KEYUP:
		{
			keyUp = (Key)wparam;
			for (unsigned int i=0;i<keysDown.size();i++)
			{
				if (keysDown[i] == keyUp)
					keysDown.erase(keysDown.begin() + i);
			}
			triggerEvent(KeyUp);
			return 0;//An application should return zero if it processes this message.
		}
		case WM_LBUTTONDOWN://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 513)
		{
			for (unsigned int i=0;i<children->size();i++)
			{
				if (children->at(i).type == tw::Text)
				{
					twText *text = static_cast<twText*>(children->at(i).child);
					if (getCursorPosition().X >= text->getPosition().X &&
						getCursorPosition().Y >= text->getPosition().Y &&
						getCursorPosition().X <= text->getPosition().X + text->getTextSize().width &&
						getCursorPosition().Y <= text->getPosition().Y + text->getTextSize().height)
					{
						text->triggerEvent(twText::Clicked);
					}
				}
			}
			m_mouseDown = Left;
			mouseLeftClicked = true;
			triggerEvent(MouseDown);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_RBUTTONDOWN://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 516)
		{
			m_mouseDown = Right;
			mouseRightClicked = true;
			triggerEvent(MouseDown);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_MBUTTONDOWN://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 519)
		{
			m_mouseDown = Middle;
			mouseMiddleClicked = true;
			triggerEvent(MouseDown);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_LBUTTONUP://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 514)
		{
			m_mouseUp = Left;
			mouseLeftClicked = false;
			triggerEvent(MouseUp);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_RBUTTONUP://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 517)
		{
			m_mouseUp = Right;
			mouseRightClicked = false;
			triggerEvent(MouseUp);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_MBUTTONUP://|| (umsg == WM_SETCURSOR && HIWORD(lparam) == 520)
		{
			m_mouseUp = Middle;
			mouseMiddleClicked = false;
			triggerEvent(MouseUp);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_MOUSEWHEEL:
		{
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
				m_mouseWheel = true;
			else
				m_mouseWheel = false;
			triggerEvent(MouseWheel);
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_DROPFILES:
		{
			dropFiles.clear();
			HDROP hdrop = (HDROP)wparam;
			int count = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
			for (int i = 0; i < count; i++)
			{
				int pathLen = DragQueryFile(hdrop, i, NULL, 0);
				char buffer[pathLen + 1];
				DragQueryFile(hdrop, i, buffer, pathLen + 1);
				dropFiles.push_back(buffer);
			}
			triggerEvent(DropFiles);
			return 0;//An application should return zero if it processes this message.
		}
		case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS *windowPos = (WINDOWPOS*)lparam;
			if (m_maximumSize.width > 0 && windowPos->cx > m_maximumSize.width)
				windowPos->cx = m_maximumSize.width;
			if (m_maximumSize.height > 0 && windowPos->cy > m_maximumSize.height)
				windowPos->cy = m_maximumSize.height;
			if (m_minimumSize.width > 0 && windowPos->cx < m_minimumSize.width)
				windowPos->cx = m_minimumSize.width;
			if (m_minimumSize.height > 0 && windowPos->cy < m_minimumSize.height)
				windowPos->cy = m_minimumSize.height;
			return 0;//If an application processes this message, it should return zero.
		}
		case WM_ERASEBKGND://fait buguer les tabsbox ATTENTION BUG GLOBAL
		{
			if (m_backgroundColor.a > 0)
			{
				RECT rect;
				GetClientRect(hwnd, &rect);
				HBRUSH brush = CreateSolidBrush(RGB(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b));
				FillRect((HDC)wparam, &rect, brush);
				DeleteObject(brush);
				return true;
			}
			return DefWindowProc(hwnd, umsg, wparam, lparam);//An application should return nonzero if it erases the background; otherwise, it should return zero.
		}
		/*
		if (umsg == WM_NCPAINT)
		{
			HDC hdc;
			hdc = GetDCEx(hwnd, (HRGN)wparam, DCX_WINDOW|DCX_INTERSECTRGN);

			RECT r;
			GetClientRect((HWND)wparam, &r);
			HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, hbrush);
			Rectangle(hdc, 4, 4, 100, 100);
			cout << r.right << endl;
			DeleteObject(hbrush);

			ReleaseDC(hwnd, hdc);
		}
		*/
		/* PLUS BESOIN AVEC RICHEDIT
		if (umsg == WM_CTLCOLOREDIT)
		{
			twTextEdit *textEdit = (twTextEdit*)GetWindowLong((HWND)lparam, GWL_USERDATA);
			if (textEdit->getBackgroundColor().a > 0)
				SetBkColor((HDC)wparam, RGB(textEdit->getBackgroundColor().r, textEdit->getBackgroundColor().g, textEdit->getBackgroundColor().b));
			if (textEdit->getTextColor().a > 0)
				SetTextColor((HDC)wparam, RGB(textEdit->getTextColor().r, textEdit->getTextColor().g, textEdit->getTextColor().b));
			if (textEdit->getBackgroundColor().a > 0 || textEdit->getTextColor().a > 0)
				return (LRESULT)GetStockObject(NULL_BRUSH);
		}
	*/
		case WM_CTLCOLORBTN:
		{
			twButton *button = (twButton*)GetWindowLong((HWND)lparam, GWL_USERDATA);
			SetBkMode((HDC)wparam, TRANSPARENT);
			if (button->getBackgroundColor().a > 0)
				SetBkColor((HDC)wparam, RGB(button->getBackgroundColor().r, button->getBackgroundColor().g, button->getBackgroundColor().b));
			if (button->getTextColor().a > 0)
				SetTextColor((HDC)wparam, RGB(button->getTextColor().r, button->getTextColor().g, button->getTextColor().b));
			if (button->getBackgroundColor().a > 0 || button->getTextColor().a > 0 || m_backgroundColor.a > 0)
				return (LRESULT)CreateSolidBrush(RGB(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b));
			return 0;//If an application processes this message, it must return a handle to a brush. The system uses the brush to paint the background of the button.
		}

		case WM_CTLCOLORSTATIC:
		{
			if ((HWND)lparam == *m_handle)
			{
				if (m_backgroundColor.a > 0)
					return (LRESULT)CreateSolidBrush(RGB(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b));
			}
			else
			{
				for (unsigned int i=0;i<children->size();i++)
				{
					if (children->at(i).type == tw::Area && (twObject*)GetWindowLong((HWND)lparam, GWL_USERDATA) == children->at(i).child)
					{
						twArea *area = static_cast<twArea*>(children->at(i).child);
						if (area->getBackgroundColor().a > 0)
							return (LRESULT)CreateSolidBrush(RGB(area->getBackgroundColor().r, area->getBackgroundColor().g, area->getBackgroundColor().b));
					}
					if (m_backgroundColor.a > 0)
					{
						if (children->at(i).type == tw::Slider && (twObject*)GetWindowLong((HWND)lparam, GWL_USERDATA) == children->at(i).child)
						{
							twSlider *slider = static_cast<twSlider*>(children->at(i).child);
							return (LRESULT)CreateSolidBrush(RGB(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b));
						}
					}
				}

			}
			return 0;//If an application processes this message, the return value is a handle to a brush that the system uses to paint the background of the static control.
		}
		/*
		case WM_SETCURSOR:
		{
			return 0;//If an application processes this message, it should return TRUE to halt further processing or FALSE to continue.
			if (long(GetWindowLong((HWND)wparam, GWL_USERDATA)) == 0)
				return 0;
			twObject *object = (twObject*)GetWindowLong((HWND)wparam, GWL_USERDATA);
			if (HIWORD(lparam) == 512)
				object->triggerEvent(twObject::Over);
			if (HIWORD(lparam) == 513)
				object->triggerEvent(twObject::Clicked);
				*/
				/*
			m_sender = object;
			if (object->getType() == twObject::TextEdit)
			{
				twTextEdit *textEdit = (twTextEdit*)GetWindowLong((HWND)wparam, GWL_USERDATA);
				if (HIWORD(lparam) == 512)
				{
					for (unsigned int j=0;j<textEdit->events.size();j++)
						if (textEdit->events[j].event == twTextEdit::Over)
							(textEdit->events[j].object->*textEdit->events[j].function)();
				}
				if (HIWORD(lparam) == 513)
				{
					for (unsigned int j=0;j<textEdit->events.size();j++)
						if (textEdit->events[j].event == twTextEdit::Click)
							(textEdit->events[j].object->*textEdit->events[j].function)();
				}
			}


		*/


		//}

		/*
		case WM_CTLCOLORSTATIC)
		{
			//static HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
			//return (LRESULT)brush;
		}
		*/
		/*
		case WM_KEYDOWN)
		{
			if (wparam == VK_RETURN)
			{
				twDialog::information(this, "", "salut");
				for (unsigned int i=0;i<children->size();i++)
				{
					if (children->at(i).type == twObject::TextEdit)
					{
						twObject *textEdit = children->at(i).child;
						for (unsigned int j=0;j<textEdit->events.size();j++)
						{
							if (textEdit->events[j].event == twTextEdit::EnterPressed)
								(textEdit->events[j].object->*textEdit->events[j].function)();
						}
					}
				}
			}
		}
		*/
		case WM_TIMER:
		{
			for (unsigned int i=0;i<children->size();i++)
			{
				/*
				if (children->at(i).type == twObject::Menu && LOWORD(wparam) == i && umsg == WM_COMMAND)
				{
					twMenu *menu = static_cast<twMenu*>(children->at(i).child);
					menu->sendEvent(twMenu::Clicked);
				}
				*/

				/*
				if (children->at(i).type == twObject::Slider && (unsigned int)GetWindowLong((HWND)lparam, GWL_USERDATA) == i)
				{
					twSlider *slider = static_cast<twSlider*>(children->at(i).child);
					for (unsigned int j=0;j<slider->events.size();j++)
					{
						if (slider->events[j].event == twSlider::Move && umsg == WM_HSCROLL)
							(slider->events[j].object->*slider->events[j].function)();
					}
				}
				*/
				if (children->at(i).type == tw::Timer)
				{
					twTimer *timer = static_cast<twTimer*>(children->at(i).child);
					for (unsigned int j=0;j<timer->events.size();j++)
					{
						if (timer->events[j].event == twTimer::TimeOut && umsg == WM_TIMER)
							(timer->events[j].object->*timer->events[j].function)();
					}
				}
				/*
				ESSAI POINTEUR DANS USER DATA
				if (children->at(i).type == twObject::TextEdit && HIWORD(wparam) == EN_CHANGE)
				{
					twTextEdit *textEdit = static_cast<twTextEdit*>(children->at(i).child);
					if (!textEdit->getMultiLine() && textEdit->getNumberLines() > 1)
					{
						twString text;
						for (int j = 0; j < textEdit->getNumberLines(); j++)
							text += textEdit->getLine(j);
						textEdit->setText(text);
						//textEdit->goCharacter(-1);
						for (unsigned int j=0;j<textEdit->events.size();j++)
						{
							if (textEdit->events[j].event == twTextEdit::EnterPressed)
								(textEdit->events[j].object->*textEdit->events[j].function)();
						}
					}
					for (unsigned int j=0;j<textEdit->events.size();j++)
					{
						if (textEdit->events[j].event == twTextEdit::TextChanged)// && umsg == EN_CHANGE
							(textEdit->events[j].object->*textEdit->events[j].function)();
					}
				}
				*/
			}
			return 0;//An application should return zero if it processes this message.
		}
		case WM_HSCROLL:
		{
			twObject *object = (twObject*)GetWindowLong((HWND)lparam, GWL_USERDATA);
			if (object->getType() == tw::Slider)
			{
				twSlider *slider = (twSlider*)GetWindowLong((HWND)lparam, GWL_USERDATA);
				slider->triggerEvent(twSlider::Moved);
			}
			return 0;//If an application processes this message, it should return zero.
		}
		/*
		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT drayItem = (LPDRAWITEMSTRUCT)lparam;
			SetTextColor(drayItem->hDC, RGB(255, 0, 0));
			//ExtTextOut(drayItem->hDC, 0, 0, 0 , &drayItem->rcItem,drayItem->itemData, strlen(drayItem->itemData), NULL);
			return true;//If an application processes this message, it should return TRUE.
		}
		*/
		/*
		case WM_LBUTTONDOWN:
		{
			if (GetWindowLong(GetFocus(), GWL_USERDATA) < 100)
				return false;
			twObject *object = (twObject*)GetWindowLong(GetFocus(), GWL_USERDATA);
			if (object->getType() == tw::Slider)
				twDialog::information(this, "", "");//g_mainWindow->focus();
			return false;
		}
		*/
		case WM_USER + 1://tray icon
		{
			for (unsigned int i=0;i<children->size();i++)
			{
				if (children->at(i).type == tw::SystemTrayIcon)
				{
					twTrayIcon *systemTrayIcon = static_cast<twTrayIcon*>(children->at(i).child);
					int event = LOWORD(lparam);
					if (event == WM_LBUTTONUP)
						systemTrayIcon->triggerEvent(twTrayIcon::Clicked);
					else if (event == WM_RBUTTONUP)
						systemTrayIcon->triggerEvent(twTrayIcon::RightClicked);
					else if (event == 1029)
						systemTrayIcon->triggerEvent(twTrayIcon::MessageClicked);
				}
			}
			return 0;
		}
		/*
		case WM_NCACTIVATE:
		{
			if (wparam == 1)
			{
				m_activated = true;
				//if (m_flachingTimer != NULL && m_flachingAutoStop == true)
				//{
				//	delete m_flachingTimer;
				//	m_flachingTimer = NULL;
				//}
				return 0;
			}
			else if (wparam == 0)
			{
				m_activated = false;
				return true;//When the wParam parameter is FALSE, an application should return TRUE to indicate that the system should proceed with the default processing, or it should return FALSE to prevent the change. When wParam is TRUE, the return value is ignored.
			}
		}
		*/
		case WM_NOTIFY:
		{
			LPNMHDR pnmhdr = (LPNMHDR)lparam;
			if (pnmhdr->code  == EN_MSGFILTER)
			{
				bool ctrlOn = GetKeyState(VK_CONTROL) & 0x8000;
				if (ctrlOn)
				{
					MSGFILTER* mf = (MSGFILTER*)lparam;
					if (mf->msg == 258)
					{
						if (mf->wParam == 22)
						{
							twTextEdit *textEdit = (twTextEdit*)GetWindowLong(mf->nmhdr.hwndFrom, GWL_USERDATA);
							EnableWindow(*textEdit->m_handle, false);
							int pos = textEdit->getStartingSelection();
							twString text = textEdit->getText();

							twString textBefore = text;

							for (unsigned int i = 0; i < textEdit->wordsStyle.size(); i++)
							{
								int findPos = 0;
								findPos = textBefore.find(textEdit->wordsStyle[i].word, findPos);
								while (findPos != -1)
								{
									bool cond;
									if (textEdit->wordsStyle[i].word.size() == 1 && isSpecialChar(textEdit->wordsStyle[i].word[0]))
										cond = true;
									else
										cond = (findPos == 0 || isSpecialChar(textBefore[findPos - 1])) && (findPos + textEdit->wordsStyle[i].word.size() == textBefore.size() || isSpecialChar(textBefore[findPos + textEdit->wordsStyle[i].word.size()]));
									if (cond)
									{
										textEdit->setSelection(findPos, findPos + textEdit->wordsStyle[i].word.size());
										textEdit->setSelectionStyle(textEdit->wordsStyle[i].ts);
									}
									findPos = textBefore.find(textEdit->wordsStyle[i].word, findPos + 1);
								}
							}


							textEdit->setSelection(pos, pos);

							EnableWindow(*textEdit->m_handle, true);
							textEdit->focus();
						}
					}
				}
			}
			else if (pnmhdr->code == LVN_ITEMCHANGED)
			{
				LPNM_LISTVIEW pnlv = (LPNM_LISTVIEW)lparam;
				twListView *listView = (twListView*)GetWindowLong(pnlv->hdr.hwndFrom, GWL_USERDATA);
				if (pnlv->uNewState == 3 && pnlv->iItem >= 0)
				{
					listView->m_selectedItem = pnlv->iItem;
					listView->triggerEvent(twListView::ItemChanged);
				}
			}
			else if (pnmhdr->code == NM_DBLCLK)
			{
				LPNMITEMACTIVATE pnlv = (LPNMITEMACTIVATE)lparam;
				twListView *listView = (twListView*)GetWindowLong(pnlv->hdr.hwndFrom, GWL_USERDATA);
				if (pnlv->iItem >= 0)
					listView->triggerEvent(twListView::ItemDoubleClicked);
			}
			else if (pnmhdr->code == NM_CLICK)
			{
				LPNMITEMACTIVATE pnlv = (LPNMITEMACTIVATE)lparam;
				twListView *listView = (twListView*)GetWindowLong(pnlv->hdr.hwndFrom, GWL_USERDATA);
				if (pnlv->iItem >= 0)
					listView->triggerEvent(twListView::ItemClicked);
			}
			else if (pnmhdr->code == NM_RCLICK)
			{
				LPNMITEMACTIVATE pnlv = (LPNMITEMACTIVATE)lparam;
				twListView *listView = (twListView*)GetWindowLong(pnlv->hdr.hwndFrom, GWL_USERDATA);
				if (pnlv->iItem >= 0)
					listView->triggerEvent(twListView::ItemRightClicked);
			}
			else if (pnmhdr->code == NM_CUSTOMDRAW)
			{
				LPNM_LISTVIEW pnlv = (LPNM_LISTVIEW)lparam;
				twListView *listView = (twListView*)GetWindowLong(pnlv->hdr.hwndFrom, GWL_USERDATA);
				if (listView != 0)
				{
					listView->triggerEvent(twListView::SelectionChanged);
				}
			}
			else if (pnmhdr->code == LVN_GETDISPINFO)
			{
				NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lparam;
				twListView *listView = (twListView*)GetWindowLong(plvdi->hdr.hwndFrom, GWL_USERDATA);
				twListViewItem *item = listView->getItem(plvdi->item.iItem);
				if (plvdi->item.iSubItem == 0)
					plvdi->item.pszText = (char*)item->getText(0).c_str();
				for (int j = 1; j < listView->getColumnCount(); j++)
				{
					if (plvdi->item.iSubItem == j)
						plvdi->item.pszText = (char*)item->getText(j).c_str();
				}
				return true;
			}
			else if (pnmhdr->code == LVN_ENDLABELEDIT)
			{
				LPNMLVDISPINFO pdi = (LPNMLVDISPINFO)lparam;
				twListView *listView = (twListView*)GetWindowLong(pdi->hdr.hwndFrom, GWL_USERDATA);
				if (pdi->item.pszText != NULL)
				{
					twString text = pdi->item.pszText;
					listView->getItem(pdi->item.iItem)->setText(text, pdi->item.iSubItem);
				}
				listView->triggerEvent(twListView::TextChanged);
			}
			else if (pnmhdr->code == TCN_SELCHANGE)
			{
				twTabsBox *tabsBox = (twTabsBox*)GetWindowLong(pnmhdr->hwndFrom, GWL_USERDATA);
				for (unsigned int j=0;j<tabsBox->m_tabs.size();j++)
				{
					if(TabCtrl_GetCurSel(tabsBox->m_tabsBox) == (int)j)
					{
						/*
						for (unsigned int k=0;k<tabsBox->m_tabs.size();k++)
						{
							if (k != j)
							{
								if (tabsBox->m_tabs[k] != NULL)
									tabsBox->m_tabs[k]->setVisible(false);//ShowWindow(*tabsBox->m_tabs[k]->m_handle, SW_HIDE);
							}
						}
						*/
						if (tabsBox->m_tabs[j] != NULL)
						{
							tabsBox->m_tabs[j]->setVisible(true);
							tabsBox->m_tabs[j]->focus();
						}
					}
					else
					{
						if (tabsBox->m_tabs[j] != NULL)
							tabsBox->m_tabs[j]->setVisible(false);
					}
				}
				tabsBox->recalculateSize();
				tabsBox->sendEvent(twTabsBox::CurrentChanged);
			}
			else if (pnmhdr->code == MCN_SELECT)
			{
				twCalendar *calendar = (twCalendar*)GetWindowLong(pnmhdr->hwndFrom, GWL_USERDATA);
				calendar->triggerEvent(twCalendar::SelectionChanged);
				//LPNMSELCHANGE lpNMSelChange = (LPNMSELCHANGE)lparam;
				//twDialog::information(this, "", lpNMSelChange->stSelStart.wDay);
			}
			return 0;//The return value is ignored except for notification messages that specify otherwise.
		}
		case WM_CLOSE:
		{
			for (unsigned int i = 0; i < events.size(); i++)
			{
				if (events[i].event == Quit)
				{
					events[i].object->m_sender = this;
					bool Return = (events[i].object->*events[i].function)();
					if (!Return)
						return 1;
				}
			}
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		case WM_DESTROY:
		{
			//if (hwnd == getWindow())
			//    PostQuitMessage(0);//DestroyWindow(hwnd);
			//else
		   // {
				if (this == g_mainWindow)// && !parent->getEnable()
				{
					PostQuitMessage(0);
				}
				else
				{
					((twWidget*)getParent())->setEnable(true);
					//parent->setVisible(true);
					DestroyWindow(hwnd);
				}
		   // }
		   return 0;//If an application processes this message, it should return zero.
		}
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	return 0;
}
