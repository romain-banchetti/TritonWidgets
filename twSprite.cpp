#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "twSprite.h"

extern HINSTANCE instance;

using namespace std;

twSprite::twSprite(twImage *image)
{
    m_image = image;
    m_scale = 1;
    m_color = twColor(255, 255, 255);
    m_relativePosition = twPosition(-1, -1);
    m_rotation = 0;
    m_mirrorH = false;
    m_mirrorV = false;
}

void twSprite::setSize(twSize size)
{
	m_size = size;
}

twSize twSprite::getSize()
{
	if (m_size.width == 0)
		return getImage()->getSize();
	return m_size;
}

void twSprite::setImagePosition(twPosition position)
{
	m_imagePosition = position;
}

twPosition twSprite::getImagePosition()
{
	return m_imagePosition;
}


twPosition twSprite::getRelativePosition()
{
    return m_relativePosition;
}

void twSprite::setRotation(int rotation, twPosition position)
{
    m_positionRotation = position;
    int lastRotation = getRotation();
    m_rotation = rotation;
	for (int i = 0; i < m_attachs.size(); i++)
        m_attachs[i]->setRotation(m_attachs[i]->getRotation() - lastRotation + getRotation(), position);
}

int twSprite::getRotation()
{
    return m_rotation;
}

twPosition twSprite::getRotationPosition()
{
    return m_positionRotation;
}

void twSprite::attach(twSprite *sprite)
{
    sprite->setPosition(sprite->getPosition() + getPosition());
    m_attachs.push_back(sprite);
}

void twSprite::setRelativePosition(twPosition position)
{
    m_relativePosition = position;
}

void twSprite::setHorizontalMirror(bool h)
{
    m_mirrorH = h;
    for (int i = 0; i < m_attachs.size(); i++)
        m_attachs[i]->setHorizontalMirror(h);
}

void twSprite::setVerticalMirror(bool v)
{
    m_mirrorV = v;
    for (int i = 0; i < m_attachs.size(); i++)
        m_attachs[i]->setVerticalMirror(v);
}

bool twSprite::getHorizontalMirror()
{
    return m_mirrorH;
}

bool twSprite::getVerticalMirror()
{
    return m_mirrorV;
}

void twSprite::setPosition(twPosition position)
{
    twPosition lastPosition = getPosition();
    m_position = position;
	for (int i = 0; i < m_attachs.size(); i++)
        m_attachs[i]->setPosition(m_attachs[i]->getPosition() - lastPosition + getPosition());
}

twPosition twSprite::getPosition()
{
	if (m_positionf.X != 0 || m_positionf.Y != 0)
		return twPosition(m_positionf.X, m_positionf.Y);
    return m_position;
}

void twSprite::setPositionf(twPositionf position)
{
    twPositionf lastPosition = getPositionf();
    m_positionf = position;
	for (int i = 0; i < m_attachs.size(); i++)
        m_attachs[i]->setPositionf(m_attachs[i]->getPositionf() - lastPosition + getPositionf());
}

twPositionf twSprite::getPositionf()
{
    return m_positionf;
}

void twSprite::move(int x, int y)
{
	setPosition(twPosition(m_position.X + x, m_position.Y + y));
}

twImage *twSprite::getImage()
{
	return m_image;
}

void twSprite::setImage(twImage *image)
{
	m_image = image;
}

void twSprite::clear()
{
	m_image = NULL;
	m_position = twPosition();
}

void twSprite::setScale(double scale)
{
    double lastScale = getScale();
    m_scale = scale;
	for (int i = 0; i < m_attachs.size(); i++)
        m_attachs[i]->setScale(m_attachs[i]->getScale() - lastScale + getScale());
}

double twSprite::getScale()
{
	return m_scale;
}

twColor twSprite::getColor()
{
    return m_color;
}

void twSprite::setColor(twColor color)
{
    m_color = color;
}
