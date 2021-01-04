#ifndef TWTABSBOX_H_INCLUDED
#define TWTABSBOX_H_INCLUDED

#include "twWindow.h"
#include "twWidget.h"
#include <commctrl.h>

class twTabsBox : public twWidget
{
    public:
        enum Event{CurrentChanged};
        twTabsBox(twObject *parent, twPosition position = twPosition(0, 0), twSize size = twSize(300, 200));
        virtual ~twTabsBox();
        int addWidget(twWidget *widget, twString title);
        int getCurrentIndex();
        void setCurrentIndex(int index);
        void removeTab(int index);
        int getCount();
        twWidget *getWidget(int index);
        std::vector<twWidget*> m_tabs;
        HWND m_tabsBox;
        void setText(int index, twString text);
        twString getText(int index);
        void recalculateSize();

    private:
        TC_ITEM tie;
};

#endif // TWTABSBOX_H_INCLUDED
