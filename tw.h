#ifndef TW_H_INCLUDED
#define TW_H_INCLUDED

#include <windows.h>

namespace tw
{
    enum Object{None, Window, Button, Slider, Timer, TextEdit, Menu, Text, TabsBox, Image, ListView, CheckBox, ListBox, ProgressBar, Area, SystemTrayIcon, GraphicsArea, StatusBar, Calendar, ToolBar};
	enum Key{Key_Escape = VK_ESCAPE, Key_Space = VK_SPACE, Key_Left = VK_LEFT, Key_Up = VK_UP, Key_Right = VK_RIGHT, Key_Down = VK_DOWN, Key_A = 0x41, Key_B = 0x42, Key_C = 0x43, Key_D = 0x44, Key_E = 0x45, Key_F = 0x46, Key_G = 0x47, Key_H = 0x48,
	Key_I = 0x49, Key_J = 0x4A, Key_K = 0x4B, Key_L = 0x4C, Key_M = 0x4D, Key_N = 0x4E, Key_O = 0x4F, Key_P = 0x50, Key_Q = 0x51, Key_R = 0x52, Key_S = 0x53, Key_T = 0x54, Key_U = 0x55, Key_V = 0x56, Key_W = 0x57, Key_X = 0x58, Key_Y = 0x59,
	Key_Z = 0x5A, Key_F1 = VK_F1, Key_F2 = VK_F2, Key_F3 = VK_F3, Key_F4 = VK_F4, Key_F5 = VK_F5, Key_F6 = VK_F6, Key_F7 = VK_F7, Key_F8 = VK_F8, Key_F9 = VK_F9, Key_F10 = VK_F10, Key_F11 = VK_F11, Key_F12 = VK_F12, Key_Shift = VK_SHIFT,
	Key_Ctrl = VK_CONTROL, Key_Tab = VK_TAB, Key_Back = VK_BACK, Key_Alt = VK_MENU, Key_Enter = VK_RETURN, Key_Delete = VK_DELETE, Key_Capital = VK_CAPITAL, Key_Home = VK_HOME, Key_End = VK_END, Key_Clear = VK_CLEAR, Key_Return = VK_RETURN,
	Key_Pause = VK_PAUSE, Key_Prior = VK_PRIOR, Key_Next = VK_NEXT, Key_Select = VK_SELECT, Key_Execute = VK_EXECUTE, Key_ScreenShot = VK_SNAPSHOT, Key_Insert = VK_INSERT, Key_Help = VK_HELP, Key_0 = VK_NUMPAD0, Key_1 = VK_NUMPAD1, Key_2 = VK_NUMPAD2,
	Key_3 = VK_NUMPAD3, Key_4 = VK_NUMPAD4, Key_5 = VK_NUMPAD5, Key_6 = VK_NUMPAD6, Key_7 = VK_NUMPAD7, Key_8 = VK_NUMPAD8, Key_9 = VK_NUMPAD9, Key_Multiply = VK_MULTIPLY, Key_Add = VK_ADD, Key_Separator = VK_SEPARATOR, Key_Subtract = VK_SUBTRACT,
	Key_Decimal = VK_DECIMAL, Key_Divide = VK_DIVIDE, Key_NumLock = VK_NUMLOCK, Key_Scroll = VK_SCROLL, Key_LeftShift = VK_LSHIFT, Key_RightShift = VK_RSHIFT, Key_LeftControl = VK_LCONTROL, Key_RightControl = VK_RCONTROL, Key_LeftAlt = VK_LMENU,
	Key_RightAlt = VK_RMENU, Key_ATTN = VK_ATTN, Key_CRSEL = VK_CRSEL, Key_EXSEL = VK_EXSEL, Key_EraseEOF = VK_EREOF, Key_Play = VK_PLAY, Key_Zoom = VK_ZOOM, Key_Empty = VK_NONAME, Key_PA1 = VK_PA1};
	enum SpecialKey{Ctrl, Alt, Shift, Null};
	enum MouseButton{Left, Right, Middle};
	enum State{Maximized, Minimized, FullScreen, Normal};
	enum Style{NormalWindow = WS_OVERLAPPEDWINDOW, NoSizingWindow = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, Dialog = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, TitleBar = WS_OVERLAPPED | WS_CAPTION, SizingTitleBar = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX, MinimizeTitleBar = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX};
    char keyToChar(Key key);
}

#endif // TW_H_INCLUDED
