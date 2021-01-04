#ifndef TWTOOLBAR_H_INCLUDED
#define TWTOOLBAR_H_INCLUDED

#include "twApplication.h"

struct Button
{
	twString text;
	twImage *image;
};

class twToolBar : public twWidget
{
		public:
			enum Event{Clicked};
			twToolBar(twWidget *parent, twPosition position = twPosition());
			virtual ~twToolBar();
			void setIconSize(twSize size);
			void addButton(twImage *icon, twString text = "");
			int getClickedButton();
			twString getText(int index = -1);
			twImage* getIcon(int index = -1);
			int m_clickedButton;
			void setIcon(int index, twImage *icon);
			void setText(int index, twString text);

		private:
			HWND toolBar;
			HIMAGELIST imageList;
			std::vector<Button> buttons;
};

#endif // TWTOOLBAR_H_INCLUDED
