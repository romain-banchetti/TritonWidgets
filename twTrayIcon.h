#ifndef TWSYSTEMTRAYICON_H_INCLUDED
#define TWSYSTEMTRAYICON_H_INCLUDED

#include "twWindow.h"

class twTrayIcon : public twObject
{
    public:
        enum Event{Clicked, MessageClicked, RightClicked};
        twTrayIcon(twObject *parent = NULL, twString text = "");
        virtual ~twTrayIcon();
        void setIcon(twImage *image);
        void setText(twString text);
        void setMessage(twString message, twString title = "", int timeout = 0);
        void setVisible(bool visible);
        twString getText();
        twString getMessage();

    private:
        twString m_message;
        twString m_title;
        int m_timeOut;
        twString m_text;
        NOTIFYICONDATA *systemTrayIcon;
        //char *m_secure;
};

#endif // TWSYSTEMTRAYICON_H_INCLUDED
