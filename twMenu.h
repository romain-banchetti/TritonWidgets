#ifndef TWMENU_H_INCLUDED
#define TWMENU_H_INCLUDED

#include "twApplication.h"
#include "twImage.h"

class twMenu : public twObject
{
    public:
        enum Event{Clicked};
        twMenu(twObject *parent, twString text = "", twImage *icon = NULL);
        virtual ~twMenu();
        void addMenu(twMenu *menu);
        void addSeparator();
        void setEnable(bool enable);
        void setShortcut(tw::Key key, tw::SpecialKey specialKey = tw::Null);
        void setText(twString text);
        HMENU getMenu();
        int getId();
        twString getText();
        HMENU m_parentMenu;
        bool m_enable;
        int m_id;
        void display(twPosition position, bool byWindow = true);
        void setIcon(twImage *icon);
        twImage* getIcon();

    private:
        HMENU m_menu;
        twString m_text;
        twImage *m_icon;
};

#endif // TWMENU_H_INCLUDED
