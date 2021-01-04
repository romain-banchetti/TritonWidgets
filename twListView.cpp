#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <uxtheme.h>
#include <Commctrl.h>

#include "twListView.h"

extern HINSTANCE instance;

using namespace std;

twListView::twListView(twObject *parent, bool multiSelection, twPosition position, twSize size) : twWidget(&m_listView)
{
    twWidget *widget = static_cast<twWidget*>(parent);
    m_multiSelection = 0;
    m_editable = 0;
    if (!multiSelection)
        m_multiSelection = LVS_SINGLESEL;
    m_listView = CreateWindow(WC_LISTVIEW, "", LBS_EXTENDEDSEL|WS_CHILD|WS_VISIBLE|LVS_LIST|m_multiSelection, position.X, position.Y, size.width, size.height, *widget->m_handle, NULL, instance, NULL);
    setType(tw::ListView);
    setParent(parent);
    SetWindowLong(m_listView, GWL_USERDATA, (long)this);
    m_imageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 100);
    columnCount = 0;
    if (parent != NULL && widget->getType() == tw::Window && ((twWindow*)widget)->getBackgroundColor().a != 0)
		setBackgroundColor(((twWindow*)widget)->getBackgroundColor());
    SetWindowTheme(m_listView, L"Explorer", NULL);
    SendMessage(m_listView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, 32 | 16 | 0x00010000);

}

void twListView::setMultiSelection(bool multiSelection)
{
    if (multiSelection)
        m_multiSelection = 0;
    else
        m_multiSelection = LVS_SINGLESEL;
    SetWindowLong(m_listView, GWL_STYLE, WS_CHILD|WS_VISIBLE|LVS_LIST|m_editable|m_multiSelection);
}

bool twListView::getMultiSelection()
{
    return m_multiSelection;
}

void twListView::setEditable(bool editable)
{
    if (editable)
        m_editable = LVS_EDITLABELS;
    else
        m_editable = 0;
    SetWindowLong(m_listView, GWL_STYLE, WS_CHILD|WS_VISIBLE|LVS_LIST|m_editable|m_multiSelection);
}

bool twListView::getEditable()
{
    return m_editable;
}

void twListView::setBackgroundColor(twColor color)
{
	ListView_SetBkColor(m_listView, RGB(color.r, color.g, color.b));
	ImageList_SetBkColor(m_imageList, RGB(color.r, color.g, color.b));
	setTextBackgroundColor(color);
}

void twListView::setTextBackgroundColor(twColor color)
{
	ListView_SetTextBkColor(m_listView, RGB(color.r, color.g, color.b));
}

void twListView::setTextColor(twColor color)
{
	ListView_SetTextColor(m_listView, RGB(color.r, color.g, color.b));
}

twListViewItem* twListView::getItem(int index)
{
	if (index >= 0 && index < items.size())
		return items[index];
	return NULL;
}

void twListView::setIconSize(twSize size)
{
	ImageList_SetIconSize(m_imageList, size.width, size.height);
}

void twListView::addItem(twListViewItem *item)
{
	LV_ITEM lvi;
	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.iItem = items.size();
	if (!item->getText().empty())
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	if (item->getIcon() != NULL && item->getIcon()->isLoaded())
	{
		lvi.mask = lvi.mask | LVIF_IMAGE;
		lvi.iImage = ImageList_AddIcon(m_imageList, item->getIcon()->toIcon());
		ListView_SetImageList(m_listView, m_imageList, LVSIL_SMALL);
		ImageList_SetBkColor(m_imageList, CLR_NONE);
		ListView_SetImageList(m_listView, m_imageList, LVSIL_SMALL);
	}
	lvi.iImage = -1;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	items.push_back(item);
	ListView_InsertItem(m_listView, &lvi);
	item->m_parent = this;
	item->id = lvi.iItem;
}

twListViewItem* twListView::getSelectedItem()
{
	return items[m_selectedItem];
}

vector<twListViewItem*> twListView::getSelectedItems()
{
    vector<twListViewItem*> selectedItems;
    for (int i = 0; i < items.size(); i++)
    {
        if (ListView_GetItemState(m_listView, i, LVIS_SELECTED) == LVIS_SELECTED)
            selectedItems.push_back(items[i]);
    }
	return selectedItems;
}

void twListView::clear()
{
	for (int i = 0; i < items.size(); i++)
		delete items[i];
	items.clear();
	ListView_DeleteAllItems(m_listView);
	ImageList_Destroy(m_imageList);
	m_imageList = ImageList_Create(16, 16, ILC_COLOR32 , 0, 100);
}

int twListView::idByItem(twListViewItem *item)
{
	for (int i = 0; i < items.size(); i++)
		if (items[i] == item)
			return i;
}

void twListView::deleteItem(twListViewItem *item)
{
	for (unsigned int i = getItemIndex(item); i < getCount(); i++)
		getItem(i)->id --;
	ListView_DeleteItem(m_listView, getItemIndex(item));
	delete item;
	items.erase(items.begin() + idByItem(item));
}

int twListView::addColumn(twString text, int width)
{
    setView(Details);
	LVCOLUMN column;
	column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	column.fmt = LVCFMT_LEFT;
	column.pszText = (char*)text.c_str();
	column.cx = width;
	ListView_InsertColumn(m_listView, getColumnCount(), &column);
	columnCount++;
	return columnCount;
}

int twListView::getColumnCount()
{
	return columnCount;
}

void twListView::deleteColumn(int index)
{
    ListView_DeleteColumn(m_listView, index);
}

twString twListView::getColumn(int index)
{
    LVCOLUMN column;
    ListView_GetColumn(m_listView, index, &column);
    return column.pszText;
}

int twListView::getCount()
{
	return ListView_GetItemCount(m_listView);
}

int twListView::getSelectedCount()
{
    return ListView_GetSelectedCount(m_listView);
}

int twListView::getItemIndex(twListViewItem *item)
{
    for (unsigned int i = 0; i < items.size(); i++)
    {
        if (items[i] == item)
            return i;
    }
    return -1;
}

void twListView::setSelectedItem(twListViewItem *item, bool Scroll)
{
    if (item == NULL)
    {
        ListView_SetItemState(m_listView, -1, 0, LVIS_SELECTED | LVIS_FOCUSED);
        return;
    }
    int index = getItemIndex(item);
    m_selectedItem = index;
    ListView_SetItemState(m_listView, index, LVIS_SELECTED, LVIS_SELECTED);
    if (Scroll)
        scroll(item);
}

void twListView::scroll(twListViewItem *item)
{
    int index = getItemIndex(item);
    RECT rect;
    ListView_GetItemRect(m_listView, index, &rect, LVIR_LABEL);
    ListView_Scroll(m_listView, 0, rect.top - (rect.bottom - rect.top));
}

void twListView::setView(View view)
{
	ListView_SetView(m_listView, view);
}

//---------------------------------------------------------------------

twListViewItem::twListViewItem(twString text)
{
    m_image = NULL;
	if(!text.empty())
		setText(text);
}

twListViewItem::~twListViewItem()
{
	if (getIcon() != NULL)
		ImageList_Remove(((twListView*)m_parent)->m_imageList, id);
}

void twListViewItem::setText(twString text, int index)
{
	if (index == -1)
		texts.push_back(text);
    else
        texts[index] = text;
}

void twListViewItem::setIcon(twImage *image)
{
	LVITEM lvi;
	if (getIcon() == NULL && image == NULL)
		return;
	lvi.iImage = -1;
	if (getIcon() == NULL)
	{
		 lvi.iImage = ImageList_AddIcon(((twListView*)m_parent)->m_imageList, image->toIcon());
	}
	else
	{
		if (image == NULL)
			ImageList_Remove(((twListView*)m_parent)->m_imageList, id);
		else
			ImageList_ReplaceIcon(((twListView*)m_parent)->m_imageList, id, image->toIcon());
	}
	if (getText().empty())
		lvi.mask = LVIF_IMAGE;
	else
	{
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.pszText = LPSTR_TEXTCALLBACK;
	}
	lvi.iItem = id;
	lvi.iSubItem = 0;
	ListView_SetItem(*((twListView*)m_parent)->m_handle, &lvi);
	ListView_SetImageList(*((twListView*)m_parent)->m_handle, ((twListView*)m_parent)->m_imageList, LVSIL_SMALL);
	m_image = image;
}

twString twListViewItem::getText(int index)
{
	if (texts.size() <= index)
		return "";
	return texts[index];
}

twImage* twListViewItem::getIcon()
{
	return m_image;
}
