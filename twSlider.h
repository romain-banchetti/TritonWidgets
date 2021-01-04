#ifndef TWSLIDER_H_INCLUDED
#define TWSLIDER_H_INCLUDED

#include "twObject.h"
#include "twApplication.h"

class twSlider : public twWidget
{
    public:
        enum Event{Moved};
        twSlider(twObject *parent, twPosition position = twPosition(0, 0), twSize size = twSize(400, 25));
        void setMaximum(int maximum);
        void setMinimum(int minimum);
        void setCursorPosition(int position);
        void setPosition(twPosition position);
        void setSize(twSize size);
        int getCursorPosition();
        int getMaximum();
        int getMinimum();
        int id;

    private:
        HWND slider;
};

#endif // TWSLIDER_H_INCLUDED
