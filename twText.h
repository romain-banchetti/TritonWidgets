#ifndef TWTEXT_H_INCLUDED
#define TWTEXT_H_INCLUDED

#include "twObject.h"
#include "twFont.h"
#include "twApplication.h"
#include "twRect.h"
#include "twWindow.h"

class twText : public twObject
{
    public:
        enum event{Clicked};
        twText(twString text, twWidget *parent = NULL, twPosition position = twPosition(0, 0), twColor color = twColor::white(), twFont *font = new twFont("arial", 15));
        virtual ~twText();
        twPosition getPosition();
        void setFont(twFont *font);
        void setColor(twColor color);
        twFont* getFont();
        twColor getColor();
        twString getText();
        void setText(twString text);
        void setPosition(twPosition position);
        twSize getTextSize();
        void redraw();
        void setRelativePosition(twPosition position);
        twPosition getRelativePosition();
        void setRelativeSize(twSize size);
        twSize getRelativeSize();
        void setSize(twSize size);
        twSize getSize();

    private:
        HWND m_handle;
        twPosition m_position;
        twString m_text;
        twFont *m_font;
        twColor m_color;
        twWidget *m_parent;
        twPosition m_relativePosition;
        twSize m_relativeSize;
        twSize m_size;
};

#endif // TWTEXT_H_INCLUDED
