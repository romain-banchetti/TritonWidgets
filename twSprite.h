#ifndef TWSPRITE_H_INCLUDED
#define TWSPRITE_H_INCLUDED

#include "twImage.h"
#include "twWidget.h"

class twSprite : public twObject
{
    public:
        twSprite(twImage *image = NULL);
        void setPosition(twPosition position = twPosition(0, 0));
        twPosition getPosition();
        void setPositionf(twPositionf position = twPositionf(0, 0));
        twPositionf getPositionf();
        void move(int x, int y);
        twImage* getImage();
        void setImage(twImage *image);
        void clear();
        void setScale(double scale);
        double getScale();
        twColor getColor();
        void setColor(twColor color);
        void setRelativePosition(twPosition position);
        twPosition getRelativePosition();
        void setRotation(int rotation, twPosition position = twPosition(0, 0));
        int getRotation();
        twPosition getRotationPosition();
        void attach(twSprite *sprite);
        void setHorizontalMirror(bool h);
        void setVerticalMirror(bool v);
        bool getHorizontalMirror();
        bool getVerticalMirror();
        void setSize(twSize size);
        twSize getSize();
        void setImagePosition(twPosition position);
        twPosition getImagePosition();

    private:
		twWidget *m_parent;
		twImage *m_image;
		twPosition m_position;
		twPositionf m_positionf;
		double m_scale;
		twColor m_color;
		twPosition m_relativePosition;
		int m_rotation;
		twPosition m_positionRotation;
		std::vector<twSprite*> m_attachs;
		bool m_mirrorH, m_mirrorV;
		twSize m_size;
		twPosition m_imagePosition;

};

#endif // TWSPRITE_H_INCLUDED
