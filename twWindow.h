#ifndef TWWINDOW_H_INCLUDED
#define TWWINDOW_H_INCLUDED

#include "twObject.h"
#include "twWidget.h"
#include "twMenu.h"
#include "twTimer.h"
#include "twSprite.h"
#include "twShape.h"
#include "twText.h"
#include "twString.h"
#include "tw.h"

class twWindow : public twWidget
{
    public:
        enum Event{Resize, KeyDown, KeyUp, Event, Update, MouseMove, MouseLeave, MouseJoin, MouseDown, MouseUp, MouseWheel, Quit, DropFiles, Activated, Deactivated};
        twWindow(twString title, twSize size, tw::Style style = tw::NormalWindow, twWindow *parent = NULL);
        virtual ~twWindow();
        HWND getWindow();
        void flash();//int flashing = 1000, bool autoStop = false
        void setTitle(twString title);
        void setCentralWidget(twWidget *widget);
        twString getTitle();
        void setBackgroundColor(twColor color);
        void show(tw::State state = tw::Normal);
        void setWindowState(tw::State state = tw::Normal);
        void setWindowStyle(tw::Style style);
        void setVisible(bool visible);
        void hide();
        void setTop(bool top);
        void setMinimumSize(twSize minimumSize);
        void setMaximumSize(twSize maximumSize);
        //void setViewPosition(twPosition position);
        void setDragFiles(bool dragFiles);
        //twPosition getViewPosition();
        void addMenu(twMenu *menu);
        void close();
        void flashingTimeOut();
        bool isKeyDown(tw::Key key);
        bool isMouseDown(tw::MouseButton mouseButton);
        void addSprite(twSprite *sprite);
        void redraw(bool erase = false);
        /*
        void dray(twSprite *sprite);
        void dray(twShape *shape);
        void dray(twText *text);
        void clear(twColor color = twColor(0, 0, 0));
        */
        tw::Key getKeyDown();
        tw::Key getKeyUp();
        static LRESULT CALLBACK windowProcedure(HWND, UINT, WPARAM, LPARAM);
        LRESULT CALLBACK windowProcedure2(HWND, UINT, WPARAM, LPARAM);
        std::vector<ACCEL> accels;
        HACCEL haccel;
        std::vector<twSprite*> m_sprites;
        void onUpdate();
        twPosition getCursorPosition();
        void setCursorPosition(twPosition position);
        void showCursor(bool showCursor);
        int getElapsedTime();
        int getFrameTime();
        int getFPS();
        void setIcon(twImage *image);
        //void setGraphicAreaPosition(twPosition point);
        //void setGraphicAreaSize(twSize size);
        //void setGraphicMode(bool graphicMode, twPosition point = twPosition(), twSize size = twSize());
		//bool m_graphicMode;
		tw::MouseButton getMouseDown();
		tw::MouseButton getMouseUp();
		//void setScaleView(double scale);
		//double getScaleView();
		bool getMouseWheel();
		void setInteriorMode(bool interiorMode);
		void setSize(twSize size);
		twSize getSize();
		std::vector<twString> getDropFiles();
		//twPosition getGraphicAreaPosition();
		//twSize getGraphicAreaSize();
		void setAlwaysTop(bool alwaysTop);
		void modal();
		void closeModal();
		twImage* capture();
		void recalculateSize(twWidget *rw = NULL);
		bool isSpecialChar(char sc);
        tw::State getWindowState();
        bool getVisible();
        twColor getBackgroundColor();

    private:
		bool m_hide;
        HWND m_window;
        HMENU m_menu;
        bool m_interiorMode;
        twWidget *m_centralWidget;
        bool mb_centralWidget;
        twColor m_backgroundColor;
        twSize m_minimumSize;
        twSize m_maximumSize;
        tw::Key keyDown;
        //twPosition m_viewPosition;
        //double m_scaleView;
        tw::Key keyUp;
        std::vector<tw::Key> keysDown;
        bool mouseLeftClicked;
        bool mouseRightClicked;
        bool mouseMiddleClicked;
        twPosition m_cursorPosition;
        bool m_mouseLeave;
        std::vector<twString> dropFiles;
        //twTimer *m_flachingTimer;
        //bool m_flachingAutoStop;
        bool m_activated;
        int m_lastTime;
        int m_fps;
        int m_frameTime;
        tw::MouseButton m_mouseDown;
        tw::MouseButton m_mouseUp;
        bool m_mouseWheel;
        twWindow *m_parent;
        HDC m_hdc;
        tw::State m_state;
};

#endif // TWBUTTON_H_INCLUDED
