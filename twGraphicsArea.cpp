#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "twGraphicsArea.h"

extern HINSTANCE instance;
//extern HDC hDC;
extern bool g_graphicsMode;

using namespace std;

twGraphicsArea::twGraphicsArea(twWidget *parent, twPosition position, twSize size) : twWidget(&m_graphicsArea)
{
    m_scaleView = 1;
    g_graphicsMode = true;
    PIXELFORMATDESCRIPTOR pfd;
    int format;
    m_graphicsArea = CreateWindow("STATIC", "", WS_CHILD|WS_VISIBLE, position.X, position.Y, size.width, size.height, *parent->m_handle, 0, NULL, NULL);
    hDC = GetDC(m_graphicsArea);
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    format = ChoosePixelFormat( hDC, &pfd );
    SetPixelFormat(hDC, format, &pfd );
    hrc = wglCreateContext( hDC);
    wglMakeCurrent(hDC, hrc);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glMatrixMode(GL_PROJECTION);
    //glOrtho(0, size.width, size.height, 0, 0, 1);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    setType(tw::GraphicsArea);
    setParent(parent);
}

twGraphicsArea::~twGraphicsArea()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(wglGetCurrentContext());
    ReleaseDC(m_graphicsArea, hDC);
}

twImage* twGraphicsArea::capture()
{
    int width  = getSize().width;
    int height = getSize().height;
    vector<unsigned char> pixels(width * height * 4);
    unsigned char* pixelsPtr = &pixels[0];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelsPtr);
    unsigned int pitch = width * 4;
    for (unsigned int y = 0; y < height / 2; y++)
        swap_ranges(pixelsPtr + y * pitch, pixelsPtr + (y + 1) * pitch, pixelsPtr + (height - y - 1) * pitch);
    twImage *image = new twImage;
    image->setSize(twSize(width, height));
    twColor* ptr = reinterpret_cast<twColor*>(pixelsPtr);
    image->m_pixels.assign(ptr, ptr + width * height);
    return image;
}

twImage* twGraphicsArea::capture(twSize size, twPosition position)
{
    /*
    int width  = size.width;
    int height = size.height;
    vector<unsigned char> pixels(width * height * 4);
    unsigned char* pixelsPtr = &pixels[0];
    glReadPixels(0, 0, width, height, GL_RGBA | , GL_UNSIGNED_BYTE, pixelsPtr);
    unsigned int pitch = width * 4;
    for (unsigned int y = 0; y < height / 2; y++)
        swap_ranges(pixelsPtr + y * pitch, pixelsPtr + (y + 1) * pitch, pixelsPtr + (height - y - 1) * pitch);
    twImage *image = new twImage;
    image->setSize(twSize(width, height));
    twColor* ptr = reinterpret_cast<twColor*>(pixelsPtr);
    image->m_pixels.assign(ptr, ptr + width * height);
    return image;
    */
    return new twImage;
}

void twGraphicsArea::setSize(twSize size, bool redray)
{
    glScaled(1 / m_scaleView, 1 / m_scaleView, 1);
	//glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	if (redray)
        SetWindowPos(m_graphicsArea, NULL, 0, 0, size.width, size.height, SWP_NOZORDER|SWP_NOMOVE);
    else
        SetWindowPos(m_graphicsArea, NULL, 0, 0, size.width, size.height, SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);
	glOrtho(0, size.width, size.height, 0, 0, 1);
	glViewport(0, 0, size.width, size.height);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glPushMatrix();
	//glPopMatrix();
	glScaled(m_scaleView, m_scaleView, 1);
}

twPosition twGraphicsArea::getCursorPosition()
{
	POINT position;
	GetCursorPos(&position);
	ScreenToClient(m_graphicsArea, &position);
	return twPosition(position.x * 1 / getScaleView(), position.y * 1 / getScaleView()) + getViewPosition();
}

void twGraphicsArea::setViewPosition(twPosition position)
{
	glTranslated(m_viewPosition.X - position.X, m_viewPosition.Y - position.Y, 0);
	m_viewPosition = position;
}

twPosition twGraphicsArea::getViewPosition()
{
	return m_viewPosition;
}

void twGraphicsArea::setScaleView(double scale)
{
	glScaled(1 / m_scaleView, 1 / m_scaleView, 1);
	m_scaleView = scale;
	glScaled(m_scaleView, m_scaleView, 1);
}

double twGraphicsArea::getScaleView()
{
	return m_scaleView;
}

void twGraphicsArea::draw(twSprite *sprite)
{
    glEnable(GL_TEXTURE_2D);
	twImage *image = sprite->getImage();
	if (image == NULL || !image->isLoaded())
		return;
	float width = sprite->getSize().width;
	float height = sprite->getSize().height;
	twPosition position = getViewPosition();
	twSize size = getSize();
	float x = (sprite->getPositionf().X != 0) ? sprite->getPositionf().X : sprite->getPosition().X;
	float y = (sprite->getPositionf().Y != 0) ? sprite->getPositionf().Y : sprite->getPosition().Y;
	//le getScaleView() == 1 est à virer, j'avais pas envie de tout adapter à un scaleView
	//getScaleView() == 1 &&
	if ((((x + width) * sprite->getScale() * 2) < position.X || (x * sprite->getScale() / 2) > position.X + (size.width * sprite->getScale()) || ((y + height) * sprite->getScale() * 2) < position.Y || ((y * sprite->getScale()) / 2) > position.Y + (size.height * sprite->getScale())))
		return;
	glColor4ub(sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, sprite->getColor().a);
	unsigned int texture = image->m_texture;
	if (texture == 0)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)image->getSize().width, (int)image->getSize().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image->m_pixels[0]);//

		//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, (int)width, (int)height, GL_RGBA, GL_UNSIGNED_BYTE, 0);//, &image->m_pixels[0]

		//glBindTexture(GL_TEXTURE_2D, texture);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (int)width, (int)height, GL_RGBA, GL_UNSIGNED_BYTE, &image->m_pixels[0]);
		//glLoadIdentity();
		//glBindTexture(GL_TEXTURE_2D, texture);
		image->m_texture = texture;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScaled(sprite->getScale(), sprite->getScale(), 1);
	glTranslated(sprite->getRotationPosition().X, sprite->getRotationPosition().Y, 0);
	glRotated(sprite->getRotation(), 0, 0, 1);
	glTranslated(-sprite->getRotationPosition().X, -sprite->getRotationPosition().Y, 0);
	if (sprite->getHorizontalMirror())
	{
		glTranslated(0, height, 0);
        glScaled(1, -1, 1);
	}
	if (sprite->getVerticalMirror())
	{
		glTranslated(width, 0, 0);
        glScaled(-1, 1, 1);
	}
	glBegin(GL_QUADS);
		glTexCoord2d(float(sprite->getImagePosition().X) / float(image->getSize().width), float(sprite->getImagePosition().Y) / float(image->getSize().height));
		glVertex2d(0, 0);
		glTexCoord2d(float(sprite->getImagePosition().X) / float(image->getSize().width), float(sprite->getImagePosition().Y + sprite->getSize().height) / float(image->getSize().height));
		glVertex2d(0, height);
		glTexCoord2d(float(sprite->getImagePosition().X + sprite->getSize().width) / float(image->getSize().width), float(sprite->getImagePosition().Y + sprite->getSize().height) / float(image->getSize().height));
		glVertex2d(width, height);
		glTexCoord2d(float(sprite->getImagePosition().X + sprite->getSize().width) / float(image->getSize().width), float(sprite->getImagePosition().Y) / float(image->getSize().height));
		glVertex2d(width, 0);
	glEnd();
	glPopMatrix();
	glColor4ub(255, 255, 255, 255);
}

void twGraphicsArea::draw(twShape *shape)
{
    glDisable(GL_TEXTURE_2D);
	float width = shape->getSize().width;
	float height = shape->getSize().height;
	int x = shape->getPosition().X;
	int y = shape->getPosition().Y;
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
}

void twGraphicsArea::draw(twText *text)
{
    glDisable(GL_TEXTURE_2D);
    glColor4ub(text->getColor().r, text->getColor().g, text->getColor().b, text->getColor().a);
    glListBase(text->getFont()->m_genLists);
    twString string = text->getText();
    int height = text->getTextSize().height;
    int i = 0;
    do
    {
        twString line;
        if (string.find("\r\n") == twString::npos)
        {
            line = string;
            string = "";
        }
        else
        {
            line = string.substr(0, string.find("\r\n"));
            string = string.substr(string.find("\r\n") + 2);
        }
        glRasterPos2d(text->getPosition().X, text->getPosition().Y + 10 + i * height);
        glCallLists(line.size(), GL_UNSIGNED_BYTE, line.c_str());
        i++;
    } while(!string.empty());
    glColor4ub(255, 255, 255, 255);
}

void twGraphicsArea::clear(twColor color)
{
    glClearColor(double(color.r) / 255, double(color.g) / 255, double(color.b) / 255, double(color.a) / 255);
	glClear(GL_COLOR_BUFFER_BIT);
}
