#ifndef TWLISTVIEW_H_INCLUDED
#define TWLISTVIEW_H_INCLUDED

#include "twWindow.h"

class twListViewItem
{
	public:
		twListViewItem(twString text = "");
		virtual ~twListViewItem();
		void setText(twString text, int index = -1);
		void setIcon(twImage *image);
		twString getText(int index = 0);
        twImage* getIcon();
        twWidget *m_parent;
        int id;

	private:
		std::vector<twString> texts;
		twImage *m_image;

};

class twListView : public twWidget
{
    public:
		enum Event{ItemChanged, ItemDoubleClicked, SelectionChanged, TextChanged, ItemClicked, ItemRightClicked};
		enum View{List = 0x0003, Icon = 0x0000, SmallIcon = 0x0002, Details = 0x0001, Tile = 0x0004};
        twListView(twObject *parent, bool multiSelection = false, twPosition position = twPosition(0, 0), twSize size = twSize(200, 200));
        void addItem(twListViewItem *item);
        twListViewItem* getSelectedItem();
        std::vector<twListViewItem*> getSelectedItems();
        void deleteItem(twListViewItem *item);
        int getCount();
        int getColumnCount();
        int getSelectedCount();
        int m_selectedItem;
        void setView(View view);
        void setBackgroundColor(twColor color);
        void setTextBackgroundColor(twColor color);
        void setTextColor(twColor color);
        void setIconSize(twSize size);
        void clear();
        int addColumn(twString text, int width = 100);
        twListViewItem* getItem(int index);
        int getItemIndex(twListViewItem *item);
        void setEditable(bool editable);
        bool getEditable();
        void setMultiSelection(bool multiSelection);
        bool getMultiSelection();
        void setSelectedItem(twListViewItem *item, bool Scroll = true);
        void scroll(twListViewItem *item);
        HIMAGELIST m_imageList;
        void deleteColumn(int index);
        twString getColumn(int index);

    private:
        HWND m_listView;
        std::vector<twListViewItem*> items;
        int idByItem(twListViewItem *item);
        int columnCount;
        int m_editable;
        int m_multiSelection;
};

#endif // TWLISTVIEW_H_INCLUDED
