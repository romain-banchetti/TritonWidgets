#ifndef TWRICHEDIT_H_INCLUDED
#define TWRICHEDIT_H_INCLUDED

#include "twObject.h"
#include "twFont.h"
#include "twApplication.h"

class twTextStyle
{
    public:
        twTextStyle();
        void setColor(twColor color);
        void setFont(twString font);
        void setBold(bool bold);
        void setItalic(bool italic);

        twColor getColor();
        twString getFont();
        bool getBold();
        bool getItalic();

    private:
        twColor m_color;
        twString m_font;
        bool m_bold;
        bool m_italic;
};

struct WordStyle
{
    twString word;
    twTextStyle ts;
};

class twTextEdit : public twWidget
{
    public:
        enum Event{TextChanged, EnterPressed};
        twTextEdit(twObject *parent, bool multiLine = false, twPosition position = twPosition(0, 0), twSize size = twSize(92, 24));
        void setText(twString text);
        void setFont(twFont font);
        int getNumberLines();
        bool getMultiLine();
        void setReadOnly(bool readOnly);
        void append(twString text);
        void goLine(int line);
        void goCharacter(int character);
        twString getLine(int line);
        twString getText();
        void setLine(int numberLine, twString line);
        int getLengthLine(int line);
        int getCurrentLine();
        int getCurrentCharacter();
        twColor getBackgroundColor();
        twColor getTextColor();
        void setBackgroundColor(twColor backgroundColor);
        void setTextColor(twColor textColor);
        void setScrollBar(bool scrollBar);
        void setInteriorMargin(int margin);
        int getInteriorMargin();
        twString getSelection();
        void setSelection(int start, int end);
        int getStartingSelection();
        int getEndingSelection();
        void undo();
        bool canUndo();
        void setZoom(double value);
        double getZoom();
        void setRichMode(bool richMode);
        void setSelectionStyle(twTextStyle ts);
        void setTextStyle(twTextStyle ts);
        void setWordStyle(twString word, twTextStyle ts);
        std::vector<WordStyle> wordsStyle;
        int getTextLength();
        void setPassword(bool value);
        bool getPassword();
        void clear();


    private:
        HWND textEdit;
        bool m_multiLine;
        twColor m_backgroundColor;
        twColor m_textColor;
        int m_scroolBar;
        int m_margin;
        bool m_password;
};

#endif // TWRICHEDIT_H_INCLUDED
