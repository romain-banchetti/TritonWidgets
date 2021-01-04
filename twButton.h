#ifndef TWBUTTON_H_INCLUDED
#define TWBUTTON_H_INCLUDED
#include <Commctrl.h>

#include "twImage.h"
#include "twWindow.h"

class twButton : public twWidget
{
    public:
        enum Event{Clicked};
        twButton(twWidget *parent, twString text, twPosition position = twPosition(0, 0), twSize size = twSize(92, 24));
        HWND getButton();
        void setText(twString text);
        twColor getBackgroundColor();
        twColor getTextColor();
        void setBackgroundColor(twColor backgroundColor);
        void setTextColor(twColor textColor);
        twString getText();

    private:
        HWND button;
        twColor m_backgroundColor;
        twColor m_textColor;
};

#endif // TWBUTTON_H_INCLUDED
